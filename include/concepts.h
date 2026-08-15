#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <stddef.h>

#define RECORD_VALUE_CAPACITY 32

typedef struct
{
    int id;
    char value[RECORD_VALUE_CAPACITY];
} record_t;

/*
 * Pure helper: walks a pointer to find where a buffer's logical string ends,
 * without using strlen(), to make pointer arithmetic explicit and testable.
 */
size_t concepts_pointer_strlen(const char *buffer);

/* Prints a heavily commented walkthrough of pointers vs. arrays vs. buffers. */
void concepts_demonstrate(void);

#endif
