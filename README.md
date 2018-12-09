# npgsql_wrapper (C++)
PgSQL C++ Wrapper for web_jsx (CGI Application)
```c++
const char* pg_conn = "Server=localhost; Port=5432; UserId=postgres;Password=*****;Database=sow; keepalive=10; CommandTimeout=100000;";
npgsql* pgsql = new npgsql();
int rec = !pgsql->is_iniit ? -1 : 0;
if (rec < 0) {
	std::cout << "UNABLE TO LOAD PGSQL LIB==>" << pgsql->lib_absolute_path << "<br/>";
}
```
#Call to Stored Procedure with param
```c++
std::list<npgsql_params*>* sql_param = new std::list<npgsql_params*>();
malloc(sizeof sql_param);
sql_param->push_back(new npgsql_params("login_id", npgsql_db_type::Varchar, parameter_direction::Input, "rajibs"));
sql_param->push_back(new npgsql_params("form_data", npgsql_db_type::Jsonb, parameter_direction::Input, "{}"));
sql_param->push_back(new npgsql_params("_ret_data_table", npgsql_db_type::Jsonb, parameter_direction::Output));
sql_param->push_back(new npgsql_params("_ret_val", npgsql_db_type::Bigint, parameter_direction::Output));
sql_param->push_back(new npgsql_params("_ret_msg", npgsql_db_type::Varchar, parameter_direction::Output));
std::map<std::string, char*>* result = new std::map<std::string, char*>();
malloc(sizeof result);
rec = pgsql->execute_scalar("auth.__check_npgsql", *sql_param, *result);
free(sql_param);
if (rec < 0) {
	std::cout << pgsql->get_last_error();
	pgsql->close();
	free(pgsql);
}else{
	pgsql->close();
	auto search = result->find("_ret_data_table");
	if (search != result->end()) {
		try {
			std::cout <<  search->second;
		} catch (std::exception&e) {
			std::cout << e.what() << '\n';
		}
	}
	else {
		std::cout << "Not found\n";
	}
	free(result);
}
```
#Call raw query
```c++
pgsql->execute_scalar("select * from information_schema.columns", [](int i, std::vector<char*>&row) {
	for (size_t i = 0; i < row.size(); ++i) {
		std::cout << row[i] << '\r\n';
	}
	return;
});
```
