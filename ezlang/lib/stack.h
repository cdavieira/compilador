#pragma once

#include <stddef.h>

/* some notes about dstack
 * the push, pop, is_empty, is_full and getter functions aren't null protected
 * why? well, we're c programmers... thats how things roll
 * but stack_destroy is null protected tho! yay */

typedef struct Stack Stack;

Stack* stack_new(size_t min);
void stack_destroy(Stack** st, void (*free_element)(void*));
void stack_dealloc(Stack** st);


size_t stack_get_size(Stack const * const st);
size_t stack_get_maxsize(Stack const * const st);


void stack_push(Stack* const st, void* item);
void* stack_pop(Stack* const st);


int stack_is_empty(Stack const * const st);
int stack_is_full(Stack const * const st);
