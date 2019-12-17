#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_npgsql_db_type_h)
#include "npgsql_db_type.h"
#endif//!_npgsql_db_type_h
#if !defined(_npgsql_param_type_h)
#define _npgsql_param_type_h
#pragma warning (disable : 4231)
class NPGSQL_API npgsql_param_type {
public:
	const char* value;
	npgsql_db_type db_type;
	npgsql_param_type(const char* v, npgsql_db_type dt) {
		value = v; db_type = dt;
	};
	~npgsql_param_type() {};
};
#endif//!_global_h