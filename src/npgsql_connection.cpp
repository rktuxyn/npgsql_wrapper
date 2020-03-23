/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#	include "npgsql_connection.h"
#	include "npgsql_global.h"
#	include <postgres_ext.h>
#	include <regex>
#if !defined(FALSE)
#	define FALSE               0
#endif//!FALSE

#if !defined(TRUE)
#	define TRUE                1
#endif//!FALSE
/*Delete execution result*/
void clear_response(PGconn* conn) {
	PGresult* res;
	while ((res = PQgetResult(conn))) {
		PQclear(res);
	}
}
npgsql_connection::npgsql_connection() {
	_active_pools = NULL;
	_internal_error = NULL;
	_errc = FALSE; _conn_info = NULL;
	_conn_state = CLOSED;
}
void npgsql_connection::clear_conn_info() {
	if (_conn_info != NULL) {
		_free_obj(_conn_info->server);
		_free_obj(_conn_info->database);
		_free_obj(_conn_info->user);
		_free_obj(_conn_info->pwd);
		_free_obj(_conn_info->port);
		delete _conn_info;
		_conn_info = NULL;
	}
}
npgsql_connection::~npgsql_connection(){
	clear_conn_info();
	this->exit_all();
}

int npgsql_connection::connect(pg_connection_info* conn){
	if (_conn_state == OPEN) {
		close_all_connection(); this->clear_conn_info();
	}
	this->_conn_info = conn;
	if (validate_cinfo( ) < 0) {
		return _errc;
	}
	pg_connection_pool* cpool = create_connection_pool();/** open one connection*/
	if (cpool->conn_state != OPEN) {
		return -1;
	}
	if (_errc < 0)return _errc;
	free_connection_pool(cpool);
	return _errc;
}

int npgsql_connection::connect(const char* conn){
	if (this->parse_connection_string(conn) < 0) {
		return _errc;
	}
	return this->connect();
}

int npgsql_connection::connect(){
	if (this->_conn_info == NULL) {
		throw new std::exception("Connection info required...");
	}
	if (validate_cinfo() < 0) {
		return _errc;
	}
	close_all_connection();
	pg_connection_pool* cpool = create_connection_pool();/** open one connection*/
	if (cpool->conn_state != OPEN) {
		return -1;
	}
	if (_errc < 0)return _errc;
	free_connection_pool(cpool);
	return _errc;
}

pg_connection_pool* npgsql_connection::create_connection_pool() {
	pg_connection_pool* cpool = NULL;
	if (_active_pools != NULL) {
		for (cpool = _active_pools; cpool; cpool = cpool->next) {
			if (cpool->busy < 0 || !cpool->busy) break;
		}
		if (cpool && cpool->busy) {
			cpool = NULL;
		}
	}
	if (cpool == NULL || cpool->busy < 0 || cpool->conn_state == CLOSED) {
		if (cpool == NULL) {
			cpool = new pg_connection_pool;
		}
		cpool->conn = PQsetdbLogin(
			/*const char *pghost*/_conn_info->server->c_str(),
			/*const char *pgport*/_conn_info->port->c_str(),
			/*const char *pgoptions*/NULL,
			/*const char *pgtty*/NULL,
			/*const char *dbName*/_conn_info->database->c_str(),
			/*const char *login*/_conn_info->user->c_str(),
			/*const char *pwd*/_conn_info->pwd->c_str()
		);
		if (PQstatus(cpool->conn) != CONNECTION_OK) {
			cpool->error_code = -2;
			panic(PQerrorMessage(cpool->conn));
			cpool->error_msg = get_last_error();
			cpool->conn_state = CLOSED;
		}
		else {
			cpool->conn_state = OPEN;
		}
		if (cpool->conn_state == OPEN) {
			cpool->error_code = 0;
			cpool->error_msg = NULL;
			cpool->busy = -1;
		}
		else {
			cpool->busy = 1;
		}
		cpool->next = _active_pools;
		_active_pools = cpool;
		_conn_state = OPEN;
		return cpool;
	}
	else {
		cpool->busy++;
	}
	return cpool;
}

void npgsql_connection::free_connection_pool(pg_connection_pool* cpool){
	cpool->busy = 0;
	cpool->error_code = 0;
	cpool->error_msg = NULL;
}

void npgsql_connection::exit_all(){
	if (_internal_error != NULL) {
		delete[]_internal_error; _internal_error = NULL;
	}
	close_all_connection(); clear_conn_info();
}
void npgsql_connection::exit_nicely(pg_connection_pool* cpool) {
	if (cpool == NULL || cpool->conn == NULL)return;
	clear_response(cpool->conn);
	PQfinish(cpool->conn); cpool->conn = NULL;
	cpool->busy = -1;
	cpool->conn_state = CLOSED;
	cpool->error_code = 0;
	cpool->error_msg = NULL;
}

void npgsql_connection::close_all_connection(){
	if (_conn_state == CLOSED)return;
	if (_active_pools == NULL)return;
	pg_connection_pool* cpool;
	for (cpool = _active_pools; cpool; cpool = cpool->next) {
		if (cpool->busy < 0)continue;
		clear_response(cpool->conn);
		PQfinish(cpool->conn);
	}
	while (_active_pools) {
		cpool = _active_pools;
		_active_pools = _active_pools->next;
		if (cpool->busy) {
			//fprintf(stderr,"destroying Database object before Connect object(s)\n");
		}
		cpool->conn = NULL;
		cpool->busy = -1;
		cpool->error_msg = NULL;
		delete cpool;
	}
	_conn_state = CLOSED;
	if (_active_pools != NULL)
		delete _active_pools;
	_active_pools = NULL;
}

const char* npgsql_connection::get_last_error(){
	if (_errc >= 0) return "No Error Found!!!";
	return const_cast<const char*>(_internal_error);
}

connection_state npgsql_connection::conn_state(){
	return _conn_state;
}
//constexpr const char*
#define _conn_user_error	"No user defined (e.g. postgress) in given connection string as `UserId`!!!"
#define _conn_pwd_error		"No password defined (e.g. 123456) in given connection string as `Password`!!!"
#define _conn_db_error		"No database defined (e.g. sow) in given connection string as `Database`!!!"
#define _conn_server_error	"No server defined (e.g. localhost) in given connection string as `Password`!!!"
#define _conn_port_error	"No port defined (e.g. 5432) in given connection string as `Port`!!!"

int npgsql_connection::parse_connection_string(const char* conn){
	if (conn == NULL || strlen(conn) == 0) {
		panic("No connection string found!!!");
		return -1;
	}
	std::string* query = new std::string(conn);
	std::regex pattern("([\\w+%]+)=([^;]*)");
	std::map<std::string, std::string> conn_obj;
	auto words_begin = std::sregex_iterator(query->begin(), query->end(), pattern);
	auto words_end = std::sregex_iterator();
	for (std::sregex_iterator i = words_begin; i != words_end; i++) {
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		if (conn_obj.find(key) != conn_obj.end()) {
			key = "Duplicate key found in connection string	==> `" + key + "`!!!";
			panic(key.c_str());
			_free_obj(query);
			goto _ERROR;
		}
		conn_obj[key] = value;
	}
	_free_obj(query);
	if (_conn_info == NULL) {
		_conn_info = new pg_connection_info();
	}
	_conn_info->user = new std::string(conn_obj["UserId"]);
	if (_conn_info->user->empty()) {
		panic(_conn_user_error);
		goto _ERROR;
	}
	_conn_info->pwd = new std::string(conn_obj["Password"]);
	if (_conn_info->pwd->empty()) {
		panic(_conn_pwd_error);
		goto _ERROR;
	}
	_conn_info->database = new std::string(conn_obj["Database"]);
	if (_conn_info->database->empty()) {
		panic(_conn_db_error);
		goto _ERROR;
	}
	_conn_info->server = new std::string(conn_obj["Server"]);
	if (_conn_info->server->empty()) {
		panic(_conn_server_error);
		goto _ERROR;
	}
	_conn_info->port = new std::string(conn_obj["Port"]);
	if (_conn_info->port->empty()) {
		panic(_conn_port_error);
		goto _ERROR;
	}
	goto _END;
_ERROR:
	conn_obj.clear();
	return _errc;
_END:
	conn_obj.clear();
	return 1;
}

int npgsql_connection::validate_cinfo(){
	if (_conn_info == NULL) {
		panic("No connection info found!!!");
		return _errc;
	}
	if (_conn_info->user == NULL || _conn_info->user->empty()) {
		panic(_conn_user_error);
		return _errc;
	}
	if (_conn_info->pwd->empty()) {
		panic(_conn_pwd_error);
		return _errc;
	}
	if (_conn_info->database->empty()) {
		panic(_conn_db_error);
		return _errc;
	}
	if (_conn_info->server->empty()) {
		panic(_conn_server_error);
		return _errc;
	}
	if (_conn_info->port->empty()) {
		panic(_conn_port_error);
		return _errc;
	}
	return 1;
}

void npgsql_connection::panic(const char* error){
	if (_internal_error != NULL) {
		delete[]_internal_error; _internal_error = NULL;
	}
	size_t len = strlen(error);
	_internal_error = new char[len + sizeof(char)];
	strcpy_s(_internal_error, len, error);
	_internal_error[len] = '\000';
	_errc = -1;
}

void npgsql_connection::panic(char* erro_msg){
	if (_internal_error != NULL) {
		delete[]_internal_error; _internal_error = NULL;
	}
	size_t len = strlen(erro_msg);
	_internal_error = new char[len + sizeof(char)];
	strcpy_s(_internal_error, len, erro_msg);
	_internal_error[len] = '\000';
	_errc = -1;
}
