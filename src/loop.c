#include "shared.h"

static struct {
    int x, y;
    SharedButton up, down, left, right;
} context;

void loop_setup(void)
{
    context.x = shared_getWidth() / 2;
    context.y = shared_getHeight() / 2;

    context.up = g_buttons[SHARED_BUTTON_UP];
    context.down = g_buttons[SHARED_BUTTON_DOWN];
    context.left = g_buttons[SHARED_BUTTON_LEFT];
    context.right = g_buttons[SHARED_BUTTON_RIGHT];
}

void loop_tick(void)
{
    if(shared_pressed(context.up)) {
        context.y--;
    } else if(shared_pressed(context.down)) {
        context.y++;
    }

    if(shared_pressed(context.left)) {
        context.x--;
    } else if(shared_pressed(context.right)) {
        context.x++;
    }
}

void loop_draw(void)
{
    shared_fill(false);
    shared_rect(context.x - 16, context.y - 16, 32, 32, true);
}
