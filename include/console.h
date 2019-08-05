#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>
#include <curses.h>
#include <ecl/ecl.h>

struct console_state
{
    // Used to signal "no more input" after feeding a character to readline
    bool input_avail;
    // Input character for readline
    int input;
    WINDOW* msg_win;
    WINDOW* cmd_win;
    char* msg_win_str;
    bool should_exit;
    cl_object echo_func;
};

void set_initial_console_state(struct console_state* pstate);
void deinit_readline(void);
bool init_readline(void);

#endif // CONSOLE_H
