#ifndef SIMULATE_H_4BYDQ0IK
#define SIMULATE_H_4BYDQ0IK

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef SIMULATE

#include <stdlib.h>
#include <stdint.h>

unsigned long millis();
unsigned long micros();

#define OUTPUT ('O')
#define INPUT  ('I')
#define NOTSET ('X')
#define UNDEFINED (0xffff)
void pinMode(uint8_t pin, uint8_t mode);

void analogWrite(uint8_t pin, uint8_t value);
void digitalWrite(uint8_t pin, uint8_t value);
#define LOW 25
#define HIGH 90

#endif

#ifdef __cplusplus
};
#endif

#endif /* end of include guard: SIMULATE_H_4BYDQ0IK */
