#ifdef SIMULATE
#include <Arduino.h>
#include <stdio.h>

extern "C" void setup();
extern "C" void loop();

int main(int argc, const char *argv[])
{
    (void) argc;
    (void) argv;

    return ardsim_run();
}
#endif
