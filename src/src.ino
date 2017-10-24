#include <Arduino.h>
#include <Arduboy2.h>
#include "shared.h"
extern "C" {
#include "loop.h"
}

Arduboy2 g_arduboy;

void setup()
{
    g_arduboy.begin();
    g_arduboy.setFrameRate(30);

    s_setup();
    loop_setup();
}

void loop()
{
    if(!g_arduboy.nextFrame()) {
        return;
    }

    g_arduboy.pollButtons();

    loop_tick();
    loop_draw();

    g_arduboy.display();
}
