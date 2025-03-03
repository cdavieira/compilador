#pragma once

enum Type {
	TYPE_VOID = 0, //TYPE_VOID = NO TYPE
	TYPE_INT,
	TYPE_FLT,
	TYPE_CHAR,
	TYPE_STR,
};

const char* type_name(enum Type type);
