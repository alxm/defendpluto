#ifndef ARDUINO

#include <a2x.h>
#include "shared.h"

SharedButton g_buttons[SHARED_BUTTON_NUM];

void shared_setup(void)
{
    g_buttons[SHARED_BUTTON_UP] = a_button_new("key.up");
    g_buttons[SHARED_BUTTON_DOWN] = a_button_new("key.down");
    g_buttons[SHARED_BUTTON_LEFT] = a_button_new("key.left");
    g_buttons[SHARED_BUTTON_RIGHT] = a_button_new("key.right");
}

bool shared_pressed(SharedButton Button)
{
    return a_button_getPressed(Button);
}

int shared_getWidth(void)
{
    return a_screen_getWidth();
}

int shared_getHeight(void)
{
    return a_screen_getHeight();
}

void shared_fill(bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_fill();
}

void shared_rect(int X, int Y, int W, int H, bool White)
{
    a_pixel_setHex(White ? 0xffffff : 0);
    a_draw_rectangle(X, Y, W, H);
}

#endif // ifndef ARDUINO
