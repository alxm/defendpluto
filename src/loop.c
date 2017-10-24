#include "shared.h"

static struct {
    int x, y;
    SButton up, down, left, right;
} context;

void loop_setup(void)
{
    context.x = s_screen_getWidth() / 2;
    context.y = s_screen_getHeight() / 2;

    context.up = s_buttons[S_BUTTON_UP];
    context.down = s_buttons[S_BUTTON_DOWN];
    context.left = s_buttons[S_BUTTON_LEFT];
    context.right = s_buttons[S_BUTTON_RIGHT];
}

void loop_tick(void)
{
    if(s_button_pressed(context.up)) {
        context.y--;
    } else if(s_button_pressed(context.down)) {
        context.y++;
    }

    if(s_button_pressed(context.left)) {
        context.x--;
    } else if(s_button_pressed(context.right)) {
        context.x++;
    }
}

void loop_draw(void)
{
    s_draw_fill(false);
    s_draw_rectangle(context.x - 16, context.y - 16, 32, 32, true);
}
