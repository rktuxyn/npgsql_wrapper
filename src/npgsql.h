////8:44 AM 11/19/2018 Start
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_npgsql_h)
#pragma warning (disable : 4231)
#define _npgsql_h
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
//5:27 PM 11/19/2018
#if !defined(_pg_sql_h)
#include "pg_sql.h"
#endif//!_global_h
#if !defined(_npgsql_db_type_h)
#include "npgsql_db_type.h"
#endif//!_npgsql_db_type_h
#if !defined(_parameter_direction_h)
#include "npgsql_params.h"
#endif//!_parameter_direction_h
#if !defined(_npgsql_param_type_h)
#include "npgsql_param_type.h"
#endif//!_npgsql_param_type_h
#if !defined(_npgsql_result_h)
#include "npgsql_result.h"
#endif//!_npgsql_result_h
#if !defined(_connection_state_h)
#include "connection_state.h"
#endif//!_connection_state_h
#ifndef _REGEX_
#include <regex>
#endif// !_REGEX_
//class npgsql;
//typedef struct pg_conn_pool {
//	struct pg_conn_pool* next;  /* pointer to next member*/
//	pg_sql* pgsql;              /* PgSQL connection handle*/
//	int busy;                   /* connection busy flag*/
//}pg_connection_pool;
class NPGSQL_API npgsql {
public:
	connection_state conn_state;
private:
	//pg_connection_pool _active_conn; //!TODO
	pg_sql* _pgsql;
	std::string* _conn;
	char* _internal_error;
	void panic(const char* error);
	int check_con_status();
protected:
	int _is_disposed;
	void create_instance();
public:
	npgsql(const char* lib_path);
	npgsql();
	~npgsql();
	void quote_literal(std::string&str) {
		str = "'" + str + "'";
		return;
	}
	virtual const char * get_last_error();
	virtual int connect(const char* conn);
	virtual int connect();
	virtual int execute_scalar(const char *query, char *result);
	virtual int execute_scalar(const char *sp, std::list<npgsql_params*>&sql_param, std::map<std::string, char*>& result);
	virtual int execute_io(const char *sp, const char *login_id, const char *form_data, std::map<std::string, char*>& result);
	// Execute the statement
	virtual int execute_non_query(const char *query);
	virtual const char* execute_query(const char * query, int&rec);
	template<class _func>
	int execute_scalar(const char *query, _func func);
	template<class _func>
	int execute_scalar(const char *query, std::list<npgsql_params*>&sql_param, _func func);
	template<class _func>
	int execute_scalar_l(const char *query, std::list<npgsql_param_type*>&sql_param, _func func);
	virtual int close();
	virtual void release();
};
#pragma warning (default : 4231)
template<class _func>
inline int npgsql::execute_scalar(const char * query, _func func) {
	return _pgsql->execute_scalar(query, func);
}

template<class _func>
inline int npgsql::execute_scalar( const char* query, std::list<npgsql_params*>& sql_param, _func func ) {
	if ( check_con_status() < 0 )return -1;
	std::string* param_stmt = new std::string( "" );
	int param_count = 0;
	for ( auto s = sql_param.begin(); s != sql_param.end(); ++s ) {
		npgsql_params* param = *s;
		if ( param->direction != parameter_direction::Input ) {
			throw new std::exception( "You should pass input param" );
		}
		std::string* val = new std::string( param->value );
		quote_literal( *val );
		if ( param_count == 0 ) {
			param_count++;
			param_stmt->append( " where " );
			param_stmt->append( param->parameter_name );
			param_stmt->append( "=" );
			param_stmt->append( val->c_str() );
			free( val );
			param_stmt->append( "::" );
			param_stmt->append( get_db_type( param->db_type ) );
			continue;
		}
		param_count++;
		param_stmt->append( ", " );
		param_stmt->append( param->parameter_name );
		param_stmt->append( "=" );
		param_stmt->append( val->c_str() );
		free( val );
		param_stmt->append( "::" );
		param_stmt->append( get_db_type( param->db_type ) );
	}
	int ret = 0;
	if ( param_count <= 0 ) {
		free( param_stmt );
		ret = _pgsql->execute_scalar( query, func );
	} else {
		std::string* stmt = new std::string( query );
		stmt->append( param_stmt->c_str() ); free( param_stmt );
		ret = _pgsql->execute_scalar( stmt->c_str(), func );
		free( stmt );
	}
	return ret;
}
template<class _func>
inline int npgsql::execute_scalar_l( const char* query, std::list<npgsql_param_type*>& sql_param, _func func ) {
	if ( check_con_status() < 0 )return -1;
	std::string* _query = new std::string( query );
	if ( !sql_param.empty() ) {
		int count = 0;
		for ( auto itr = sql_param.begin(); itr != sql_param.end(); ++itr ) {
			count++;
			std::string* str = new std::string( "\\$" );
			str->append( std::to_string( count ) );
			std::regex* re = new std::regex( *str );
			npgsql_param_type* key = *itr;
			std::string* val;
			if ( key->db_type == npgsql_db_type::NULL_ ) {
				val = new std::string( "null" );
			} else {
				val = new std::string( key->value );
				if ( key->db_type != npgsql_db_type::COMMON ) {
					val->append( "::" ); val->append( get_db_type( key->db_type ) );
				} else {
					quote_literal( *val );
				}
			}
			std::string& copy = *_query;
			copy = std::regex_replace( copy, *re, val->c_str() );
			//std::cout << val->c_str() << "<br/>";
			//std::cout << copy << "<br/>";
			str->clear(); val->clear();
			delete str; delete re; delete val;
		}
	}
	//std::cout << _query->c_str() << "<br/>";
	int ret = _pgsql->execute_scalar( _query->c_str(), func );
	//free(_query);
	delete _query;
	return ret;
}
#endif // !npgsql_h