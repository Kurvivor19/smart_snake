//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "snake.h"

bool check_snake(char *display, struct map* pmap) {
	//int offset = pmap->seed_x*(pmap->width + 1) + pmap->seed_y;
	//int offset =  (pmap->seed_x + 1) + pmap->seed_y;
	int offset = ((pmap->width + 1) * pmap->seed_y) + pmap->seed_x;
	if (display[offset] == '0' || display[offset] == 'o' || display[offset] == '*') {
		return false;
	}
	else {
		return true;
	}
}

void seed_up(struct map* pmap) {
	if (pmap->seed_y == 0) {
		pmap->seed_y = pmap->height - 1;
		pmap->seed_x = ((pmap->seed_x - 1) + pmap->width) % pmap->width;
	}
	else {
		pmap->seed_y--;
	}
}

void seed_right(struct map* pmap) {
	if (pmap->seed_x == pmap->width - 1) {
		pmap->seed_x = 0;
		pmap->seed_y = ((pmap->seed_y - 1) + pmap->height) % pmap->height;
	}
	else
		pmap->seed_x++;
}

void seed_down(struct map* pmap) {
	if (pmap->seed_y == pmap->height - 1) {
		pmap->seed_y = 0;
		pmap->seed_x = ((pmap->seed_x - 1) + pmap->width) % pmap->width;
	}
	else
		pmap->seed_y++;
}

void seed_left(struct map* pmap) {
	if (pmap->seed_x == 0) {
		pmap->seed_x = pmap->width - 1;
		pmap->seed_y = ((pmap->seed_y - 1) + pmap->height) % pmap->height;
	}
	else
		pmap->seed_x--;
}

struct map game;

void create_map(struct map* pmap, int w, int h)
{
	pmap->width = w;
	pmap->height = h;
	game.snake.head_x = rand() % w;
	game.snake.head_y = rand() % h;
	born(&(pmap->snake), game.snake.head_x, game.snake.head_y);
	pmap->seed_x = rand() % w;
	pmap->seed_y = rand() % h;
	make_seed(&(pmap->snake), pmap->seed_x, pmap->seed_y);
}