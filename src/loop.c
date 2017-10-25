#include "shared.h"

#define NUM_STARS 64

typedef struct ZStar {
    struct ZStar* next;
    int x, y;
} ZStar;

static struct {
    int x, y;
    SButton up, down, left, right;

    struct {
        ZStar* freeList;
        ZStar* activeList;
        ZStar pool[NUM_STARS];
    } stars;
} g_context;

static void z_star_new(void)
{
    if(g_context.stars.freeList == NULL) {
        return;
    }

    ZStar* star = g_context.stars.freeList;
    g_context.stars.freeList = g_context.stars.freeList->next;

    star->next = g_context.stars.activeList;
    g_context.stars.activeList = star;

    star->x = rand() % 128;
    star->y = 0;
}

static ZStar* z_star_free(ZStar* Star, ZStar* LastStar)
{
    ZStar* nextStar = Star->next;

    if(LastStar == NULL) {
        g_context.stars.activeList = nextStar;
    } else {
        LastStar->next = nextStar;
    }

    Star->next = g_context.stars.freeList;
    g_context.stars.freeList = Star;

    return nextStar;
}

static void z_star_tick(void)
{
    if(rand() % 3 == 0) {
        z_star_new();
    }

    ZStar* star = g_context.stars.activeList;
    ZStar* last = NULL;

    while(star != NULL) {
        star->y++;

        if(star->y >= s_screen_getHeight()) {
            star = z_star_free(star, last);
        } else {
            last = star;
            star = star->next;
        }
    }
}

static void z_star_draw(void)
{
    for(ZStar* s = g_context.stars.activeList; s != NULL; s = s->next) {
        s_draw_rectangle(s->x, s->y, 1, 1, true);
    }
}

void loop_setup(void)
{
    g_context.x = s_screen_getWidth() / 2;
    g_context.y = s_screen_getHeight() / 2;

    g_context.up = s_buttons[S_BUTTON_UP];
    g_context.down = s_buttons[S_BUTTON_DOWN];
    g_context.left = s_buttons[S_BUTTON_LEFT];
    g_context.right = s_buttons[S_BUTTON_RIGHT];

    for(int i = 0; i < NUM_STARS - 1; i++) {
        g_context.stars.pool[i].next = &g_context.stars.pool[i + 1];
    }

    g_context.stars.pool[NUM_STARS - 1].next = NULL;
    g_context.stars.freeList = &g_context.stars.pool[0];
    g_context.stars.activeList = NULL;
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

    z_star_tick();
}

void loop_draw(void)
{
    s_draw_fill(false);
    s_draw_rectangle(g_context.x - 16, g_context.y - 16, 32, 32, true);
    z_star_draw();
}
