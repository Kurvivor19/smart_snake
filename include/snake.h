#ifndef _SNAKE_H_ 
#define _SNAKE_H_
#include <stdbool.h>
#include "node.h"

enum commands
{
	cmd_left,   //0
	cmd_right,  //1
	cmd_up,     //2
	cmd_down,   //3
	cmd_stop,    //5
	cmd_NULL,    //4
	cmd_help,
};

enum directions
{
	direc_left = cmd_left,   //0
	direc_right = cmd_right,  //1
	direc_up = cmd_up,     //2
	direc_down = cmd_down,   //3
	direc_stop = cmd_stop,    //4
};

struct snake_data
{
	int x[1000];
	int y[1000];
	int length;
	//struct node* head;
	enum directions dir;
	int head_x, head_y;
};

bool check_bite(struct snake_data* Pnake, int code_step);

struct map
{
	int width, height;
	struct snake_data snake;
	struct coord seed;
	int nseed, code_step;
	enum commands lastcom;
	int seed_x, seed_y;
};

/// x, y - positions of the snake's head
void born(struct snake_data* Psnake, int x, int y);

void create_map(struct map* pmap, int w, int h);

void create_seed(struct map* pmap, int w, int h);

bool check_snake(char *display, struct map* pmap);

void seed_up(struct map* pmap);
void seed_left(struct map* pmap);
void seed_down(struct map* pmap);
void seed_right(struct map* pmap);

#endif // _SNAKE_H_