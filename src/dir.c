//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "dir.h"

bool check_seed(struct snake_data* psnake, int code_step, int sx, int sy)
{
    for (int num = 0; psnake->length > num; num++)
    {
        int idx = (code_step % psnake->buffer_size) - num;
        if (psnake->x[idx] == sx)
        {
            if (psnake->y[idx] == sy)
            {
                return false;
            }
        }
    }
    return true;
}

void recreate_seed(struct map* pmap)
{
    pmap->seed_x = rand() % pmap->width;
    pmap->seed_y = rand() % pmap->height;
    if (check_seed(&pmap->snake, pmap->code_step, pmap->seed_x, pmap->seed_y) == false)
    {
        int seed_dir = rand() % 4;
        void(*seed_move)(struct map*);
        switch (seed_dir)
        {
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
        while (check_seed(&pmap->snake, pmap->code_step, pmap->seed_x, pmap->seed_y) == false)
        {
            (*seed_move)(pmap);
        }
        //pmap->seed_x = pmap->snake.x[pmap->code_step - pmap->snake.length - 1];
        //pmap->seed_y = pmap->snake.y[pmap->code_step - pmap->snake.length - 1];
    }
}

void seed_up(struct map* pmap)
{
    if (pmap->seed_y == 0)
    {
        pmap->seed_y = pmap->height - 1;
        pmap->seed_x = ((pmap->seed_x - 1) + pmap->width) % pmap->width;
    }
    else
    {
        pmap->seed_y--;
    }
}

void seed_right(struct map* pmap)
{
    if (pmap->seed_x == pmap->width - 1)
    {
        pmap->seed_x = 0;
        pmap->seed_y = ((pmap->seed_y - 1) + pmap->height) % pmap->height;
    }
    else
        pmap->seed_x++;
}

void seed_down(struct map* pmap)
{
    if (pmap->seed_y == pmap->height - 1)
    {
        pmap->seed_y = 0;
        pmap->seed_x = ((pmap->seed_x - 1) + pmap->width) % pmap->width;
    }
    else
        pmap->seed_y++;
}

void seed_left(struct map* pmap)
{
    if (pmap->seed_x == 0)
    {
        pmap->seed_x = pmap->width - 1;
        pmap->seed_y = ((pmap->seed_y - 1) + pmap->height) % pmap->height;
    }
    else
        pmap->seed_x--;
}

int cur_idx(struct map* pmap, int offset)
{
    return (pmap->code_step + offset) % pmap->snake.buffer_size;
}

bool dir_up(struct map* pmap)
{
    if(NULL == pmap)
        return false;

    if (pmap->snake.head_y != 0)
    {
        pmap->snake.x[cur_idx(pmap, 0)] = pmap->snake.head_x;
        pmap->snake.y[cur_idx(pmap, 0)] = pmap->snake.head_y;
        pmap->snake.head_y--;
        return true;
    }
    else
    {
        return false;
    }
}

bool dir_down(struct map* pmap)
{
    if(NULL == pmap)
        return false;

    if (pmap->snake.head_y != pmap->height - 1)
    {
        pmap->snake.x[cur_idx(pmap, 0)] = pmap->snake.head_x;
        pmap->snake.y[cur_idx(pmap, 0)] = pmap->snake.head_y;
        pmap->snake.head_y++;
        return true;
    }
    else
    {
        return false;
    }
}

bool dir_right(struct map* pmap)
{
    if(NULL == pmap)
        return false;

    if (pmap->snake.head_x != pmap->width - 1)
    {
        pmap->snake.x[cur_idx(pmap, 0)] = pmap->snake.head_x;
        pmap->snake.y[cur_idx(pmap, 0)] = pmap->snake.head_y;
        pmap->snake.head_x++;
        return true;
    }
    else
    {
        return false;
    }
}

bool dir_left(struct map* pmap)
{
    if(NULL == pmap)
        return false;

    if (pmap->snake.head_x != 0)
    {
        pmap->snake.x[cur_idx(pmap, 0)] = pmap->snake.head_x;
        pmap->snake.y[cur_idx(pmap, 0)] = pmap->snake.head_y;
        pmap->snake.head_x--;
        return true;
    }
    else
    {
        return false;
    }
}

bool dir_stop(struct map* pmap)
{
    if(NULL == pmap)
        return false;

    pmap->code_step--;
    return true;
}
