#include "table.h"

void table_init(table *table)
{
    table->count = 0;
}

void table_insert(table *table, table_record record)
{
    if (table->count >= TABLE_CAPACITY)
    {
        return;
    }

    table->records[table->count] = record;
    table->count++;
}

int table_find_by_id(const table *table, int id, table_record *record)
{
    size_t index;

    if (table == NULL || record == NULL)
    {
        return 0;
    }

    for (index = 0; index < table->count; index++)
    {
        if (table->records[index].id == id)
        {
            *record = table->records[index];
            return 1;
        }
    }

    return 0;
}

int table_delete_by_id(table *table, int id)
{
    size_t index;

    if (table == NULL)
    {
        return 0;
    }

    for (index = 0; index < table->count; index++)
    {
        if (table->records[index].id == id)
        {
            table->count--;
            table->records[index] = table->records[table->count];
            return 1;
        }
    }

    return 0;
}