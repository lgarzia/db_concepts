#include "concepts.h"

#include <stdio.h>

size_t concepts_pointer_strlen(const char *buffer)
{
    const char *cursor = buffer; /* array decays to pointer at the call boundary */

    /* Walk the pointer instead of indexing to show pointer arithmetic directly. */
    while (*cursor != '\0')
    {
        cursor++;
    }

    /* Pointer subtraction yields the number of elements walked, not bytes. */
    return (size_t)(cursor - buffer);
}

void concepts_demonstrate(void)
{
    char buffer[16] = "hi";

    /* An array knows its own storage size; sizeof here is the full capacity. */
    printf("buffer capacity (sizeof array): %zu\n", sizeof(buffer));

    /* concepts_pointer_strlen only sees a pointer, so it reports logical length, not capacity. */
    printf("buffer logical length (pointer walk): %zu\n", concepts_pointer_strlen(buffer));

    /* Passing buffer to a function decays it to a pointer, losing the array's size info. */
    printf("array-to-pointer decay: a char* parameter cannot recover sizeof(buffer).\n");
}
