#include <curses.h>
#include "game.h"

enum game_states console_cycle(struct map* game, dynamic_settings* config, void* context)
{
    return STATE_CONSOLE;
}
