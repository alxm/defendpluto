A_APP_NAME = defendpluto
A_DIR_GFX = assets/gfx
A_DIR_SFX = assets/sfx
A_DIR_SRC = ../DefendPluto

Z_LEVELS_COMPILER = ../assets/levels/compile.py
Z_LEVELS_TXT = ../assets/levels/data.txt
Z_LEVELS_H = $(A_DIR_SRC)/data_levels.h

Z_GFX_COMPILER = ../$(A_DIR_GFX)/ArduboyImage.py
Z_GFX_PNG_FILES = $(notdir $(wildcard ../$(A_DIR_GFX)/*.png))
Z_GFX_H_FILES = $(addprefix $(A_DIR_SRC)/data_gfx_, $(Z_GFX_PNG_FILES:.png=.h))

Z_SFX_COMPILER = ../$(A_DIR_SFX)/compile.py
Z_SFX_WAV_FILES = $(notdir $(wildcard ../$(A_DIR_SFX)/*.wav))
Z_SFX_H_FILES = $(addprefix $(A_DIR_SRC)/data_sfx_, $(Z_SFX_WAV_FILES:.wav=.h))

A_RULES_CLEAN = genclean

#
# You can set A2X_PATH to a hard-coded value
# if you do not have a2x/bin in your $PATH
#
A2X_PATH := $(shell a2x_path)
include $(A2X_PATH)/make/project/Makefile

$(A_DIR_OBJ_PLATFORM)/util_vm.c.o : $(Z_LEVELS_H)
$(A_DIR_OBJ_PLATFORM)/util_graphics.c.o : $(Z_GFX_H_FILES)
$(A_DIR_OBJ_PLATFORM)/util_sound.c.o : $(Z_SFX_H_FILES)

$(Z_LEVELS_H): $(Z_LEVELS_TXT) $(Z_LEVELS_COMPILER)
	python3 $(Z_LEVELS_COMPILER) $< > $@

$(A_DIR_SRC)/data_gfx_%.h: ../$(A_DIR_GFX)/%.png $(Z_GFX_COMPILER)
	python3 $(Z_GFX_COMPILER) ../$(A_DIR_GFX)/palette.png $< $(patsubst %.png, %, $(notdir $<)) > $@

$(A_DIR_SRC)/data_sfx_%.h: ../$(A_DIR_SFX)/%.wav $(Z_SFX_COMPILER)
	python3 $(Z_SFX_COMPILER) $< $(patsubst %.wav, %, $(notdir $<)) > $@

genclean:
	@ echo "Removing generated $(Z_LEVELS_H) $(Z_GFX_H_FILES) $(Z_SFX_H_FILES)"
	@ rm -rf $(Z_LEVELS_H) $(Z_GFX_H_FILES) $(Z_SFX_H_FILES)
