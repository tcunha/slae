PROG=	egghunter

# Define the egghunter and second stage shellcode to use. The later can be given
# from the command-line.
CPPFLAGS=	-DSHELLCODE=`asm-opcodes $(PROG)`
CPPFLAGS+=	-DSTAGE2=`asm-opcodes $(STAGE2)`
# The skeleton C file that is going to be used is special, since it needs the
# egghunter in contrast with the first two.
SKELETON=	skel.c

include ../Makefile.common
