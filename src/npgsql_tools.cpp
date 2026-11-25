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
#	include "npgsql_tools.h"
void json_array_stringify_s(std::vector<char*>& json_array_obj, std::string & json_str) {
	std::stringstream*ss = new std::stringstream();
	std::stringstream& copy = *ss;
	copy << "[";
	for (size_t i = 0, l = json_array_obj.size(); i < l; ++i) {
		if (i != 0)
			copy << ",";
		copy << "\"" << json_array_obj[i] << "\"";
	}
	copy << "]";
	json_str = ss->str();
	delete ss;
	return;
}
void json_parse(const char * json_str, std::list<std::map<std::string, std::string>>& json_obj) {
#if defined(_WIN64)
	//
#endif//!_WIN64
	std::regex pattern("([\\w+%]+):([^,]*)");
}
void json_array_stringify(std::list<std::map<std::string, std::string>>& json_obj, std::string& json_str) {
	json_str = "[";
	for (auto s = json_obj.begin(); s != json_obj.end(); ++s) {
		json_str += "{";
		auto _map = *s;
		bool is_first = true;
		for (auto itr = _map.begin(); itr != _map.end(); ++itr) {
			if (is_first) { 
				is_first = false;
				json_str += itr->first;
			}
			else {
				json_str += ",";
				json_str += itr->first;
			}
			json_str += ":";
			json_str += itr->second;
		}
		json_str += "}";
	}
	json_str += "]";
	return;
}
void json_array_stringify_char(std::list<std::map<std::string, char*>>& json_obj, std::string & json_str) {
	json_str = "[";
	for (auto s = json_obj.begin(); s != json_obj.end(); ++s) {
		json_str += "{";
		auto _map = *s;
		bool is_first = true;
		for (auto itr = _map.begin(); itr != _map.end(); ++itr) {
			if (is_first) {
				is_first = false;
				json_str += itr->first;
			}
			else {
				json_str += ",";
				json_str += itr->first;
			}
			json_str += ":";
			json_str.append(itr->second);
		}
		json_str += "}";
	}
	json_str += "]";
	return;
}
void json_array_stringify_cchar(std::list<std::map<std::string, const char*>>& json_obj, std::string & json_str) {
	json_str = "[";
	for (auto s = json_obj.begin(); s != json_obj.end(); ++s) {
		json_str += "{";
		auto _map = *s;
		bool is_first = true;
		for (auto itr = _map.begin(); itr != _map.end(); ++itr) {
			if (is_first) {
				is_first = false;
				json_str += itr->first;
			}
			else {
				json_str += ",";
				json_str += itr->first;
			}
			json_str += ":";
			json_str.append("\"");
			json_str.append(itr->second);
			json_str.append("\"");
		}
		json_str += "}";
	}
	json_str += "]";
	return;
}
void json_obj_stringify(std::map<std::string, std::string>& json_obj, std::string & json_str) {
	json_str += "{";
	bool is_first = true;
	for (auto itr = json_obj.begin(); itr != json_obj.end(); ++itr) {
		if (is_first) {
			is_first = false;
			json_str += itr->first;
		}
		else {
			json_str += ",";
			json_str += itr->first;
		}
		json_str += ":";
		json_str.append(itr->second);
	}
	json_str += "}";
	return;
}
void json_obj_stringify_char(std::map<std::string, char*>& json_obj, std::string & json_str) {
	json_str += "{";
	bool is_first = true;
	for (auto itr = json_obj.begin(); itr != json_obj.end(); ++itr) {
		if (is_first) {
			is_first = false;
			json_str += itr->first;
		}
		else {
			json_str += ",";
			json_str += itr->first;
		}
		json_str += ":";
		json_str.append(itr->second);
	}
	json_str += "}";
	return;
}
void json_obj_stringify_cchar(std::map<std::string, const char*>& json_obj, std::string & json_str) {
	json_str += "{";
	bool is_first = true;
	for (auto itr = json_obj.begin(); itr != json_obj.end(); ++itr) {
		//std::cout << itr->first << "<br/>";
		if (is_first) {
			is_first = false;
			json_str += itr->first;
		}
		else {
			json_str += ",";
			json_str += itr->first;
		}
		json_str += ":";
		//json_str.append("\"");
		json_str.append(itr->second);
		//json_str.append("\"");
	}
	json_str += "}";
	//std::cout << json_str << "";
	return;
}