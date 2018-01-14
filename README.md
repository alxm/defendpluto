# Defend Pluto

## Level Scripting

### Ops

#### over

Reset the level VM.

    over

#### var

Declare a variable to use in the script. Variables may be passed by name to other ops, instead of literal numbers.

    var x

#### set

Set a declared variable to a value `[-128, 127]`.

    set x 30

#### inc

Increment a variable by a value `[-128, 127]`.

    inc x 1

#### flip

Negate a boolean variable.

    set x 1
    flip x ; x becomes 0
    flip x ; x becomes 1

#### loop, end

A loop block that runs a specified number of times `[0, 255]`.

    loop 4
        ; Increment x 4 times by 1
        inc x 1
    end

#### iter, endi

A block inside a loop block, that only runs during the specified loop iteration `[0, 255]`.

    loop 4
        inc x 1

        iter 0
            ; Only increment y in the first iteration
            inc y 1
        endi
    end

#### wait

Delay the script by a specified number of deciseconds `[0, 255]` or until all active enemies are gone, whichever happens first.

    wait 10 ; Block for 1 second

#### clear

Stop running the script until all active enemies are gone.

    clear

#### spawn

Spawn a game entity at the specified coordinates `[-128, 127]`, with the specified starting state `[0x0, 0xf]` and flags `[0x0, 0xff]`.

    ; Place an enemy ship in the middle of the screen
    spawn 50 50 enemy0

    ; Place an asteroid halfway on the x-axis, at the top of the screen
    spawn 50 -1 asteroid

    ; Place an enemy starting in AI state 1 with AI flags 0x4
    spawn 50 50 enemy 0x1 0x4

#### done

Go to the Level Cleared screen.

## License

Copyright 2017-2018 Alex Margarit (alex@alxm.org)

* Code licensed under [GNU GPL3](https://www.gnu.org/licenses/gpl.html) (see `COPYING`)
* Graphics licensed under [CC BY-NC-ND 4.0](https://creativecommons.org/licenses/by-nc-nd/4.0/) (see `CC-BY-NC-ND`)
