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
#	include "os_support.h"
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
HMODULE os_support::load_library(LPCWSTR * name) {
	return HMODULE();
};
void* os_support::load_library(const char *name) {
	return dlopen(name, RTLD_NOW);
};
void os_support::get_module_file_name(void* module, char *out_path, int len) {
};
void* os_support::get_proc_address(void* module, const char *name) {
	return dlsym(module, name);
};
#else
HMODULE os_support::load_library(LPCWSTR name) {
	//return ::LoadLibrary(name);
	return LoadLibraryEx(name, NULL, 0x00000008);
}
void* os_support::get_proc_address(HMODULE module, const char *name) {
	return ::GetProcAddress(module, name);
}
void os_support::get_module_file_name(HMODULE module, char *out_path, int len) {
	::GetModuleFileName(module, (LPWSTR)out_path, (DWORD)len);
}
#endif
char* os_support::load_library_error() {
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	return dlerror(); 
#else
	return NULL;
#endif
};
// Sleep the specified number of seconds
void os_support::sleep (unsigned int sec) {
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	sleep(sec);
#else
	::Sleep(sec * 1000); 
#endif
};
// Get error message of the last system error
void os_support::get_last_error_text(const char *prefix, char *output, int len) {
	if (output == NULL || len <= 0)
		return;
	char error[2048];
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	return;
#else
	DWORD rc = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
		1033, (LPWSTR)error, 1024, NULL);
	// No error found
	if (rc == 0) {
		*output = '\x0';
		return;
	}
	// On Windows OS error messages terminated with 0D 0A 00, remove new lines
	if (rc > 2 && error[rc - 2] == '\r')
		error[rc - 2] = '\x0';
	//
	if (((prefix != NULL) && (prefix[0] == '\0')) || prefix == NULL) {
	//if (prefix != NULL) {
		*output = '\x0';
	}
	else {
		size_t len = strlen(prefix) + 1;
		output = new char[len];
		strcpy_s(output, len + 1, prefix);
	}
	strcat_s(output, sizeof output, error);
#endif
};
// Get current time in milliseconds
size_t os_support::get_tick_count() {
	return (size_t)::GetTickCount64();
	//return ::GetTickCount();
};