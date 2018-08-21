CFLAGS=		-W -Wall -Wformat=2 -fno-stack-protector -zexecstack -ggdb
LDFLAGS=	-lcrypto -lcurl -zexecstack

# Define the pre-processor flags for the crypter and decrypter. The former only
# needs to retrieve the opcodes of the specified shellcode, while the latter
# needs the hex encoded shellcode.
ifeq ($(MAKECMDGOALS),crypter)
CPPFLAGS=	-DSHELLCODE=`asm-opcodes $(SHELLCODE)`
else
CPPFLAGS=	-DSHELLCODE=`./crypter|tail -n1`
endif

all:

.PHONY: clean
clean:
	rm -f crypter decrypter *.o

crypter: crypter.o common.o
decrypter: decrypter.o common.o
