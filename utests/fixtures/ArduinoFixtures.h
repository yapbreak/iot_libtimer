#ifndef ARDUINO_H_NID3LW9T
#define ARDUINO_H_NID3LW9T

#include <map>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include "config.h"

#include <Arduino.h>

/*******************
*  Test fixtures  *
*******************/
#define NOTSET ('X')
#define UNDEFINED (0xffff)

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


class memory : public Print {
    public:
        memory()
            : Print()
            , m_str()
        {
            m_null = fopen("/dev/null", "w");
        }

        memory(const memory &o)
        {
            (void) o;
            m_null = fopen("/dev/null", "w");
        }

        ~memory()
        {
            fclose(m_null);
        }

        size_t print(const std::string &str) override
            { m_str << str; return str.size(); }
        size_t print(const char c_str[]) override
            { m_str << c_str; return strlen(c_str); }
        size_t print(char c) override
            { m_str << c; return 1; }
        size_t print(unsigned char c, int base = DEC) override
            { (void) base; m_str << c; return fprintf(m_null, "%u", c); }
        size_t print(int i, int base = DEC) override
            { (void) base; m_str << i; return fprintf(m_null, "%d", i); }
        size_t print(unsigned int i, int base = DEC) override
            { (void) base; m_str << i; return fprintf(m_null, "%u", i); }
        size_t print(long i, int base = DEC) override
            { (void) base; m_str << i; return fprintf(m_null, "%ld", i); }
        size_t print(unsigned long i, int base = DEC) override
            { (void) base; m_str << i; return fprintf(m_null, "%lu", i); }
        size_t print(double d, int floating = 2) override
            { (void) floating; m_str << d; return fprintf(m_null, "%f", d); }
        size_t print(const Printable &p) override
            { return p.printTo(*this); };

        size_t println(const std::string &str) override
            { size_t l = print(str); m_str << "\n"; return l + 1; }
        size_t println(const char c_str[]) override
            { size_t l = print(c_str); m_str << "\n"; return l + 1; }
        size_t println(char c) override
            { size_t l = print(c); m_str << "\n"; return l + 1; }
        size_t println(unsigned char c, int base = DEC) override
            { size_t l = print(c, base); m_str << "\n"; return l + 1; }
        size_t println(int c, int base = DEC) override
            { size_t l = print(c, base); m_str << "\n"; return l + 1; }
        size_t println(unsigned int c, int base = DEC) override
            { size_t l = print(c, base); m_str << "\n"; return l + 1; }
        size_t println(long c, int base = DEC) override
            { size_t l = print(c, base); m_str << "\n"; return l + 1; }
        size_t println(unsigned long c, int base = DEC) override
            { size_t l = print(c, base); m_str << "\n"; return l + 1; }
        size_t println(double d, int floating = 2) override
            { size_t l = print(d, floating); m_str << "\n"; return l + 1; }
        size_t println(const Printable &p) override
            { size_t l = print(p); m_str << "\n"; return l + 1; }
        size_t println() override
            { m_str << "\n"; return 1; };

        void clear()
        {
            m_str.clear();
        }

        std::string getcontent() const
        {
            return m_str.str();
        }

        memory &operator=(const memory &) = delete;

    private:
        std::stringstream m_str;
        FILE *m_null;
};

#endif /* end of include guard: ARDUINO_H_NID3LW9T */
