#include "utils/Vector.h"
#include <stdio.h> //printf
#include <stdlib.h> //malloc, calloc, free, realloc
#include <assert.h>

/* 
 * this vector implementation has (always) at least a certain number (MIN) of elements
 *
 * once the vector hits its limit, its capacity is doubled.
 *
 * if the vector size becomes oversized (size < 0.25*capacity), its capacity is halfed */
struct vector{
	void** elements;
	size_t size;
	size_t capacity;
	size_t min;
};



static void vector_grow(Vector* const arr, const size_t f);
static void vector_shrink(Vector* const arr, const size_t f);
//vector oversize: its current size is less than 25% of its current capacity
static int vector_is_oversized(const Vector* const arr);



Vector* vector_new(const size_t min){
	Vector* arr = malloc(sizeof(Vector));
	arr->size = 0;
	arr->min  = min;
	arr->capacity = min;
	arr->elements = calloc(arr->capacity, sizeof(void*));
	return arr;
}

void vector_destroy(Vector** arr, void (*destroy_element)(void*)){
	if(!arr || !*arr){
		return ;
	}
	if(destroy_element != NULL){
		for(size_t i=0; i<(*arr)->size; i++){
			destroy_element((*arr)->elements[i]);
		}
	}
	free((*arr)->elements);
	free(*arr);
	*arr = NULL;
}

void vector_restart(Vector** arr, void (*destroy_element)(void*)){
	size_t min = (*arr)->min;
	vector_destroy(arr, destroy_element);
	*arr = vector_new(min);
}

void vector_clear(Vector* arr){
	free(arr->elements);
	arr->size = 0;
	arr->capacity = arr->min;
	arr->elements = calloc(arr->capacity, sizeof(void*));
	assert(arr->elements);
}



size_t vector_get_size(const Vector* const arr){
	return arr->size;
}

size_t vector_get_capacity(const Vector* const arr){
	return arr->capacity;
}

void* vector_get_item(Vector* arr, size_t idx){
	return arr->elements[idx];
}

void* vector_search(Vector* arr, void* item, int (*compare)(void*, void*)){
	for(size_t i=0; i<arr->size; i++){
		if(compare(arr->elements[i], item)){
			return arr->elements[i];
		}
	}
	return NULL;
}

ssize_t vector_search_idx(Vector* arr, void* item, int (*compare)(void*, void*)){
	for(ssize_t i=0; i<arr->size; i++){
		if(compare(arr->elements[i], item)){
			return i;
		}
	}
	return -1;
}




int vector_append(Vector* const arr, void* item){
	if(vector_is_full(arr)){
		vector_grow(arr, 1);
	}
	int idx = arr->size++;
	arr->elements[idx] = item;
	return idx;
}

void* vector_pop(Vector* const arr){
	if(vector_is_empty(arr)){
		return NULL;
	}
	if(vector_is_oversized(arr)){
		vector_shrink(arr, 1);
	}
	void* item = arr->elements[arr->size-1];
	arr->elements[--arr->size] = NULL;
	return item;
}



int vector_is_empty(const Vector* const arr){
	return !arr->size;
}

int vector_is_full(const Vector* const arr){
	return arr->size == arr->capacity;
}





static void vector_grow(Vector* const arr, size_t const f){
	arr->capacity <<= f;
	arr->elements = realloc(arr->elements, sizeof(void*)*arr->capacity);
	assert(arr->elements);
}

static void vector_shrink(Vector* const arr, size_t const f){
	arr->capacity >>= f;
	arr->elements = realloc(arr->elements, sizeof(void*)*arr->capacity);
	assert(arr->elements);
}

static int vector_is_oversized(const Vector* const arr){
	if(arr->size <= arr->min){
		return 0;
	}

	if(4*arr->size < arr->capacity){
		return 1;
	}

	return 0;
}
