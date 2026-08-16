#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "concepts.h"

int main(void)
{
    record_t record = {0};
    size_t layout_size = sizeof(record.id) + sizeof(record.value);

    puts("[test_concepts] starting manual record initialization checks");

    record.id = 7;
    strcpy(record.value, "hello");

    printf("[test_concepts] record.id = %d\n", record.id);
    printf("[test_concepts] record.value = %s\n", record.value);
    printf("[test_concepts] sizeof(record.value) = %zu, RECORD_VALUE_CAPACITY = %d\n",
           sizeof(record.value), RECORD_VALUE_CAPACITY);
    printf("[test_concepts] layout size = %zu, sizeof(record) = %zu\n",
           layout_size, sizeof(record));

    assert(record.id == 7);
    assert(strcmp(record.value, "hello") == 0);
    assert(sizeof(record.value) == RECORD_VALUE_CAPACITY);
    assert(layout_size <= sizeof(record));

    record_t designated = {.id = 7, .value = "hello"};
    printf("[test_concepts] designated.id = %d\n", designated.id);
    printf("[test_concepts] designated.value = %s\n", designated.value);
    assert(designated.id == 7);
    assert(strcmp(designated.value, "hello") == 0);

    printf("[test_concepts] concepts_pointer_strlen(\"hello\") = %zu\n",
           concepts_pointer_strlen("hello"));
    printf("[test_concepts] concepts_pointer_strlen(\"\") = %zu\n",
           concepts_pointer_strlen(""));
    assert(concepts_pointer_strlen("hello") == 5);
    assert(concepts_pointer_strlen("") == 0);

    puts("[test_concepts] all checks passed");
    return 0;
}
