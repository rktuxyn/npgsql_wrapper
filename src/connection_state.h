/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#pragma once
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_connection_state_h)
#define _connection_state_h
#pragma warning (disable : 4231)
typedef NPGSQL_API enum {
	OPEN,	
	CLOSED
} connection_state;
#endif//!_parameter_direction_h