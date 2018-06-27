#ifdef SIMULATE

#include "timer.h"
#include "simulate.h"
#include <time.h>
#include <stdio.h>

extern void setup();
extern void loop();

int main(int argc, const char *argv[])
{
    (void) argc;
    (void) argv;

    setup();

    for (;;)
        loop();

    return 0;
}

void pinMode(uint8_t pin, uint8_t mode)
{
    printf("Set pin#%d to %c\n", pin, mode);
}

/*
void analogWrite(uint8_t pin, uint8_t value)
{
    printf("Write on pin#%d to 0x%x\n", pin, value);
}
*/

void digitalWrite(uint8_t pin, uint8_t value)
{
    switch (value) {
        case LOW:
            printf("Write on pin#%d to LOW\n", pin);
            break;
        case HIGH:
            printf("Write on pin#%d to HIGH\n", pin);
            break;
        default:
            printf("Write on pin#%d unexpected value 0x%x\n", pin, value);
            break;
    }
}

/*
unsigned long millis()
{
    struct timespec time;

    clock_gettime(CLOCK_MONOTONIC, &time);

    return (time.tv_sec * 1000) + (time.tv_nsec % 1000000);
}
*/

unsigned long micros()
{
    struct timespec time;

    clock_gettime(CLOCK_MONOTONIC, &time);

    return (time.tv_sec * 1000000) + (time.tv_nsec % 1000);
}

#endif
