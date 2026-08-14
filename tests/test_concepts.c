#include <assert.h>
#include <string.h>

#include "concepts.h"

int main(void)
{
    assert(concepts_pointer_strlen("hello") == 5);
    assert(concepts_pointer_strlen("") == 0);
    return 0;
}
