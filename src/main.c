#include <Windows.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "curses.h"
#include "node.h"
#include "snake.h"

void fill_display(char* display, int width, int height);
void printf_display(char* display, int width, int height);
void printw_display(char* display, int width, int height, WINDOW *win);
void recreate_seed(char *display, struct map* pmap);
bool dir_left(int width, int height);
bool dir_right(int width, int height);
bool dir_up(int width, int height);
bool dir_down(int width, int height);
bool dir_stop(int width, int height);
//void help();
enum commands get_command(WINDOW* win);
struct map game;
HANDLE timer;
typedef bool(*move_func)(int, int);
//WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
int S;

int main(int arcc, const char*argv[])
{
	LARGE_INTEGER delay_time;
	delay_time.QuadPart = -5000000LL;
	timer = NULL;
	initscr();
	cbreak();
	noecho();
	//timeout(100);
	halfdelay(1);
	WINDOW *win_map = NULL;
	WINDOW *win_message = NULL;
	srand(time(NULL));
	bool life = true;

	//if (arcc != 3)
	//{
	//	puts("help:\n");
	//	puts("This is a simple game that named 'snake'\n");
	//	puts("Command line parameters:\n");
	//	puts("<width> <height>\n");
	//	return 0;
	//}

	int width = 4;
	int height = 4;
	char* display;

	S = width * height;
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
	mvwprintw(win_message, 1, 0, "%s%i%s%i", "length-", game.snake.length, " need ", S);
	//mvwprintw(win_message, 1, 9, "%s%i", "/", S);
	fill_display(display, width, height);
	printw_display(display, width, height, win_map);
	wrefresh(win_map);
	wrefresh(win_message);

	timer = CreateWaitableTimer(NULL, FALSE, NULL);
	SetWaitableTimer(timer, &delay_time, 2000, NULL, NULL, 0);
	while (S > game.snake.length)
	{
		//system("cls");
		newcom = get_command(win_map);
		if (newcom != cmd_NULL)
			lastcom = newcom;
		life = life && check_bite(&game.snake, game.code_step);
		if (WAIT_OBJECT_0 == WaitForSingleObject(timer, 0))
		{
			game.code_step++;
			if (life == true) {
				switch (lastcom)
				{
				case cmd_stop:
					dir_stop(width, height);
					game.snake.dir = lastcom;
					break;

				case cmd_NULL:
					life = (*movements[game.snake.dir])(width, height);
					// printw_display(display, width, height, win_map);
					break;

				default:
					life = (*movements[lastcom])(width, height);
					game.snake.dir = lastcom;
					break;
				}

				if (game.seed_x == game.snake.head_x) {
					if (game.seed_y == game.snake.head_y) {
						if (game.snake.length == (width * height) - 1) {
							printw_display(display, width, height, win_map);
							mvwprintw(win_message, 2, 0, "YOU WIN");
							break;
						}
						recreate_seed(display, &game);
					}
				}
				//		display = cursor;
				mvwprintw(win_message, 0, 0, "%s%i\n", "step-", game.code_step);
				mvwprintw(win_message, 1, 0, "%s%i%s%i", "length-", game.snake.length, " need ", S);
				//mvwprintw(win_message, 1, 9, "%s%i", "/", S);
				fill_display(display, width, height);
				printw_display(display, width, height, win_map);
				wrefresh(win_map);
				wrefresh(win_message);
				//printf_display(display, width, height);
				// DEBUG


			}//if
			else {
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
	wgetch(win_message);
	free(display);

	delwin(win_message);
	delwin(win_map);
	endwin();
	CloseHandle(timer);
	return 0;
}

void fill_display(char* display, int width, int height)
{
	//	printf_display(display, width, height);
	char* cursor = display;
	memset(display, 0, (width + 1) * height);
	int step = 0;


	for (int i = 0; height > i; i++) {		//printf("%s\n", display + width * i);		//putchar('\n');
		for (int l = 0; width > l; l++) {		//printf("%s\n", display + width * i);
			int time = i * 10 + l;
			*cursor = ('.');

			if (game.seed_x == l) {
				if (game.seed_y == i) {
					*cursor = ('*');
				}
			}

			//for (node_num = 1; node_num <= length; node_num++)
			for (int node_num = 1; game.snake.length > node_num; node_num++)
			{
				if (game.snake.x[game.code_step - node_num + 1] == l) {
					if (game.snake.y[game.code_step - node_num + 1] == i) {
						*cursor = ('o');
					}
				}
			}

			if (game.snake.head_x == l) {
				if (game.snake.head_y == i) {
					//game.place.x[game.code_step] = l;
					//game.place.y[game.code_step] = i;
					*cursor = ('0');
				}
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

void printw_display(char* display, int width, int height, WINDOW *win) {
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

void printf_display(char* display, int width, int height) {
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

bool dir_up(int width, int height) {
	if (game.snake.head_y != 0) {
		game.snake.x[game.code_step] = game.snake.head_x;
		game.snake.y[game.code_step] = game.snake.head_y;
		game.snake.head_y--;
		return true;
	}
	else {
		return false;
	}
}

bool dir_down(int width, int height) {
	if (game.snake.head_y != height - 1) {
		game.snake.x[game.code_step] = game.snake.head_x;
		game.snake.y[game.code_step] = game.snake.head_y;
		game.snake.head_y++;
		return true;
	}
	else {
		return false;
	}
}

bool dir_right(int width, int height) {
	if (game.snake.head_x != width - 1) {
		game.snake.x[game.code_step] = game.snake.head_x;
		game.snake.y[game.code_step] = game.snake.head_y;
		game.snake.head_x++;
		return true;
	}
	else {
		return false;
	}
}

bool dir_left(int width, int height) {
	if (game.snake.head_x != 0) {
		game.snake.x[game.code_step] = game.snake.head_x;
		game.snake.y[game.code_step] = game.snake.head_y;
		game.snake.head_x--;
		return true;
	}
	else {
		return false;
	}
}

bool dir_stop(int width, int height) {
	game.code_step--;
	return true;
}

enum commands get_command(WINDOW* win)
{
	keypad(win, TRUE);
	//char symbol = wgetch(win);
	int symbol = wgetch(win);
	//if(ERR != symbol)
	//	rewind(stdin);
	//static char buf = ' ';
	int buf = ' ';
char_switch:
	switch (symbol)
	{

	case KEY_UP:
		buf = 'w';
		return cmd_up;

	case KEY_DOWN:
		buf = 's';
		return cmd_down;

	case KEY_LEFT:
		buf = 'a';
		return cmd_left;

	case KEY_RIGHT:
		buf = 'd';
		return cmd_right;

	case 'w':
		buf = symbol;
		return cmd_up;

	case 'a':
		buf = symbol;
		return cmd_left;

	case 's':
		buf = symbol;
		return cmd_down;

	case 'd':
		buf = symbol;
		return cmd_right;

	case 'q':
		buf = symbol;
		mvwprintw(win, 0, 0, "pause");
		mvwprintw(win, 1, 0, "press w or arrow_up to go up");
		mvwprintw(win, 2, 0, "press a or arrow_left to go left");
		mvwprintw(win, 3, 0, "press s or arrow_down to go down");
		mvwprintw(win, 4, 0, "press d or arrow_right to go right");
		mvwprintw(win, 5, 0, "press any button to continue");
		wgetch(win);
		return cmd_stop;

	case ' ': 
		if (buf == ' ')
			return cmd_NULL;

	case ERR:
		return cmd_NULL;

	default:
		symbol = buf;
		goto char_switch;
		//return cmd_stop;
	}
} //препятствия(читать поле из текстового файла)
