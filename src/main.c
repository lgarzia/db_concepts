#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "concepts.h"
#include "table.h"

#define CLI_INPUT_CAPACITY 512

static int line_is_complete(const char *line)
{
    size_t length = 0;

    while (line[length] != '\0')
    {
        if (line[length] == '\n')
        {
            return 1;
        }
        length++;
    }

    return 0;
}

int main(void)
{
    char line[CLI_INPUT_CAPACITY];
    table records;

    table_init(&records);

    /*
     * fgets() reads at most one less than the buffer capacity and always
     * adds a null terminator when it reads data. That makes this loop easy
     * to reason about while still giving us a clear maximum line length.
     */
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        cli_command_result result;

        /*
         * If no newline was read, the input may have been longer than our
         * buffer. Discard the rest of that line so it cannot become a new
         * command, then report the boundary error to the user.
         */
        if (!line_is_complete(line) && !feof(stdin))
        {
            int character;

            do
            {
                character = getchar();
            } while (character != '\n' && character != EOF);

            fprintf(stderr, "Invalid command: input line is too long.\n");
            continue;
        }

        result = cli_parse_line(line);

        if (result.type == CLI_COMMAND_QUIT)
        {
            break;
        }

        if (result.type == CLI_COMMAND_CONCEPTS)
        {
            concepts_demonstrate();
            continue;
        }

        if (result.type == CLI_COMMAND_READ ||
            result.type == CLI_COMMAND_WRITE)
        {
            puts(result.argument);
            continue;
        }

        if (result.type == CLI_COMMAND_INSERT)
        {
            table_record record;

            record.id = result.id;
            strcpy(record.value, result.value);

            if (table_insert(&records, record))
            {
                printf("Inserted record %d.\n", result.id);
            }
            else
            {
                fprintf(stderr, "Failed to insert record %d: table is full.\n", result.id);
            }
            continue;
        }

        fprintf(stderr,
                "Invalid command. Usage: read <string>, write <string>, insert <id> <value>, or quit.\n");
    }

    return 0;
}
