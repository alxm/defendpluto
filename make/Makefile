A_APP_NAME = arduboy-shooter

Z_LEVELS_COMPILER = ../levels/compile.py
Z_LEVELS_TXT = ../levels/data.txt
Z_LEVELS_H = ../src/data_levels.h

Z_GFX_COMPILER = ../gfx/ArduboyImage.py
Z_GFX_PNG_FILES = $(notdir $(wildcard ../gfx/*.png))
Z_GFX_H_FILES = $(addprefix ../src/data_gfx_, $(Z_GFX_PNG_FILES:.png=.h))

#
# You can set A2X_PATH to a hard-coded value
# if you do not have a2x/bin in your $PATH
#
A2X_PATH := $(shell a2x_path)
include $(A2X_PATH)/make/project/Makefile

$(O_DIR)/util_vm.o : $(Z_LEVELS_H)
$(O_DIR)/platform.o : $(Z_GFX_H_FILES)

$(Z_LEVELS_H): $(Z_LEVELS_TXT) $(Z_LEVELS_COMPILER)
	python3 $(Z_LEVELS_COMPILER) $< > $@

../src/data_gfx_%.h: ../gfx/%.png $(Z_GFX_COMPILER)
	python3 $(Z_GFX_COMPILER) ../gfx/palette.png $< $(patsubst %.png, %, $(notdir $<)) > $@
