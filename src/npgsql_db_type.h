//5:27 PM 11/19/2018
#pragma once
#if !defined(_npgsql_global_h)
#include "npgsql_global.h"
#endif//!_global_h
#if !defined(_npgsql_db_type_h)
#define _npgsql_db_type_h
#pragma warning (disable : 4231)
typedef enum  {
	COMMON = -1,
	NULL_=-2,
	
	//
	// Summary:
	//     Corresponds to the PostgreSQL "array" type, a variable-length multidimensional
	//     array of another type. This value must be combined with another value from NpgsqlTypes.NpgsqlDbType
	//     via a bit OR (e.g. NpgsqlDbType.Array | NpgsqlDbType.Integer)
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/arrays.html
	Array = INT_MIN,
	//
	// Summary:
	//     Corresponds to the PostgreSQL 8-byte "bigint" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-numeric.html
	Bigint = 1,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "boolean" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-boolean.html
	Boolean = 2,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "box" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	Box = 3,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "bytea" type, holding a raw byte string.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-binary.html
	Bytea = 4,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "circle" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	Circle = 5,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "char(n)"type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-character.html
	Char = 6,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "date" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	Date = 7,
	//
	// Summary:
	//     Corresponds to the PostgreSQL 8-byte floating-point "double" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-numeric.html
	Double = 8,
	//
	// Summary:
	//     Corresponds to the PostgreSQL 4-byte "integer" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-numeric.html
	Integer = 9,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "line" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	Line = 10,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "lseg" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	LSeg = 11,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "money" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-money.html
	Money = 12,
	//
	// Summary:
	//     Corresponds to the PostgreSQL arbitrary-precision "numeric" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-numeric.html
	Numeric = 13,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "path" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	Path = 14,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "point" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	Point = 15,
	//
	// Summary:
	//     Corresponds to the PostgreSQL geometric "polygon" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-geometric.html
	//Polygon = 16,
	//
	// Summary:
	//     Corresponds to the PostgreSQL floating-point "real" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-numeric.html
	Real = 17,
	//
	// Summary:
	//     Corresponds to the PostgreSQL 2-byte "smallint" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-numeric.html
	Smallint = 18,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "text" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-character.html
	Text = 19,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "time" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	Time = 20,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "timestamp" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	Timestamp = 21,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "varchar" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-character.html
	Varchar = 22,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "refcursor" type.
	Refcursor = 23,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "inet" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-net-types.html
	Inet = 24,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "bit" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-bit.html
	Bit = 25,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "timestamp with time zone" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	TimestampTZ = 26,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "uuid" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-uuid.html
	Uuid = 27,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "xml" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-xml.html
	Xml = 28,
	//
	// Summary:
	//     Corresponds to the PostgreSQL internal "oidvector" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-oid.html
	Oidvector = 29,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "interval" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	Interval = 30,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "time with time zone" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	TimeTZ = 31,
	//
	// Summary:
	//     Corresponds to the PostgreSQL internal "name" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-character.html
	Name = 32,
	//
	// Summary:
	//     Corresponds to the obsolete PostgreSQL "abstime" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-datetime.html
	Abstime = 33,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "macaddr" type, a field storing a 6-byte physical
	//     address.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-net-types.html
	MacAddr = 34,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "json" type, a field storing JSON in text format.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-json.html
	Json = 35,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "jsonb" type, a field storing JSON in an optimized
	//     binary format.
	//
	// Remarks:
	//     Supported since PostgreSQL 9.4. See http://www.postgresql.org/docs/current/static/datatype-json.html
	Jsonb = 36,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "hstore" type, a dictionary of string key-value
	//     pairs.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/hstore.html
	Hstore = 37,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "char" type.
	//
	// Remarks:
	//     This is an internal field and should normally not be used for regular applications.
	//     See http://www.postgresql.org/docs/current/static/datatype-text.html
	InternalChar = 38,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "varbit" type, a field storing a variable-length
	//     string of bits.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-boolean.html
	Varbit = 39,
	//
	// Summary:
	//     A special value that can be used to send parameter values to the database without
	//     specifying their type, allowing the database to cast them to another value based
	//     on context. The value will be converted to a string and send as text.
	//
	// Remarks:
	//     This value shouldn't ordinarily be used, and makes sense only when sending a
	//     data type unsupported by Npgsql.
	//Unknown = 40,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "oid" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-oid.html
	//Oid = 41,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "xid" type, an internal transaction identifier.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-oid.html
	Xid = 42,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "cid" type, an internal command identifier.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-oid.html
	Cid = 43,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "cidr" type, a field storing an IPv4 or IPv6 network.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-net-types.html
	Cidr = 44,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "tsvector" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-textsearch.html
	TsVector = 45,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "tsquery" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-textsearch.html
	TsQuery = 46,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "enum" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/datatype-enum.html
	Enum = 47,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "composite" type.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/rowtypes.html
	Composite = 48,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "regtype" type, a numeric (OID) ID of a type in
	//     the pg_type table.
	Regtype = 49,
	//
	// Summary:
	//     The geometry type for postgresql spatial extension postgis.
	Geometry = 50,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "citext" type for the citext module.
	//
	// Remarks:
	//     See http://www.postgresql.org/docs/current/static/citext.html
	Citext = 51,
	//
	// Summary:
	//     Corresponds to the PostgreSQL internal "int2vector" type.
	Int2Vector = 52,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "tid" type, a tuple id identifying the physical
	//     location of a row within its table.
	Tid = 53,
	//
	// Summary:
	//     Corresponds to the PostgreSQL "array" type, a variable-length multidimensional
	//     array of another type. This value must be combined with another value from NpgsqlTypes.NpgsqlDbType
	//     via a bit OR (e.g. NpgsqlDbType.Array | NpgsqlDbType.Integer)
	//
	// Remarks:
	//     Supported since PostgreSQL 9.2. See http://www.postgresql.org/docs/9.2/static/rangetypes.html
	Range = 1073741824
}npgsql_db_type;
static const char* get_db_type(npgsql_db_type db_type) {
	switch (db_type) {
	case npgsql_db_type::Bigint: return "bigint";
	case npgsql_db_type::Json:return "json";
	case npgsql_db_type::Jsonb:return "jsonb";
	case npgsql_db_type::Varchar:return "varchar";
	case npgsql_db_type::Boolean:return "boolean";
	default:return "unknown";
	}
};
#endif//!_npgsql_db_type_h