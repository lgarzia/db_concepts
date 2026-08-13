#include "cli.h"

#include <stddef.h>
#include <string.h>

static cli_command_result invalid_result(void)
{
    cli_command_result result = {CLI_COMMAND_INVALID, ""};
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
