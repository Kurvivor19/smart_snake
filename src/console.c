// mostly taken from readline-ncirses
// https://github.com/ulfalizer/readline-and-ncurses
//

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <assert.h>
#include <glib.h>
#include <ecl/ecl.h>
#include "game.h"
#include "console.h"

// for come reason this declarationis not picked up
extern int wcwidth(const wint_t WC);

static struct console_state* pl_state = NULL;

// utility functions

cl_object make_cl_string(const char * pstring)
{
    return ecl_make_constant_base_string(pstring, strlen(pstring));
}

void set_initial_console_state(struct console_state* pstate)
{
   memset(pstate, 0, sizeof(struct console_state));
   cl_object function_name = make_cl_string("READ-STRING");
   cl_object package_name = make_cl_string("EMBEDDED-CONSOLE");
   pstate->work_func = cl_find_symbol(2, function_name, package_name);
   assert(ECL_SYMBOLP(pstate->work_func));
}

static long max(long l, long r)
{
   return l > r? l: r;
}

// Calculates the cursor column for the readline window in a way that supports
// multibyte, multi-column and combining characters. readline itself calculates
// this as part of its default redisplay function and does not export the
// cursor column.
//
// Returns the total width (in columns) of the characters in the 'n'-byte
// prefix of the null-terminated multibyte string 's'. If 'n' is larger than
// 's', returns the total width of the string. Tries to emulate how readline
// prints some special characters.
//
// 'offset' is the current horizontal offset within the line. This is used to
// get tab stops right.
//
// Makes a guess for malformed strings.
static size_t strnwidth(const char *s, size_t n, size_t offset)
{
   mbstate_t shift_state;
   wchar_t wc;
   size_t wc_len;
   size_t width = 0;

   // Start in the initial shift state
   memset(&shift_state, '\0', sizeof shift_state);

   for (size_t i = 0; i < n; i += wc_len) {
      // Extract the next multibyte character
      wc_len = mbrtowc(&wc, s + i, MB_CUR_MAX, &shift_state);
      switch (wc_len) {
         case 0:
            // Reached the end of the string
            goto done;

         case (size_t)-1: case (size_t)-2:
            // Failed to extract character. Guess that each character is one
            // byte/column wide each starting from the invalid character to
            // keep things simple.
            width += strnlen(s + i, n - i);
            goto done;
      }

      if (wc == '\t')
         width = ((width + offset + 8) & ~7) - offset;
      else
         // TODO: readline also outputs ~<letter> and the like for some
         // non-printable characters
         width += iswcntrl(wc) ? 2 : max(0, wcwidth(wc));
   }

  done:
   return width;
}

// Like strnwidth, but calculates the width of the entire string
static size_t strwidth(const char *s, size_t offset)
{
   return strnwidth(s, SIZE_MAX, offset);
}

// Not bothering with 'input_avail' and just returning 0 here seems to do the
// right thing too, but this might be safer across readline versions
static int readline_input_avail(void)
{
   return pl_state->input_avail;
}

static int readline_getc(FILE *dummy)
{
   pl_state->input_avail = false;
   return pl_state->input;
}

static void forward_to_readline(int c)
{
   pl_state->input = c;
   pl_state->input_avail = true;
   rl_callback_read_char();
}

static void msg_win_redisplay(bool for_resize)
{
   if (pl_state == NULL)
      return;
   
   // werase(pl_state->msg_win);
   scrollok(pl_state->msg_win, true);
   waddstr(pl_state->msg_win, pl_state->msg_win_str ? pl_state->msg_win_str : "");
   waddch(pl_state->msg_win, '\n');

   // We batch window updates when resizing
   if (for_resize)
      wnoutrefresh(pl_state->msg_win);
   else
      wrefresh(pl_state->msg_win);
}


static void got_command(char *line)
{
   if (!line)
      // Ctrl-D pressed on empty line
      pl_state->should_exit = true;
   else
   {
      if (*line)
         add_history(line);

      if (pl_state->msg_win_str)
      {
          g_free(pl_state->msg_win_str);
          pl_state->msg_win_str = NULL;
      }

      cl_env_ptr env = ecl_process_env();

      cl_object call_result = cl_funcall(2, pl_state->work_func, make_cl_string(line));
      cl_object string_buffer = ecl_nth_value(env, 1);

      assert(ecl_t_of(string_buffer) == t_base_string);

      if (call_result == ECL_NIL)
      {
          pl_state->msg_win_str = g_strdup_printf("No object read.\n Buffer contents:\n%s", string_buffer->string.self);
      }
      else
      {
          pl_state->msg_win_str = g_strdup_printf("An object.\n Buffer remains:\n%s", string_buffer->string.self);
      }
      msg_win_redisplay(false);
   }
}

static void cmd_win_redisplay(bool for_resize)
{
   if (pl_state == NULL)
      return;
   
   size_t prompt_width = strwidth(rl_display_prompt, 0);
   size_t cursor_col = prompt_width +
      strnwidth(rl_line_buffer, rl_point, prompt_width);

   werase(pl_state->cmd_win);
   // This might write a string wider than the terminal currently, so don't
   // check for errors
   mvwprintw(pl_state->cmd_win, 0, 0, "%s%s", rl_display_prompt, rl_line_buffer);
   int w, h;
   getmaxyx(pl_state->cmd_win, w, h);
   if (cursor_col >= w)
      // Hide the cursor if it lies outside the window. Otherwise it'll
      // appear on the very right.
      curs_set(0);
   else
   {
      wmove(pl_state->cmd_win, 0, cursor_col);
      curs_set(2);
   }
   // We batch window updates when resizing
   if (for_resize)
      wnoutrefresh(pl_state->cmd_win);
   else
      wrefresh(pl_state->cmd_win);
}

static void readline_redisplay(void)
{
   cmd_win_redisplay(false);
}

enum game_states console_cycle(struct map* pgame, dynamic_settings* config, void* context, bool redraw_now)
{
   pl_state = context;
   if (redraw_now)
   {
      cmd_win_redisplay(false);
      wrefresh(pl_state->msg_win);
   }
   keypad(pl_state->msg_win, TRUE);
   int symbol = wgetch(pl_state->cmd_win);
   // if things work correctly, this eventually calls got_command
   forward_to_readline(symbol);

   if (pl_state->should_exit)
   {
      pl_state->should_exit = false;
      return STATE_PLAY;
   }

   return STATE_CONSOLE;
}

bool init_readline(void)
{
   // Disable completion. TODO: Is there a more robust way to do this?
   if (rl_bind_key('\t', rl_insert))
      return false;

   // Let ncurses do all terminal and signal handling
   rl_catch_signals = 0;
   rl_catch_sigwinch = 0;
   rl_deprep_term_function = NULL;
   rl_prep_term_function = NULL;

   // Prevent readline from setting the LINES and COLUMNS environment
   // variables, which override dynamic size adjustments in ncurses. When
   // using the alternate readline interface (as we do here), LINES and
   // COLUMNS are not updated if the terminal is resized between two calls to
   // rl_callback_read_char() (which is almost always the case).
   rl_change_environment = 0;

   // Handle input by manually feeding characters to readline
   rl_getc_function = readline_getc;
   rl_input_available_hook = readline_input_avail;
   rl_redisplay_function = readline_redisplay;

   rl_callback_handler_install("> ", got_command);
   return true;
}

void deinit_readline(void)
{
   rl_callback_handler_remove();
}
