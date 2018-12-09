/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "os_support.h"
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
};
void* os_support::get_proc_address(HMODULE module, const char *name) {
	return ::GetProcAddress(module, name);
};
void os_support::get_module_file_name(HMODULE module, char *out_path, int len) {
	::GetModuleFileName(module, (LPWSTR)out_path, (DWORD)len);
};
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
	char error[1024];
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

	if (prefix != NULL) {
		output = new char[strlen(prefix) + 1];
		strcpy_s(output, sizeof output, prefix);
	}
	else
		*output = '\x0';
	strcat_s(output, sizeof output, error);
#endif
};
// Get current time in milliseconds
size_t os_support::get_tick_count() {
	return ::GetTickCount();
};