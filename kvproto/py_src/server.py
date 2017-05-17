import struct
import socket

ip_port = ('127.0.0.1', 20160)
sk = socket.socket()
sk.bind(ip_port)
sk.listen(5)
while True:
	print ('server waiting...')
	conn,addr = sk.accept()
	client_data = conn.recv(1024)
	print("data:[%b]", client_data)
	magic_num, version, msg_len, msg_id = struct.unpack('!2HIq', client_data)
	print magic_num
	print version
	print msg_len
	print msg_id
	conn.close()

