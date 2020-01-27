/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#	include "pg_sql.h"
pg_sql::pg_sql() {
	_pq_error_text = new char;
	_n_error_text = new char;
	_connected = false;
	_conn = NULL; _n_error = 0; _pq_error = 0;
	_is_disposed = 0;
};
pg_sql::~pg_sql() {
	if (_is_disposed != 0 )return;
	//if (_conn == NULL)return;
	_is_disposed = 1;
	if (_connected) { exit_nicely(); }
	//_pg_result = NULL;
	_conn = NULL;
	delete _pq_error_text; delete _n_error_text;
	_pq_error_text = NULL; _n_error_text = NULL;
}
void pg_sql::clear_response() {
	PGresult* res;
	while ((res = PQgetResult(_conn))) {
		PQclear(res);
	}
}
int pg_sql::parse_connection_string(const char * conn, std::string & user, std::string & pwd, std::string & server, std::string & port, std::string & db) {
	if (((conn != NULL) && (conn[0] == '\0')) || conn == NULL) {
		panic("No connection string found!!!");
		return -1;
	}
	std::string* query = new std::string(conn);
	std::regex pattern("([\\w+%]+)=([^;]*)");
	std::map<std::string, std::string> conn_obj;
	auto words_begin = std::sregex_iterator(query->begin(), query->end(), pattern);
	auto words_end = std::sregex_iterator();
	for (std::sregex_iterator i = words_begin; i != words_end; i++) {
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		/*conn_obj[key] = value;*/
		if (conn_obj.find(key) != conn_obj.end()) {
			key = "Duplicate key found in connection string	==> `" + key + "`!!!";
			panic(key.c_str());
			query->clear(); delete query;
			goto _ERROR;
		}
		conn_obj[key] = value;
	};
	query->clear(); delete query;
	user = conn_obj["UserId"];
	if (user.empty()) {
		panic("No user defined (e.g. postgress) in given connection string as `UserId`!!!");
		goto _ERROR;
	}
	pwd = conn_obj["Password"];
	if (pwd.empty()) {
		panic("No password defined (e.g. 123456) in given connection string as `Password`!!!");
		goto _ERROR;
	}
	db = conn_obj["Database"];
	if (db.empty()) {
		panic("No database defined (e.g. sow) in given connection string as `Database`!!!");
		goto _ERROR;
	}
	server = conn_obj["Server"];
	if (server.empty()) {
		panic("No server defined (e.g. localhost) in given connection string as `Password`!!!");
		goto _ERROR;
	}
	port = conn_obj["Port"];
	if (port.empty()) {
		panic("No port defined (e.g. 5432) in given connection string as `Port`!!!");
		goto _ERROR;
	}
	goto _END;
_ERROR:
	conn_obj.clear();
	return _n_error;
_END:
	//conn_obj.clear();
	return 1;
};
int pg_sql::connect(const char *conn) {
	// Check if already connected
	if (_connected == true)return 1;
	int ret = 0;
	std::string*user = new std::string();
	std::string*pwd = new std::string();
	std::string*server = new std::string();
	std::string*port = new std::string();
	std::string*db = new std::string();
	ret = parse_connection_string(conn, *user, *pwd, *server, *port, *db);
	if (ret < 0)goto _END;
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
	if (PQstatus(_conn) != CONNECTION_OK) {
		panic();
		goto _ERROR;
	}
	_connected = true;
	goto _END;
_ERROR:
	ret = -1;
	/*close the connection to the database and cleanup*/
	PQfinish(_conn);
	_conn = NULL;
	goto _END;

_END:
	user->clear(); pwd->clear(); server->clear(); port->clear(); db->clear();
	delete user; delete pwd; delete server; delete port; delete db;
	return ret;
	//set_error(std::to_string(ret).c_str());
	//return  -1;
}
void pg_sql::exit_nicely() {
	if (!_connected)return;
	if (_conn != NULL) {
		clear_response();
		/*close the connection to the database and cleanup*/
		PQfinish(_conn); _conn = NULL;
		_connected = false;
	}
	return;
}
// Get row count for the specified object
int pg_sql::get_row_count(const char *object, char *value) {
	if (((object != NULL) && (object[0] == '\0')) || object == NULL) {
		panic("SQL Statement required!!!");
		return -1;
	}
	std::string query = "SELECT COUNT(*) FROM ";
	query += object;
	// Execute the query
	int rc = 0;
	execute_query(query.c_str(), rc);
	query.clear();
	return rc;
};
//!TODO
const char* pg_sql::execute_cmd(const char *command, int&ret) {
	if (((command != NULL) && (command[0] == '\0')) || command == NULL) {
		panic("SQL Statement required!!!");
		return '\0';
	}
	panic("pg_sql::execute_cmd not implemented yet!!!");
	return '\0';
	/*PGresult *PQexecParams(PGconn *conn,
			const char *command,
			int nParams,
			const Oid *paramTypes,
			const char *const * paramValues,
			const int *paramLengths,
			const int *paramFormats,
			int resultFormat)*/
	command = "INSERT INTO  person(name) values($1) returning person_id";
	int nParams = 0;
	const char*name = "";
	const char *values[1] = { name };
	int lengths[1] = {  (int)strlen(name) };
	int binary[1] = { 1 };
	PGresult *res = PQexecParams(
		/*PGconn *conn*/_conn,//Active connection
		/*const char *command*/command,//SQL Statement
		/*int nParams*/nParams,//number of parameters
		/*const Oid *paramTypes*/NULL,//let the backend deduce param type
		/*const char *const * paramValues*/values,//values to substitute $1 and $2
		/*const int *paramLengths*/lengths,//the lengths, in bytes, of each of the parameter values
		/*const int *paramFormats*/binary,//whether the values are binary or not
		/*int resultFormat*/0//we want the result in text format
	);
	//std::vector<char> chars;
	//const char* char_arr = chars.data();
	const char* result = '\0';
	ExecStatusType exs_type = PQresultStatus(res);
	if (exs_type == PGRES_FATAL_ERROR) goto _ERROR;
	if (exs_type == PGRES_NONFATAL_ERROR) goto _ERROR;
	if (exs_type == PGRES_TUPLES_OK) {
		char*resp = PQgetvalue(res, 0, 0);
		result = const_cast<const char*>(resp);
	}
	ret = 0;
	goto _END;
_ERROR:
	panic();
	goto _END;
_END:
	/*Delete execution result*/
	PQclear(res);
	return result;
}
const char* pg_sql::execute_query(const char *query, int&ret) {
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) { 
		panic("SQL Statement required!!!");
		return '\0'; 
	}
	//https://timmurphy.org/2009/11/19/pqexecparams-example-postgresql-query-execution-with-parameters/
	// Execute the statement
	PGresult *res = PQexec(_conn, query);
	const char* result = '\0';
	ExecStatusType exs_type = PQresultStatus(res);
	if (exs_type == PGRES_FATAL_ERROR) goto _ERROR;
	if (exs_type == PGRES_NONFATAL_ERROR) goto _ERROR;
	// Get the value of the first column of the first row
	if (exs_type == PGRES_TUPLES_OK) {
		char*resp = PQgetvalue(res, 0, 0);
		result = const_cast<const char*>(resp);
	}
	ret = 0;
	goto _END;

_ERROR:
	panic();
	goto _END;
_END:
	/*Delete execution result*/
	PQclear(res);
	return result;
};

//https://www.postgresql.org/docs/8.3/libpq-example.html
// Execute the statement and get scalar result
int pg_sql::execute_scalar_x(const char *query, std::list< std::string>&out_param_array, std::map<std::string, char*>&out_param_map) {
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return -1;
	}
	bool exists = false;
	try {
		// Execute the statement
		PGresult *res = PQexec(_conn, query);
		ExecStatusType exs_type = PQresultStatus(res);
		if (exs_type == PGRES_FATAL_ERROR) goto _ERROR;
		if (exs_type == PGRES_NONFATAL_ERROR) goto _ERROR;
		// Get the value of the first column of the first row
		if (exs_type == PGRES_TUPLES_OK) {
			int field_map = 0;
			std::string prop;
			size_t len = 0;
			for (auto s = out_param_array.begin(); s != out_param_array.end(); ++s) {
				prop = *s;
				char* resp = PQgetvalue(res, 0, field_map);
				len = strlen(resp);
				char* copy_resp = new char[len + 1];
				//strcpy_s(copy_resp, len, resp);
				strcpy(copy_resp, resp);
				out_param_map[prop.c_str()] = copy_resp;
				field_map++;
				//free(resp);
			}
			exists = true;
			goto _END;
		}
		if (exs_type == PGRES_COMMAND_OK) { 
			exists = true; goto _END;
		}
		panic("Invalid response defined!!!!");
		goto _END;
	_ERROR:
		panic(); goto _END;
	_END:
		/*Delete execution result*/
		PQclear(res);
	} catch (std::exception&e) {
		panic(e.what());
	}
	return (exists == true) ? 0 : -1;
};
// Execute the statement
int pg_sql::execute_non_query(const char *query) {
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return _n_error;
	}
	// Execute the query
	PGresult *result = PQexec(_conn, query);
	bool error = false;
	int rc = PQresultStatus(result);
	// Error raised
	if (rc != PGRES_COMMAND_OK) {
		panic();
		error = true;
	}
	/*Delete execution result*/
	PQclear(result);
	return (error == true) ? -1 : 0;
};
const int pg_sql::is_api_error() {
	return (_n_error >= 0 && _pq_error >= 0) ? -1 : 0;
};
const char * pg_sql::get_last_error() {
	if (_n_error >= 0 && _pq_error >= 0)return "No Error Found!!!";
	if (_pq_error < 0) {
		return const_cast<const char*>(_pq_error_text);
	}
	return const_cast<const char*>(_n_error_text);
};
// Set error code and message for the last API call
void pg_sql::panic() {
	_pq_error = -1;
	//free(_pq_error_text);
	char* erro_msg = PQerrorMessage(_conn);
	size_t len = strlen(erro_msg);
	_pq_error_text = new char[len + 1];
	strcpy(_pq_error_text, erro_msg);
	//free(erro_msg);
	_n_error = 0;
};
void pg_sql::panic(const char* std_error_msg) {
	_n_error = -1;
	//free(_n_error_text);
	size_t len = strlen(std_error_msg);
	_n_error_text = new char[len + 1];
	strcpy(_n_error_text, std_error_msg);
	_pq_error = 0;
};