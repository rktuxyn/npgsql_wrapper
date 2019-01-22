/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_npgsql_result_h)
#define _npgsql_result_h
#pragma warning (disable : 4231)
typedef struct {
	int ret_val;
	const char* ret_msg;
	char* ret_data_table;
}npgsql_result;
#endif//!_global_h