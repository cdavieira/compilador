#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tables.h"

// Strings Table
// ----------------------------------------------------------------------------

#define STRING_MAX_SIZE 128
#define STRINGS_TABLE_MAX_SIZE 100

struct str_table {
    char t[STRINGS_TABLE_MAX_SIZE][STRING_MAX_SIZE];
    int size;
};

StrTable* create_str_table() {
    StrTable *st = malloc(sizeof * st);
    st->size = 0;
    return st;
}

int add_string(StrTable* st, char* s) {
    for (int i = 0; i < st->size; i++) {
        if (strcmp(st->t[i], s) == 0) {
            return i;
        }
    }
    strcpy(st->t[st->size], s);
    int idx_added = st->size;
    st->size++;
    return idx_added;
}

char* get_string(StrTable* st, int i) {
    return st->t[i];
}

void print_str_table(StrTable* st) {
	printf("Strings table:\n");
	for (int i = 0; i < st->size; i++) {
			printf("Entry %d -- %s\n", i, get_string(st, i));
	}
}

void free_str_table(StrTable* st) {
    free(st);
}

// Variables Table
// ----------------------------------------------------------------------------
struct var_table {
    Variable t[100];
    int size;
};

VarTable* create_var_table() {
    VarTable *vt = malloc(sizeof * vt);
    vt->size = 0;
    return vt;
}

int lookup_var(VarTable* vt, char* s, int scope_level) {
	for (int i = 0; i < vt->size; i++) {
			if (strcmp(vt->t[i].name, s) == 0 && vt->t[i].scope_level == scope_level)  {
					return i;
			}
	}
	return -1;
}

int add_var(VarTable* vt, char* s, int line, enum Type type, int scope_level){
	//strcpy(vt->t[vt->size].name, s);
	vt->t[vt->size].name = s;
	vt->t[vt->size].line = line;
	vt->t[vt->size].literal.type = type;
	vt->t[vt->size].scope_level = scope_level;
	int idx_added = vt->size;
	vt->size++;
	return idx_added;
}

char* get_name(VarTable* vt, int i) {
	return vt->t[i].name;
}

int get_line(VarTable* vt, int i) {
	return vt->t[i].line;
}

enum Type get_type(VarTable* vt, int i) {
	return vt->t[i].literal.type;
}

int get_scope_level(VarTable* vt, int i){
	return vt->t[i].scope_level;
}

void set_val(VarTable* vt, int idx, Value value){
	vt->t[idx].literal.value = value;
}

Value get_val(VarTable* vt, int idx){
	return vt->t[idx].literal.value;
}

void print_var_table(VarTable* vt) {
	printf("Variables table:\n");
	for (int i = 0; i < vt->size; i++) {
			 printf("Entry %d -- name: %s, line: %d, type: %d\n", i,
							get_name(vt, i), get_line(vt, i), get_type(vt, i));
	}
}

void free_var_table(VarTable* vt) {
    free(vt);
}
