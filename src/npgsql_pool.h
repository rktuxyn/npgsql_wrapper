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
#if !defined(_npgsql_pool_h)
#pragma warning (disable : 4231)
#pragma warning(disable : 4996)
#	define _npgsql_pool_h
#	include "npgsql_connection.h"
#	include "npgsql_query.h"
class NPGSQL_API npgsql_pool {
public:
	npgsql_pool();
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
