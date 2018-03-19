#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus

#include <functional>
#include <list>

typedef std::function<void(void *)> timer_callback_t;

class arduino_timer_t;

class arduino_event_t {
    private:
        uint32_t m_activation;
        int32_t m_count;
        bool m_overflow;
        uint32_t m_waiting_time;
        timer_callback_t m_callback;
        void *m_arg;

    public:
        arduino_event_t(uint32_t wait_time,
                        const char *wait_time_unit,
                        int count,
                        timer_callback_t callback,
                        void *arg);

        ~arduino_event_t();
        bool operator==(const arduino_event_t &obj) const;

    protected:

        bool process(uint32_t current, bool overflow);

        friend class arduino_timer_t;
};

class arduino_timer_t {
    private:
        size_t m_event_max;
        uint32_t m_last_time_seen;
        std::list<arduino_event_t> m_event_list;

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
