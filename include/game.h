#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "config.h"

enum game_states
{
    STATE_PLAY,
    STATE_HELP,
    STATE_DEATH,
    STATE_WIN,
    STATE_CONSOLE,
    STATE_END
};

typedef enum game_states (*transition)(struct map* game, dynamic_settings* config, void* context);

enum game_states snake_cycle(struct map* game, dynamic_settings* config, void* context);
enum game_states help_cycle(struct map* game, dynamic_settings* config, void* context);
enum game_states console_cycle(struct map* game, dynamic_settings* config, void* context);
enum game_states death_cycle(struct map* game, dynamic_settings* config, void* context);
enum game_states win_cycle(struct map* game, dynamic_settings* config, void* context);

#endif // GAME_H
