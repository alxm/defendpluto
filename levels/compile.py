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

class Op:
    numInstructions = 0

    def __init__(self, NumArgs = 0):
        self.numArgs = NumArgs
        self.opcode = Op.numInstructions

        Op.numInstructions += 1

    def compile(self, Compiler, Args):
        if len(Args) < 1 + self.numArgs:
            Compiler.error('{} requires at least {} arguments'
                            .format(Args[0], self.numArgs))

        if self.numArgs > 0:
            #
            # 8b 8b
            # op flags
            #
            bytecode = [self.opcode, 0]
        else:
            #
            # 8b
            # op
            #
            bytecode = [self.opcode]

        return self.custom_compile(Compiler, Args[1 :], bytecode)

    def custom_compile(self, Compiler, Args, Bytecode):
        return Bytecode

class OpSet(Op):
    def __init__(self):
        Op.__init__(self, 2)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b     8b
        # var_id value
        # x      32
        #
        var_id = Compiler.getVarId(Args[0])
        value = Compiler.checkVar(Bytecode, Args, 1)

        Bytecode.append(var_id)
        Bytecode.append(value)

        return Bytecode

class OpInc(Op):
    def __init__(self):
        Op.__init__(self, 2)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b     8b
        # var_id value
        # x      16
        #
        var_id = Compiler.getVarId(Args[0])
        value = Compiler.checkVar(Bytecode, Args, 1)

        Bytecode.append(var_id)
        Bytecode.append(value)

        return Bytecode

class OpLoop(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        Compiler.nestedLoopsCount += 1

        if Compiler.nestedLoopsCount > Compiler.nestedLoopsLimit:
            Compiler.error('Exceeded max nested loops limit of {}'
                            .format(Compiler.nestedLoopsLimit))

        #
        # 8b
        # num_times
        # 10
        #
        num_times = Compiler.checkVar(Bytecode, Args, 0)

        Bytecode.append(num_times)

        return Bytecode

class OpEnd(Op):
    def __init__(self):
        Op.__init__(self, 0)

    def custom_compile(self, Compiler, Args, Bytecode):
        Compiler.nestedLoopsCount -= 1

        if Compiler.nestedLoopsCount < 0:
            Compiler.error('Mismatched end instruction')

        return Bytecode

class OpWait(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b
        # frames
        # 30
        #
        frames = Compiler.checkVar(Bytecode, Args, 0)

        Bytecode.append(frames)

        return Bytecode

class OpSpawn(Op):
    def __init__(self):
        Op.__init__(self, 3)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b      8b      4b      4b     4b    4b
        # x_coord y_coord type_id ai_id  delay flipX
        # 64      -8      enemy0  zigzag 0     0
        #
        x_coord = Compiler.checkVar(Bytecode, Args, 0)
        y_coord = Compiler.checkVar(Bytecode, Args, 1)
        type_id = Compiler.getEnemyId(Args[2])
        ai_id = Compiler.getAiId(Args[3]) if len(Args) > 3 else 0
        delay = Compiler.checkVar(Bytecode, Args, 4) if len(Args) > 4 else 0
        flipX = Compiler.checkVar(Bytecode, Args, 5) if len(Args) > 5 else 0

        Bytecode.append(x_coord)
        Bytecode.append(y_coord)
        Bytecode.append((type_id << 4) | ai_id)
        Bytecode.append((delay << 4) | flipX)

        return Bytecode

class OpBind(Op):
    def __init__(self):
        Op.__init__(self, 2)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # var_name var_id
        # x        0
        #
        var_name = Args[0]
        var_id = int(Args[1])

        Compiler.setVarId(var_name, var_id)

        return None

class CompilerTool:
    def __init__(self, LevelFile):
        self.levelFile = LevelFile

        self.varMaxId = 2

        self.nestedLoopsLimit = 2
        self.nestedLoopsCount = 0

        self.lineNumber = 0
        self.varIds = {}

        self.enemyIds = {
            'asteroid': 0,
            'enemy0': 1,
            'enemy1': 2,
            'enemy2': 3,
        }

        self.aiIds = {
            'down': 0,
            'zigzag': 1,
            'curve': 2,
        }

        self.ops = {
            'over': Op(),
            'set': OpSet(),
            'inc': OpInc(),
            'loop': OpLoop(),
            'end': OpEnd(),
            'wait': OpWait(),
            'waitclear': Op(),
            'spawn': OpSpawn(),

            # Must always be last, doesn't get compiled
            'bind': OpBind(),
        }

    def error(self, Text):
        print('\033[1;31m[Script Error]\033[0m Line {}: {}'
                .format(self.lineNumber + 1, Text),
              file = sys.stderr)
        sys.exit(1)

    def checkVar(self, Bytecode, Tokens, Index):
        token = Tokens[Index]

        if self.hasVar(token):
            if Index >= 8:
                self.error('Only the first 8 arguments may be variables')

            Bytecode[1] |= 1 << Index

            return self.getVarId(token)
        else:
            return int(token)

    def hasVar(self, Name):
        return Name in self.varIds

    def getVarId(self, Name):
        if Name not in self.varIds:
            self.error('Unknown var {}'.format(Name))

        return self.varIds[Name]

    def setVarId(self, Name, VarId):
        if Name in self.varIds:
            self.error('Var {} was already bound'.format(Name))
        elif VarId >= self.varMaxId:
            self.error('Max var ID is {}'.format(self.varMaxId))

        self.varIds[Name] = VarId

    def getEnemyId(self, Name):
        if Name not in self.enemyIds:
            self.error('Unknown enemy {}'.format(Name))

        return self.enemyIds[Name]

    def getAiId(self, Name):
        if Name not in self.aiIds:
            self.error('Unknown AI {}'.format(Name))

        return self.aiIds[Name]

    def getOp(self, Name):
        if Name not in self.ops:
            self.error('Unknown op {}'.format(Name))

        return self.ops[Name]

    def run(self):
        bytecode = []

        with open(self.levelFile, 'rU') as f:
            for self.lineNumber, line in enumerate(f):
                tokens = []
                rawTokens = line.split()

                for t in rawTokens:
                    if t[0] == ';':
                        break

                    tokens.append(t)

                if len(tokens) == 0:
                    continue

                instruction = self.getOp(tokens[0])
                compiled = instruction.compile(self, tokens)

                if compiled:
                    bytecode.append(compiled)

        fmt_bytecode = ''

        for line in bytecode:
            fmt_bytecode += '\n    '

            for byte in line:
                fmt_bytecode += '0x{:0>2x},'.format((byte + 2**8) % (2**8))

        contents = """\
// Generated by levels/compile.py
PROGMEM static const uint8_t z_data_levels[] = {{{}
}};\
""".format(fmt_bytecode)

        print(contents)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: {} data.txt'.format(sys.argv[0]))
    else:
        CompilerTool(sys.argv[1]).run()
