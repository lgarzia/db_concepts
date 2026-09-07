#ifndef CLI_H
#define CLI_H

#include "table.h"

/*
 * The parser copies command arguments into a fixed-size result buffer.
 * Keeping the limit in one public constant makes the boundary visible to
 * callers and easy to exercise in tests.
 */
#define CLI_ARGUMENT_CAPACITY 256

typedef enum
{
    CLI_COMMAND_INVALID,
    CLI_COMMAND_READ,
    CLI_COMMAND_WRITE,
    CLI_COMMAND_QUIT,
    CLI_COMMAND_CONCEPTS,
    CLI_COMMAND_INSERT,
    CLI_COMMAND_DELETE,
    CLI_COMMAND_GET
} cli_command_type;

typedef struct
{
    cli_command_type type;
    char argument[CLI_ARGUMENT_CAPACITY];
    int id;
    char value[TABLE_VALUE_CAPACITY];
} cli_command_result;

/*
 * Parse one command line without reading input or producing output.
 * The caller owns the input string, and the returned result owns its copy
 * of the argument. A NULL input is treated as an invalid command.
 */
cli_command_result cli_parse_line(const char *line);

#endif
