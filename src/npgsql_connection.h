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
#if !defined(_npgsql_connection_h)
#pragma warning (disable : 4231)
#pragma warning(disable : 4996)
#	define _npgsql_connection_h
#	include <libpq-fe.h>
#	include "connection_state.h"
#	include <string>
#	include "npgsql_config.h"
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
	npgsql_connection(const npgsql_connection&) = delete;
	npgsql_connection& operator=(const npgsql_connection&) = delete;
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