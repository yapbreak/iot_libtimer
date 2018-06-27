#if defined(TESTFIRMWARE) || defined(SIMULATE)

#ifndef SIMULATE
#include <Arduino.h>
#else
#include "simulation/simulate.h"
#endif

#include "timer.h"

#define LED_PIN (2)

static uint8_t led_state = LOW;
arduino_timer_t *timer = NULL;

/* Callback used in timer */
void blink(void *data)
{
    uint8_t *state = static_cast<uint8_t *>(data);
    (*state) = ((*state) == LOW) ? HIGH : LOW;
    digitalWrite(LED_PIN, (*state));
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    // Create a new timer that can handle at most 1 event.
    timer = new arduino_timer_t();

    // Associate "blink" event in previously create timer.
    // This event will happen in 1s, and repeated continuously (-1).
    timer->add_event(arduino_event_t(1, "s", -1, blink, &led_state));
}

void loop()
{
    // Process timer.
    timer->loop();
}

#endif
