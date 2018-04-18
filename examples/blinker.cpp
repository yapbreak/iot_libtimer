/******************************************************************************
 *        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2017 Adrien Oliva <olivaa+gitlab@yapbreak.fr>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 ******************************************************************************/
#include <Arduino.h>
#include "timer.h"

#define LED_PIN (2)

static uint8_t led_state = LOW;
arduino_timer_t *timer = NULL;

/* Callback used in timer */
void blink(void *data)
{
    uint8_t *state = (uint8_t *) data;
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

#ifndef ARDUINO
int main(int argc, const char *argv[])
{
    (void) argc;
    (void) argv;

    setup();
    for (;;)
        loop();

    return 0;
}
#endif
