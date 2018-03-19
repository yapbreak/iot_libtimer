#include <Arduino.h>
#include "timer.h"

arduino_event_t::arduino_event_t(uint32_t wait_time,
                const char *wait_time_unit,
                int count,
                timer_callback_t callback,
                void *arg)
    : m_count(count)
    , m_callback(callback)
    , m_arg(arg)
{
    uint32_t next_trigger = 0;
    uint32_t current = micros();

    if (strcmp(wait_time_unit, "s") == 0)
        /* Seconds */
        m_waiting_time = wait_time * 1000000LL;
    else if (strcmp(wait_time_unit, "ms") == 0)
        /* Milliseconds */
        m_waiting_time = wait_time * 1000LL;
    else if (strcmp(wait_time_unit, "us") == 0)
        /* Microseconds */
        m_waiting_time = wait_time;
    else
        m_waiting_time = 0;

    if (m_waiting_time != 0) {
        m_activation = current + m_waiting_time;
        m_overflow = (m_activation < current);
    } else {
        m_activation = 0;
        m_overflow = false;
    }
}

arduino_event_t::~arduino_event_t()
{

}

bool arduino_event_t::process(uint32_t current, bool overflow)
{
    if (overflow)
        m_overflow = false;

    if (m_activation != 0 && m_activation <= current && !m_overflow) {
        m_callback(m_arg);
        if (m_count > 0)
            m_count--;
        if (m_count == 0) {
            m_activation = 0;
        } else {
            m_activation = current + m_waiting_time;
            m_overflow = (m_activation < current);
        }
    }

    return (m_activation == 0);
}

bool arduino_event_t::operator==(const arduino_event_t &obj) const
{
    return  m_arg == obj.m_arg
        && m_activation == obj.m_activation
        && m_count == obj.m_count
        && m_overflow == obj.m_overflow
        && m_waiting_time == obj.m_waiting_time;
}

arduino_timer_t::arduino_timer_t()
    : m_last_time_seen(micros())
    , m_event_list()
{
}

arduino_timer_t::~arduino_timer_t()
{
}

void arduino_timer_t::add_event(arduino_event_t event)
{
    m_event_list.push_back(event);
}

void arduino_timer_t::loop()
{
    uint32_t current = micros();
    bool overflow_decrement = (m_last_time_seen > current);

    m_last_time_seen = current;
    for (auto it = m_event_list.begin(); it != m_event_list.end(); ++it) {
        bool to_delete = it->process(m_last_time_seen, overflow_decrement);
        if (to_delete) {
            m_event_list.remove(*it);
        }
    }
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
