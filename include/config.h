
/**
 * \file config.h
 * configuration that can be externally configured
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <gmodule.h>

#include "snake.h"

GHashTable* get_keyboard_config();

void init_config();
void free_config();

enum commands get_command_for_code(int keycode);

typedef struct _dynamic_settings
{
    int winning_length;
    int timer_msec_interval;
} dynamic_settings;

#endif /* CONFIG_H */
