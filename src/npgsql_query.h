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
class NPGSQL_API npgsql_query {
public:
	npgsql_query(pg_connection_pool* cpool);
	~npgsql_query();
	void free_connection();
	int execute_scalar_x(const char* query, std::list< std::string>& out_param_array, std::map<std::string, char*>& out_param_map);
	int execute_non_query(const char* query);
	const char* get_last_error();
	void exit_nicely();
private:
	pg_connection_pool* _cpool;
	int _errc;
	char* _internal_error;
	void panic(const char* error);
	void panic(char* erro_msg);
};
#endif//!_npgsql_query_h