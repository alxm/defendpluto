#ifndef ARDUINO

#include <a2x.h>
#include "shared.h"

SButton s_buttons[S_BUTTON_NUM];

void s_setup(void)
{
    s_buttons[S_BUTTON_UP] = a_button_new("key.up");
    s_buttons[S_BUTTON_DOWN] = a_button_new("key.down");
    s_buttons[S_BUTTON_LEFT] = a_button_new("key.left");
    s_buttons[S_BUTTON_RIGHT] = a_button_new("key.right");
}

bool s_button_pressed(SButton Button)
{
    return a_button_getPressed(Button);
}

int s_screen_getWidth(void)
{
    return a_screen_getWidth();
}

int s_screen_getHeight(void)
{
    return a_screen_getHeight();
}

void s_draw_fill(bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_fill();
}

void s_draw_rectangle(int X, int Y, int W, int H, bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_rectangle(X, Y, W, H);
}

#endif // ifndef ARDUINO
