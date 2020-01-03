/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:16 AM 11/19/2018
//#pragma warning(suppress : 4996)
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
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
//libpq.lib
//libeay32.dll, libiconv-2.dll, libintl-8.dll, libpq.dll, ssleay32.dll, zlib1.dll
class pg_sql {
private:
	bool							_connected;
	PGconn*							_conn;
	int								_n_error;
	char*							_n_error_text;
	int								_pq_error;
	char*							_pq_error_text;
	int								_is_disposed;
public:
	pg_sql();
	~pg_sql();
	virtual const int is_api_error();
	virtual const char * get_last_error();
	// Connect to the database
	virtual int connect(const char *conn);
	// Close database connection
	virtual void exit_nicely();
	virtual int get_row_count(const char *object, char *value);
	virtual const char* execute_cmd(const char *command, int&ret);
	virtual const char* execute_query(const char *query, int&ret);
	template<class _func>
	int execute_scalar(const char *query, _func fn) {
		if (((query != NULL) && (query[0] == '\0')) || query == NULL) {
			panic("SQL Statement required!!!");
			return -1;
		}
		bool exists = false;
		PGresult *res = PQexec(_conn, query);
		ExecStatusType exs_type = PQresultStatus(res);
		if (exs_type == PGRES_FATAL_ERROR) goto _ERROR;
		if (exs_type == PGRES_NONFATAL_ERROR) goto _ERROR;
		if (exs_type == PGRES_TUPLES_OK) {
			exists = true;
			int nFields = PQnfields(res);
			size_t len = 0;
			for (int i = 0; i < PQntuples(res); i++) {
				std::vector<char*>*rows = new std::vector<char*>();
				for (int j = 0; j < nFields; j++) {
					char* c = PQgetvalue(res, i, j);
					len = strlen(c);
					char* copy = new char[len + 1];
					//strcpy_s(copy, len, c);
					strcpy(copy, c);
					rows->push_back(copy);
					//free(c);
				}
				fn(i, *rows);
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
		panic();
		goto _END;
	_END:
		/*Delete execution result*/
		PQclear(res);
		return (exists == true) ? 0 : -1;
	};
	virtual int execute_scalar_x(const char *query, std::list<std::string>&out_param_array, std::map<std::string, char*>&out_param_map);
	virtual int execute_non_query(const char *query);
private:
	virtual int parse_connection_string(const char *conn, std::string& user, std::string& pwd, std::string& server, std::string& port, std::string& db);
	virtual void clear_response();
	virtual void panic(const char* std_error_msg);
	virtual void panic();
};
//5:30 AM 11/19/2018 Connected
//7:55 AM 11/19/2018 END
#endif//!_pg_sql_h