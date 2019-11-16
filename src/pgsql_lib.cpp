/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "pgsql_lib.h"
LPWSTR s2ws(const char* s) {
	/*wchar_t wtext[FILENAME_MAX];
	mbstowcs(wtext, s, strlen(s) + 1);//Plus null
	LPWSTR ptr = wtext;*/
	//size_t wn = mbsrtowcs(NULL, &s, 0, NULL);
	wchar_t * buf = new wchar_t[_MAX_PATH]();
	mbsrtowcs(buf, &s, _MAX_PATH, NULL);
	LPWSTR& ptr = buf;
	//delete[] buf;
	return ptr;
};
pg_sql_lib::pg_sql_lib() {
	_pg_result = NULL; _conn = NULL; _pgsql_proc_iddl = NULL; _pgsql_module = NULL;
	_PQclear = NULL; _PQerrorMessage = NULL; _PQexec = NULL; _PQfmod = NULL;
	_PQfname = NULL; _PQfsize = NULL; _PQftype = NULL; _PQgetisnull = NULL;
	_PQgetlength = NULL; _PQgetResult = NULL; _PQgetvalue = NULL; _PQnfields = NULL;
	_PQntuples = NULL; _PQputCopyData = NULL; _PQputCopyEnd = NULL; _PQresultErrorMessage = NULL;
	_PQresultStatus = NULL; _PQsetClientEncoding = NULL; _PQsetdbLogin = NULL; _PQstatus = NULL;
	_PQconnectdb = NULL; _PQfinish = NULL;
	_pq_error_text = new char;
	_n_error_text = new char;
};
pg_sql_lib::~pg_sql_lib() {
	if (_connected) { exit_nicely(); }
	_pg_result = NULL; _conn = NULL; _pgsql_proc_iddl = NULL; _pgsql_module = NULL;
	_PQclear = NULL; _PQerrorMessage = NULL; _PQexec = NULL; _PQfmod = NULL;
	_PQfname = NULL; _PQfsize = NULL; _PQftype = NULL; _PQgetisnull = NULL;
	_PQgetlength = NULL; _PQgetResult = NULL; _PQgetvalue = NULL; _PQnfields = NULL;
	_PQntuples = NULL; _PQputCopyData = NULL; _PQputCopyEnd = NULL; _PQresultErrorMessage = NULL;
	_PQresultStatus = NULL; _PQsetClientEncoding = NULL; _PQsetdbLogin = NULL; _PQstatus = NULL;
	_PQconnectdb = NULL; _PQfinish = NULL;
	delete _pq_error_text; delete _n_error_text;
}

// Initialize API
int pg_sql_lib::init(const char* lib_path) {
	if (_pgsql_module == NULL) {
		// Try to load the library by default path
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
		_pgsql_module = dlopen(lib_path, RTLD_NOW);
#else
		_pgsql_module = LoadLibraryEx(s2ws(lib_path), NULL, 0x00000008);
#endif
		// DLL load failed
		if (_pgsql_module == NULL) {
			return -1;
		}
	}
	// Get functions
	if (_pgsql_module != NULL) {
		_PQclear = (PQclearPQFunc)os_support::get_proc_address(_pgsql_module, "PQclear");
		_PQerrorMessage = (PQerrorMessagePQFunc)os_support::get_proc_address(_pgsql_module, "PQerrorMessage");
		_PQexec = (PQexecPQFunc)os_support::get_proc_address(_pgsql_module, "PQexec");
		_PQfmod = (PQfmodPQFunc)os_support::get_proc_address(_pgsql_module, "PQfmod");
		_PQfname = (PQfnamePQFunc)os_support::get_proc_address(_pgsql_module, "PQfname");
		_PQfsize = (PQfsizePQFunc)os_support::get_proc_address(_pgsql_module, "PQfsize");
		_PQftype = (PQftypePQFunc)os_support::get_proc_address(_pgsql_module, "PQftype");
		_PQgetisnull = (PQgetisnullPQFunc)os_support::get_proc_address(_pgsql_module, "PQgetisnull");
		_PQgetlength = (PQgetlengthPQFunc)os_support::get_proc_address(_pgsql_module, "PQgetlength");
		_PQgetResult = (PQgetResultPQFunc)os_support::get_proc_address(_pgsql_module, "PQgetResult");
		_PQgetvalue = (PQgetvaluePQFunc)os_support::get_proc_address(_pgsql_module, "PQgetvalue");
		_PQnfields = (PQnfieldsPQFunc)os_support::get_proc_address(_pgsql_module, "PQnfields");
		_PQntuples = (PQntuplesPQFunc)os_support::get_proc_address(_pgsql_module, "PQntuples");
		_PQputCopyData = (PQputCopyDataPQFunc)os_support::get_proc_address(_pgsql_module, "PQputCopyData");
		_PQputCopyEnd = (PQputCopyEndPQFunc)os_support::get_proc_address(_pgsql_module, "PQputCopyEnd");
		_PQresultErrorMessage = (PQresultErrorMessagePQFunc)os_support::get_proc_address(_pgsql_module, "PQresultErrorMessage");
		_PQresultStatus = (PQresultStatusPQFunc)os_support::get_proc_address(_pgsql_module, "PQresultStatus");
		_PQsetClientEncoding = (PQsetClientEncodingPQFunc)os_support::get_proc_address(_pgsql_module, "PQsetClientEncoding");
		_PQsetdbLogin = (PQsetdbLoginPQFunc)os_support::get_proc_address(_pgsql_module, "PQsetdbLogin");
		_PQstatus = (PQstatusPQFunc)os_support::get_proc_address(_pgsql_module, "PQstatus");
		_PQconnectdb = (PQconnectdbPQFunc)os_support::get_proc_address(_pgsql_module, "PQconnectdb");
		_PQfinish = (PQfinishPQFunc)os_support::get_proc_address(_pgsql_module, "PQfinish");

		if (_PQclear == NULL || _PQerrorMessage == NULL || _PQexec == NULL || _PQfmod == NULL ||
			_PQfname == NULL || _PQfsize == NULL || _PQftype == NULL || _PQgetisnull == NULL ||
			_PQgetlength == NULL || _PQgetResult == NULL || _PQgetvalue == NULL || _PQnfields == NULL ||
			_PQntuples == NULL || _PQputCopyData == NULL || _PQputCopyEnd == NULL ||
			_PQresultErrorMessage == NULL || _PQresultStatus == NULL || _PQsetClientEncoding == NULL ||
			_PQsetdbLogin == NULL || _PQstatus == NULL)
			return -1;
	}
	else {
		os_support::get_last_error_text(LIBPQ_DLL_LOAD_ERROR, _n_error_text, 1024);
		return -1;
	}

	return 1;
};
void pg_sql_lib::parse_connection_string(const char * conn, std::string & user, std::string & pwd, std::string & server, std::string & port, std::string & db) {
	if (conn == NULL)
		return;
	std::string* query = new std::string(conn);
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
int pg_sql_lib::connect(const char *conn) {
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
	_conn = _PQsetdbLogin(
		(server->empty() ? NULL : server->c_str()),
		(port->empty() ? NULL : port->c_str()), NULL, NULL,
		(db->empty() ? NULL : db->c_str()),
		(user->empty() ? NULL : user->c_str()),
		(pwd->empty() ? NULL : pwd->c_str()));
	free(user); free(pwd); free(server); free(port); free(db);
	if (_PQstatus(_conn) != CONNECTION_OK) {
		set_error();
		return -1;
	}
	_connected = true;
	return 1;
};
void pg_sql_lib::exit_nicely() {
	if (!_connected)return;
	_PQfinish (_conn);
	return;
};
// Get row count for the specified object
int pg_sql_lib::get_row_count(const char *object, char *value) {
	if (((object != NULL) && (object[0] == '\0')) || object == NULL) {
		return -1;
	}
	std::string query = "SELECT COUNT(*) FROM ";
	query += object;
	// Execute the query
	int rc = execute_scalar(query.c_str(), value);
	query.clear();
	return rc;
};
int pg_sql_lib::execute_scalar(const char *query, char *value) {
	if ((query != NULL) && (query[0] == '\0'))return -1;
	// Execute the statement
	PGresult *res = _PQexec(_conn, query);
	bool exists = false;
	// Get the value of the first column of the first row
	if (_PQresultStatus(res) == PGRES_TUPLES_OK) {
		value = _PQgetvalue(res, 0, 0);
		exists = true;
	}
	else
		set_error();
	_PQclear(res);
	return (exists == true) ? 0 : -1;
};

//https://www.postgresql.org/docs/8.3/libpq-example.html
// Execute the statement and get scalar result
int pg_sql_lib::execute_scalar_x(const char *query, std::list< std::string>&out_param_array, std::map<std::string, char*>&out_param_map) {
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		return -1;
	}
	bool exists = false;
	try {
		// Execute the statement
		PGresult *res = _PQexec(_conn, query);
		// Get the value of the first column of the first row
		if (_PQresultStatus(res) == PGRES_TUPLES_OK) {
			int field_map = 0;
			std::string prop;
			for (auto s = out_param_array.begin(); s != out_param_array.end(); ++s) {
				prop = *s;
				char* resp = _PQgetvalue(res, 0, field_map);
				char* copy_resp = new char[strlen(resp) + 1];
				strcpy(copy_resp, resp);
				out_param_map[prop] = copy_resp;
				field_map++;
			}
			exists = true;
		}
		else set_error();
		_PQclear(res);
	} catch (std::exception&e) {
		this->set_error(e.what());
	}
	return (exists == true) ? 0 : -1;
};
// Execute the statement
int pg_sql_lib::execute_non_query(const char *query) {
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		return -1;
	}
	// Execute the query
	PGresult *result = _PQexec(_conn, query);
	bool error = false;
	int rc = _PQresultStatus(result);
	// Error raised
	if (rc != PGRES_COMMAND_OK) {
		set_error();
		error = true;
	}
	_PQclear(result);
	return (error == true) ? -1 : 0;
};
const int pg_sql_lib::is_api_error() {
	return (_n_error >= 0 && _pq_error >= 0) ? -1 : 0;
};
const char * pg_sql_lib::get_last_error() {
	if (_n_error >= 0 && _pq_error >= 0)return nullptr;
	if (_pq_error < 0) {
		return const_cast<const char*>(_pq_error_text);
	}
	return const_cast<const char*>(_n_error_text);
};
// Set error code and message for the last API call
void pg_sql_lib::set_error() {
	_pq_error = -1;
	char* erro_msg = _PQerrorMessage(_conn);
	_pq_error_text = new char[strlen(erro_msg) + 1];
	strcpy(_pq_error_text, erro_msg);
	_n_error = 0;
};
void pg_sql_lib::set_error(const char* std_error_msg) {
	_n_error = -1;
	_n_error_text = new char[strlen(std_error_msg) + 1];
	strcpy(_n_error_text, std_error_msg);
	_pq_error = 0;
};
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
int pg_sql_lib::load_pgsql_lib(const char* name) {
	if (_pgsql_proc_iddl) {
		return 1;
	}
	_pgsql_proc_iddl = os_support::load_library(name);
	if (!_pgsql_proc_iddl) {
		return -1;
	}
	return 1;
};
#else
int pg_sql_lib::load_pgsql_lib(const char* name) {
	if (_pgsql_proc_iddl) {
		return 1;
	}
	_pgsql_proc_iddl = os_support::load_library(s2ws(name));
	if (!_pgsql_proc_iddl) {
		return -1;
	}
	return 1;

};
#endif