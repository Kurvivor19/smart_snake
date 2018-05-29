//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "snake.h"

void born(struct snake_data* psnake, int x, int y, int max_len)
{
    psnake->length = 1;
    psnake->dir = direc_stop;
    psnake->buffer_size = 10 + max_len;
    psnake->x = (int*)malloc(sizeof(int) * psnake->buffer_size);
    psnake->y = (int*)malloc(sizeof(int) * psnake->buffer_size);
}

//void unmake_nodes(struct node* p)
//{
//	struct node* temp;
//	if (p == 0)
//		return;
//	temp = p->tail;
//	free(p);
//	unmake_nodes(temp);
//}

//void insert_after(struct node* p, int x, int y)
//{
//	struct node* temp;
//	temp = make_node(x, y);
//	temp->tail = p->tail;
//	p->tail = temp;
//}

bool check_bite(struct snake_data* psnake, int code_step) {
    for (int num = 1; psnake->length > num; num++)
    {
        if (psnake->x[(code_step % psnake->buffer_size) - num] == psnake->head_x) {
            if (psnake->y[(code_step % psnake->buffer_size) - num] == psnake->head_y) {
                return false;
            }
        }
    }
    return true;
};

void create_map(struct map* pmap, int w, int h)
{
    pmap->width = w;
    pmap->height = h;
    pmap->snake.head_x = rand() % w;
    pmap->snake.head_y = rand() % h;
    born(&(pmap->snake), pmap->snake.head_x, pmap->snake.head_y, w * h);
    pmap->seed_x = rand() % w;
    pmap->seed_y = rand() % h;
}
