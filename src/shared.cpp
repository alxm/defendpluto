#include <Arduino.h>
#include <Arduboy2.h>
#include "shared.h"

extern Arduboy2 g_arduboy;
SButton s_buttons[S_BUTTON_NUM];

void s_setup(void)
{
    s_buttons[S_BUTTON_UP] = UP_BUTTON;
    s_buttons[S_BUTTON_DOWN] = DOWN_BUTTON;
    s_buttons[S_BUTTON_LEFT] = LEFT_BUTTON;
    s_buttons[S_BUTTON_RIGHT] = RIGHT_BUTTON;
    s_buttons[S_BUTTON_A] = A_BUTTON;
    s_buttons[S_BUTTON_B] = B_BUTTON;
}

bool s_button_pressed(SButton Button)
{
    return g_arduboy.pressed(Button);
}

int s_screen_getWidth(void)
{
    return g_arduboy.width();
}

int s_screen_getHeight(void)
{
    return g_arduboy.height();
}

void s_draw_fill(bool White)
{
    g_arduboy.fillScreen(White ? WHITE : BLACK);
}

void s_draw_rectangle(int X, int Y, int W, int H, bool White)
{
    g_arduboy.fillRect(X, Y, W, H, White ? WHITE : BLACK);
}
