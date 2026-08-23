#include <assert.h>
#include <stddef.h>
#include <stdio.h>
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

static void test_table_inserts_records_in_order(void)
{
    table table;
    table_record first = {1, "first"};
    table_record second = {2, "second"};

    // Start with an empty table before testing insertion order.
    table_init(&table);
    printf("[table insert] initialized table with count=%zu\n", table.count);

    // Insert records in a known order.
    table_insert(&table, first);
    printf("[table insert] inserted id=%d at index 0\n", first.id);
    table_insert(&table, second);
    printf("[table insert] inserted id=%d at index 1\n", second.id);

    // Change the source record to verify that insertion copied its value.
    first.id = 99;
    strcpy(first.value, "changed");
    printf("[table insert] changed source record to id=%d, value=%s\n",
           first.id,
           first.value);

    // Confirm count, insertion order, and independent stored values.
    assert(table.count == 2);
    assert(table.records[0].id == 1);
    assert(strcmp(table.records[0].value, "first") == 0);
    assert(table.records[1].id == 2);
    assert(strcmp(table.records[1].value, "second") == 0);
    printf("[table insert] stored records: (%d, %s), (%d, %s)\n",
           table.records[0].id,
           table.records[0].value,
           table.records[1].id,
           table.records[1].value);
}

int main(void)
{
    test_table_initializes_empty();
    test_table_stores_records_inline();
    test_table_inserts_records_in_order();
    return 0;
}
