MODULE := engines/composer

MODULE_OBJS = \
	composer.o \
	detection.o \
	graphics.o \
	resource.o \
	scripting.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
