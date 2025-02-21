#pragma once

#include <stddef.h>

/* some notes about Vector
 * the append, pop, is_empty, is_full and getter functions aren't null protected
 * why? well, we're c programmers... thats how things roll
 * but vector_destroy is null protected tho! yay */

typedef struct vector Vector;

Vector* vector_new(const size_t min);
void vector_destroy(Vector** arr, void (*destroy_element)(void*));
void vector_restart(Vector** arr, void (*destroy_element)(void*));
void vector_clear(Vector* arr);

size_t vector_get_size(const Vector * const arr);
size_t vector_get_capacity(const Vector * const arr);
void* vector_get_item(Vector* arr, size_t idx);

void* vector_search(Vector* arr, void* item, int (*compare)(void*, void*));
void vector_append(Vector* const arr, void* item);
void* vector_pop(Vector* const arr);

int vector_is_empty(const Vector * const arr);
int vector_is_full(const Vector * const arr);
