#!/usr/bin/env python3

"""
    Copyright 2017, 2018 Alex Margarit <alex@alxm.org>

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
"""

import sys
import time

class Op:
    numInstructions = 0

    def __init__(self, NumArgs = 0):
        self.__numArgs = NumArgs
        self.__opcode = Op.numInstructions

        Op.numInstructions += 1

    def compile(self, Compiler, Args):
        if len(Args) < 1 + self.__numArgs:
            Compiler.error('{} requires at least {} arguments'
                            .format(Args[0], self.__numArgs))

        if self.__numArgs > 0:
            #
            # 8b 8b
            # op flags
            #
            bytecode = [self.__opcode, 0]
        else:
            #
            # 8b
            # op
            #
            bytecode = [self.__opcode]

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

class OpIter(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        Compiler.iterInc()

        #
        # 8b
        # iteration
        # 0
        #
        iteration = Compiler.checkArg(Bytecode, Args, 0, 0, 255)

        Bytecode.append(iteration)

        return Bytecode

class OpEndI(Op):
    def __init__(self):
        Op.__init__(self, 0)

    def custom_compile(self, Compiler, Args, Bytecode):
        Compiler.iterDec()

        return Bytecode

class OpWait(Op):
    def __init__(self):
        Op.__init__(self, 1)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b
        # ds
        # 25
        #
        ds = Compiler.checkArg(Bytecode, Args, 0, 0, 255)

        Bytecode.append(ds)

        return Bytecode

class OpSpawn(Op):
    def __init__(self):
        Op.__init__(self, 3)

    def custom_compile(self, Compiler, Args, Bytecode):
        #
        # 8b      8b      4b      4b     4b
        # x_coord y_coord type_id fly_id attack_id
        # 40      -1      enemy0  down   target
        #
        x_coord = Compiler.checkArg(Bytecode, Args, 0, -128, 127)
        y_coord = Compiler.checkArg(Bytecode, Args, 1, -128, 127)
        type_id = Compiler.getIdValue('enemy', Args, 2)
        fly_id = Compiler.getIdValue('fly', Args, 3)
        attack_id = Compiler.getIdValue('attack', Args, 4)

        Bytecode.append(x_coord)
        Bytecode.append(y_coord)
        Bytecode.append((type_id << 4) | fly_id)
        Bytecode.append((attack_id << 4) | 0)

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
        self.__levelFile = LevelFile
        self.__nextVarId = 0
        self.__nestedLoopsMax = 0
        self.__nestedLoopsCount = 0
        self.__nestedIterCount = 0
        self.__lineNumber = 0
        self.__varIds = {}

        self.__ids = {
            'enemy': {
                'asteroid': 0,
                'enemy0': 1,
                'enemy1': 2,
                'enemy2': 3,
            },

            'fly': {
                'still': 0,
                'down': 1,
                'follow': 2,
                'circleAbs': 3,
                'circleRel': 4,
            },

            'attack': {
                'none': 0,
                'front': 1,
                'target': 2,
            },
        }

        self.__ops = {
            'over': Op(),
            'set': OpSet(),
            'inc': OpInc(),
            'flip': OpFlip(),
            'loop': OpLoop(),
            'end': OpEnd(),
            'iter': OpIter(),
            'endi': OpEndI(),
            'wait': OpWait(),
            'clear': Op(),
            'spawn': OpSpawn(),
            'done': Op(),

            # Must always be last, doesn't get compiled
            'var': OpVar(),
        }

    def error(self, Text):
        print('\033[1;31m[Script Error]\033[0m Line {}: {}'
                .format(self.__lineNumber + 1, Text),
              file = sys.stderr)
        sys.exit(1)

    def getIdValue(self, Category, Args, Index):
        if Index >= len(Args):
            return 0

        ids = self.__ids[Category]
        name = Args[Index]

        if name not in ids:
            self.error('Unknown id {}.{}'.format(Category, name))

        return ids[name]

    def checkArg(self, Bytecode, Args, Index, Min, Max, Hex = False):
        if Index >= len(Args):
            return 0

        arg = Args[Index]

        try:
            value = int(arg, 16 if Hex else 10)

            if value < Min or value > Max:
                self.error('Value {} is out of range [{}, {}]'
                            .format(value, Min, Max))

            return value
        except ValueError:
            if arg not in self.__varIds:
                self.error('Arg {} is neither a var nor an int'.format(arg))

            if Index >= 8:
                self.error('Only the first 8 arguments may be variables')

            Bytecode[1] |= 1 << Index
            varId = self.getVarId(arg)

            if varId < Min or varId > Max:
                self.error('Var {} ({}) is out of range [{}, {}]'
                            .format(arg, varId, Min, Max))

            return varId


    def getVarId(self, Name):
        if Name not in self.__varIds:
            self.error('Unknown var {}'.format(Name))

        return self.__varIds[Name]

    def setVarId(self, Name):
        if Name in self.__varIds:
            self.error('Var {} was already declared'.format(Name))

        self.__varIds[Name] = self.__nextVarId
        self.__nextVarId += 1

    def getOp(self, Name):
        if Name not in self.__ops:
            self.error('Unknown op {}'.format(Name))

        return self.__ops[Name]

    def loopInc(self):
        self.__nestedLoopsCount += 1
        self.__nestedLoopsMax = max(self.__nestedLoopsMax,
                                    self.__nestedLoopsCount)

    def loopDec(self):
        self.__nestedLoopsCount -= 1

        if self.__nestedLoopsCount < 0:
            self.error('Mismatched loop end')

    def iterInc(self):
        self.__nestedIterCount += 1

        if self.__nestedIterCount > 1:
            self.error('Nested iter block')

        if self.__nestedLoopsCount == 0:
            self.error('iter block outside a loop block')

    def iterDec(self):
        self.__nestedIterCount -= 1

        if self.__nestedIterCount < 0:
            self.error('Mismatched iter endi')

    def run(self):
        bytecode = []
        bytesSize = 0

        with open(self.__levelFile, 'rU') as f:
            for self.__lineNumber, line in enumerate(f):
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
                    bytesSize += len(compiled)

        fmt_bytecode = ''

        for line in bytecode:
            fmt_bytecode += '\n    '

            for byte in line:
                fmt_bytecode += '0x{:0>2x},'.format((byte + 2**8) % (2**8))

        contents = """\
/*
    Copyright {year} Alex Margarit <alex@alxm.org>
    This file is part of Defend Pluto.
    Generated by levels/compile.py from {path}
*/

#define Z_LEVELS_VARS_NUM {varsNum}
#define Z_LEVELS_NESTED_LOOPS_MAX {loopsMax}
#define Z_LEVELS_BYTES_SIZE {bytesSize}

PROGMEM static const uint8_t z_data_levels[Z_LEVELS_BYTES_SIZE] = {{{data}
}};\
""".format(varsNum = self.__nextVarId,
           loopsMax = self.__nestedLoopsMax,
           bytesSize = bytesSize,
           data = fmt_bytecode,
           year = time.strftime('%Y'),
           path = self.__levelFile)

        print(contents)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: {} data.txt'.format(sys.argv[0]))
    else:
        CompilerTool(sys.argv[1]).run()
