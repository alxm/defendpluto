/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Defend Pluto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Defend Pluto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Defend Pluto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"
#include "util_input.h"

void z_input_reset(void)
{
    for(uint8_t b = 0; b < Z_BUTTON_NUM; b++) {
        z_button_release(b);
    }
}

bool z_button_pressedOnce(uint8_t Button)
{
    bool pressed = z_button_pressed(Button);

    if(pressed) {
        z_button_release(Button);
    }

    return pressed;
}
