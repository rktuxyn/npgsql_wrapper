/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//11:42 AM 2/19/2020
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_config_h)
#	define _npgsql_config_h
#if defined(NPGSQL_EXPORTS)
#	define NPGSQL_API __declspec(dllexport)
#else
#	define NPGSQL_API __declspec(dllimport)
#endif//!NPGSQL_EXPORTS
#if !defined(_free_obj)
#	define _free_obj(obj)\
while(obj){\
	obj->clear();delete obj;obj = NULL;\
}
#endif//!_free_obj
#	define SUCCESS 1
#endif//!_npgsql_config_h