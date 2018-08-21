# Define the main shellcode and second stage shellcode to use. The later can be
# given from the command-line.
CPPFLAGS=	-DSHELLCODE=`asm-opcodes $(PROG)`
SKELETON=	../skel.c

ifeq ($(PROG),369)			# 369 has a second stage shellcode.
CPPFLAGS+=	-DSTAGE2=`asm-opcodes $(STAGE2)`
SKELETON=	$(PROG).c
endif

include ../Makefile.common
