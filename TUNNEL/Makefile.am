if OPENGL
noinst_PROGRAMS=tunnel
else
noinst_PROGRAMS=
endif

tunnel_SOURCES = tunnel.C 
tunnel_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
AM_CPPFLAGS= -I../COMMON/ -I../WPCG
