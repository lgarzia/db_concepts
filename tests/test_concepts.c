#include <assert.h>
#include <string.h>

#include "concepts.h"

int main(void)
{
    record_t record = {.id = 7, .value = "hello"};

    assert(record.id == 7);
    assert(strcmp(record.value, "hello") == 0);
    assert(sizeof(record.value) == RECORD_VALUE_CAPACITY);

    assert(concepts_pointer_strlen("hello") == 5);
    assert(concepts_pointer_strlen("") == 0);
    return 0;
}
