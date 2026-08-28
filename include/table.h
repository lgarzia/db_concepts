#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>

#define TABLE_CAPACITY 16
#define TABLE_VALUE_CAPACITY 256

/* Table-specific record model used by the fixed-size in-memory table. */
typedef struct
{
    int id;
    char value[TABLE_VALUE_CAPACITY];
} table_record;

typedef struct
{
    table_record records[TABLE_CAPACITY];
    size_t count;
} table;

/* A NULL table is ignored so setup code can use the same simple contract as the other table APIs. */
void table_init(table *table);

/*
 * Appends a record when table has space.
 * Returns 1 on insertion and 0 when table is NULL or full.
 */
int table_insert(table *table, table_record record);

/* Finds the first record with id and copies it into record. */
int table_find_by_id(const table *table, int id, table_record *record);

/* Deletes the first record with id by replacing it with the final record. */
int table_delete_by_id(table *table, int id);

#endif
