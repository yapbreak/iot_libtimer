#include <Arduino.h>

#include <string.h>

#include "timer.h"

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
        if (to_delete) {
            delete m_event_list[i];
            for (size_t j = i; j < m_event_count - 1; ++j) {
                m_event_list[j] = m_event_list[j + 1];
            }
            i--;
            m_event_count--;
            m_event_list[m_event_count] = NULL;
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

size_t arduino_timer_t::printTo(Print &p) const
{
    size_t length = 0;
    length += p.print("Event list #");
    length += p.print(m_event_count);
    length += p.print(" / #");
    length += p.println(m_event_size);
    length += p.println("/-+------+--------------------\\");
    for (size_t i = 0; i < m_event_size; ++i) {
        char valid = i < m_event_count ? 'X' : ' ';
        char buffer[48];
        sprintf(buffer, "|%c| %4zu | %18p |", valid, i, m_event_list[i]);
        length += p.println(buffer);
    }
    length += p.println("\\-+------+--------------------/");

    return length;
}

/**********************************************************************
 *                            C interface                             *
 **********************************************************************/

arduino_opaque_timer_t arduino_timer_create()
{
    return static_cast<arduino_opaque_timer_t>(new arduino_timer_t());
}

// cppcheck-suppress unusedFunction
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
