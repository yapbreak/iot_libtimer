#ifndef UT_TIMER_C_H_ET7NC52L
#define UT_TIMER_C_H_ET7NC52L

#include <CppUTest/TestHarness.h>
#include "timer.h"
#include "Arduino.h"

TEST_GROUP(timer_c)
{
    void setup()
    {
        fixtures::registerInstance(f);
        timer = arduino_timer_create();
        count = 0;
        f.set_micros(0);
    }

    void teardown()
    {
        arduino_timer_destroy(timer);
        timer = NULL;
    }

    fixtures f;
    arduino_opaque_timer_t timer;
    int count;
};

void event_counter_c(void *arg)
{
    int *count = static_cast<int *>(arg);

    CHECK(count != NULL);

    (*count)++;
}

TEST(timer_c, no_event)
{
    arduino_timer_process(timer);
    f.set_micros(100);
    arduino_timer_process(timer);
};

TEST(timer_c, one_second_event_no_repeat)
{
    f.set_millis(100);
    arduino_timer_create_event(timer, 1, "s", 1, event_counter_c, &count);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_millis(1099);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_millis(1100);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_millis(1101);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_millis(2099);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_millis(2100);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_millis(2101);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
}

TEST(timer_c, one_millisecond_event_one_repeat)
{
    f.set_micros(100);
    arduino_timer_create_event(timer, 1, "ms", 2, event_counter_c, &count);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_micros(1099);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_micros(1100);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_micros(1101);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_micros(2099);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_micros(2100);
    arduino_timer_process(timer);
    LONGS_EQUAL(2, count);
    f.set_micros(2101);
    arduino_timer_process(timer);
    LONGS_EQUAL(2, count);
    f.set_micros(3099);
    arduino_timer_process(timer);
    LONGS_EQUAL(2, count);
    f.set_micros(3100);
    arduino_timer_process(timer);
    LONGS_EQUAL(2, count);
    f.set_micros(3101);
    arduino_timer_process(timer);
    LONGS_EQUAL(2, count);
}

TEST(timer_c, one_microsecond_event_continuous_repeat)
{
    arduino_timer_create_event(timer, 10, "us", -1, event_counter_c, &count);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    for (size_t i = 1; i < 100; i++) {
        f.set_micros(i * 10 - 1);
        arduino_timer_process(timer);
        LONGS_EQUAL(i - 1, count);
        f.set_micros(i * 10);
        arduino_timer_process(timer);
        LONGS_EQUAL(i, count);
        f.set_micros(i * 10 + 1);
        arduino_timer_process(timer);
        LONGS_EQUAL(i, count);
    }
}

TEST(timer_c, malformed_event)
{
    arduino_timer_create_event(timer, 10, "ns", -1, event_counter_c, &count);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    for (size_t i = 1; i < 100; i++) {
        f.set_micros(i * 10);
        arduino_timer_process(timer);
        LONGS_EQUAL(0, count);
    }
}

TEST(timer_c, overflow_event_too_far)
{
    arduino_timer_create_event(timer, 4295, "s", 1, event_counter_c, &count);
    f.set_millis(f.get_millis() + 10000);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    for (size_t i = 0; i < 500; i++) {
        f.set_millis(f.get_millis() + 10000);
        arduino_timer_process(timer);
    }
    LONGS_EQUAL(1, count);
}

TEST(timer_c, overflow_late)
{
    f.set_micros(0xfffffffc);
    arduino_timer_create_event(timer, 1, "ms", 2, event_counter_c, &count);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_micros(f.get_micros() + 998);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, count);
    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, count);
}

TEST(timer_c, twoevents)
{
    int many_count[2] = { };

    f.set_micros(0);
    for (int i = 0; i < 2; i++) {
        arduino_timer_create_event(timer, i + 1, "ms", 1, event_counter_c, &many_count[i]);
    }
    arduino_timer_process(timer);
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);
}

TEST(timer_c, tenevents)
{
    int many_count[10] = { };

    f.set_micros(0);
    for (int i = 0; i < 10; i++) {
        arduino_timer_create_event(timer, i + 1, "ms", 1, event_counter_c, &many_count[i]);
    }
    arduino_timer_process(timer);
    for (int i = 0; i < 10; i++) {
        LONGS_EQUAL(0, many_count[i]);
    }

    f.set_micros(10001);
    arduino_timer_process(timer);

    for (int i = 0; i < 10; i++) {
        LONGS_EQUAL(1, many_count[i]);
    }
}

TEST(timer_c, twoevents_reverse)
{
    int many_count[2] = { };

    f.set_micros(0);
    for (int i = 0; i < 2; i++) {
        arduino_timer_create_event(timer, 2 - i, "ms", 1, event_counter_c, &many_count[i]);
    }
    arduino_timer_process(timer);
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    arduino_timer_process(timer);
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    arduino_timer_process(timer);
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);
}

#endif /* end of include guard: UT_TIMER_C_H_ET7NC52L */
