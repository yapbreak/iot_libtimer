#ifndef EVENT_H_VCBRSTG5
#define EVENT_H_VCBRSTG5

#include <stdint.h>

typedef void (*timer_callback_t) (void *);

/**
* @brief Main timer C++ interface.
*/
class arduino_timer_t;

/**
* @brief Internal representation of timer event.
*/
class arduino_event_t {
    private:
        /**
        * @brief Store the next timestamp when event needs to be triggered.
        */
        uint32_t m_activation;
        /**
        * @brief Store how many times event needs to be triggered again.
        */
        int32_t m_count;
        /**
        * @brief Indicate if next timestamp has overflowed.
        */
        bool m_overflow;
        /**
        * @brief Original waiting time of event (needed to retrigger event)
        *
        * The `m_waiting_time` is stored in microseconds.
        */
        uint32_t m_waiting_time;
        /**
        * @brief Function called when event is triggered.
        */
        timer_callback_t m_callback;
        /**
        * @brief Custom data passed to callback function.
        */
        void *m_arg;

    public:
        /**
        * @brief Main event constructor
        *
        * @param wait_time Amount of time to wait before event triggering,
        *                  expressed in `wait_time_unit`.
        * @param wait_time_unit Wait time unit. Could be set to "s" (seconds),
        *                       "ms" (milliseconds) or "us" (microseconds).
        * @param count Event repeating counter. Set to -1 for unlimitted
        *              repeating event.
        * @param callback Function called on event trigger.
        * @param arg Custom data passed to callback function on trigger.
        */
        arduino_event_t(uint32_t wait_time,
                        const char *wait_time_unit,
                        int count,
                        const timer_callback_t &callback,
                        void *arg);
        /**
        * @brief Copy event constructor
        *
        * @param event Event to copy.
        */
        arduino_event_t(const arduino_event_t &event);


        /**
        * @brief Default destructor.
        */
        ~arduino_event_t();

        /**
        * @brief Test event equality based on callback functions, arguments,
        *        waiting time and count.
        *
        * @param obj Other event to compare to current event.
        *
        * @return True if both event are equals. False otherwise.
        */
        bool operator==(const arduino_event_t &obj) const;

        /**
        * @brief Copy the event in the current object.
        *
        * @param obj Event to copy.
        *
        * @return Reference on the newly affected event.
        */
        arduino_event_t &operator=(const arduino_event_t &obj);

    protected:

        /**
        * @brief Internal event processing
        *
        * This method will compare current timestamp with event activation
        * timestamp, adjust overflow flags, trigger callback function if
        * needed and decrement count.
        *
        * @param current Current timestamp (in microseconds)
        * @param overflow Flag to indicate a timestamp overflow.
        *
        * @return True if event will never be triggered again and could be
        *         deleted. False otherwise.
        */
        bool process(uint32_t current, bool overflow);

        /**
        * @brief Main timer C++ interface.
        */
        friend class arduino_timer_t;
};


#endif /* end of include guard: EVENT_H_VCBRSTG5 */
