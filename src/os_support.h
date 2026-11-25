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
#if !defined(_os_support_h)
#	define _os_support_h
#if defined(WIN32) || defined(_WIN64)
#	include <windows.h>
#endif//!WIN32
#	include <iostream>
#	include <stdio.h>
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
