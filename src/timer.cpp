#include <Arduino.h>
#include "timer.h"

typedef struct __arduino_event_s
{
    uint32_t activated;
    int count;
    char overflow;
    unsigned long waiting_time;
    void (*function)(void *);
    void *arg;
} arduino_event_t;

struct __arduino_timer_s
{
    size_t m_event_max;
    unsigned long m_last_time_seen;
    arduino_event_t *m_event_list;
};

arduino_timer_t *arduino_timer_create(int event_max)
{
    arduino_timer_t *timer = NULL;

    if (event_max != 0) {
        timer = (arduino_timer_t *) malloc(sizeof(arduino_timer_t));
        timer->m_event_max = event_max;
        timer->m_event_list = (arduino_event_t *) calloc(timer->m_event_max,
                                                         sizeof(arduino_event_t));
        memset(timer->m_event_list, 0,
               timer->m_event_max * sizeof(arduino_event_t));
        timer->m_last_time_seen = micros();
    }

    return timer;
}

void arduino_timer_destroy(arduino_timer_t *timer)
{
    if (timer != NULL) {
        free(timer->m_event_list);
        free(timer);
    }
}

int arduino_timer_create_event(arduino_timer_t *timer,
                               unsigned long wait_time,
                               const char *wait_time_unit,
                               int count,
                               void (*trigger)(void *),
                               void *arg)
{
    size_t index;
    unsigned long current = micros();

    if (timer == NULL)
        return -1;
    if (wait_time == 0)
        return -1;
    if (trigger == NULL)
        return -1;

    /* Calculate triggered time */
    uint32_t next_trigger = 0;
    uint32_t waiting_time;

    if (strcmp(wait_time_unit, "s") == 0)
        /* Seconds */
        waiting_time = wait_time * 1000000LL;
    else if (strcmp(wait_time_unit, "ms") == 0)
        /* Milliseconds */
        waiting_time = wait_time * 1000LL;
    else if (strcmp(wait_time_unit, "us") == 0)
        /* Microseconds */
        waiting_time = wait_time;
    else
        return -1;

    next_trigger = current + waiting_time;

    for (index = 0; index != timer->m_event_max; index++) {
        arduino_event_t *event = &(timer->m_event_list[index]);
        if (event->activated == 0) {
            event->waiting_time = waiting_time;
            event->activated = next_trigger;
            event->overflow = (next_trigger < current);
            event->function = trigger;
            event->count = count;
            event->arg = arg;
            return 0;
        }
    }
    return -1;
}

void arduino_timer_process(arduino_timer_t *timer)
{
    if (timer == NULL)
        return;

    unsigned long current = micros();
    size_t index;
    char overflow_decrement = (timer->m_last_time_seen > current) ? 1 : 0;

    timer->m_last_time_seen = current;

    for (index = 0; index != timer->m_event_max; index++) {
        arduino_event_t *event = &(timer->m_event_list[index]);
        event->overflow -= overflow_decrement;
        if (    event->activated != 0
            &&  event->overflow == 0
            &&  event->activated <= current
        ) {
            event->function(event->arg);
            if (event->count > 0)
                event->count--;
            if (event->count == 0) {
                event->activated = 0;
            } else {
                event->activated = current + event->waiting_time;
                event->overflow = (event->activated < current);
            }
        }
    }
}
