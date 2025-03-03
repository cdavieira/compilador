#include "types/Type.h"

const char* type_name(enum Type type){
	switch(type){
		case TYPE_INT:
			return "int";
		case TYPE_FLT:
			return "float";
		case TYPE_STR:
			return "char*";
		case TYPE_CHAR:
			return "char";
		case TYPE_VOID:
			return "void";
		default:
			break;
	}
	return "unknown typename";
}
