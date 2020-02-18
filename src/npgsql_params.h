//5:27 PM 11/19/2018
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_params_h)
#	define _npgsql_params_h
#	include "npgsql_global.h"
#	include "parameter_direction.h"
#	include "npgsql_db_type.h"
#pragma warning (disable : 4231)
class NPGSQL_API npgsql_params {
public:
	npgsql_params(const char * name, npgsql_db_type dtype, parameter_direction pd, const char * data);
	npgsql_params(const char* name, npgsql_db_type dtype, parameter_direction pd);
	~npgsql_params();
public:
	const char* parameter_name;
	npgsql_db_type db_type;
	parameter_direction direction;
	const char* value;/* User value - use this for whatever. */
};
#endif//!_npgsql_params_h