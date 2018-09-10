#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus

#include <stdlib.h>

#include "event.h"
#include <Printable.h>

/**
* @brief Main timer C++ interface.
*/
class arduino_timer_t : public Printable {
    private:
        /**
        * @brief Store timestamp where timer was last processed.
        *
        * This is mainly used to detect timestamp overflow.
        */
        uint32_t m_last_time_seen;
        /**
        * @brief Event list to process in this timer.
        */
        arduino_event_t **m_event_list;
        /**
        * @brief Total number of active event.
        */
        size_t m_event_count;
        /**
        * @brief Max number of event that can be stored in this timer.
        */
        size_t m_event_size;

    public:
        /**
        * @brief Default timer contructor.
        */
        explicit arduino_timer_t();
        /**
        * @brief Copy constructor.
        */
        explicit arduino_timer_t(const arduino_timer_t &obj);

        /**
        * @brief Default timer descructor.
        */
        virtual ~arduino_timer_t();

        /**
        * @brief Register a new event to be controlled by this timer.
        *
        * @param event Event to register.
        */
        void add_event(arduino_event_t event);
        /**
        * @brief Main method to call on every loop.
        */
        void loop();

        /**
        * @brief Copy a timer.
        *
        * @param obj Timer to copy
        *
        * @return Reference to the newly affected timer.
        */
        arduino_timer_t &operator=(const arduino_timer_t &obj);

        virtual size_t printTo(Print &p) const override;
};

extern "C"
{
#endif

/**
* @brief Opaque pointer used in C interface.
*/
typedef void *arduino_opaque_timer_t;

/**
* @brief Create a new timer with C interface.
*
* @return Newly created timer or NULL on error.
*/
arduino_opaque_timer_t arduino_timer_create();
/**
* @brief Destroy the given timer.
*
* @param timer Timer to destroy.
*/
void arduino_timer_destroy(arduino_opaque_timer_t timer);
/**
* @brief Register a new event to the given timer.
*
* @param wait_time Amount of time to wait before event triggering,
*                  expressed in `wait_time_unit`.
* @param wait_time_unit Wait time unit. Could be set to "s" (seconds),
*                       "ms" (milliseconds) or "us" (microseconds).
* @param count Event repeating counter. Set to -1 for unlimitted
*              repeating event.
* @param trigger Function called on event trigger.
* @param arg Custom data passed to callback function on trigger.
*/
void arduino_timer_create_event(arduino_opaque_timer_t timer,
                                unsigned long wait_time,
                                const char *wait_time_unit,
                                int count,
                                void (*trigger)(void *),
                                void *arg);
/**
* @brief Main method to call on every loop.
*
* @param timer Timer to process.
*/
void arduino_timer_process(arduino_opaque_timer_t timer);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
