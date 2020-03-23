/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:30 AM 11/22/2018
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_tools_h)
#	define _npgsql_tools_h
#	include "npgsql_global.h"
#	include <sstream>
#pragma warning (disable : 4231)
NPGSQL_API void json_array_stringify_s(std::vector<char*>& json_array_obj, std::string& json_str);
NPGSQL_API void json_array_stringify(std::list<std::map<std::string, std::string>>& json_obj, std::string&json_str);
NPGSQL_API void json_array_stringify_char(std::list<std::map<std::string, char*>>& json_obj, std::string&json_str);
NPGSQL_API void json_array_stringify_cchar(std::list<std::map<std::string, const char*>>& json_obj, std::string&json_str);
NPGSQL_API void json_obj_stringify(std::map<std::string, std::string>& json_obj, std::string&json_str);
NPGSQL_API void json_obj_stringify_char(std::map<std::string, char*>& json_obj, std::string&json_str);
NPGSQL_API void json_obj_stringify_cchar(std::map<std::string, const char*>& json_obj, std::string&json_str);
#endif//!_npgsql_tools_h