#ifndef ARDUINO

#include <a2x.h>
#include "shared.h"
#include "loop.h"

A_SETUP
{
    a_settings_set("app.title", "arduboy-shooter");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.width", "128");
    a_settings_set("video.height", "64");
    a_settings_set("video.fps", "30");
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
    shared_setup();

    a_state_new("run", run, "", "");
    a_state_push("run");
}

#endif // ifndef ARDUINO
