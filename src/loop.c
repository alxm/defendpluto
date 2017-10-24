#include "shared.h"

static struct {
    int x, y;
    SButton up, down, left, right;
} g_context;

void loop_setup(void)
{
    g_context.x = s_screen_getWidth() / 2;
    g_context.y = s_screen_getHeight() / 2;

    g_context.up = s_buttons[S_BUTTON_UP];
    g_context.down = s_buttons[S_BUTTON_DOWN];
    g_context.left = s_buttons[S_BUTTON_LEFT];
    g_context.right = s_buttons[S_BUTTON_RIGHT];
}

void loop_tick(void)
{
    if(s_button_pressed(g_context.up)) {
        g_context.y--;
    } else if(s_button_pressed(g_context.down)) {
        g_context.y++;
    }

    if(s_button_pressed(g_context.left)) {
        g_context.x--;
    } else if(s_button_pressed(g_context.right)) {
        g_context.x++;
    }
}

void loop_draw(void)
{
    s_draw_fill(false);
    s_draw_rectangle(g_context.x - 16, g_context.y - 16, 32, 32, true);
}
