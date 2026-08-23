#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>

#define TABLE_CAPACITY 16
#define TABLE_VALUE_CAPACITY 256

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

void table_init(table *table);
void table_insert(table *table, table_record record);

#endif
