import struct

a='hello'
b='world!'
c=0x10
d=45.123
bytes=struct.pack('5s6sif',a,b,c,d)

a,b,c,d=struct.unpack('5s6sif',bytes)
print a, b, c, d

