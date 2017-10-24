#include <Arduboy2.h>

static Arduboy2 g_arduboy;

static struct {
    int x, y;
} context;

void setup()
{
    g_arduboy.begin();
    g_arduboy.setFrameRate(30);

    context.x = g_arduboy.width() / 2;
    context.y = g_arduboy.height() / 2;
}

void loop()
{
    if(!g_arduboy.nextFrame()) {
        return;
    }

    g_arduboy.pollButtons();

    if(g_arduboy.pressed(UP_BUTTON)) {
        context.y--;
    } else if(g_arduboy.pressed(DOWN_BUTTON)) {
        context.y++;
    }

    if(g_arduboy.pressed(LEFT_BUTTON)) {
        context.x--;
    } else if(g_arduboy.pressed(RIGHT_BUTTON)) {
        context.x++;
    }

    g_arduboy.fillScreen(BLACK);
    g_arduboy.fillRect(context.x - 16, context.y - 16, 32, 32, WHITE);

    g_arduboy.display();
}
