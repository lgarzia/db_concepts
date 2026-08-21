#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "table.h"

static void test_table_initializes_empty(void)
{
    table table;

    table_init(&table);

    assert(table.count == 0);
    assert(TABLE_CAPACITY > 0);
    assert(sizeof(table.records) == TABLE_CAPACITY * sizeof(table_record));
}

static void test_table_stores_records_inline(void)
{
    table table;
    table_record record = {42, "example"};

    table_init(&table);
    table.records[0] = record;

    assert(table.records[0].id == 42);
    assert(strcmp(table.records[0].value, "example") == 0);
}

int main(void)
{
    test_table_initializes_empty();
    test_table_stores_records_inline();
    return 0;
}
