#include <curses.h>
#include "game.h"
#include "util.h"
#include "dir.h"

enum commands get_command(WINDOW* win);

struct snake_display
{
    WINDOW* field;
    WINDOW* msg;
    char * display;
};

void fill_display(char* display, int width, int height);
void printf_display(char* display, int width, int height);
void printw_display(char* display, int width, int height, WINDOW *win);
void printw_help(WINDOW* win);
void printw_death(WINDOW* win, int life, int length);
void printw_win(WINDOW* win, int life, int length);

enum game_states snake_cycle(struct map* pgame, dynamic_settings* config, void* context)
{
    struct snake_display* snake_win = (struct snake_display*) context;
    int newcom = get_command(snake_win->field);
    static move_func movements[cmd_stop + 1] = {
        &dir_left,
        &dir_right,
        &dir_up,
        &dir_down,
        &dir_stop
    };
    static bool life = true;

    if (wait_for_timer_pulse(0))
    {
        pgame->code_step++;
        switch (newcom)
        {
        case cmd_stop:
            dir_stop(pgame);
            pgame->snake.dir = cmd_stop;
            break;

        case cmd_skip:
        case cmd_NULL:
            life = (*movements[pgame->snake.dir])(pgame);
            break;

        case cmd_quit:
            return STATE_DEATH;

        case cmd_pause:
            return STATE_CONSOLE;

        case cmd_help:
            return STATE_HELP;
            
        default:
            life = (*movements[newcom])(pgame);
            pgame->snake.dir = newcom;
            break;
        }
        life = life && check_bite(&pgame->snake, pgame->code_step);
        if (life == false)
            return STATE_DEATH;

        if (check_eat(&pgame->snake, pgame->seed_x, pgame->seed_y))
        {
            if (pgame->snake.length == (pgame->width * pgame->height) - 1)
                return STATE_WIN;
            recreate_seed(pgame);
        }
    }
    if (life)
    {
        mvwprintw(snake_win->msg, 0, 0, "%s%i\n", "step-", pgame->code_step);
        mvwprintw(snake_win->msg, 1, 0, "%s%i%s%i", "length-", pgame->snake.length, " need ", config->winning_length);
        
        fill_display(snake_win->display, pgame->width, pgame->height);
        printw_display(snake_win->display, pgame->width, pgame->height, snake_win->field);
        wrefresh(snake_win->field);
        wrefresh(snake_win->msg);
    }
    else
    {
        printw_display(snake_win->display, pgame->width, pgame->height, snake_win->field);
        return STATE_DEATH;
    }
    return STATE_PLAY;
}

enum game_states help_cycle(struct map* game, dynamic_settings* config, void* context)
{
    WINDOW* help_window = (WINDOW*) context;
    wait_for_timer_pulse(10);
    printw_help(help_window);
    wrefresh(help_window);
    wgetch(help_window);
    return STATE_PLAY;
}

enum game_states death_cycle(struct map* pgame, dynamic_settings* config, void* context)
{
    WINDOW* help_window = (WINDOW*) context;
    wait_for_timer_pulse(10);
    printw_death(help_window, pgame->code_step, pgame->snake.length);
    wrefresh(help_window);
    wgetch(help_window);
    return STATE_END;
}

enum game_states win_cycle(struct map* pgame, dynamic_settings* config, void* context)
{
    WINDOW* help_window = (WINDOW*) context;
    wait_for_timer_pulse(10);
    printw_death(help_window, pgame->code_step, pgame->snake.length);
    wrefresh(help_window);
    wgetch(help_window);
    return STATE_END;
}
