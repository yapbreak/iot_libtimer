#ifndef UT_TIMER_H_PTS0JNL4
#define UT_TIMER_H_PTS0JNL4

#include <CppUTest/TestHarness.h>
#include "timer.h"
#include "Arduino.h"

TEST_GROUP(basic_event)
{
    void setup()
    {
        fixtures::registerInstance(f);
    }

    void teardown()
    {
    }

    fixtures f;
};

void event_callback(void *data)
{
    (void) data;
}

void event_callback2(void *data)
{
    (void) data;
}

TEST(basic_event, simple_constructor)
{
    arduino_event_t *event = new arduino_event_t(10, "s", 1,
                                                 event_callback, NULL);
    CHECK(event != NULL);

    delete event;
}

TEST(basic_event, event_equality)
{
    arduino_event_t event1(10, "s", 1, event_callback, NULL);
    arduino_event_t event2(10, "s", 1, event_callback2, NULL);
    arduino_event_t event3(10, "s", 1, event_callback2, NULL);

    CHECK(event1 == event1);
    CHECK(event1 == event2);
    CHECK(event2 == event3);
    CHECK(event3 == event1);
}

TEST(basic_event, event_inequality)
{
    arduino_event_t event[] = {
        arduino_event_t( 10,  "s", 1, event_callback, NULL),
        arduino_event_t(100,  "s", 1, event_callback, NULL),
        arduino_event_t( 10, "ms", 1, event_callback, NULL),
        arduino_event_t( 10,  "s", 2, event_callback, NULL),
        arduino_event_t( 10,  "s", 1, event_callback, &f),
    };

    for (size_t i = 0; i < sizeof(event) / sizeof(event[0]); i++) {
        for (size_t j = 0; j < i; j++) {
            CHECK_FALSE(event[i] == event[j]);
        }
    }

    arduino_event_t event1(10, "s", 1, event_callback, NULL);
    f.set_micros(100);
    arduino_event_t event2(10, "s", 1, event_callback, NULL);
    CHECK_FALSE(event1 == event2);
}

TEST_GROUP(timer)
{
    void setup()
    {
        fixtures::registerInstance(f);
        timer = new arduino_timer_t();
        count = 0;
        f.set_micros(0);
    }

    void teardown()
    {
        delete timer;
        timer = NULL;
    }

    fixtures f;
    arduino_timer_t *timer;
    int count;
};

void event_counter(void *arg)
{
    int *count = static_cast<int *>(arg);

    CHECK(count != NULL);

    (*count)++;
}

TEST(timer, no_event)
{
    timer->loop();
    f.set_micros(100);
    timer->loop();
};

TEST(timer, one_second_event_no_repeat)
{
    f.set_millis(100);
    arduino_event_t event(1, "s", 1, event_counter, &count);
    timer->add_event(event);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_millis(1099);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_millis(1100);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_millis(1101);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_millis(2099);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_millis(2100);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_millis(2101);
    timer->loop();
    LONGS_EQUAL(1, count);
}

TEST(timer, one_millisecond_event_one_repeat)
{
    f.set_micros(100);
    arduino_event_t event(1, "ms", 2, event_counter, &count);
    timer->add_event(event);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_micros(1099);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_micros(1100);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_micros(1101);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_micros(2099);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_micros(2100);
    timer->loop();
    LONGS_EQUAL(2, count);
    f.set_micros(2101);
    timer->loop();
    LONGS_EQUAL(2, count);
    f.set_micros(3099);
    timer->loop();
    LONGS_EQUAL(2, count);
    f.set_micros(3100);
    timer->loop();
    LONGS_EQUAL(2, count);
    f.set_micros(3101);
    timer->loop();
    LONGS_EQUAL(2, count);
}

TEST(timer, one_microsecond_event_continuous_repeat)
{
    arduino_event_t event(10, "us", -1, event_counter, &count);
    timer->add_event(event);
    timer->loop();
    LONGS_EQUAL(0, count);
    for (size_t i = 1; i < 100; i++) {
        f.set_micros(i * 10 - 1);
        timer->loop();
        LONGS_EQUAL(i - 1, count);
        f.set_micros(i * 10);
        timer->loop();
        LONGS_EQUAL(i, count);
        f.set_micros(i * 10 + 1);
        timer->loop();
        LONGS_EQUAL(i, count);
    }
}

TEST(timer, malformed_event)
{
    arduino_event_t event(10, "ns", -1, event_counter, &count);
    timer->add_event(event);
    timer->loop();
    LONGS_EQUAL(0, count);
    for (size_t i = 1; i < 100; i++) {
        f.set_micros(i * 10);
        timer->loop();
        LONGS_EQUAL(0, count);
    }
}

TEST(timer, overflow_event_too_far)
{
    arduino_event_t event(4295, "s", 1, event_counter, &count);
    timer->add_event(event);
    f.set_millis(f.get_millis() + 10000);
    timer->loop();
    LONGS_EQUAL(0, count);
    for (size_t i = 0; i < 500; i++) {
        f.set_millis(f.get_millis() + 10000);
        timer->loop();
    }
    LONGS_EQUAL(1, count);
}

TEST(timer, overflow_late)
{
    f.set_micros(0xfffffffc);
    arduino_event_t event(1, "ms", 2, event_counter, &count);
    timer->add_event(event);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_micros(f.get_micros() + 998);
    timer->loop();
    LONGS_EQUAL(0, count);
    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(1, count);
    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(1, count);
}

TEST(timer, twoevents)
{
    int many_count[2] = { };

    f.set_micros(0);
    for (int i = 0; i < 2; i++) {
        timer->add_event(arduino_event_t(i + 1, "ms", 1, event_counter, &many_count[i]));
    }
    timer->loop();
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    timer->loop();
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    timer->loop();
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);
}

TEST(timer, tenevents)
{
    int many_count[10] = { };

    f.set_micros(0);
    for (int i = 0; i < 10; i++) {
        timer->add_event(arduino_event_t(i + 1, "ms", 1, event_counter, &many_count[i]));
    }
    timer->loop();
    for (int i = 0; i < 10; i++) {
        LONGS_EQUAL(0, many_count[i]);
    }

    f.set_micros(10001);
    timer->loop();

    for (int i = 0; i < 10; i++) {
        LONGS_EQUAL(1, many_count[i]);
    }
}

TEST(timer, twoevents_reverse)
{
    int many_count[2] = { };

    f.set_micros(0);
    for (int i = 0; i < 2; i++) {
        timer->add_event(arduino_event_t(2 - i, "ms", 1, event_counter, &many_count[i]));
    }
    timer->loop();
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    timer->loop();
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(0, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);

    f.set_micros(f.get_micros() + 999);
    timer->loop();
    LONGS_EQUAL(0, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);

    f.set_micros(f.get_micros() + 1);
    timer->loop();
    LONGS_EQUAL(1, many_count[0]);
    LONGS_EQUAL(1, many_count[1]);
}


#endif /* end of include guard: UT_TIMER_H_PTS0JNL4 */
