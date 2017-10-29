A_APP_NAME = arduboy-shooter

#
# You can set A2X_PATH to a hard-coded value
# if you do not have a2x/bin in your $PATH
#
A2X_PATH := $(shell a2x_path)
include $(A2X_PATH)/make/project/Makefile

Z_LEVELS_COMPILER = ../src/levels/compile.py
Z_LEVELS_TXT = ../src/levels/data.txt
Z_LEVELS_C = ../src/levels_data.c

$(Z_LEVELS_C): $(Z_LEVELS_TXT)
	$(Z_LEVELS_COMPILER) $< > $@
