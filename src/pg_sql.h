/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:16 AM 11/19/2018
//#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)
#pragma once
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_os_support_h)
#include "os_support.h"
#endif
#include <libpq-fe.h>
#if !defined(_pg_sql_h)
#define _pg_sql_h
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#define LIBPQ_C_DLL				"libpq.so.5"
#else
#define LIBPQ_C_DLL				"libpq.dll"
#endif
#define LIBPQ_DLL_LOAD_ERROR	"Error loading PostgreSQL " LIBPQ_C_DLL ": " 
// libpq C library functions
typedef void (*PQclearPQFunc)(PGresult *res);
typedef char* (*PQerrorMessagePQFunc)(const PGconn *conn);
typedef PGresult* (*PQdescribePreparedPQFunc)(PGconn *conn, const char *stmt);
typedef PGresult* (*PQexecPQFunc)(PGconn *conn, const char *query);
typedef int (*PQfmodPQFunc)(const PGresult *res, int column_number);
typedef char* (*PQfnamePQFunc)(const PGresult *res, int column_number);
typedef int (*PQfsizePQFunc)(const PGresult *res, int column_number);
typedef Oid (*PQftypePQFunc)(const PGresult *res, int column_number);
typedef int (*PQgetisnullPQFunc)(const PGresult *res, int row_number, int column_number);
typedef int (*PQgetlengthPQFunc)(const PGresult *res, int row_number, int column_number);
typedef PGresult* (*PQgetResultPQFunc)(PGconn *conn);
typedef char* (*PQgetvaluePQFunc)(const PGresult *res, int row_number, int column_number);
typedef int (*PQnfieldsPQFunc)(const PGresult *res);
typedef int (*PQntuplesPQFunc)(const PGresult *res);
typedef int (*PQputCopyDataPQFunc)(PGconn *conn, const char *buffer, int nbytes);
typedef int (*PQputCopyEndPQFunc)(PGconn *conn, const char *errormsg);
typedef char* (*PQresultErrorMessagePQFunc)(const PGresult *res);
typedef ExecStatusType (*PQresultStatusPQFunc)(const PGresult *res);
typedef int	(*PQsetClientEncodingPQFunc)(PGconn *conn, const char *encoding);
typedef PGconn* (*PQsetdbLoginPQFunc)(const char *pghost, const char *pgport, const char *pgoptions, const char *pgtty, const char *dbName, const char *login, const char *pwd);
typedef ConnStatusType (*PQstatusPQFunc)(const PGconn *conn);
typedef PGconn* (*PQconnectdbPQFunc)(const char *conninfo);
typedef void (*PQfinishPQFunc)(PGconn *conn);

#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
typedef void* h_get_proc_iddl;
typedef void* h_module;
#else
typedef HINSTANCE h_get_proc_iddl;
typedef HMODULE h_module;
#endif
class pg_sql {
private:
	bool							_connected;
	PGconn*							_conn;
	PGresult*						_pg_result;
	int								_cursor_rows_fetched;
	size_t							_copy_cols_count;
	h_get_proc_iddl					_pgsql_proc_iddl;
	h_module						_pgsql_module;
	PQclearPQFunc					_PQclear;
	PQerrorMessagePQFunc			_PQerrorMessage;
	PQexecPQFunc					_PQexec;
	PQfmodPQFunc					_PQfmod;
	PQfnamePQFunc					_PQfname;
	PQfsizePQFunc					_PQfsize;
	PQftypePQFunc					_PQftype;
	PQgetisnullPQFunc				_PQgetisnull;
	PQgetlengthPQFunc				_PQgetlength;
	PQgetResultPQFunc				_PQgetResult;//
	PQgetvaluePQFunc				_PQgetvalue;//
	PQnfieldsPQFunc					_PQnfields;//
	PQntuplesPQFunc					_PQntuples;//
	PQputCopyDataPQFunc				_PQputCopyData;
	PQputCopyEndPQFunc				_PQputCopyEnd;
	PQresultErrorMessagePQFunc		_PQresultErrorMessage;
	PQresultStatusPQFunc			_PQresultStatus;
	PQsetClientEncodingPQFunc		_PQsetClientEncoding;
	PQsetdbLoginPQFunc				_PQsetdbLogin;
	PQconnectdbPQFunc				_PQconnectdb;
	PQfinishPQFunc					_PQfinish;
	PQstatusPQFunc					_PQstatus;
	int								_n_error;
	char*							_n_error_text;
	int								_pq_error;
	char*							_pq_error_text;
public:
	pg_sql();
	~pg_sql();
	virtual const int is_api_error();
	virtual const char * get_last_error();
	// Initialize API
	virtual int init(const char* lib_path);
	// Connect to the database
	virtual int connect(const char *conn);
	virtual void exit_nicely();
	virtual int get_row_count(const char *object, char *value);
	virtual int execute_scalar(const char *query, char *value);
	template<class _func>
	int execute_scalar(const char *query, _func func);
	virtual int execute_scalar_x(const char *query, std::list<std::string>&out_param_array, std::map<std::string, char*>&out_param_map);
	virtual int execute_non_query(const char *query);
private:
	virtual void parse_connection_string(const char *conn, std::string& user, std::string& pwd, std::string& server, std::string& port, std::string& db);
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	virtual int load_pgsql_lib(const char* name);
#else
	virtual int load_pgsql_lib(const char* name);
#endif
	virtual void set_error(const char* std_error_msg);
	virtual void set_error();
};
//5:30 AM 11/19/2018 Connected
//7:55 AM 11/19/2018 END
#endif//!_pg_sql_h

template<class _func>
inline int pg_sql::execute_scalar(const char * query, _func func) {
	if ((query != NULL) && (query[0] == '\0')) {
		return -1;
	}
	bool exists = false;
	PGresult *res = _PQexec(_conn, query);
	// Get the value of the first column of the first row
	/*if (_PQresultStatus(res) == PGRES_TUPLES_OK) {
		exists = true;
		//_PQntuples
		int nFields = _PQnfields(res);
		for (int i = 0; i < _PQntuples(res); i++) {
			for (int j = 0; j < nFields; j++) {
				func(i, j, _PQgetvalue(res, i, j));
			}
		}
	}
	else set_error();
	_PQclear(res);
	return (exists == true) ? 0 : -1;*/
	//!TODO
	if (_PQresultStatus(res) == PGRES_TUPLES_OK) {
		exists = true;
		//_PQntuples
		int nFields = _PQnfields(res);
		for (int i = 0; i < _PQntuples(res); i++) {
			std::vector<char*>*rows = new std::vector<char*>();
			for (int j = 0; j < nFields; j++) {
				char* c = _PQgetvalue(res, i, j);
				char* copy = new char[strlen(c) + 1];
				strcpy(copy, c);
				rows->push_back(copy);
			}
			func(i, *rows);
			free(rows);
		}
	}
	else set_error();
	_PQclear(res);
	return (exists == true) ? 0 : -1;
};