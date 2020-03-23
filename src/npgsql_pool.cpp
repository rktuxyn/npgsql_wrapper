/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#	include "npgsql_pool.h"
#	include "npgsql_query.h"
npgsql_pool::npgsql_pool(){
	_errc = 0; _conn = new npgsql_connection();
	_internal_error = new char;
	_conn_info = NULL;
}

npgsql_pool::~npgsql_pool(){
	if (_conn == NULL)return;
	if (_internal_error != NULL) {
		free(_internal_error); _internal_error = NULL;
	}
	_conn->exit_all();
	delete _conn; _conn = NULL;
	_conn_info = NULL;
}

connection_state npgsql_pool::connect(const char* conn){
	if (this->_conn != NULL) {
		this->_conn->exit_all();
		delete this->_conn; this->_conn = NULL;
	}
	int rec = this->_conn->connect(conn);
	if (rec < 0) {
		this->panic(this->_conn->get_last_error(), -1);
		return CLOSED;
	}
	return OPEN;
}

connection_state npgsql_pool::connect(pg_connection_info* conn){
	if (this->_conn != NULL) {
		this->_conn->exit_all();
		delete this->_conn; this->_conn = NULL;
	}
	int rec = this->_conn->connect(conn);
	if (rec < 0) {
		this->panic(this->_conn->get_last_error(), -1);
		return CLOSED;
	}
	return OPEN;
}

int npgsql_pool::execute_scalar_x(const char* query, std::list<std::string>& out_param_array, std::map<std::string, char*>& out_param_map){
	if (state() == CLOSED) {
		this->panic("No active connectio found...", -1);
		return _errc;
	}
	pg_connection_pool* cpool = this->_conn->create_connection_pool();
	if (cpool->error_code < 0) {
		this->panic(cpool->error_msg, cpool->error_code);
		this->_conn->exit_nicely(cpool);
		return -1;
	}
	npgsql_query* pg_query = new npgsql_query(cpool);
	int rec = pg_query->execute_scalar_x(query, out_param_array, out_param_map);
	if (rec < 0) {
		this->panic(pg_query->get_last_error(), -1);
	}
	pg_query->free_connection();
	delete pg_query;
	return rec;
}

int npgsql_pool::execute_non_query(const char* query){
	if (state() == CLOSED) {
		this->panic("No active connectio found...", -1);
		return _errc;
	}
	pg_connection_pool* cpool = this->_conn->create_connection_pool();
	if (cpool->error_code < 0) {
		this->panic(cpool->error_msg, cpool->error_code);
		this->_conn->exit_nicely(cpool);
		return -1;
	}
	npgsql_query* pg_query = new npgsql_query(cpool);
	int rec = pg_query->execute_non_query(query);
	if (rec < 0) {
		this->panic(pg_query->get_last_error(), -1);
	}
	pg_query->free_connection();
	delete pg_query;
	return rec;
}

const char* npgsql_pool::get_last_error(){
	if (_errc >= 0) return "No Error Found!!!";
	return const_cast<const char*>(_internal_error);
}

connection_state npgsql_pool::state(){
	return _conn == NULL ? CLOSED : _conn->conn_state();
}

void npgsql_pool::exit_all(){
	if (state() == CLOSED)return;
	_conn->exit_all();
}

void npgsql_pool::close_all_connection(){
	if (state() == CLOSED)return;
	_conn->close_all_connection();
}
void npgsql_pool::panic(const char* error, int code = -1){
	free(_internal_error);
	_internal_error = new char[strlen(error) + 1];
	strcpy(_internal_error, error);
	_errc = code;
}
