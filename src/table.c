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