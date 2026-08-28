#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <stddef.h>

#define RECORD_VALUE_CAPACITY 32

/* Standalone record model used for struct initialization and copy lessons. */
typedef struct
{
    int id;
    char value[RECORD_VALUE_CAPACITY];
} record_t;

/* Copies source by value. source must not be NULL. */
record_t concepts_copy_record(const record_t *source);

/* Copies source through destination pointers; returns -1 if either is NULL. */
int concepts_copy_record_to(const record_t *source, record_t *destination);

/*
 * Pure helper: walks a pointer to find where a buffer's logical string ends,
 * without using strlen(), to make pointer arithmetic explicit and testable.
 */
size_t concepts_pointer_strlen(const char *buffer);

/* Prints a heavily commented walkthrough of pointers vs. arrays vs. buffers. */
void concepts_demonstrate(void);

#endif
