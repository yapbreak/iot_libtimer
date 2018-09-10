#ifndef PRINTABLE_H_UHSMHUMN
#define PRINTABLE_H_UHSMHUMN

#include "config.h"

#if HAVE_ARDSIM == 1
#include "ardsim/Printable.h"
#else

class Print;

class Printable {
    public:
        virtual size_t printTo(Print &p) const = 0;
};

#endif

#endif /* end of include guard: PRINTABLE_H_UHSMHUMN */
