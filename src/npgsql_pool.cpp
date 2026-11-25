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
#	include "npgsql_pool.h"

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
		return connection_state::CLOSED;
	}
	return connection_state::OPEN;
}

connection_state npgsql_pool::connect(pg_connection_info* conn){
	if (this->_conn != NULL) {
		this->_conn->exit_all();
		delete this->_conn; this->_conn = NULL;
	}
	int rec = this->_conn->connect(conn);
	if (rec < 0) {
		this->panic(this->_conn->get_last_error(), -1);
		return connection_state::CLOSED;
	}
	return connection_state::OPEN;
}

int npgsql_pool::execute_scalar_x(const char* query, std::list<std::string>& out_param_array, std::map<std::string, char*>& out_param_map){
	if (state() == connection_state::CLOSED) {
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
	if (state() == connection_state::CLOSED) {
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
	return _conn == NULL ? connection_state::CLOSED : _conn->conn_state();
}

void npgsql_pool::exit_all(){
	if (state() == connection_state::CLOSED)return;
	_conn->exit_all();
}

void npgsql_pool::close_all_connection(){
	if (state() == connection_state::CLOSED)return;
	_conn->close_all_connection();
}
void npgsql_pool::panic(const char* error, int code = -1){
	free(_internal_error);
	_internal_error = new char[strlen(error) + 1];
	strcpy(_internal_error, error);
	_errc = code;
}
