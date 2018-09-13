#include <Arduino.h>
#include <timer.h>

#ifndef LED_PIN
#define LED_PIN (15)
#endif

static uint8_t led_state = LOW;
arduino_timer_t *timer = NULL;

/* Callback used in timer */
void blink(void *data)
{
    uint8_t *state = static_cast<uint8_t *>(data);
    (*state) = ((*state) == LOW) ? HIGH : LOW;
    digitalWrite(LED_PIN, (*state));
}

extern "C" void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);

    // Create a new timer that can handle at most 1 event.
    timer = new arduino_timer_t();

    // Associate "blink" event in previously create timer.
    // This event will happen in 1s, and repeated continuously (-1).
    timer->add_event(arduino_event_t(1, "s", -1, blink, &led_state));

    // Allow printing event queue on a Print instance
    timer->printTo(Serial);
    // Or through Printable interface
    Serial.println(*timer);
}

extern "C" void loop()
{
    // Process timer.
    timer->loop();
    Serial.print((*timer));
    delay(1000);
}
