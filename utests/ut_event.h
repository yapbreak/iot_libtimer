#ifndef UT_EVENT_H_JFXUZJPP
#define UT_EVENT_H_JFXUZJPP

#include <CppUTest/TestHarness.h>
#include "timer.h"
#include "ArduinoFixtures.h"

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
};

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
};

TEST(basic_event, event_copy)
{
    arduino_event_t event1(10, "s", 1, event_callback, NULL);
    arduino_event_t event2(event1);
    arduino_event_t event3(100, "ms", -1, event_callback, &f);

    CHECK(event1 == event2);
    CHECK_FALSE(event1 == event3);

    event3 = event1;

    CHECK(event1 == event3);
};


#endif /* end of include guard: UT_EVENT_H_JFXUZJPP */
