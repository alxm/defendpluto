A_CONFIG_APP_NAME := defendpluto
A_CONFIG_DIR_SRC := DefendPluto
A_CONFIG_MAKE_CLEAN := genclean

include $(A2X_PATH)/make/project/Makefile

Z_ROOT := ..
Z_DIR_GEN := $(Z_ROOT)/$(A_DIR_SRC)/generated
Z_DIR_GFX := $(Z_ROOT)/assets/gfx
Z_DIR_SFX := $(Z_ROOT)/assets/sfx

Z_LEVELS_COMPILER := $(Z_ROOT)/assets/levels/compile.py
Z_LEVELS_TXT := $(Z_ROOT)/assets/levels/data.txt
Z_LEVELS_H := $(Z_DIR_GEN)/data_levels.h

Z_GFX_COMPILER := $(Z_DIR_GFX)/compile.py
Z_GFX_PNG_FILES := $(notdir $(wildcard $(Z_DIR_GFX)/*.png))
Z_GFX_H_FILES := $(addprefix $(Z_DIR_GEN)/data_gfx_, $(Z_GFX_PNG_FILES:.png=.h))

Z_SFX_COMPILER := $(Z_DIR_SFX)/compile.py
Z_SFX_WAV_FILES := $(notdir $(wildcard $(Z_DIR_SFX)/*.wav))
Z_SFX_H_FILES := $(addprefix $(Z_DIR_GEN)/data_sfx_, $(Z_SFX_WAV_FILES:.wav=.h))

$(A_DIR_OBJ_PLATFORM)/util_vm.c.o : $(Z_LEVELS_H)
$(A_DIR_OBJ_PLATFORM)/util_graphics.c.o : $(Z_GFX_H_FILES)
$(A_DIR_OBJ_PLATFORM)/util_sound.c.o : $(Z_SFX_H_FILES)

$(Z_LEVELS_H): $(Z_LEVELS_TXT) $(Z_LEVELS_COMPILER)
	@ mkdir -p $(@D)
	python3 $(Z_LEVELS_COMPILER) $< > $@

$(Z_DIR_GEN)/data_gfx_%.h: $(Z_DIR_GFX)/%.png $(Z_GFX_COMPILER)
	@ mkdir -p $(@D)
	python3 $(Z_GFX_COMPILER) $(Z_DIR_GFX)/palette.png $< $(patsubst %.png, %, $(notdir $<)) > $@

$(Z_DIR_GEN)/data_sfx_%.h: $(Z_DIR_SFX)/%.wav $(Z_SFX_COMPILER)
	@ mkdir -p $(@D)
	python3 $(Z_SFX_COMPILER) $< $(patsubst %.wav, %, $(notdir $<)) > $@

genclean:
	rm -rf $(Z_DIR_GEN)
