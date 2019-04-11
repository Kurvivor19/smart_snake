#include <stdlib.h>

#include <curses.h>

#include <glib.h>

#include "config.h"

GHashTable* global_keyboard_config = NULL;

GHashTable* get_keyboard_config()
{
    return global_keyboard_config;
}

void init_config()
{
    int default_keys[] = {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        'w',
        'a',
        's',
        'd',
        'q',
        ' ',
        'e',
        'h',
        0x1B,
        0
    };
    int default_commands[] = {
        cmd_up,
        cmd_down,
        cmd_left,
        cmd_right,
        cmd_up,
        cmd_left,
        cmd_down,
        cmd_right,
        cmd_quit,
        cmd_skip,
        cmd_stop,
        cmd_help,
        cmd_quit,
        0
    };
    global_keyboard_config = g_hash_table_new(&g_direct_hash,
                                              NULL);
    
    for(int i = 0; 0 != default_keys[i]; ++i)
        g_hash_table_insert(global_keyboard_config,
                            GINT_TO_POINTER(default_keys[i]),
                            GINT_TO_POINTER(default_commands[i]));
}

void free_config()
{
    g_hash_table_unref(global_keyboard_config);
    global_keyboard_config = NULL;
}

enum commands get_command_for_code(int keycode)
{
    gpointer keyval;
    gboolean result = g_hash_table_lookup_extended(
        global_keyboard_config,
        GINT_TO_POINTER(keycode),
        NULL, &keyval);
    if(false == result)
        return cmd_NULL;

    return GPOINTER_TO_INT(keyval);
}
