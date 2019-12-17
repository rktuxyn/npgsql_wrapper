#include "npgsql_query.h"

npgsql_query::npgsql_query(pg_connection_pool* cpool){
	_cpool = cpool;
	_internal_error = new char;
	_errc = 0;
}

npgsql_query::~npgsql_query() {
	free_connection();
}

void npgsql_query::free_connection(){
	if ( _cpool ) {
		_cpool->busy = 0;
		_cpool = NULL;
	}
}
void npgsql_query::exit_nicely() {
	if ( _cpool ) {
		PQfinish(_cpool->conn);
		_cpool->conn = NULL;
		_cpool->busy = -1;
	}
}
int npgsql_query::execute_scalar_x(const char* query, std::list<std::string>& out_param_array, std::map<std::string, char*>& out_param_map){
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return -1;
	}
	bool exists = false;
	try {
		// Execute the statement
		PGresult* res = PQexec(_cpool->conn, query);
		free_connection();
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
				strcpy(copy_resp, resp);
				out_param_map[prop.c_str()] = copy_resp;
				field_map++;
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
		panic( PQerrorMessage( _cpool->conn ) ); goto _END;
	_END:
		/*Delete execution result*/
		PQclear(res);
	}
	catch (std::exception& e) {
		panic(e.what());
	}
	return (exists == true) ? 0 : -1;
}

int npgsql_query::execute_non_query(const char* query) {
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return _errc;
	}
	// Execute the query
	PGresult* result = PQexec(_cpool->conn, query);
	free_connection();
	bool error = false;
	int rc = PQresultStatus(result);
	// Error raised
	if (rc != PGRES_COMMAND_OK) {
		panic( PQerrorMessage(_cpool->conn) );
		error = true;
	}
	/*Delete execution result*/
	PQclear(result);
	return (error == true) ? -1 : 0;
};

const char* npgsql_query::get_last_error(){
	if (_errc >= 0) return "No Error Found!!!";
	return const_cast<const char*>(_internal_error);
}

void npgsql_query::panic(const char* error) {
	free(_internal_error);
	_internal_error = new char[strlen(error) + 1];
	strcpy(_internal_error, error);
	_errc = -1;
}

void npgsql_query::panic(char* erro_msg) {
	free(_internal_error);
	size_t len = strlen(erro_msg);
	_internal_error = new char[len + 1];
	strcpy(_internal_error, erro_msg);
}
