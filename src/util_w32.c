#include <Windows.h>

#include "util.h"

HANDLE timer;

bool init_timer()
{
    timer = CreateWaitableTimer(NULL, FALSE, NULL);
    return true;
}

bool clear_timer()
{
    CloseHandle(timer);
    return true;
}

bool setup_timer(int msec)
{
    LARGE_INTEGER delay_time;
    delay_time.QuadPart = -5000000LL;
    SetWaitableTimer(timer, &delay_time, 2000, NULL, NULL, 0);

    return true;
}

bool wait_for_timer_pulse(int timeout)
{
    DWORD result = WaitForSingleObject(timer, timeout);
    return result == WAIT_OBJECT_0;
}
