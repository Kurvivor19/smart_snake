//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "node.h"
#include "snake.h"

struct map game;
bool check_bite(struct snake_data* Pnake, int code_step);

void born(struct snake_data* Psnake, int x, int y)
{
	Psnake->length = 1;
	//Psnake->head = make_node(x, y);
	Psnake->dir = direc_stop;
}

void unmake_nodes(struct node* p)
{
	struct node* temp;
	if (p == 0)
		return;
	temp = p->tail;
	free(p);
	unmake_nodes(temp);
}


void insert_after(struct node* p, int x, int y)
{
	struct node* temp;
	temp = make_node(x, y);
	temp->tail = p->tail;
	p->tail = temp;
}

bool check_bite(struct snake_data* Psnake, int code_step) {
	for (int num = 1; Psnake->length > num; num++)
	{
		if (game.snake.x[game.code_step - num] == game.snake.head_x) {
			if (game.snake.y[game.code_step - num] == game.snake.head_y) {
				return false;
			}
		}
	}
	return true;
};
