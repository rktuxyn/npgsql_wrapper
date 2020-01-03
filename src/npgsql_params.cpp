/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "npgsql_params.h"
npgsql_params::npgsql_params(
	const char * name, 
	npgsql_db_type dtype,
	parameter_direction pd
) {
	this->parameter_name = name;
	this->db_type = dtype;
	this->direction = pd;
	this->value = NULL;
}

npgsql_params::npgsql_params(
	const char * name,
	npgsql_db_type dtype,
	parameter_direction pd,
	const char * data
) {
	this->parameter_name = name;
	this->db_type = dtype;
	this->direction = pd;
	this->value = data;
}
npgsql_params::~npgsql_params() {
	this->parameter_name = NULL;
	this->value = NULL;
}
