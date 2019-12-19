/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "npgsql_query.h"

npgsql_query::npgsql_query(pg_connection_pool* cpool){
	_cpool = cpool;
	_internal_error = new char;
	_errc = 0;
}

npgsql_query::~npgsql_query() {
	free_connection();
	if (_internal_error != NULL) {
		free(_internal_error);
	}
}

void npgsql_query::free_connection(){
	if ( _cpool ) {
		_cpool->busy = 0;
		_cpool->error_code = 0;
		_cpool->error_msg = NULL;
		_cpool = NULL;
	}
}
const char* npgsql_query::execute_query(const char* query, int& rec){
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return '\0';
	}
	if (_cpool->conn == NULL) {
		panic("No Connection instance found !!!");
		return '\0';
	}
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return '\0';
	}
	//https://timmurphy.org/2009/11/19/pqexecparams-example-postgresql-query-execution-with-parameters/
	// Execute the statement
	PGresult* res = PQexec(_cpool->conn, query);
	const char* result = '\0';
	ExecStatusType exs_type = PQresultStatus(res);
	if (exs_type == PGRES_FATAL_ERROR) goto _ERROR;
	if (exs_type == PGRES_NONFATAL_ERROR) goto _ERROR;
	// Get the value of the first column of the first row
	if (exs_type == PGRES_TUPLES_OK) {
		char* resp = PQgetvalue(res, 0, 0);
		result = const_cast<const char*>(resp);
	}
	rec = 0;
	goto _END;

_ERROR:
	panic( );
	goto _END;
_END:
	/*Delete execution result*/
	PQclear(res);
	return result;
}
void npgsql_query::exit_nicely() {
	if ( _cpool ) {
		PQfinish(_cpool->conn);
		_cpool->conn = NULL;
		_cpool->busy = -1;
		_cpool->conn_state = connection_state::CLOSED;
		_cpool->error_code = 0;
		_cpool->error_msg = NULL;
	}
}
int npgsql_query::execute_scalar_x(const char* query, std::list<std::string>& out_param_array, std::map<std::string, char*>& out_param_map){
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	if (_cpool->conn == NULL) {
		panic("No Connection instance found !!!");
		return -1;
	}
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return -1;
	}
	bool exists = false;
	try {
		// Execute the statement
		PGresult* res = PQexec(_cpool->conn, query);
		//free_connection();
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
		panic( ); goto _END;
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
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	if (_cpool->conn == NULL) {
		panic("No Connection instance found !!!");
		return -1;
	}
	if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
		panic("SQL Statement required!!!");
		return _errc;
	}
	// Execute the query
	PGresult* result = PQexec(_cpool->conn, query);
	//free_connection();
	bool error = false;
	int rc = PQresultStatus(result);
	// Error raised
	if (rc != PGRES_COMMAND_OK) {
		panic( );
		error = true;
	}
	/*Delete execution result*/
	PQclear(result);
	return (error == true) ? -1 : 0;
}
int npgsql_query::execute_scalar(const char* sp, std::list<npgsql_params*>& sql_param, std::map<std::string, char*>& result){
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	std::string* out_param = new std::string("");
	std::string* in_param = new std::string("");
	int in_param_count = 0;
	int out_param_count = 0;
	std::list<std::string>* out_param_array = new std::list<std::string>();
	for (auto s = sql_param.begin(); s != sql_param.end(); ++s) {
		npgsql_params* param = *s;
		if (param->direction == parameter_direction::Input || param->direction == parameter_direction::InputOutput) {
			std::string val = param->value;
			//quote_literal(val);
			if (in_param_count == 0) {
				in_param_count++;
				in_param->append("( ");
				in_param->append(val);
				in_param->append("::");
				in_param->append(get_db_type(param->db_type));
			}
			else {
				in_param_count++;
				in_param->append(", ");
				in_param->append(val);
				in_param->append("::");
				in_param->append(get_db_type(param->db_type));
			}
			if (param->direction != parameter_direction::InputOutput)
				continue;
		}
		if (param->direction == parameter_direction::Output || param->direction == parameter_direction::InputOutput) {
			result[param->parameter_name] = '\0';
			out_param_array->push_back(param->parameter_name);
			if (out_param_count == 0) {
				out_param->append("f.");
				out_param->append(param->parameter_name);
				out_param_count++;
				continue;
			}
			out_param->append(", f.");
			out_param->append(param->parameter_name);
			out_param_count++;
			continue;
		}
	}
	std::string* query = new std::string("select ");
	if (out_param_count <= 0) {
		query->append("f.* from ");
	}
	else {
		query->append(out_param->c_str());
		query->append(" from ");
	}
	if (in_param_count <= 0) {
		query->append(sp);
		query->append("(");
	}
	else {
		query->append(sp);
		query->append(in_param->c_str());
	}
	query->append(") as f ");
	int ret = this->execute_scalar_x(query->c_str(), *out_param_array, result);
	out_param->clear(); delete out_param;
	in_param->clear(); delete in_param;
	query->clear(); out_param_array->clear();
	delete query; delete out_param_array;
	return ret;
}
int npgsql_query::execute_io(const char* sp, const char* ctx, const char* form_data, std::map<std::string, char*>& result){
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	std::string* query = new std::string("select ");
	query->append("f.* from ");
	query->append(sp);
	query->append("(");
	std::string* val = new std::string(ctx);
	quote_literal(*val);
	query->append(val->c_str());
	val->clear(); delete val;
	query->append("::");
	query->append(get_db_type(npgsql_db_type::Jsonb));
	query->append(",");
	val = new std::string(form_data);
	quote_literal(*val);
	query->append(val->c_str());
	val->clear(); delete val;
	query->append("::");
	query->append(get_db_type(npgsql_db_type::Jsonb));
	query->append(") as f ");

	std::list<std::string>* out_param_array = new std::list<std::string>();
	out_param_array->push_back("_ret_data_table");
	out_param_array->push_back("_ret_val");
	out_param_array->push_back("_ret_msg");
	int ret = this->execute_scalar_x(query->c_str(), *out_param_array, result);
	out_param_array->clear(); delete out_param_array;
	query->clear(); delete query;
	return ret;
}

const char* npgsql_query::get_last_error(){
	if (_errc >= 0) return "No Error Found!!!";
	return const_cast<const char*>(_internal_error);
}

void npgsql_query::panic(const char* error) {
	if (_internal_error != NULL)
		free(_internal_error);
	_internal_error = new char[strlen(error) + 1];
	strcpy(_internal_error, error);
	_errc = -1;
}

void npgsql_query::panic() {
	if (_internal_error != NULL)
		free(_internal_error);
	//if (_cpool == NULL)return;
	//if (_cpool->conn == NULL)return;
	char* erro_msg = PQerrorMessage(_cpool->conn);
	_internal_error = new char[strlen(erro_msg) + 1];
	strcpy(_internal_error, const_cast<const char*>(erro_msg));
	_errc = -1;
}
