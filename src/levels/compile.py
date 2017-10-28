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
    'enemy1': 0
}

aiTypes = {
    'nobrain': 0
}

class Instruction:
    def __init__(self, NumTokens, Opcode):
        self.numTokens = NumTokens
        self.opcode = Opcode

    def compile(self, Tokens):
        if len(Tokens) != self.numTokens:
            print('Instruction {} requires {} tokens'
                .format(Tokens[0], self.numTokens))
            sys.exit(1)

        return self.custom_compile(Tokens)

    def custom_compile(self, Tokens):
        return [self.opcode]

class InstructionSpawn(Instruction):
    def __init__(self, NumBytes, Opcode):
        Instruction.__init__(self, NumBytes, Opcode)

    def custom_compile(self, Tokens):
        bytecode = []

        #
        # 8b    8b      8b      4b          4b      4b      4b        8b
        # spawn x_coord y_coord object_type ai_type ai_data num_units wait_between
        # spawn 64      0       enemy1      nobrain 0       1         0
        #
        x_coord = int(Tokens[1])
        y_coord = int(Tokens[2])
        object_type = objectTypes[Tokens[3]]
        ai_type = aiTypes[Tokens[4]]
        ai_data = int(Tokens[5])
        num_units = int(Tokens[6])
        wait_between = int(Tokens[7])

        bytecode.append(self.opcode)
        bytecode.append(x_coord)
        bytecode.append(y_coord)
        bytecode.append((object_type << 4) | ai_type)
        bytecode.append((ai_data << 4) | num_units)
        bytecode.append(wait_between)

        return bytecode

class InstructionWait(Instruction):
    def __init__(self, NumBytes, Opcode):
        Instruction.__init__(self, NumBytes, Opcode)

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
        'spawn': InstructionSpawn(8, 0x00),
        'wait': InstructionWait(2, 0x01),
        'over': Instruction(1, 0xff),
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

    for line in bytecode:
        for byte in line:
            print('0x{:0>2x}, '.format(byte), end = '')

        print()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: {} level.txt'.format(sys.argv[0]))
    else:
        main(sys.argv[1])
