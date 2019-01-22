/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:16 AM 11/19/2018
//#pragma warning(suppress : 4996)
#pragma once
#if !defined(_pg_sql_h)
#pragma warning(disable : 4996)
#define _pg_sql_h
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_os_support_h)
#include "os_support.h"
#endif
#if !defined(LIBPQ_FE_H)
#include <libpq-fe.h>
#endif//!LIBPQ_FE_H
#if !defined(POSTGRES_EXT_H)
#include <postgres_ext.h>
#endif//!POSTGRES_EXT_H
class pg_sql {
private:
	bool							_connected;
	PGconn*							_conn;
	PGresult*						_pg_result;
	int								_cursor_rows_fetched;
	size_t							_copy_cols_count;
	int								_n_error;
	char*							_n_error_text;
	int								_pq_error;
	char*							_pq_error_text;
public:
	pg_sql();
	~pg_sql();
	virtual const int is_api_error();
	virtual const char * get_last_error();
	// Connect to the database
	virtual int connect(const char *conn);
	virtual void exit_nicely();
	virtual int get_row_count(const char *object, char *value);
	virtual int execute_scalar(const char *query, char *value);
	template<class _func>
	int execute_scalar(const char *query, _func func) {
		if ((query != NULL) && (query[0] == '\0')) {
			return -1;
		}
		bool exists = false;
		PGresult *res = PQexec(_conn, query);
		if (PQresultStatus(res) == PGRES_TUPLES_OK) {
			exists = true;
			int nFields = PQnfields(res);
			for (int i = 0; i < PQntuples(res); i++) {
				std::vector<char*>*rows = new std::vector<char*>();
				for (int j = 0; j < nFields; j++) {
					char* c = PQgetvalue(res, i, j);
					char* copy = new char[strlen(c) + 1];
					strcpy_s(copy, strlen(c), c);
					rows->push_back(copy);
				}
				func(i, *rows);
				free(rows);
			}
		}
		else set_error();
		PQclear(res);
		return (exists == true) ? 0 : -1;
	};
	virtual int execute_scalar_x(const char *query, std::list<std::string>&out_param_array, std::map<std::string, char*>&out_param_map);
	virtual int execute_non_query(const char *query);
private:
	virtual void parse_connection_string(const char *conn, std::string& user, std::string& pwd, std::string& server, std::string& port, std::string& db);
	virtual void set_error(const char* std_error_msg);
	virtual void set_error();
};
//5:30 AM 11/19/2018 Connected
//7:55 AM 11/19/2018 END
#endif//!_pg_sql_h