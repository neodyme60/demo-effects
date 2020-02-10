if OPENGL
noinst_PROGRAMS=terrain
else
noinst_PROGRAMS=
endif

terrain_SOURCES = terrain.C 
terrain_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
AM_CPPFLAGS= -I../COMMON/ -I../WPCG
