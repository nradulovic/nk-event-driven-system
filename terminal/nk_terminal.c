/*
 * nk_terminal.c
 *
 *  Created on: Apr 29, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 */

#include <stdint.h>

#include "generic/nk_string.h"
#include "generic/terminal/nk_terminal.h"

static void
process_command(struct terminal_descriptor *terminal, const struct nk_string *command_string, struct nk_string *output)
{

}

void
terminal__init(struct terminal_descriptor *terminal,
               const struct terminal_commands *commands,
               struct nk_string *arg_buffer,
               struct nk_string *error_message)
{
    terminal->p__commands = commands;
    terminal->p__working_buffer = arg_buffer;
    terminal->p__error_message = error_message;
}

void
terminal__interpret(struct terminal_descriptor *terminal, const struct nk_string *input, struct nk_string *output)
{
    struct nk_string__find__result find_n;
    struct nk_string__find__result find_r;
    struct nk_string__find__result find_or;
    struct
        NK_STRING__BUCKET_T(1)
    enter_r = NK_STRING__BUCKET_INITIALIZER(&enter_r, "\r");
    struct
        NK_STRING__BUCKET_T(1)
    enter_n = NK_STRING__BUCKET_INITIALIZER(&enter_n, "\n");

    nk_string__append(terminal->p__working_buffer, input);
    nk_string__copy(output, input);

    /* See if the user pressed enter */
    find_n = nk_string__find(terminal->p__working_buffer, &enter_n.array, 0, SIZE_MAX);
    find_r = nk_string__find(terminal->p__working_buffer, &enter_r.array, 0, SIZE_MAX);

    if ((find_n.error == NK_ERROR__OK) && (find_r.error == NK_ERROR__OK)) {
        /* Both finds found a character */
        find_or.error = NK_ERROR__OK;
        find_or.value = MIN(find_n.value, find_r.value);
    } else if (find_n.error == NK_ERROR__OK) {
        find_or = find_n;
    } else if (find_r.error == NK_ERROR__OK) {
        find_or = find_r;
    } else {
        find_or.error = NK_ERROR__NOT_FOUND;
        find_or.value = 0u;
    }

    if (find_or.error == NK_ERROR__OK) {
        struct nk_string command_string;
        /* Found '\n' in the string */
        command_string = nk_string__view(terminal->p__working_buffer, 0, find_or.value);
        process_command(terminal, &command_string, output);
        nk_string__lstrip(terminal->p__working_buffer, &command_string);
    }
}

void
terminal__discard_command(struct terminal_descriptor *terminal)
{
    nk_string__clear_all(terminal->p__working_buffer);
}

void
terminal__set_terminal_context(struct terminal_descriptor *terminal, void *terminal_context)
{
    terminal->p__terminal_context = terminal_context;
}
