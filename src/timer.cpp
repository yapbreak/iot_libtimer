#include <Arduino.h>

#include <string.h>

#include "timer.h"

#ifdef __ARDUINO_TIMER_DEBUG__
#include <stdio.h>
void dump_event_list(arduino_event_t **list, size_t count, size_t size)
{
    printf("Event list #%lu / #%lu\n", count, size);
    printf("/-+------+--------------------\\\n");
    for (size_t i = 0; i < size; ++i) {
        char valid = i < count ? 'X' : ' ';
        printf("|%c| %4lu | %18p |\n",
                valid, i, list[i]);
    }
    printf("\\-+------+--------------------/\n");
}
#else
#define dump_event_list(list, count, size)
#endif

arduino_timer_t::arduino_timer_t()
    : m_last_time_seen(micros())
    , m_event_list(NULL)
    , m_event_count(0)
    , m_event_size(1)
{
    m_event_list = new arduino_event_t *[m_event_size];
}

arduino_timer_t::arduino_timer_t(const arduino_timer_t &obj)
    : m_last_time_seen(obj.m_last_time_seen)
    , m_event_list(NULL)
    , m_event_count(obj.m_event_count)
    , m_event_size(obj.m_event_size)
{
    m_event_list = new arduino_event_t *[m_event_size];
    for (size_t i = 0; i < m_event_count; ++i) {
        m_event_list[i] = new arduino_event_t(*(obj.m_event_list[i]));
    }
}

arduino_timer_t::~arduino_timer_t()
{
    for (size_t i = 0; i < m_event_count; ++i) {
        delete m_event_list[i];
    }
    delete [] m_event_list;
}

void arduino_timer_t::add_event(arduino_event_t event)
{
    if (m_event_count == m_event_size) {
        size_t new_size = m_event_size * 2;
        arduino_event_t **list = new arduino_event_t *[new_size];
        memcpy(list, m_event_list, m_event_size * sizeof(arduino_event_t *));
        delete [] m_event_list;
        m_event_list = list;
        m_event_size = new_size;
    }

    m_event_list[m_event_count] = new arduino_event_t(event);
    m_event_count++;
}

void arduino_timer_t::loop()
{
    uint32_t current = micros();
    bool overflow_decrement = (m_last_time_seen > current);

    m_last_time_seen = current;

    for (size_t i = 0; i < m_event_count; ++i) {
        bool to_delete = m_event_list[i]->process(m_last_time_seen,
                                                  overflow_decrement);
        dump_event_list(m_event_list, m_event_count, m_event_size);
        if (to_delete) {
            delete m_event_list[i];
            for (size_t j = i; j < m_event_count - 1; ++j) {
                m_event_list[j] = m_event_list[j + 1];
            }
            i--;
            m_event_count--;
            m_event_list[m_event_count] = NULL;
            dump_event_list(m_event_list, m_event_count, m_event_size);
        }
    }
}

arduino_timer_t &arduino_timer_t::operator=(const arduino_timer_t &obj)
{
    if (&obj == this)
        return (*this);

    m_last_time_seen = obj.m_last_time_seen;
    for (size_t i = 0; i < m_event_count; ++i) {
        delete m_event_list[i];
    }
    delete [] m_event_list;

    m_event_count = obj.m_event_count;
    m_event_size = obj.m_event_size;
    m_event_list = new arduino_event_t *[m_event_size];
    for (size_t i = 0; i < m_event_count; ++i) {
        m_event_list[i] = new arduino_event_t(*(obj.m_event_list[i]));
    }

    return (*this);
}

/**********************************************************************
 *                            C interface                             *
 **********************************************************************/

arduino_opaque_timer_t arduino_timer_create()
{
    return static_cast<arduino_opaque_timer_t>(new arduino_timer_t());
}

void arduino_timer_destroy(arduino_opaque_timer_t timer)
{
    delete static_cast<arduino_timer_t *>(timer);
}

void arduino_timer_create_event(arduino_opaque_timer_t timer,
                                unsigned long wait_time,
                                const char *wait_time_unit,
                                int count,
                                void (*trigger)(void *),
                                void *arg)
{
    arduino_timer_t *internal_timer = static_cast<arduino_timer_t *>(timer);
    arduino_event_t event(wait_time, wait_time_unit, count, trigger, arg);

    internal_timer->add_event(event);
}

void arduino_timer_process(arduino_opaque_timer_t timer)
{
    static_cast<arduino_timer_t *>(timer)->loop();
}
