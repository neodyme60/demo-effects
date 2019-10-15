AC_INIT(Opcode.h)

AM_INIT_AUTOMAKE(OPCODE, 1.2.1)

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
	
#file OPC_TreeCollider.cpp generates run-time errors when compiled with optimisation -O2 when using gcc >= 3.0 
#Therefore this check is made, so when gcc >= 3.0, file OPC_TreeCollider.cpp is compiled with -O1
AC_MSG_CHECKING(checking gcc version)
AM_CONDITIONAL(GCCVERSION3, gcc -dumpversion | grep ^3. > /dev/null)
AC_MSG_RESULT()

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE

dnl Checks for library functions.

AC_OUTPUT(Makefile)
