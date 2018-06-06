#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

#include "util.h"

// mutex and condition
pthread_cond_t timer_went_off;
int timer_flag;
pthread_mutex_t timer_flag_lock;
timer_t the_timer;

void timer_notify(union sigval par)
{
    pthread_mutex_lock(&timer_flag_lock);
    timer_flag = 1;
    pthread_cond_signal(&timer_went_off);
    pthread_mutex_unlock(&timer_flag_lock);
}

bool init_timer()
{
    // always returns 0
    pthread_mutex_init(&timer_flag_lock, NULL);
    timer_flag = 0;
    int res = pthread_cond_init(&timer_went_off, NULL);

    if(!res)
    {
        struct sigevent timer_event = {
            .sigev_notify = SIGEV_THREAD,
            .sigev_notify_function = &timer_notify
        };
        res = timer_create(CLOCK_REALTIME, &timer_event, &the_timer);
    }
    return 0 == res;
}

bool clear_timer()
{
    int res = 0;
    res = res || timer_delete(the_timer);
    res = res || pthread_mutex_destroy(&timer_flag_lock);
    res = res || pthread_cond_destroy(&timer_went_off);
    return 0 == res;
}

bool setup_timer(int msec_interval)
{
    struct timespec interval = {
        msec_interval / 1000,
        (msec_interval % 1000) * 1000000
    };
    struct itimerspec specification = { interval, interval };
    
    return 0 == timer_settime(the_timer, 0, &specification, NULL);
}

bool wait_for_timer_pulse(int timeout_msec)
{
    struct timeval now;
    struct timespec timeout;
    int retcode;
    bool res = false;

    pthread_mutex_lock(&timer_flag_lock);
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + timeout_msec / 1000;
    timeout.tv_nsec = now.tv_usec * 1000 + (timeout_msec % 1000) * 1000000;
    retcode = 0;
    while(0 == timer_flag && retcode != ETIMEDOUT)
    {
        retcode = pthread_cond_timedwait(
            &timer_went_off, &timer_flag_lock, &timeout);
    }

    if(retcode != ETIMEDOUT)
    {
        res = true;
        timer_flag = 0;        
    }
    pthread_mutex_unlock(&timer_flag_lock);
    
    return res;
}
