#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "table.h"

static void test_table_initializes_empty(void)
{
    table table;

    table_init(NULL);
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

    table_init(&table);
    assert(table_insert(&table, first) != 0);
    assert(table_insert(&table, second) != 0);

    first.id = 99;
    strcpy(first.value, "changed");

    assert(table.count == 2);
    assert(table.records[0].id == 1);
    assert(strcmp(table.records[0].value, "first") == 0);
    assert(table.records[1].id == 2);
    assert(strcmp(table.records[1].value, "second") == 0);
}

static void test_table_rejects_null_and_full_insertions(void)
{
    table table;
    table_record record = {0, "stored"};
    size_t index;

    assert(table_insert(NULL, record) == 0);

    table_init(&table);
    for (index = 0; index < TABLE_CAPACITY; index++)
    {
        record.id = (int)index;
        assert(table_insert(&table, record) != 0);
    }

    assert(table.count == TABLE_CAPACITY);
    assert(table_insert(&table, record) == 0);
    assert(table.count == TABLE_CAPACITY);
}

static void test_table_finds_record_by_id(void)
{
    table table;
    table_record first = {1, "first"};
    table_record second = {2, "second"};
    table_record found;
    int status;

    table_init(&table);
    assert(table_insert(&table, first) != 0);
    assert(table_insert(&table, second) != 0);

    status = table_find_by_id(&table, second.id, &found);

    assert(status != 0);
    assert(found.id == 2);
    assert(strcmp(found.value, "second") == 0);
    assert(table.count == 2);
    assert(table.records[0].id == 1);
    assert(strcmp(table.records[0].value, "first") == 0);
    assert(table.records[1].id == 2);
    assert(strcmp(table.records[1].value, "second") == 0);
}

static void test_table_reports_not_found_for_missing_id(void)
{
    table table;
    table_record record = {1, "stored"};
    table_record found = {99, "unchanged"};

    table_init(&table);
    assert(table_insert(&table, record) != 0);

    assert(table_find_by_id(&table, 2, &found) == 0);
    assert(found.id == 99);
    assert(strcmp(found.value, "unchanged") == 0);
}

static void test_table_reports_not_found_when_empty(void)
{
    table table;
    table_record found = {99, "unchanged"};

    table_init(&table);

    assert(table_find_by_id(&table, 1, &found) == 0);
    assert(found.id == 99);
    assert(strcmp(found.value, "unchanged") == 0);
}

static void test_table_finds_first_duplicate_id(void)
{
    table table;
    table_record first = {7, "first"};
    table_record second = {7, "second"};
    table_record found;

    table_init(&table);
    assert(table_insert(&table, first) != 0);
    assert(table_insert(&table, second) != 0);

    assert(table_find_by_id(&table, 7, &found) != 0);
    assert(found.id == 7);
    assert(strcmp(found.value, "first") == 0);
}

static void test_table_deletes_id_by_swapping_with_last(void)
{
    table table;
    table_record first = {1, "first"};
    table_record second = {2, "second"};
    table_record third = {3, "third"};

    table_init(&table);
    assert(table_insert(&table, first) != 0);
    assert(table_insert(&table, second) != 0);
    assert(table_insert(&table, third) != 0);

    assert(table_delete_by_id(&table, second.id) != 0);
    assert(table.count == 2);
    assert(table.records[0].id == first.id);
    assert(table.records[1].id == third.id);
    assert(strcmp(table.records[1].value, "third") == 0);
}

static void test_table_does_not_delete_missing_id(void)
{
    table table;
    table_record record = {1, "stored"};

    table_init(&table);
    assert(table_insert(&table, record) != 0);

    assert(table_delete_by_id(&table, 2) == 0);
    assert(table.count == 1);
    assert(table.records[0].id == record.id);
    assert(strcmp(table.records[0].value, record.value) == 0);
}

static void test_table_handles_null_arguments(void)
{
    table table;
    table_record record = {1, "stored"};

    table_init(&table);
    assert(table_insert(&table, record) != 0);

    assert(table_find_by_id(NULL, record.id, &record) == 0);
    assert(table_find_by_id(&table, record.id, NULL) == 0);
    assert(table_delete_by_id(NULL, record.id) == 0);
}

static void test_table_deletes_only_and_last_records(void)
{
    table table;
    table_record first = {1, "first"};
    table_record second = {2, "second"};

    table_init(&table);
    assert(table_insert(&table, first) != 0);
    assert(table_delete_by_id(&table, first.id) != 0);
    assert(table.count == 0);

    assert(table_insert(&table, first) != 0);
    assert(table_insert(&table, second) != 0);
    assert(table_delete_by_id(&table, second.id) != 0);
    assert(table.count == 1);
    assert(table.records[0].id == first.id);
}

static void test_table_deletes_first_duplicate_id(void)
{
    table table;
    table_record first = {7, "first"};
    table_record second = {7, "second"};
    table_record found;

    table_init(&table);
    assert(table_insert(&table, first) != 0);
    assert(table_insert(&table, second) != 0);

    assert(table_delete_by_id(&table, 7) != 0);
    assert(table.count == 1);
    assert(table_find_by_id(&table, 7, &found) != 0);
    assert(strcmp(found.value, "second") == 0);
}

int main(void)
{
    test_table_initializes_empty();
    test_table_stores_records_inline();
    test_table_inserts_records_in_order();
    test_table_rejects_null_and_full_insertions();
    test_table_finds_record_by_id();
    test_table_reports_not_found_for_missing_id();
    test_table_reports_not_found_when_empty();
    test_table_finds_first_duplicate_id();
    test_table_deletes_id_by_swapping_with_last();
    test_table_does_not_delete_missing_id();
    test_table_handles_null_arguments();
    test_table_deletes_only_and_last_records();
    test_table_deletes_first_duplicate_id();
    return 0;
}
