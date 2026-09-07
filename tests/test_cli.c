#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <io.h>
#define DUP _dup
#define DUP2 _dup2
#define FILE_NO _fileno
#else
#include <unistd.h>
#define DUP dup
#define DUP2 dup2
#define FILE_NO fileno
#endif

#include "cli.h"

#define main cli_main
#include "../src/main.c"
#undef main

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

static void test_insert_command_parses_id_and_value(void)
{
    cli_command_result result = cli_parse_line("insert 42 example");

    /* A valid insert exposes its parsed ID and copied value to the caller. */
    assert(result.type == CLI_COMMAND_INSERT);
    assert(result.id == 42);
    assert(strcmp(result.value, "example") == 0);
}

static void test_insert_command_preserves_signed_id_and_spaces(void)
{
    cli_command_result result = cli_parse_line("insert -1 value with spaces\r\n");

    /* Signed IDs and internal value whitespace must survive CRLF parsing. */
    assert(result.type == CLI_COMMAND_INSERT);
    assert(result.id == -1);
    assert(strcmp(result.value, "value with spaces") == 0);
}

static void test_delete_command_parses_id(void)
{
    cli_command_result result = cli_parse_line("delete 42");

    /* A valid delete exposes its parsed ID so the session can remove that record. */
    assert(result.type == CLI_COMMAND_DELETE);
    assert(result.id == 42);
}

static void test_delete_missing_id_reports_not_found(void)
{
    FILE *input = tmpfile();
    FILE *stdout_capture = tmpfile();
    FILE *stderr_capture = tmpfile();
    char stdout_buffer[256] = {0};
    char stderr_buffer[256] = {0};
    int saved_stdin = DUP(FILE_NO(stdin));
    int saved_stdout = DUP(FILE_NO(stdout));
    int saved_stderr = DUP(FILE_NO(stderr));

    fputs("delete 999\nquit\n", input);
    rewind(input);

    DUP2(FILE_NO(input), FILE_NO(stdin));
    DUP2(FILE_NO(stdout_capture), FILE_NO(stdout));
    DUP2(FILE_NO(stderr_capture), FILE_NO(stderr));

    cli_main();

    fflush(stdout);
    fflush(stderr);
    rewind(stdout_capture);
    rewind(stderr_capture);

    if (fgets(stdout_buffer, sizeof(stdout_buffer), stdout_capture) != NULL)
    {
        /* consume output so the read pointer is positioned for the test assertion */
    }
    if (fgets(stderr_buffer, sizeof(stderr_buffer), stderr_capture) != NULL)
    {
        /* consume output so the read pointer is positioned for the test assertion */
    }

    assert(strstr(stdout_buffer, "not found") != NULL ||
           strstr(stderr_buffer, "not found") != NULL);

    DUP2(saved_stdin, FILE_NO(stdin));
    DUP2(saved_stdout, FILE_NO(stdout));
    DUP2(saved_stderr, FILE_NO(stderr));
    fclose(input);
    fclose(stdout_capture);
    fclose(stderr_capture);
    close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);
}

static void test_get_command_parses_id(void)
{
    cli_command_result result = cli_parse_line("get 42");

    /* A valid get exposes its parsed ID so the session can retrieve that record. */
    assert(result.type == CLI_COMMAND_GET);
    assert(result.id == 42);
}

static void test_list_command_parses_without_arguments(void)
{
    cli_command_result result = cli_parse_line("list");

    /* The list command should be recognized without trailing arguments and reject extra input. */
    assert(result.type == CLI_COMMAND_LIST);
    assert(cli_parse_line("list extra").type == CLI_COMMAND_INVALID);
}

static void test_const_input_buffer(void)
{
    const char line[] = "read immutable input";
    cli_command_result result = cli_parse_line(line);

    assert(result.type == CLI_COMMAND_READ);
    assert(strcmp(result.argument, "immutable input") == 0);
    assert(strcmp(line, "read immutable input") == 0);
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
    test_insert_command_parses_id_and_value();
    test_insert_command_preserves_signed_id_and_spaces();
    test_delete_command_parses_id();
    test_delete_missing_id_reports_not_found();
    test_get_command_parses_id();
    test_list_command_parses_without_arguments();
    test_const_input_buffer();
    test_quit_command();
    test_concepts_command();
    test_invalid_commands();
    test_argument_boundary();
    return 0;
}
