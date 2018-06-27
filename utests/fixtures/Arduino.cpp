#include <map>
#include <sstream>
#include <string>
#include <iomanip>
#include "Arduino.h"
#include <CppUTest/TestHarness.h>

fixtures *fixtures::s_fixtures;
fixtures *fixtures::getInstance()
{
    return s_fixtures;
}
void fixtures::registerInstance(fixtures &f)
{
    s_fixtures = &f;
}

void analogWrite(uint8_t pin, uint8_t value)
{
    fixtures *f = fixtures::getInstance();
    BYTES_EQUAL(OUTPUT, f->get_actual_pin_mode(pin));

    f->set_actual_analog_value(pin, value);
    f->call("analogWrite");
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    fixtures *f = fixtures::getInstance();
    BYTES_EQUAL(OUTPUT, f->get_actual_pin_mode(pin));

    f->set_actual_digital_value(pin, value);
    f->call("digitalWrite");
}

int digitalRead(uint8_t pin)
{
    fixtures *f = fixtures::getInstance();
    BYTES_EQUAL(INPUT, f->get_actual_pin_mode(pin));

    f->call("digitalRead");
    return f->get_actual_digital_value(pin);
}

unsigned long millis()
{
    fixtures *f = fixtures::getInstance();

    return static_cast<uint32_t>(f->get_millis());
}

unsigned long micros()
{
    fixtures *f = fixtures::getInstance();

    return static_cast<uint32_t>(f->get_micros());
}

void pinMode(uint8_t pin, uint8_t mode)
{
    fixtures *f = fixtures::getInstance();

    f->set_actual_pin_mode(pin, mode);
    f->set_actual_analog_value(pin, UNDEFINED);
    f->set_actual_digital_value(pin, UNDEFINED);
    f->call("pinMode");
}

fixtures::fixtures()
    : m_expected_pin_map()
    , m_actual_pin_map()
    , m_expected_function_call_map()
    , m_actual_function_call_map()
    , m_micros(0)
{
}

fixtures::~fixtures()
{
}

void fixtures::set_expected_call(const char *function, int count)
{
    m_expected_function_call_map[function] = count;
}

int fixtures::get_expected_call(const char *function) const
{
    return m_expected_function_call_map.at(function);
}

void fixtures::set_expected_pin_mode(uint8_t pin, uint8_t mode)
{
    m_expected_pin_map[pin].mode = mode;
}

uint8_t fixtures::get_expected_pin_mode(uint8_t pin) const
{
    return m_expected_pin_map.at(pin).mode;
}

void fixtures::set_expected_digital_value(uint8_t pin, uint16_t level)
{
    m_expected_pin_map[pin].digital_value = level;
}

uint8_t fixtures::get_expected_digital_value(uint8_t pin) const
{
    return m_expected_pin_map.at(pin).digital_value;
}

void fixtures::set_expected_analog_value(uint8_t pin, uint16_t value)
{
    m_expected_pin_map[pin].analog_value = value;
}

uint8_t fixtures::get_expected_analog_value(uint8_t pin) const
{
    return m_expected_pin_map.at(pin).analog_value;
}

void fixtures::set_actual_call(const char *function, int count)
{
    m_actual_function_call_map[function] = count;
}

void fixtures::call(const char *function)
{
    int count;
    try {
        count = m_actual_function_call_map.at(function);
    } catch (std::out_of_range &) {
        count = 0;
    }

    m_actual_function_call_map[function] = count + 1;
}

int fixtures::get_actual_call(const char *function) const
{
    return m_actual_function_call_map.at(function);
}

void fixtures::set_actual_pin_mode(uint8_t pin, uint8_t mode)
{
    m_actual_pin_map[pin].mode = mode;
}

uint8_t fixtures::get_actual_pin_mode(uint8_t pin) const
{
    return m_actual_pin_map.at(pin).mode;
}

void fixtures::set_actual_digital_value(uint8_t pin, uint16_t level)
{
    m_actual_pin_map[pin].digital_value = level;
}

uint8_t fixtures::get_actual_digital_value(uint8_t pin) const
{
    return m_actual_pin_map.at(pin).digital_value;
}

void fixtures::set_actual_analog_value(uint8_t pin, uint16_t value)
{
    m_actual_pin_map[pin].analog_value = value;
}

uint8_t fixtures::get_actual_analog_value(uint8_t pin) const
{
    return m_actual_pin_map.at(pin).analog_value;
}

void fixtures::set_micros(uint64_t us)
{
    m_micros = us;
}

void fixtures::set_millis(uint64_t ms)
{
    m_micros = ms * 1000;
}

unsigned long fixtures::get_micros() const
{
    return m_micros;
}

unsigned long fixtures::get_millis() const
{
    return m_micros / 1000;
}

void fixtures::check() const
{
    // Function call check
    for (auto it = m_expected_function_call_map.begin();
            it != m_expected_function_call_map.end(); it++) {
        int expected_count = it->second;
        int actual_count;
        try {
            actual_count = m_actual_function_call_map.at(it->first);
        } catch (std::out_of_range &) {
            if (expected_count != 0) {
                std::stringstream ss;
                ss << "No call to " << it->first;
                FAIL(ss.str().c_str());
            }
            actual_count = 0;
        }

        std::stringstream expected;
        expected << it->first << ":" << expected_count;

        std::stringstream actual;
        actual << it->first << ":" << actual_count;
        STRCMP_EQUAL(expected.str().c_str(), actual.str().c_str());
    }

    // Pin state check
    for (auto it = m_expected_pin_map.begin();
            it != m_expected_pin_map.end(); it++) {
        const pin_attribute_t &expected_attribute = it->second;
        pin_attribute_t actual_attribute;

        try {
            actual_attribute = m_actual_pin_map.at(it->first);
        } catch (std::out_of_range &) {
            if (expected_attribute.mode != NOTSET) {
                std::stringstream ss;
                ss << "Pin " << static_cast<int>(it->first);
                ss << " not set at all";
                FAIL(ss.str().c_str());
            }
        }

        std::stringstream expected;
        expected << "#" << static_cast<int>(it->first);
        expected << ":m(" << expected_attribute.mode << "):";
        expected << "d(" << std::hex << expected_attribute.digital_value << "):";
        expected << "a(" << std::hex <<expected_attribute.analog_value << ")";

        std::stringstream actual;
        actual << "#" << static_cast<int>(it->first);
        actual << ":m(" << actual_attribute.mode << "):";
        actual << "d(" << std::hex << actual_attribute.digital_value << "):";
        actual << "a(" << std::hex << actual_attribute.analog_value << ")";

        STRCMP_EQUAL(expected.str().c_str(), actual.str().c_str());
    }
}
