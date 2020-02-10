if OPENGL
noinst_PROGRAMS=plasmacube
else
noinst_PROGRAMS=
endif

plasmacube_SOURCES = plasmacube.C 
plasmacube_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
AM_CPPFLAGS= -I../COMMON/ -I../WPCG
