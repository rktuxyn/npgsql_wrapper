/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_result_h)
#	define _npgsql_result_h
typedef struct {
	int ret_val;
	const char* ret_msg;
	char* ret_data_table;
}npgsql_result;
#endif//!_global_h