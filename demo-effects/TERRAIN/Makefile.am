if OPENGL
noinst_PROGRAMS=terrain
else
noinst_PROGRAMS=
endif

terrain_SOURCES = terrain.C 
terrain_LDADD = ../COMMON/libTDEC.a ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
INCLUDES= -I../COMMON/ -I../WPCG
