#!/usr/bin/env python3

"""
    Copyright 2017 Alex Margarit <alex@alxm.org>

    arduboy-shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arduboy-shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arduboy-shooter.  If not, see <http://www.gnu.org/licenses/>.
"""

import sys

objectTypes = {
    'enemy': 0
}

aiTypes = {
    'nobrain': 0
}

class Instruction:
    numInstructions = 0

    def __init__(self, NumTokens = 1):
        self.numTokens = NumTokens
        self.opcode = Instruction.numInstructions
        Instruction.numInstructions += 1

    def compile(self, Tokens):
        if len(Tokens) != self.numTokens:
            print('Instruction {} requires {} tokens'
                .format(Tokens[0], self.numTokens))
            sys.exit(1)

        return self.custom_compile(Tokens)

    def custom_compile(self, Tokens):
        return [self.opcode]

class InstructionSpawn(Instruction):
    def __init__(self):
        Instruction.__init__(self, 9)

    def custom_compile(self, Tokens):
        bytecode = []

        #
        # 8b    8b      8b      4b          4b        4b      4b      4b        4b
        # spawn x_coord y_coord object_type sprite_id ai_type ai_data num_units wait_between
        # spawn 64      -8      enemy       0         nobrain 0       1         0
        #
        x_coord = int(Tokens[1])
        y_coord = int(Tokens[2])
        object_type = objectTypes[Tokens[3]]
        sprite_id = int(Tokens[4])
        ai_type = aiTypes[Tokens[5]]
        ai_data = int(Tokens[6])
        num_units = int(Tokens[7])
        wait_between = int(Tokens[8])

        bytecode.append(self.opcode)
        bytecode.append(x_coord)
        bytecode.append(y_coord)
        bytecode.append((object_type << 4) | sprite_id)
        bytecode.append((ai_type << 4) | ai_data)
        bytecode.append((num_units << 4) | wait_between)

        return bytecode

class InstructionWait(Instruction):
    def __init__(self):
        Instruction.__init__(self, 2)

    def custom_compile(self, Tokens):
        bytecode = []

        #
        # 8b   8b
        # wait frames
        # wait 30
        #
        frames = int(Tokens[1])

        bytecode.append(self.opcode)
        bytecode.append(frames)

        return bytecode

def main(LevelFile):
    instructions = {
        'spawn': InstructionSpawn(),
        'wait': InstructionWait(),
        'waitclear': Instruction(),
        'over': Instruction(),
    }

    bytecode = []

    with open(LevelFile, 'rU') as f:
        for line in f:
            tokens = []
            rawTokens = line.split()

            for t in rawTokens:
                if t[0] == ';':
                    break

                tokens.append(t)

            if len(tokens) == 0:
                continue

            instruction = instructions[tokens[0]]
            bytecode.append(instruction.compile(tokens))

    formatted_bytecode = ''

    for line in bytecode:
        formatted_bytecode += '    '

        for byte in line:
            formatted_bytecode += '0x{:0>2x}, '.format((byte + 2**8) % (2**8))

        formatted_bytecode += '\n'

    contents = """\
// Generated by levels/compile.py
static const uint8_t z_data_levels[] = {{
{}}};\
""".format(formatted_bytecode)

    print(contents)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: {} data.txt'.format(sys.argv[0]))
    else:
        main(sys.argv[1])
