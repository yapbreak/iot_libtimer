#ifndef EVENT_H_VCBRSTG5
#define EVENT_H_VCBRSTG5

#include <stdint.h>

typedef void (*timer_callback_t) (void *);

class arduino_timer_t;

class arduino_event_t {
    private:
        uint32_t m_activation;
        int32_t m_count;
        bool m_overflow;
        uint32_t m_waiting_time;
        const timer_callback_t m_callback;
        void *m_arg;

    public:
        arduino_event_t(uint32_t wait_time,
                        const char *wait_time_unit,
                        int count,
                        const timer_callback_t &callback,
                        void *arg);
        arduino_event_t(const arduino_event_t &event);


        ~arduino_event_t();
        bool operator==(const arduino_event_t &obj) const;

    protected:

        bool process(uint32_t current, bool overflow);

        friend class arduino_timer_t;
};


#endif /* end of include guard: EVENT_H_VCBRSTG5 */
