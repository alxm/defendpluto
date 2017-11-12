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

varIds = {}

spriteIds = {
    'asteroid': 0,
    'enemy0': 1,
    'enemy1': 2,
    'enemy2': 3,
}

aiIds = {
    'straightdown': 0,
    'zigzag': 1,
}

class Instruction:
    numInstructions = 0

    def __init__(self, NumArgs = 0):
        self.numArgs = NumArgs
        self.opcode = Instruction.numInstructions

        Instruction.numInstructions += 1

    def checkVar(self, Bytecode, Tokens, Index):
        token = Tokens[Index]

        if token in varIds:
            if 0 <= Index <= 7:
                Bytecode[0] |= 1 << Index
                return varIds[token]
            else:
                return 0
        else:
            return int(token)

    def compile(self, Tokens):
        if len(Tokens) != 1 + self.numArgs:
            print('{} requires {} arguments'.format(Tokens[0], self.numArgs))
            sys.exit(1)

        #
        # 8b    8b
        # flags op
        #
        bytecode = [0, self.opcode]

        return self.custom_compile(Tokens[1 :], bytecode)

    def custom_compile(self, Tokens, Bytecode):
        return Bytecode

class InstructionSpawn(Instruction):
    def __init__(self):
        Instruction.__init__(self, 5)

    def custom_compile(self, Tokens, Bytecode):
        #
        # 8b      8b      4b      4b     8b
        # x_coord y_coord type_id ai_id  ai_args
        # 64      -8      enemy0  zigzag 0
        #
        x_coord = self.checkVar(Bytecode, Tokens, 0)
        y_coord = self.checkVar(Bytecode, Tokens, 1)
        type_id = spriteIds[Tokens[2]]
        ai_id = aiIds[Tokens[3]]
        ai_args = self.checkVar(Bytecode, Tokens, 4)

        Bytecode.append(x_coord)
        Bytecode.append(y_coord)
        Bytecode.append((type_id << 4) | ai_id)
        Bytecode.append(ai_args)

        return Bytecode

class InstructionWait(Instruction):
    def __init__(self):
        Instruction.__init__(self, 1)

    def custom_compile(self, Tokens, Bytecode):
        #
        # 8b
        # frames
        # 30
        #
        frames = self.checkVar(Bytecode, Tokens, 0)

        Bytecode.append(frames)

        return Bytecode

class InstructionLoop(Instruction):
    def __init__(self):
        Instruction.__init__(self, 1)

    def custom_compile(self, Tokens, Bytecode):
        #
        # 8b
        # num_times
        # 10
        #
        num_times = self.checkVar(Bytecode, Tokens, 0)

        Bytecode.append(num_times)

        return Bytecode

class InstructionSet(Instruction):
    def __init__(self):
        Instruction.__init__(self, 2)

    def custom_compile(self, Tokens, Bytecode):
        #
        # 8b     8b
        # var_id value
        # x      32
        #
        var_id = varIds[Tokens[0]]
        value = self.checkVar(Bytecode, Tokens, 1)

        Bytecode.append(var_id)
        Bytecode.append(value)

        return Bytecode

class InstructionInc(Instruction):
    def __init__(self):
        Instruction.__init__(self, 2)

    def custom_compile(self, Tokens, Bytecode):
        #
        # 8b     8b
        # var_id value
        # x      16
        #
        var_id = varIds[Tokens[0]]
        value = self.checkVar(Bytecode, Tokens, 1)

        Bytecode.append(var_id)
        Bytecode.append(value)

        return Bytecode

class InstructionBind(Instruction):
    def __init__(self):
        Instruction.__init__(self, 2)

    def custom_compile(self, Tokens, Bytecode):
        #
        # var_name var_id
        # x        0
        #
        var_name = Tokens[0]
        var_id = int(Tokens[1])

        varIds[var_name] = var_id

        return None

def main(LevelFile):
    instructions = {
        'spawn': InstructionSpawn(),
        'wait': InstructionWait(),
        'waitclear': Instruction(),
        'loop': InstructionLoop(),
        'end': Instruction(),
        'over': Instruction(),
        'set': InstructionSet(),
        'inc': InstructionInc(),
        'bind': InstructionBind(),
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
            compiled = instruction.compile(tokens)

            if compiled:
                bytecode.append(compiled)

    formatted_bytecode = ''

    for line in bytecode:
        formatted_bytecode += '\n    '

        for byte in line:
            formatted_bytecode += '0x{:0>2x},'.format((byte + 2**8) % (2**8))

    contents = """\
// Generated by levels/compile.py
PROGMEM static const uint8_t z_data_levels[] = {{{}
}};\
""".format(formatted_bytecode)

    print(contents)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: {} data.txt'.format(sys.argv[0]))
    else:
        main(sys.argv[1])
