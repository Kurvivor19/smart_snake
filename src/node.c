//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "snake.h"

struct node* make_node(int x, int y)
{
	struct node* res = (struct node*)malloc(sizeof(struct node));
	res->data.x = x;
	res->data.y = y;
	res->tail = 0;
	return res;
}

struct seed* make_seed(int x, int y)
{
	struct seed* res = (struct seed*)malloc(sizeof(struct seed));
	res->data.x = x;
	res->data.y = y;
	return res;
}

void recreate_seed(char *display, struct map* pmap) {
	pmap->seed_x = rand() % pmap->width;
	pmap->seed_y = rand() % pmap->height;
	if (check_snake(display, pmap) == false) {
		int seed_dir = rand() % 4;
		void(*seed_move)(struct map*);
		switch (seed_dir) {
		case 1:
			seed_move = &seed_up;
			break;

		case 2:
			seed_move = &seed_down;
			break;

		case 3:
			seed_move = &seed_left;
			break;

		default:
			seed_move = &seed_right;
			break;
		}
		while (check_snake(display, pmap) == false) {
			(*seed_move)(pmap);
		}
		//pmap->seed_x = pmap->snake.x[pmap->code_step - pmap->snake.length - 1];
		//pmap->seed_y = pmap->snake.y[pmap->code_step - pmap->snake.length - 1];
	}
	pmap->snake.length++;// = 3;
}
