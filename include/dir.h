#ifndef DIR_H
#define DIR_H

#include <stdbool.h>
#include "snake.h"

bool check_seed(struct snake_data* psnake, int code_step, int sx, int sy);
void recreate_seed(struct map* pmap);

void seed_up(struct map* pmap);
void seed_right(struct map* pmap);
void seed_down(struct map* pmap);
void seed_left(struct map* pmap);

int cur_idx(struct map* pmap, int offset);

bool dir_up(struct map* pmap);
bool dir_down(struct map* pmap);
bool dir_right(struct map* pmap);
bool dir_left(struct map* pmap);
bool dir_stop(struct map* pmap);

typedef bool (*move_func)(struct map* pmap);

#endif /* DIR_H */
