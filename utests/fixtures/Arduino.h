#ifndef ARDUINO_H_NID3LW9T
#define ARDUINO_H_NID3LW9T

#include <map>
#include <CppUTest/TestHarness.h>
#include <iostream>
#include <string>
#include <stdint.h>

/* Arduino fixtures */

/********************
*  Arduino wiring  *
********************/
void analogWrite(uint8_t pin, uint8_t value);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
#define LOW 25
#define HIGH 90

#define OUTPUT ('O')
#define INPUT  ('I')
#define NOTSET ('X')
#define UNDEFINED (0xffff)
void pinMode(uint8_t pin, uint8_t mode);

/********************
*  Arduino timing  *
********************/
unsigned long millis();
unsigned long micros();


/*******************
*  Test fixtures  *
*******************/

struct pin_attribute_t
{
    uint8_t mode;
    uint16_t digital_value;
    uint16_t analog_value;

    pin_attribute_t()
        : mode(NOTSET)
        , digital_value(UNDEFINED)
        , analog_value(UNDEFINED)
    {}
};

class fixtures
{
    private:
        std::map<uint8_t, pin_attribute_t> m_expected_pin_map;
        std::map<uint8_t, pin_attribute_t> m_actual_pin_map;
        std::map<std::string, int> m_expected_function_call_map;
        std::map<std::string, int> m_actual_function_call_map;
        uint64_t m_micros;

        static fixtures *s_fixtures;

    public:
        static fixtures *getInstance();
        static void registerInstance(fixtures &f);
        fixtures();
        ~fixtures();

        void set_expected_call(const char *function, int count);
        int get_expected_call(const char *function) const;

        void set_expected_pin_mode(uint8_t pin, uint8_t mode);
        uint8_t get_expected_pin_mode(uint8_t pin) const;

        void set_expected_digital_value(uint8_t pin, uint16_t level);
        uint8_t get_expected_digital_value(uint8_t pin) const;

        void set_expected_analog_value(uint8_t pin, uint16_t value);
        uint8_t get_expected_analog_value(uint8_t pin) const;

        void set_actual_call(const char *function, int count);
        int get_actual_call(const char *function) const;
        void call(const char *function);

        void set_actual_pin_mode(uint8_t pin, uint8_t mode);
        uint8_t get_actual_pin_mode(uint8_t pin) const;

        void set_actual_digital_value(uint8_t pin, uint16_t level);
        uint8_t get_actual_digital_value(uint8_t pin) const;

        void set_actual_analog_value(uint8_t pin, uint16_t value);
        uint8_t get_actual_analog_value(uint8_t pin) const;

        void set_micros(uint64_t us);
        void set_millis(uint64_t ms);

        unsigned long get_micros() const;
        unsigned long get_millis() const;

        void check() const;
};

#endif /* end of include guard: ARDUINO_H_NID3LW9T */
