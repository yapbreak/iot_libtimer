#ifndef PRINT_H_OZDPEFUG
#define PRINT_H_OZDPEFUG

#if HAVE_ARDSIM == 1
#include "ardsim/Print.h"
#else

#include <string>
#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT  8
#define BIN  2

class Printable;
class Print {
    public:
        virtual size_t print(const std::string &str) = 0;
        virtual size_t print(const char str[]) = 0;
        virtual size_t print(char c) = 0;
        virtual size_t print(unsigned char c, int base = DEC) = 0;
        virtual size_t print(int i, int base = DEC) = 0;
        virtual size_t print(unsigned int i, int base = DEC) = 0;
        virtual size_t print(long l, int base = DEC) = 0;
        virtual size_t print(unsigned long l, int base = DEC) = 0;
        virtual size_t print(double d, int floating = 2) = 0;
        virtual size_t print(const Printable &p) = 0;
        virtual size_t println(const std::string &str) = 0;
        virtual size_t println(const char str[]) = 0;
        virtual size_t println(char c) = 0;
        virtual size_t println(unsigned char c, int base = DEC) = 0;
        virtual size_t println(int i, int base = DEC) = 0;
        virtual size_t println(unsigned int i, int base = DEC) = 0;
        virtual size_t println(long l, int base = DEC) = 0;
        virtual size_t println(unsigned long l, int base = DEC) = 0;
        virtual size_t println(double d, int floating = 2) = 0;
        virtual size_t println(const Printable &p) = 0;
        virtual size_t println() = 0;
};

#endif


#endif /* end of include guard: PRINT_H_OZDPEFUG */
