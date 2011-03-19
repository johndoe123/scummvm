MODULE := engines/tsage

MODULE_OBJS := \
	converse.o \
	core.o \
	debugger.o \
	detection.o \
	dialogs.o \
	events.o \
	globals.o \
	graphics.o \
	resources.o \
	ringworld_logic.o \
	ringworld_scenes1.o \
	ringworld_scenes2.o \
	ringworld_scenes3.o \
	ringworld_scenes4.o \
	saveload.o \
	scenes.o \
	sound.o \
	staticres.o \
	tsage.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules 
include $(srcdir)/rules.mk

