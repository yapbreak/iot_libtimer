#include <Arduino.h>

#include <string.h>

#include "event.h"


arduino_event_t::arduino_event_t(uint32_t wait_time,
                const char *wait_time_unit,
                int count,
                const timer_callback_t &callback,
                void *arg)
    : m_count(count)
    , m_callback(callback)
    , m_arg(arg)
{
    uint32_t current = micros();
    uint64_t waiting_time_micros = 0;

    if (strcmp(wait_time_unit, "s") == 0)
        /* Seconds */
        waiting_time_micros = wait_time * 1000000LL;
    else if (strcmp(wait_time_unit, "ms") == 0)
        /* Milliseconds */
        waiting_time_micros = wait_time * 1000LL;
    else if (strcmp(wait_time_unit, "us") == 0)
        /* Microseconds */
        waiting_time_micros = wait_time;

    if (waiting_time_micros != 0) {
        m_waiting_time = static_cast<uint32_t>(waiting_time_micros);
        m_activation = current + m_waiting_time;
        m_overflow =    (m_activation < current)
                    ||  (static_cast<uint64_t>(m_waiting_time)
                            != waiting_time_micros);
    } else {
        m_waiting_time = 0;
        m_activation = 0;
        m_overflow = false;
    }
}

arduino_event_t::arduino_event_t(const arduino_event_t &event)
    : m_activation(event.m_activation)
    , m_count(event.m_count)
    , m_overflow(event.m_overflow)
    , m_waiting_time(event.m_waiting_time)
    , m_callback(event.m_callback)
    , m_arg(event.m_arg)
{
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

arduino_event_t &arduino_event_t::operator=(const arduino_event_t &obj)
{
    uint32_t current = micros();

    m_count = obj.m_count;
    m_callback = obj.m_callback;
    m_arg = obj.m_arg;
    m_waiting_time = obj.m_waiting_time;
    m_activation = current + m_waiting_time;
    m_overflow = (m_activation < current);

    return (*this);
}
