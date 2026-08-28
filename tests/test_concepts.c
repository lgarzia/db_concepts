#include <assert.h>
#include <string.h>

#include "concepts.h"

int main(void)
{
       record_t record = {0};
       size_t layout_size = sizeof(record.id) + sizeof(record.value);

       record.id = 7;
       strcpy(record.value, "hello");

       assert(record.id == 7);
       assert(strcmp(record.value, "hello") == 0);
       assert(sizeof(record.value) == RECORD_VALUE_CAPACITY);
       assert(layout_size <= sizeof(record));

       record_t designated = {.id = 7, .value = "hello"};
       assert(designated.id == 7);
       assert(strcmp(designated.value, "hello") == 0);

       assert(concepts_pointer_strlen("hello") == 5);
       assert(concepts_pointer_strlen("") == 0);

       record_t value_copy = concepts_copy_record(&record);
       record_t pointer_copy = {0};

       assert(value_copy.id == record.id);
       assert(strcmp(value_copy.value, record.value) == 0);
       assert(concepts_copy_record_to(&record, &pointer_copy) == 0);
       assert(pointer_copy.id == record.id);
       assert(strcmp(pointer_copy.value, record.value) == 0);

       value_copy.id = 8;
       strcpy(value_copy.value, "value copy");
       pointer_copy.id = 9;
       strcpy(pointer_copy.value, "pointer copy");
       assert(record.id == 7);
       assert(strcmp(record.value, "hello") == 0);

       assert(concepts_copy_record_to(NULL, &pointer_copy) == -1);
       assert(concepts_copy_record_to(&record, NULL) == -1);

       return 0;
}
