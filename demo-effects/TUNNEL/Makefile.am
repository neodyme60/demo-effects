if OPENGL
noinst_PROGRAMS=tunnel
else
noinst_PROGRAMS=
endif

tunnel_SOURCES = tunnel.C 
tunnel_LDADD = ../COMMON/libTDEC.a ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
INCLUDES= -I../COMMON/ -I../WPCG
