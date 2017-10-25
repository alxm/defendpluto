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
    s_buttons[S_BUTTON_A] = a_button_new("key.z gamepad.b.a");
    s_buttons[S_BUTTON_B] = a_button_new("key.x gamepad.b.b");
}

bool s_button_pressed(SButton Button)
{
    return a_button_getPressed(Button);
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

void s_draw_pixel(int X, int Y, bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_pixel(X, Y);
}

#endif // ifndef ARDUINO
