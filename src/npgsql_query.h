/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
//5:32 PM 12/17/2019
#if !defined(_npgsql_query_h)
#pragma warning (disable : 4231)
#pragma warning(disable : 4996)//Disable strcpy warning
#define _npgsql_query_h
#if !defined(_npgsql_connection_h)
#include "npgsql_connection.h"
#endif//!_npgsql_connection_h
#if !defined(_npgsql_db_type_h)
#include "npgsql_db_type.h"
#endif//!_npgsql_db_type_h
#if !defined(_npgsql_params_h)
#include "npgsql_params.h"
#endif//!_parameter_direction_h
#if !defined(_npgsql_param_type_h)
#include "npgsql_param_type.h"
#endif//!_npgsql_param_type_h
#if !defined(_npgsql_result_h)
#include "npgsql_result.h"
#endif//!_npgsql_result_h
class NPGSQL_API npgsql_query {
public:
	npgsql_query(pg_connection_pool* cpool);
	~npgsql_query();
	void free_connection();
	const char* execute_query(const char* query, int& rec);
	int execute_scalar_x(const char* query, std::list< std::string>& out_param_array, std::map<std::string, char*>& out_param_map);
	int execute_non_query(const char* query);
	int execute_scalar(const char* sp, std::list<npgsql_params*>& sql_param, std::map<std::string, char*>& result);
	int execute_io(const char* sp, const char* ctx, const char* form_data, std::map<std::string, char*>& result);
	template<class _func>
	int execute_scalar_l(const char* query, std::list<npgsql_param_type*>& sql_param, _func func);
	template<class _func>
	int execute_scalar(const char* query, _func func);
	template<class _func>
	int execute_scalar(const char* query, std::list<npgsql_params*>& sql_param, _func func);
	void quote_literal(std::string& str) {
		str = "'" + str + "'";
		return;
	}
	const char* get_last_error();
	void exit_nicely();
private:
	pg_connection_pool* _cpool;
	int _errc;
	char* _internal_error;
	void panic(const char* error);
	void panic();
};

template<class _func>
int npgsql_query::execute_scalar_l(const char* query, std::list<npgsql_param_type*>& sql_param, _func func) {
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	std::string* _query = new std::string(query);
	if (!sql_param.empty()) {
		int count = 0;
		for (auto itr = sql_param.begin(); itr != sql_param.end(); ++itr) {
			count++;
			std::string* str = new std::string("\\$");
			str->append(std::to_string(count));
			std::regex* re = new std::regex(*str);
			npgsql_param_type* key = *itr;
			std::string* val;
			if (key->db_type == npgsql_db_type::NULL_) {
				val = new std::string("null");
			}
			else {
				val = new std::string(key->value);
				if (key->db_type != npgsql_db_type::COMMON) {
					val->append("::"); val->append(get_db_type(key->db_type));
				}
				else {
					quote_literal(*val);
				}
			}
			std::string& copy = *_query;
			copy = std::regex_replace(copy, *re, val->c_str());
			str->clear(); val->clear();
			delete str; delete re; delete val;
		}
	}

	int ret = this->execute_scalar(_query->c_str(), func);
	//free(_query);
	delete _query;
	return ret;
}
template<class _func>
int npgsql_query::execute_scalar(const char* query, _func func) {
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	bool exists = false;
	PGresult* res = PQexec(_cpool->conn, query );
	ExecStatusType exs_type = PQresultStatus(res);
	if (exs_type == PGRES_FATAL_ERROR) goto _ERROR;
	if (exs_type == PGRES_NONFATAL_ERROR) goto _ERROR;
	if (exs_type == PGRES_TUPLES_OK) {
		exists = true;
		int nFields = PQnfields(res);
		size_t len = 0;
		for (int i = 0; i < PQntuples(res); i++) {
			std::vector<char*>* rows = new std::vector<char*>();
			for (int j = 0; j < nFields; j++) {
				char* c = PQgetvalue(res, i, j);
				len = strlen(c);
				char* copy = new char[len + 1];
				//strcpy_s(copy, len, c);
				strcpy(copy, c);
				rows->push_back(copy);
				//free(c);
			}
			func(i, *rows);
			delete rows;
		}
		goto _END;
	}

	if (exs_type == PGRES_COMMAND_OK) {
		exists = true; goto _END;
	}
	panic("Invalid response defined!!!!");
	exists = false;
	goto _END;
_ERROR:
	panic( );
	goto _END;
_END:
	/*Delete execution result*/
	PQclear(res);
	return (exists == true) ? 0 : -1;
}
template<class _func>
int npgsql_query::execute_scalar(const char* query, std::list<npgsql_params*>& sql_param, _func func) {
	if (_cpool->conn_state == connection_state::CLOSED) {
		panic("Connection state not opend!!!");
		return -1;
	}
	std::string* param_stmt = new std::string("");
	int param_count = 0;
	for (auto s = sql_param.begin(); s != sql_param.end(); ++s) {
		npgsql_params* param = *s;
		if (param->direction != parameter_direction::Input) {
			throw new std::exception("You should pass input param");
		}
		std::string* val = new std::string(param->value);
		quote_literal(*val);
		if (param_count == 0) {
			param_count++;
			param_stmt->append(" where ");
			param_stmt->append(param->parameter_name);
			param_stmt->append("=");
			param_stmt->append(val->c_str());
			free(val);
			param_stmt->append("::");
			param_stmt->append(get_db_type(param->db_type));
			continue;
		}
		param_count++;
		param_stmt->append(", ");
		param_stmt->append(param->parameter_name);
		param_stmt->append("=");
		param_stmt->append(val->c_str());
		free(val);
		param_stmt->append("::");
		param_stmt->append(get_db_type(param->db_type));
	}
	int ret = 0;
	if (param_count <= 0) {
		delete param_stmt;
		ret = this->execute_scalar(query, func);
	}
	else {
		std::string* stmt = new std::string(query);
		stmt->append(param_stmt->c_str());
		ret = this->execute_scalar(stmt->c_str(), func);
		delete stmt; delete param_stmt;
	}
	return ret;
}
#endif//!_npgsql_query_h