/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "pg_sql.h"

pg_sql::pg_sql() {
	_pq_error_text = new char;
	_n_error_text = new char;
};
pg_sql::~pg_sql() {
	if (_connected) { exit_nicely(); }
	_pg_result = NULL; _conn = NULL; 
	delete _pq_error_text; delete _n_error_text;
};
void pg_sql::parse_connection_string(const char * conn, std::string & user, std::string & pwd, std::string & server, std::string & port, std::string & db) {
	if (conn == NULL)
		return;
	std::string* query=new std::string(conn);
	std::regex pattern("([\\w+%]+)=([^;]*)");
	std::map<std::string, std::string> conn_obj;
	auto words_begin = std::sregex_iterator(query->begin(), query->end(), pattern);
	auto words_end = std::sregex_iterator();
	for (std::sregex_iterator i = words_begin; i != words_end; i++) {
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		conn_obj[key] = value;
	};
	free(query);
	user = conn_obj["UserId"];
	pwd = conn_obj["Password"];
	db = conn_obj["Database"];
	server = conn_obj["Server"];
	port = conn_obj["Port"];
	conn_obj.clear();
};
int pg_sql::connect(const char *conn) {
	// Check if already connected
	if (_connected == true)
		return 1;
	std::string*user = new std::string();
	std::string*pwd = new std::string();
	std::string*server = new std::string();
	std::string*port = new std::string(); 
	std::string*db = new std::string();
	parse_connection_string(conn, *user, *pwd, *server, *port, *db);
	/*_PQsetdbLogin(
		const char *pghost, const char *pgport, 
		const char *pgoptions, const char *pgtty, 
		const char *dbName, const char *login,
		const char *pwd
	)*/
	_conn = PQsetdbLogin(
		(server->empty() ? NULL : server->c_str()),
		(port->empty() ? NULL : port->c_str()), NULL, NULL,
		(db->empty() ? NULL : db->c_str()),
		(user->empty() ? NULL : user->c_str()),
		(pwd->empty() ? NULL : pwd->c_str()));
	free(user); free(pwd); free(server); free(port); free(db);
	if (PQstatus(_conn) != CONNECTION_OK) {
		set_error();
		return -1;
	}
	_connected = true;
	return 1;
};
void pg_sql::exit_nicely() {
	if (!_connected)return;
	PQfinish (_conn);
	return;
};
// Get row count for the specified object
int pg_sql::get_row_count(const char *object, char *value) {
	if ((object != NULL) && (object[0] == '\0')) {
		return -1;
	}
	std::string query = "SELECT COUNT(*) FROM ";
	query += object;
	// Execute the query
	int rc = execute_scalar(query.c_str(), value);
	query.clear();
	return rc;
};
int pg_sql::execute_scalar(const char *query, char *value) {
	if ((query != NULL) && (query[0] == '\0'))return -1;
	// Execute the statement
	PGresult *res = PQexec(_conn, query);
	bool exists = false;
	// Get the value of the first column of the first row
	if (PQresultStatus(res) == PGRES_TUPLES_OK) {
		value = PQgetvalue(res, 0, 0);
		exists = true;
	}
	else
		set_error();
	PQclear(res);
	return (exists == true) ? 0 : -1;
};

//https://www.postgresql.org/docs/8.3/libpq-example.html
// Execute the statement and get scalar result
int pg_sql::execute_scalar_x(const char *query, std::list< std::string>&out_param_array, std::map<std::string, char*>&out_param_map) {
	if ((query != NULL) && (query[0] == '\0')) {
		return -1;
	}
	bool exists = false;
	try {
		// Execute the statement
		PGresult *res = PQexec(_conn, query);
		// Get the value of the first column of the first row
		if (PQresultStatus(res) == PGRES_TUPLES_OK) {
			int field_map = 0;
			std::string prop;
			for (auto s = out_param_array.begin(); s != out_param_array.end(); ++s) {
				prop = *s;
				char* resp = PQgetvalue(res, 0, field_map);
				char* copy_resp = new char[strlen(resp) + 1];
				strcpy(copy_resp, resp);
				out_param_map[prop] = copy_resp;
				field_map++;
			}
			exists = true;
		}
		else set_error();
		PQclear(res);
	} catch (std::exception&e) {
		this->set_error(e.what());
	}
	return (exists == true) ? 0 : -1;
};
// Execute the statement
int pg_sql::execute_non_query(const char *query) {
	if ((query != NULL) && (query[0] == '\0')) {
		return -1;
	}
	// Execute the query
	PGresult *result = PQexec(_conn, query);
	bool error = false;
	int rc = PQresultStatus(result);
	// Error raised
	if (rc != PGRES_COMMAND_OK) {
		set_error();
		error = true;
	}
	PQclear(result);
	return (error == true) ? -1 : 0;
};
const int pg_sql::is_api_error() {
	return (_n_error >= 0 && _pq_error >= 0) ? -1 : 0;
};
const char * pg_sql::get_last_error() {
	if (_n_error >= 0 && _pq_error >= 0)return nullptr;
	if (_pq_error < 0) {
		return const_cast<const char*>(_pq_error_text);
	}
	return const_cast<const char*>(_n_error_text);
};
// Set error code and message for the last API call
void pg_sql::set_error() {
	_pq_error = -1;
	char* erro_msg = PQerrorMessage(_conn);
	_pq_error_text = new char[strlen(erro_msg) + 1];
	strcpy(_pq_error_text, erro_msg);
	_n_error = 0;
};
void pg_sql::set_error(const char* std_error_msg) {
	_n_error = -1;
	_n_error_text = new char[strlen(std_error_msg) + 1];
	strcpy(_n_error_text, std_error_msg);
	_pq_error = 0;
};