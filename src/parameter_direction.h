//5:27 PM 11/19/2018
#pragma once
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_parameter_direction_h)
#define _parameter_direction_h
#pragma warning (disable : 4231)
typedef enum  {
	//
	// Summary:
	//     The parameter is an input parameter.
	Input = 1,
	//
	// Summary:
	//     The parameter is an output parameter.
	Output = 2,
	//
	// Summary:
	//     The parameter is capable of both input and output.
	InputOutput = 3,
	//
	// Summary:
	//     The parameter represents a return value from an operation such as a stored procedure,
	//     built-in function, or user-defined function.
	ReturnValue = 6
}parameter_direction;
#endif//!_parameter_direction_h