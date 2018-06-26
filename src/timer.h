#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus

#include <stdlib.h>

#include "event.h"

class arduino_timer_t {
    private:
        uint32_t m_last_time_seen;
        arduino_event_t **m_event_list;
        size_t m_event_count;
        size_t m_event_size;

    public:
        explicit arduino_timer_t();
        ~arduino_timer_t();

        void add_event(arduino_event_t event);
        void loop();
};

extern "C"
{
#endif

typedef void *arduino_opaque_timer_t;

arduino_opaque_timer_t arduino_timer_create();
void arduino_timer_destroy(arduino_opaque_timer_t timer);
void arduino_timer_create_event(arduino_opaque_timer_t timer,
                                unsigned long wait_time,
                                const char *wait_time_unit,
                                int count,
                                void (*trigger)(void *),
                                void *arg);
void arduino_timer_process(arduino_opaque_timer_t timer);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
