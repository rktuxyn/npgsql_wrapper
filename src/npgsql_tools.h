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
NPGSQL_API void json_parse(const char* json_str, std::list<std::map<std::string, std::string>>&json_obj);
NPGSQL_API void json_array_stringify(std::list<std::map<std::string, std::string>>& json_obj, std::string&json_str);
NPGSQL_API void json_array_stringify_char(std::list<std::map<std::string, char*>>& json_obj, std::string&json_str);
NPGSQL_API void json_array_stringify_cchar(std::list<std::map<std::string, const char*>>& json_obj, std::string&json_str);
NPGSQL_API void json_obj_stringify(std::map<std::string, std::string>& json_obj, std::string&json_str);
NPGSQL_API void json_obj_stringify_char(std::map<std::string, char*>& json_obj, std::string&json_str);
NPGSQL_API void json_obj_stringify_cchar(std::map<std::string, const char*>& json_obj, std::string&json_str);
#endif//!_npgsql_tools_h