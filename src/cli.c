#include "cli.h"

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static cli_command_result invalid_result(void)
{
    cli_command_result result = {CLI_COMMAND_INVALID, "", 0, ""};
    return result;
}

static cli_command_result parse_insert(const char *argument, size_t argument_length)
{
    cli_command_result result = invalid_result();
    const char *id_start = argument;
    const char *id_end = argument;
    const char *value_start;
    char id_buffer[32];
    size_t id_length;
    size_t value_length;
    long parsed_id;
    char *endptr;

    while ((size_t)(id_end - argument) < argument_length &&
           *id_end != ' ' && *id_end != '\t')
    {
        id_end++;
    }

    id_length = (size_t)(id_end - id_start);

    /* The ID must be followed by a separator so a value can still be present. */
    if (id_length == 0 || id_length >= sizeof(id_buffer) ||
        (size_t)(id_end - argument) >= argument_length)
    {
        return result;
    }

    memcpy(id_buffer, id_start, id_length);
    id_buffer[id_length] = '\0';

    errno = 0;
    parsed_id = strtol(id_buffer, &endptr, 10);
    if (endptr == id_buffer || *endptr != '\0' || errno == ERANGE ||
        parsed_id < INT_MIN || parsed_id > INT_MAX)
    {
        return result;
    }

    value_start = id_end;
    while ((size_t)(value_start - argument) < argument_length &&
           (*value_start == ' ' || *value_start == '\t'))
    {
        value_start++;
    }

    value_length = argument_length - (size_t)(value_start - argument);
    if (value_length == 0 || value_length >= TABLE_VALUE_CAPACITY)
    {
        return result;
    }

    result.type = CLI_COMMAND_INSERT;
    result.id = (int)parsed_id;
    memcpy(result.value, value_start, value_length);
    result.value[value_length] = '\0';

    return result;
}

static cli_command_result parse_delete(const char *argument, size_t argument_length)
{
    cli_command_result result = invalid_result();
    const char *id_end = argument;
    const char *trailing;
    char id_buffer[32];
    size_t id_length;
    long parsed_id;
    char *endptr;

    while ((size_t)(id_end - argument) < argument_length &&
           *id_end != ' ' && *id_end != '\t')
    {
        id_end++;
    }

    id_length = (size_t)(id_end - argument);
    if (id_length == 0 || id_length >= sizeof(id_buffer))
    {
        return result;
    }

    trailing = id_end;
    while ((size_t)(trailing - argument) < argument_length &&
           (*trailing == ' ' || *trailing == '\t'))
    {
        trailing++;
    }

    if ((size_t)(trailing - argument) != argument_length)
    {
        return result;
    }

    memcpy(id_buffer, argument, id_length);
    id_buffer[id_length] = '\0';

    errno = 0;
    parsed_id = strtol(id_buffer, &endptr, 10);
    if (endptr == id_buffer || *endptr != '\0' || errno == ERANGE ||
        parsed_id < INT_MIN || parsed_id > INT_MAX)
    {
        return result;
    }

    result.type = CLI_COMMAND_DELETE;
    result.id = (int)parsed_id;
    return result;
}

static int starts_with_command(const char *line,
                               size_t line_length,
                               const char *command)
{
    size_t command_length = strlen(command);

    /*
     * A command is valid only when the keyword is followed by whitespace.
     * This prevents "readable" from being mistaken for "read".
     */
    return strncmp(line, command, command_length) == 0 &&
           command_length < line_length &&
           (line[command_length] == ' ' || line[command_length] == '\t');
}

static int copy_argument(const char *argument,
                         size_t argument_length,
                         cli_command_result *result)
{
    /*
     * Reject an argument that cannot fit, rather than silently truncating
     * user input. The final byte is reserved for the null terminator.
     */
    if (argument_length == 0 || argument_length >= CLI_ARGUMENT_CAPACITY)
    {
        return 0;
    }

    memcpy(result->argument, argument, argument_length);
    result->argument[argument_length] = '\0';
    return 1;
}

cli_command_result cli_parse_line(const char *line)
{
    cli_command_result result = invalid_result();
    const char *argument;
    size_t line_length;

    if (line == NULL)
    {
        return result;
    }

    /*
     * fgets() normally leaves the newline in its buffer. Removing both
     * newline characters makes the parser work with Unix and Windows input.
     */
    line_length = strlen(line);
    while (line_length > 0 &&
           (line[line_length - 1] == '\n' || line[line_length - 1] == '\r'))
    {
        line_length--;
    }

    if (line_length == 0)
    {
        return result;
    }

    if (line_length == strlen("quit") && strncmp(line, "quit", line_length) == 0)
    {
        result.type = CLI_COMMAND_QUIT;
        return result;
    }

    if (line_length == strlen("concepts") && strncmp(line, "concepts", line_length) == 0)
    {
        result.type = CLI_COMMAND_CONCEPTS;
        return result;
    }

    if (starts_with_command(line, line_length, "insert"))
    {
        argument = line + strlen("insert");
        while ((size_t)(argument - line) < line_length &&
               (*argument == ' ' || *argument == '\t'))
        {
            argument++;
        }

        return parse_insert(argument, line_length - (size_t)(argument - line));
    }

    if (starts_with_command(line, line_length, "delete"))
    {
        argument = line + strlen("delete");
        while ((size_t)(argument - line) < line_length &&
               (*argument == ' ' || *argument == '\t'))
        {
            argument++;
        }

        return parse_delete(argument, line_length - (size_t)(argument - line));
    }

    if (starts_with_command(line, line_length, "read"))
    {
        result.type = CLI_COMMAND_READ;
    }
    else if (starts_with_command(line, line_length, "write"))
    {
        result.type = CLI_COMMAND_WRITE;
    }
    else
    {
        return result;
    }

    /* Skip the separator so the argument begins at its first meaningful byte. */
    argument = line + (result.type == CLI_COMMAND_READ ? 4 : 5);
    while ((size_t)(argument - line) < line_length &&
           (*argument == ' ' || *argument == '\t'))
    {
        argument++;
    }

    if (!copy_argument(argument, line_length - (size_t)(argument - line), &result))
    {
        return invalid_result();
    }

    return result;
}
