/**
* MIT License
*
* Copyright (c) 2022 FSys Tech Ltd.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
//5:32 PM 12/17/2019
#if !defined(_npgsql_connection_h)
#pragma warning (disable : 4231)
#pragma warning(disable : 4996)
#	define _npgsql_connection_h
#	include "npgsql_global.h"
#	include <libpq-fe.h>
#	include <postgres_ext.h>
#	include "connection_state.h"
#	include <regex>
#if !defined(_free_obj)
#	define _free_obj(obj)\
while(obj){\
	obj->clear();delete obj;obj = NULL;\
}
#endif//!_free_obj
typedef struct pg_conn_pool {
	struct pg_conn_pool* next;  /* pointer to next member*/
	PGconn* conn;              /* PgSQL connection handle*/
	connection_state conn_state;
	int busy;                   /* connection busy flag*/
	int error_code;
	const char* error_msg;
}pg_connection_pool;
typedef struct {
	std::string* user;
	std::string* pwd;
	std::string* database;
	std::string* server;
	std::string* port;
}pg_connection_info;
class NPGSQL_API npgsql_connection {
public:
	explicit npgsql_connection();
	~npgsql_connection();
	int connect(pg_connection_info* conn);
	int connect(const char* conn);
	int connect();
	pg_connection_pool* create_connection_pool();
	void free_connection_pool(pg_connection_pool* cpool);
	void exit_all();
	void close_all_connection();
	const char* get_last_error();
	connection_state conn_state();
	void exit_nicely(pg_connection_pool* cpool);
protected:
	int parse_connection_string(const char* conn);
	int validate_cinfo();
	void clear_conn_info();
	connection_state _conn_state;
	pg_connection_info* _conn_info;
	pg_connection_pool* _active_pools;
	int _errc;
	char* _internal_error;
	void panic(const char* error);
	void panic(char* erro_msg);
};
#endif//!_npgsql_connection_h