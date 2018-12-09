/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#if !defined(_os_support_h)
#define _os_support_h
#ifndef _WINDOWS_
#include <windows.h>
#endif//!_WINDOWS_
#ifndef _IOSTREAM_
#include <iostream>
#endif//!_IOSTREAM_
#include <stdio.h>
class os_support {
public:
#if defined(WIN32) || defined(_WIN64)
	static HMODULE load_library(LPCWSTR name);
#else
	static HMODULE load_library(LPCWSTR * name);
	static void* load_library(const char *name);
#endif
	static char* load_library_error();
#if defined(WIN32) || defined(_WIN64)
	static void* get_proc_address(HMODULE module, const char *name);
#else
	static void* get_proc_address(void* module, const char *name);
#endif
	// Get the full path of the loaded library
#if defined(WIN32) || defined(_WIN64)
	static void get_module_file_name(HMODULE module, char *out_path, int len);
#else
	static void get_module_file_name(void* module, char *out_path, int len);
#endif
	// Sleep the specified number of seconds
	static void sleep (unsigned int sec);
	// Get error message of the last system error
	static void get_last_error_text(const char *prefix, char *output, int len);
	static size_t get_tick_count();
};
#endif
