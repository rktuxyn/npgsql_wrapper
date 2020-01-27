/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//8:44 AM 11/19/2018 Start
//5:27 PM 11/19/2018
#	include "npgsql.h"
void npgsql::create_instance() {
	if (_pgsql == NULL) { 
		_pgsql = new pg_sql(); 
		_is_disposed = 0;
	}
}
npgsql::npgsql(const char * absolute_path) {
	conn_state = connection_state::CLOSED;
	create_instance();
	//malloc(sizeof _pgsql);
	_internal_error = new char;
	_conn = NULL; _is_disposed = 0;
	_is_disposed = 0;
}

npgsql::npgsql() {
	conn_state = connection_state::CLOSED;
	_pgsql = new pg_sql();
	_internal_error = new char;
	_conn = NULL; _is_disposed = 0;
}
void npgsql::release() {
	if (_is_disposed != 0)return;
	if (_pgsql == NULL)return;
	_is_disposed = 1;
	if (conn_state == connection_state::OPEN) {
		close();
	};
	delete _pgsql; _pgsql = NULL; delete _internal_error;
	if (_conn != NULL) {
		_conn->clear(); delete _conn; _conn = NULL;
	}
	return;
}
npgsql::~npgsql() {
	release();
	return;
}

const char * npgsql::get_last_error() {
	if (_pgsql->is_api_error() < 0) {
		if (((_internal_error != NULL) && (_internal_error[0] == '\0')) || _internal_error == NULL) {
			return "No Error Found!!!";
		}
		return const_cast<const char*>(_internal_error);
	}
	return _pgsql->get_last_error();
}
int npgsql::connect() {
	if (conn_state == connection_state::OPEN) {
		this->close();
	}
	if (_conn != NULL && !_conn->empty()) {
		create_instance();
		_pgsql->exit_nicely();
		int rec = _pgsql->connect(_conn->c_str());
		if (rec < 0)return rec;
		conn_state = connection_state::OPEN;
		return rec;
	}
	panic("No connection string found...");
	return -1;
}
int npgsql::connect(const char* conn) {
	if (conn_state == connection_state::OPEN) {
		this->close();
	}
	if (_conn != NULL && !_conn->empty()) {
		_conn->clear(); delete _conn; _conn = NULL;
	}
	create_instance();
	int rec = _pgsql->connect(conn);
	if (rec < 0)return rec;
	_conn = new std::string(conn);
	conn_state = connection_state::OPEN;
	return rec;
}

void npgsql::panic(const char * error) {
	free(_internal_error);
	_internal_error = new char[strlen(error) + 1];
	strcpy(_internal_error, error);
}
int npgsql::check_con_status() {
	if (conn_state != connection_state::OPEN) {
		panic("Connection not open!!!");
		return -1;
	};
	return 1;
}
int npgsql::execute_scalar(const char * query, char* result) {
	if (check_con_status() < 0)return -1;
	panic("Not Implemented!!!");
	return-1;
}
/*void __inline_str(std::string&out) {
	out = std::regex_replace (out, std::regex("\\n\\s*\\n"), "\r\n");
	out = std::regex_replace (out, std::regex("\\r\\n\\s*\\r\\n"), "\n");
}*/
int npgsql::execute_scalar(const char * sp, std::list<npgsql_params*>& sql_param, std::map<std::string, char*>& result) {
	if (check_con_status() < 0)return -1;
	std::string* out_param = new std::string("");
	std::string* in_param = new std::string("");
	int in_param_count = 0;
	int out_param_count = 0;
	std::list<std::string>*out_param_array = new std::list<std::string>();
	for (auto s = sql_param.begin(); s != sql_param.end(); ++s) {
		npgsql_params* param = *s;
		if (param->direction == parameter_direction::Input || param->direction == parameter_direction::InputOutput) {
			std::string val = param->value;
			//quote_literal(val);
			if (in_param_count == 0) {
				in_param_count++;
				in_param->append("( ");
				in_param->append(val);
				in_param->append("::");
				in_param->append(get_db_type(param->db_type));
			}
			else {
				in_param_count++;
				in_param->append(", ");
				in_param->append(val);
				in_param->append("::");
				in_param->append(get_db_type(param->db_type));
			}
			if (param->direction != parameter_direction::InputOutput)
				continue;
		}
		if (param->direction == parameter_direction::Output || param->direction == parameter_direction::InputOutput) {
			result[param->parameter_name] = '\0';
			out_param_array->push_back(param->parameter_name);
			if (out_param_count == 0) {
				out_param->append("f.");
				out_param->append(param->parameter_name);
				out_param_count++;
				continue;
			}
			out_param->append(", f.");
			out_param->append(param->parameter_name);
			out_param_count++;
			continue;
		}
	}
	std::string* query = new std::string("select ");
	if (out_param_count <= 0) {
		query->append("f.* from ");
	}
	else {
		query->append(out_param->c_str());
		query->append(" from ");
	}
	if (in_param_count <= 0) {
		query->append(sp);
		query->append("(");
	}
	else {
		query->append(sp);
		query->append(in_param->c_str());
	}
	query->append(") as f ");
	//__inline_str(*query);
	//std::cout << query->c_str();
	int ret = _pgsql->execute_scalar_x(query->c_str(), *out_param_array, result);
	out_param->clear(); delete out_param;
	in_param->clear(); delete in_param;
	query->clear(); out_param_array->clear();
	delete query; delete out_param_array;
	return ret;
}
int npgsql::execute_io(const char * sp, const char * ctx, const char * form_data, std::map<std::string, char*>& result) {
	if (check_con_status() < 0) {
		return -1;
	};
	std::string* query = new std::string("select ");
	query->append("f.* from ");
	query->append(sp);
	query->append("(");
	std::string* val = new std::string(ctx);
	quote_literal(*val);
	query->append(val->c_str());
	val->clear(); delete val;
	query->append("::");
	query->append(get_db_type(npgsql_db_type::Jsonb));
	query->append(",");
	val = new std::string(form_data);
	quote_literal(*val);
	query->append(val->c_str());
	val->clear(); delete val;
	query->append("::");
	query->append(get_db_type(npgsql_db_type::Jsonb));
	query->append(") as f ");

	std::list<std::string>* out_param_array= new std::list<std::string>();
	out_param_array->push_back("_ret_data_table");
	out_param_array->push_back("_ret_val");
	out_param_array->push_back("_ret_msg");
	int ret = _pgsql->execute_scalar_x(query->c_str(), *out_param_array, result);
	//std::cout << query->c_str() << "\r\n";
	out_param_array->clear(); delete out_param_array;
	query->clear(); delete query;
	//for (auto itr = result.begin(); itr != result.end(); ++itr) {
	//	std::cout << itr->first.c_str();
	//	std::cout << const_cast<const char*>(itr->second);
	//}
	return ret;
}

int npgsql::execute_non_query(const char * query) {
	if (check_con_status() < 0)return -1;
	return _pgsql->execute_non_query(query);
}
const char* npgsql::execute_query(const char * query, int&rec) {
	rec = -1;
	if (check_con_status() < 0)return '\0';
	return _pgsql->execute_query(query, rec);
}
int npgsql::close() {
	if (conn_state != connection_state::OPEN)return 0;
	_pgsql->exit_nicely();
	conn_state = connection_state::CLOSED;
	return 0;
}
