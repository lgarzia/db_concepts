#include <assert.h>
#include <string.h>

#include "cli.h"

static void test_read_command(void)
{
    cli_command_result result = cli_parse_line("read hello\n");

    assert(result.type == CLI_COMMAND_READ);
    assert(strcmp(result.argument, "hello") == 0);
}

static void test_write_command_preserves_spaces(void)
{
    cli_command_result result = cli_parse_line("write hello   world\r\n");

    assert(result.type == CLI_COMMAND_WRITE);
    assert(strcmp(result.argument, "hello   world") == 0);
}

static void test_quit_command(void)
{
    cli_command_result result = cli_parse_line("quit");

    assert(result.type == CLI_COMMAND_QUIT);
    assert(result.argument[0] == '\0');
}

static void test_concepts_command(void)
{
    cli_command_result result = cli_parse_line("concepts");

    assert(result.type == CLI_COMMAND_CONCEPTS);
    assert(result.argument[0] == '\0');
}

static void test_invalid_commands(void)
{
    assert(cli_parse_line("").type == CLI_COMMAND_INVALID);
    assert(cli_parse_line("unknown value").type == CLI_COMMAND_INVALID);
    assert(cli_parse_line("read").type == CLI_COMMAND_INVALID);
    assert(cli_parse_line("write \t").type == CLI_COMMAND_INVALID);
    assert(cli_parse_line("QUIT").type == CLI_COMMAND_INVALID);
    assert(cli_parse_line(NULL).type == CLI_COMMAND_INVALID);
}

static void test_argument_boundary(void)
{
    char line[CLI_ARGUMENT_CAPACITY + 6];
    cli_command_result result;
    size_t index;

    memcpy(line, "read ", 5);
    for (index = 5; index < CLI_ARGUMENT_CAPACITY + 5; index++)
    {
        line[index] = 'x';
    }
    line[CLI_ARGUMENT_CAPACITY + 5] = '\0';

    result = cli_parse_line(line);
    assert(result.type == CLI_COMMAND_INVALID);
}

int main(void)
{
    test_read_command();
    test_write_command_preserves_spaces();
    test_quit_command();
    test_concepts_command();
    test_invalid_commands();
    test_argument_boundary();
    return 0;
}
