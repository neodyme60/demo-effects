noinst_PROGRAMS = multilayer
multilayer_SOURCES = multilayer.c 
multilayer_LDFLAGS = $(LIBLTDL) -export-dynamic ../COMMON/libTDEC.la "-dlopen" self "-dlopen" ../PLUGINS/PLASMA/plasma.la "-dlopen" ../PLUGINS/FIRE/fire.la
multilayer_DEPENDENCIES = $(LIBLTDL)
INCLUDES = -I../COMMON