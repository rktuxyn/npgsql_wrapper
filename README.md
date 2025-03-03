# Npgsql Wrapper

Npgsql Wrapper is a lightweight, high-performance C/C++ wrapper around the PostgreSQL client library (libpq), designed to simplify database interactions while maintaining efficiency and flexibility.

## Features

- Simple and clean API for PostgreSQL operations
- Secure parameterized queries
- Connection pooling for efficient resource management
- Asynchronous support for non-blocking operations
- Strongly-typed query results
- Lightweight and easy to integrate

## Installation

### Prerequisites

- PostgreSQL client library (`libpq`)
- C++17 or later
- CMake (for building the project)

### Build and Install

```sh
git clone https://github.com/fsystech/npgsql_wrapper.git
cd npgsql_wrapper
mkdir build && cd build
cmake ..
make
sudo make install
```

## Usage

### Basic Example

```cpp
#include "npgsql_wrapper.h"

int main() {
    npgsql::Database db("host=myserver user=myuser password=mypass dbname=mydb");
    if (!db.isConnected()) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }

    auto result = db.query("SELECT * FROM users WHERE id = $1", 1);
    for (auto& row : result) {
        std::cout << "User ID: " << row["id"].as<int>() << ", Name: " << row["name"].as<std::string>() << "\n";
    }
    return 0;
}
```

## API Reference

### `npgsql::Database`
- `Database(const std::string& connectionString)` - Initializes the database connection.
- `bool isConnected() const` - Checks if the connection is active.
- `QueryResult query(const std::string& sql, Args... args)` - Executes a query with optional parameters.
- `int execute(const std::string& sql, Args... args)` - Executes a non-query statement (INSERT, UPDATE, DELETE).

### `npgsql::QueryResult`
- `bool empty() const` - Checks if the result set is empty.
- `size_t size() const` - Returns the number of rows.
- `Row operator[](size_t index) const` - Access a specific row.

### `npgsql::Row`
- `Column operator[](const std::string& columnName) const` - Access a specific column.
- `template<typename T> T as() const` - Convert the column value to the desired type.

## License

This project is licensed under the MIT License.

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests to improve the Npgsql Wrapper.

## Contact

For any inquiries, reach out to [FSys Tech Ltd](https://github.com/fsystech).

