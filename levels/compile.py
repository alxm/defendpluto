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
        value = Compiler.checkArg(Bytecode, Args, 1, -128, 127)

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
        value = Compiler.checkArg(Bytecode, Args, 1, -128, 127)

        Bytecode.append(var_id)
        Bytecode.append(value)

        return Bytecode

class OpFlip(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b
        # var_id
        # x
        #
        var_id = Compiler.getVarId(Args[0])

        Bytecode.append(var_id)

        return Bytecode

class OpLoop(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        Compiler.loopInc()

        #
        # 8b
        # num_times
        # 10
        #
        num_times = Compiler.checkArg(Bytecode, Args, 0, 0, 255)

        Bytecode.append(num_times)

        return Bytecode

class OpEnd(Op):
    def __init__(self):
        Op.__init__(self, 0)

    def custom_compile(self, Compiler, Args, Bytecode):
        Compiler.loopDec()

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
        frames = Compiler.checkArg(Bytecode, Args, 0, 0, 255)

        Bytecode.append(frames)

        return Bytecode

class OpSpawn(Op):
    def __init__(self):
        Op.__init__(self, 3)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b      8b      4b      4b
        # x_coord y_coord type_id drop_id
        # 64      -8      enemy0  powerup
        #
        x_coord = Compiler.checkArg(Bytecode, Args, 0, -128, 127)
        y_coord = Compiler.checkArg(Bytecode, Args, 1, -128, 127)
        type_id = Compiler.getEnemyId(Args[2])
        drop_id = Compiler.getDropId(Args[3]) if len(Args) > 3 else 0

        Bytecode.append(x_coord)
        Bytecode.append(y_coord)
        Bytecode.append((type_id << 4) | drop_id)

        return Bytecode

class OpVar(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # var_name
        # x
        #
        var_name = Args[0]

        Compiler.setVarId(var_name)

        return None

class CompilerTool:
    def __init__(self, LevelFile):
        self.levelFile = LevelFile

        self.nextVarId = 0

        self.nestedLoopsMax = 0
        self.nestedLoopsCount = 0

        self.lineNumber = 0
        self.varIds = {}

        self.enemyIds = {
            'asteroid': 0,
            'enemy0': 1,
            'enemy1': 2,
            'enemy2': 3,
        }

        self.dropIds = {
            'none': 0,
        }

        self.ops = {
            'over': Op(),
            'set': OpSet(),
            'inc': OpInc(),
            'flip': OpFlip(),
            'loop': OpLoop(),
            'end': OpEnd(),
            'wait': OpWait(),
            'waitclear': Op(),
            'spawn': OpSpawn(),

            # Must always be last, doesn't get compiled
            'var': OpVar(),
        }

    def error(self, Text):
        print('\033[1;31m[Script Error]\033[0m Line {}: {}'
                .format(self.lineNumber + 1, Text),
              file = sys.stderr)
        sys.exit(1)

    def checkArg(self, Bytecode, Args, Index, Min, Max):
        arg = Args[Index]

        if arg in self.varIds:
            if Index >= 8:
                self.error('Only the first 8 arguments may be variables')

            Bytecode[1] |= 1 << Index
            varId = self.getVarId(arg)

            if varId < Min or varId > Max:
                self.error('Var {} ({}) is out of range [{}, {}]'
                            .format(arg, varId, Min, Max))

            return varId
        else:
            value = int(arg)

            if value < Min or value > Max:
                self.error('Value {} is out of range [{}, {}]'
                            .format(value, Min, Max))

            return value

    def getVarId(self, Name):
        if Name not in self.varIds:
            self.error('Unknown var {}'.format(Name))

        return self.varIds[Name]

    def setVarId(self, Name):
        if Name in self.varIds:
            self.error('Var {} was already declared'.format(Name))

        self.varIds[Name] = self.nextVarId
        self.nextVarId += 1

    def getEnemyId(self, Name):
        if Name not in self.enemyIds:
            self.error('Unknown enemy {}'.format(Name))

        return self.enemyIds[Name]

    def getDropId(self, Name):
        if Name not in self.dropIds:
            self.error('Unknown drop {}'.format(Name))

        return self.dropIds[Name]

    def getOp(self, Name):
        if Name not in self.ops:
            self.error('Unknown op {}'.format(Name))

        return self.ops[Name]

    def loopInc(self):
        self.nestedLoopsCount += 1
        self.nestedLoopsMax = max(self.nestedLoopsMax, self.nestedLoopsCount)

    def loopDec(self):
        self.nestedLoopsCount -= 1

        if self.nestedLoopsCount < 0:
            self.error('Mismatched loop end')

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

#define Z_LEVELS_VARS_NUM {}
#define Z_LEVELS_NESTED_LOOPS_MAX {}

PROGMEM static const uint8_t z_data_levels[] = {{{}
}};\
""".format(self.nextVarId, self.nestedLoopsMax, fmt_bytecode)

        print(contents)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: {} data.txt'.format(sys.argv[0]))
    else:
        CompilerTool(sys.argv[1]).run()
