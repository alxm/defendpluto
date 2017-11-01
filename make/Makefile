A_APP_NAME = arduboy-shooter

Z_LEVELS_COMPILER = ../src/levels/compile.py
Z_LEVELS_TXT = ../src/levels/data.txt
Z_LEVELS_H = ../src/data_levels.h

Z_GFX_COMPILER = ../gfx/ArduboyImage.py
Z_GFX_PNG_FILES = $(notdir $(wildcard ../gfx/*.png))
Z_GFX_H_FILES = $(addprefix ../src/data_gfx_, $(Z_GFX_PNG_FILES:.png=.h))

A_EXTRA_GEN_FILES = $(Z_LEVELS_H) $(Z_GFX_H_FILES)

#
# You can set A2X_PATH to a hard-coded value
# if you do not have a2x/bin in your $PATH
#
A2X_PATH := $(shell a2x_path)
include $(A2X_PATH)/make/project/Makefile

$(Z_LEVELS_H): $(Z_LEVELS_TXT)
	python3 $(Z_LEVELS_COMPILER) $< > $@

../src/data_gfx_%.h: ../gfx/%.png
	python3 $(Z_GFX_COMPILER) $< $(patsubst %.png, %, $(notdir $<)) > $@
