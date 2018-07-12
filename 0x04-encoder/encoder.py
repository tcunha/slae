#!/usr/bin/python

# Follows YEnc draft 1.3 with regards to the encoding process. No header or
# trailer will be used by the decoder, nor will the CRC32.
#
# An encoded character is equal to its original ASCII value plus 42, modulo 256.
# In case of a special character is detected (0x00, 0x0a, 0x0d and 0x3d), the
# resulting encoded character is prefixed with the escape 0x3d character and its
# already encoded result (with the above) will be incremented 64, modulo 256.

import sys

if len(sys.argv) == 1:
    sys.exit("usage: opcodes")
print "Original shellcode size: " + str(len(sys.argv[1]))

enc = list()
for c in sys.argv[1]:
    c = (ord(c) + 42) % 256
    if c == 0x00 or c == 0x0a or c == 0x0d or c == 0x3d:
        enc.append(hex(61))
        c = (c + 64) % 256                  # critical character; double encode
    enc.append(hex(c))                      # regular character

print "Encoded shellcode size: " + str(len(enc))
print ",".join(enc)
