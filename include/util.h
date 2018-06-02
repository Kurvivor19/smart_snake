#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

bool init_timer();
bool clear_timer();

bool setup_timer(int msec);
bool wait_for_timer_pulse(int timeout);

#endif /* UTIL_H */
