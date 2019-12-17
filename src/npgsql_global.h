//5:27 PM 11/19/2018
/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_global_h)
#define _npgsql_global_h
#define SUCCESS 1
#if !defined(_IOSTREAM_)
#include <iostream>
#endif//!_IOSTREAM_
#if !defined(_INC_STDIO)
#include <stdio.h>  /* defines FILENAME_MAX */
#endif//!_INC_STDIO
#ifndef _LIST_
#include <list>
#endif // !_LIST_
#ifndef _MAP_
#include <map>
#endif // !_MAP_
#ifndef _INC_STDLIB
#include <stdlib.h>
#endif // !_INC_STDLIB
#ifndef _REGEX_
#include <regex>
#endif// !_REGEX_
#include <cstdlib>
#if !defined(_VECTOR_)
#include <vector>
#endif//!_VECTOR_
#if !defined(NPGSQL_EXPORTS)
#define NPGSQL_EXPORTS
#endif//!NPGSQL_EXPORTS
#ifdef NPGSQL_EXPORTS
#define NPGSQL_API __declspec(dllexport)
#else
#define NPGSQL_API __declspec(dllimport)
#endif//!NPGSQL_EXPORTS
#endif//!_npgsql_global_h

