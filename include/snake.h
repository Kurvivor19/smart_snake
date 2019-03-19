#ifndef _SNAKE_H_ 
#define _SNAKE_H_
#include <stdbool.h>

enum commands
{
    cmd_left,    //0
    cmd_right,   //1
    cmd_up,      //2
    cmd_down,    //3
    cmd_stop,    //4
    cmd_NULL,    //5
    cmd_help,    //6
    cmd_pause,   //7
};

enum directions
{
    direc_left = cmd_left,    //0
    direc_right = cmd_right,  //1
    direc_up = cmd_up,        //2
    direc_down = cmd_down,    //3
    direc_stop = cmd_stop,    //4
};

struct snake_data
{
    int* x;
    int* y;
    int length;
    int buffer_size;
    enum directions dir;
    int head_x, head_y;
};

struct map
{
    int width, height;
    struct snake_data snake;
    int nseed, code_step;
    enum commands lastcom;
    int seed_x, seed_y;
};

/// x, y - positions of the snake's head
void born(struct snake_data* Psnake, int x, int y, int max_len);

void create_map(struct map* pmap, int w, int h);
void clear_map(struct map* pmap);

bool check_seed(struct snake_data* psnake, int code_step, int sx, int sy);

bool check_bite(struct snake_data* Pnake, int code_step);
bool check_eat(struct snake_data* Pnake, int x, int y);

#endif // _SNAKE_H_
