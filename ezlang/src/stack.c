#include "stack.h"
#include <stdio.h> //printf
#include <stdlib.h> //malloc, calloc, free, realloc
#include <assert.h> //assert

struct Stack{
	void** elements;
	size_t size;
	size_t capacity;
	size_t min;
};



static void stack_grow(Stack* const st, const size_t f);
static void stack_shrink(Stack* const st, const size_t f);
static int stack_is_oversized(Stack const * const st);



Stack* stack_new(size_t min){
	Stack* st = malloc(sizeof(Stack));
	st->size = 0;
	st->min = min;
	st->capacity = min;
	st->elements = calloc(st->capacity, sizeof(void*));
	return st;
}

void stack_destroy(Stack** st, void (*free_element)(void*)){
	size_t size = (*st)->size;
	for(size_t i=0; i<size; i++){
		free_element((*st)->elements[i]);
	}
	stack_dealloc(st);
}

void stack_dealloc(Stack** st){
	if(!st || !*st){
		return ;
	}
	free((*st)->elements);
	free(*st);
	*st = NULL;
}




size_t stack_get_size(Stack const * const st){
	return st->size;
}

size_t stack_get_capacity(Stack const * const st){
	return st->capacity;
}




void stack_push(Stack* const st, void* item){
	if(stack_is_full(st)){
		stack_grow(st, 1);
	}
	st->elements[st->size++] = item;
}

void* stack_pop(Stack* const st){
	if(stack_is_empty(st)){
		return NULL;
	}
	if(stack_is_oversized(st)){
		stack_shrink(st, 1);
	}
	void* item = st->elements[st->size-1];
	st->elements[--st->size] = NULL;
	return item;
}




int stack_is_empty(Stack const * const st){
	return !st->size;
}

int stack_is_full(Stack const * const st){
	return st->size == st->capacity;
}



static void stack_grow(Stack* const st, size_t const f){
	st->capacity <<= f;
	st->elements = realloc(st->elements, sizeof(void*)*st->capacity);
	assert(st->elements);
}

static void stack_shrink(Stack* const st, size_t const f){
	st->capacity >>= f;
	st->elements = realloc(st->elements, sizeof(void*)*st->capacity);
	assert(st->elements);
}

static int stack_is_oversized(Stack const * const st){
	if(st->size <= st->min){
		return 0;
	}

	if(4*st->size < st->capacity){
		return 1;
	}

	return 0;
}
