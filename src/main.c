#ifndef ARDUINO

#include <a2x.h>
#include "shared.h"
#include "loop.h"

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

A_SETUP
{
    a_settings_set("app.title", "arduboy-shooter");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.width", STRINGIFY(S_WIDTH));
    a_settings_set("video.height", STRINGIFY(S_HEIGHT));
    a_settings_set("video.fps", STRINGIFY(S_FPS));
}

A_STATE(run)
{
    A_STATE_INIT
    {
        loop_setup();
    }

    A_STATE_TICK
    {
        loop_tick();
    }

    A_STATE_DRAW
    {
        loop_draw();
    }
}

A_MAIN
{
    s_setup();

    a_state_new("run", run, "", "");
    a_state_push("run");
}

#endif // ifndef ARDUINO
