# Define the pre-processor flags by retrieving the opcodes of the current
# shellcode, as well as, the skeleton C file that is going to be used.
CPPFLAGS=	-DSHELLCODE=`asm-opcodes $(PROG)`
# The skeleton C file that is going to be used is special, since it needs to
# listen on a given port.
SKELETON=	skel.c

include ../Makefile.common
