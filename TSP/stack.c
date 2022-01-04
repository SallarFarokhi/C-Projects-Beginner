#include "stack.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Note: A great deal of these functions were provided by
// Professor Long, in the assignment documentation.

// Struct and its attributes
// Provided by Professor Long
struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

// Creates the stack and returns the pointer
// Provided by Professor Long
Stack *stack_create(uint32_t capacity) {

    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// Deletes the stack and the pointer
// Provided by Professor Long
void stack_delete(Stack **s) {

    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
}

// Checks if the stack is empty
bool stack_empty(Stack *s) {
    //    s->top = (s->top < 0) ? 0: s->top;
    return s->top == 0;
}

// Checks if the stack is full
bool stack_full(Stack *s) {

    return s->top == s->capacity;
}

// Returns the stack size
uint32_t stack_size(Stack *s) {

    return s->top;
}

// Pushes x  onto the stack
bool stack_push(Stack *s, uint32_t x) {

    if (!stack_full(s)) {
        s->items[s->top] = x;
        s->top += 1;
        return true;
    }
    return false;
}

// Takes x from the stack
bool stack_pop(Stack *s, uint32_t *x) {

    if (!stack_empty(s)) {
        s->top -= 1;
        // Provided by Professor Long
        *x = s->items[s->top];
        s->items[s->top] = 0;
        return true;
    }
    return false;
}

// Takes x without affecting the stack
bool stack_peek(Stack *s, uint32_t *x) {

    if (!stack_empty(s)) {
        *x = s->items[s->top - 1];
        return true;
    }
    return false;
}

// Copies the stack into another stack
void stack_copy(Stack *dst, Stack *src) {

    for (uint32_t i = 0; i < dst->capacity; i++) {
        dst->items[i] = src->items[i];
    }
    dst->capacity = src->capacity;
    dst->top = src->top;
}

// Prints the stack into a file
// Provided by Professor Long
void stack_print(Stack *s, FILE *outfile, char *cities[]) {

    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
