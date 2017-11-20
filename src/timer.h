#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

struct __arduino_timer_s;
typedef struct __arduino_timer_s arduino_timer_t;

arduino_timer_t *arduino_timer_create(int event_max);
void arduino_timer_destroy(arduino_timer_t *timer);
int arduino_timer_create_event(arduino_timer_t *timer,
                               unsigned long wait_time,
                               const char *wait_time_unit,
                               int count,
                               void (*trigger)(void *),
                               void *arg);
void arduino_timer_process(arduino_timer_t *timer);


#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
