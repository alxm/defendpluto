#include <Arduino.h>
#include <Arduboy2.h>
#include "shared.h"

extern Arduboy2 g_arduboy;
SharedButton g_buttons[SHARED_BUTTON_NUM];

void shared_setup(void)
{
    g_buttons[SHARED_BUTTON_UP] = UP_BUTTON;
    g_buttons[SHARED_BUTTON_DOWN] = DOWN_BUTTON;
    g_buttons[SHARED_BUTTON_LEFT] = LEFT_BUTTON;
    g_buttons[SHARED_BUTTON_RIGHT] = RIGHT_BUTTON;
}

bool shared_pressed(SharedButton Button)
{
    return g_arduboy.pressed(Button);
}

int shared_getWidth(void)
{
    return g_arduboy.width();
}

int shared_getHeight(void)
{
    return g_arduboy.height();
}

void shared_fill(bool White)
{
    g_arduboy.fillScreen(White ? WHITE : BLACK);
}

void shared_rect(int X, int Y, int W, int H, bool White)
{
    g_arduboy.fillRect(X, Y, W, H, White ? WHITE : BLACK);
}
