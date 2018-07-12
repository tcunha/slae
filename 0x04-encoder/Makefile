PROG=	decoder

# Define the pre-processor flags by retrieving the opcodes of the current
# shellcode, as well as, the skeleton C file that is going to be used.
CPPFLAGS=	-DSHELLCODE=`asm-opcodes $(PROG)`
SKELETON=	../skel.c

include ../Makefile.common
