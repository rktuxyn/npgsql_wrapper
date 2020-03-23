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
#if !defined(_npgsql_pool_h)
#pragma warning (disable : 4231)
#pragma warning(disable : 4996)
#	define _npgsql_pool_h
#	include "npgsql_connection.h"
#	include <list>
#	include <string>
#	include <map>
class NPGSQL_API npgsql_pool {
public:
	explicit npgsql_pool();
	npgsql_pool(const npgsql_pool&) = delete;
	npgsql_pool& operator=(const npgsql_pool&) = delete;
	~npgsql_pool();
	connection_state connect(const char* conn);
	connection_state connect(pg_connection_info* conn_info);
	int execute_scalar_x(const char* query, std::list< std::string>& out_param_array, std::map<std::string, char*>& out_param_map);
	int execute_non_query(const char* query);
	const char* get_last_error();
	connection_state state();
	void exit_all();
	void close_all_connection();
private:
	const pg_connection_info* _conn_info;
	npgsql_connection* _conn;
	int _errc;
	char* _internal_error;
	void panic(const char* error, int code);
};
#endif//!_npgsql_pool_h
