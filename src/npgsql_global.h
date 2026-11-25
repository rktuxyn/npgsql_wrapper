//5:27 PM 11/19/2018
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
#if !defined(_npgsql_global_h)
#	define _npgsql_global_h
#	define SUCCESS 1
#	include <iostream>
#	include <stdio.h>
#	include <list>
#	include <map>
#	include <stdlib.h>
#	include <regex>
#	include <cstdlib>
#	include <vector>
#if !defined(NPGSQL_EXPORTS)
#	define NPGSQL_EXPORTS
#endif//!NPGSQL_EXPORTS
#ifdef NPGSQL_EXPORTS
#	define NPGSQL_API __declspec(dllexport)
#else
#	define NPGSQL_API __declspec(dllimport)
#endif//!NPGSQL_EXPORTS
#endif//!_npgsql_global_h

