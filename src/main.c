#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#include <glib.h>
#include <curses.h>

#include "snake.h"
#include "dir.h"
#include "util.h"
#include "config.h"

void fill_display(char* display, int width, int height);
void printf_display(char* display, int width, int height);
void printw_display(char* display, int width, int height, WINDOW *win);
struct snake_display
{
    WINDOW* field;
    WINDOW* msg;
    char* display;
};

void help();
enum commands get_command(WINDOW* win);
struct map game;

int main(int arcc, const char*argv[])
{
    if (arcc != 3)
    {
        help();
        return 0;
    }

    initscr();
    cbreak();
    noecho();

    // TODO: uncomment for movement by timer
    // halfdelay(1);

    WINDOW *win_map = NULL;
    WINDOW *win_message = NULL;
    srand(time(NULL));
    bool life = true;

    init_config();

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    char* display;

    dynamic_settings dset = {0, 400};
    dset.winning_length = width * height;

    create_map(&game, width, height);
    display = malloc((width + 1) * height);

    // creating concole windows
    win_map = newwin(height + 2, width + 2, 3, 0);
    win_message = newwin(3, 20, 0, 0);
	
    move_func movements[cmd_stop + 1] = {
        &dir_left,
        &dir_right,
        &dir_up,
        &dir_down,
        &dir_stop
    };
    enum commands lastcom = cmd_NULL;
    enum commands newcom;
    mvwprintw(win_message, 0, 0, "%s%i\n", "step-", game.code_step);
    mvwprintw(win_message, 1, 0, "%s%i%s%i", "length-", game.snake.length, " need ", dset.winning_length);

    fill_display(display, width, height);
    printw_display(display, width, height, win_map);
    wrefresh(win_map);
    wrefresh(win_message);

    init_timer();
    setup_timer(dset.timer_msec_interval);
    while (dset.winning_length > game.snake.length)
    {
        //system("cls");
        newcom = get_command(win_map);
        if (newcom != cmd_NULL)
            lastcom = newcom;
        if (wait_for_timer_pulse(0))
        {
            game.code_step++;
            if (life == true)
            {
                switch (lastcom)
                {
                case cmd_stop:
                    dir_stop(&game);
                    game.snake.dir = lastcom;
                    break;

                case cmd_NULL:
                    life = (*movements[game.snake.dir])(&game);
                    // printw_display(display, width, height, win_map);
                    break;

                default:
                    life = (*movements[lastcom])(&game);
                    game.snake.dir = lastcom;
                    break;
                }
                life = life && check_bite(&game.snake, game.code_step);

                if (game.seed_x == game.snake.head_x)
                {
                    if (game.seed_y == game.snake.head_y)
                    {
                        if (game.snake.length == (width * height) - 1)
                        {
                            printw_display(display, width, height, win_map);
                            mvwprintw(win_message, 2, 0, "YOU WIN");
                            break;
                        }
                        recreate_seed(&game);
                    }
                }
                //		display = cursor;
                mvwprintw(win_message, 0, 0, "%s%i\n", "step-", game.code_step);
                mvwprintw(win_message, 1, 0, "%s%i%s%i", "length-", game.snake.length, " need ", dset.winning_length);

                fill_display(display, width, height);
                printw_display(display, width, height, win_map);
                wrefresh(win_map);
                wrefresh(win_message);
                //printf_display(display, width, height);
            }
            if (life == false)
            {
                printw_display(display, width, height, win_map);
                //printf_display(display, width, height);
                mvwprintw(win_message, 2, 0, "YOU DEAD");
                break;
            }
            lastcom = cmd_NULL;
        }
    } // while
    wrefresh(win_map);
    wrefresh(win_message);
    // back to breaking to ensure pause
    nocbreak();
    wgetch(win_message);
    free(display);
    clear_map(&game);
    delwin(win_message);
    delwin(win_map);
    endwin();
    clear_timer();
    free_config();
    return 0;
}

void help()
{
    puts("help:\n");
    puts("This is a simple game that named 'snake'\n");
    puts("Command line parameters:\n");
    puts("<width> <height>\n");
}

void fill_display(char* display, int width, int height)
{
    //	printf_display(display, width, height);
    char* cursor = display;
    memset(display, 0, (width + 1) * height);
    int step = 0;


    for (int i = 0; height > i; i++)
    {
        for (int l = 0; width > l; l++)
        {
            int time = i * 10 + l;
            *cursor = ('.');

            if (game.seed_x == l)
            {
                if (game.seed_y == i)
                {
                    *cursor = ('*');
                }
            }

            //for (node_num = 1; node_num <= length; node_num++)
            for (int node_num = 1; game.snake.length > node_num; node_num++)
            {
                if (game.snake.x[cur_idx(&game, 1 - node_num)] == l)
                {
                    if (game.snake.y[cur_idx(&game, 1 - node_num)] == i)
                        *cursor = ('o');
                }
            }

            if (game.snake.head_x == l)
            {
                if (game.snake.head_y == i)
                    *cursor = ('0');
            }

            cursor++;
            step++;
            //int offset = i * (width + 1) + l;
            //int offset = (i + 1) + l;
            int offset = (width * i) + l;
        }
        cursor++;
        //step++;
    }
    //printf("%i%i", game.place.x[2], game.place.y[2]);
}

void printw_display(char* display, int width, int height, WINDOW *win)
{
    char* line = malloc(width + 1);
    memset(line, '-', width);
    line[width] = 0;
    mvwprintw(win, 0, 0, "%s\n", line);
    for (int i = 0; i <= height; ++i)
        wprintw(win, " %s\n", display + (width + 1) * i);

    wprintw(win, "%s\n", line);
    free(line);
    box(win, 0, 0);
}

void printf_display(char* display, int width, int height)
{
    //printf("%s%i\n", "step-", game.code_step);
    //printf("%s%i\n", "length-", game.snake.length);
    char* line = malloc(width + 1);
    memset(line, '-', width);
    line[width] = 0;
    printf("+%s+\n", line);
    for (int i = 0; i < height; ++i)
        printf("|%s|\n", display + (width + 1) * i);

    printf("+%s+\n", line);
    free(line);
}

void printw_help(WINDOW* win)
{
    mvwprintw(win, 0, 0, "pause");
    mvwprintw(win, 1, 0, "press w or arrow_up to go up");
    mvwprintw(win, 2, 0, "press a or arrow_left to go left");
    mvwprintw(win, 3, 0, "press s or arrow_down to go down");
    mvwprintw(win, 4, 0, "press d or arrow_right to go right");
    mvwprintw(win, 5, 0, "press any button to continue");
    wgetch(win);
}

enum commands get_command(WINDOW* win)
{
    keypad(win, TRUE);
    int symbol = wgetch(win);
    return get_command_for_code(symbol);
}
