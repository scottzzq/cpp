# -*- coding:utf-8 -*-
import raft_cmdpb_pb2
import metapb_pb2
import socket
import sys
import struct
import msgpb_pb2
import uuid
import pdpb_pb2
import volume_cmdpb_pb2

pd_host = "127.0.0.1"
pd_port = 2379

pd_ip_port = (pd_host, pd_port)

def get_region(pd_sk, region_id):
	pd_header = pdpb_pb2.RequestHeader()
	pd_header.uuid = uuid.uuid1().bytes
	pd_header.cluster_id = 1024

	pd_get_region_req = pdpb_pb2.GetRegionByIDRequest()
	pd_get_region_req.region_id = region_id

	pd_req = pdpb_pb2.Request()
	pd_req.cmd_type = pdpb_pb2.GetRegionByID
	pd_req.header.MergeFrom(pd_header)
	pd_req.get_region_by_id.MergeFrom(pd_get_region_req)

	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.PdReq
	message.pd_req.MergeFrom(pd_req)
	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)

	pd_sk.sendall(header_bytes)
	pd_sk.sendall(body_bytes)

	server_reply = pd_sk.recv(1024)
	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	resp_body_bytes = server_reply[16:]

	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def get_store(pd_sk, store_id):
	pd_header = pdpb_pb2.RequestHeader()
	pd_header.uuid = uuid.uuid1().bytes
	pd_header.cluster_id = 1024

	get_store_req = pdpb_pb2.GetStoreRequest()
	get_store_req.store_id = store_id

	pd_req = pdpb_pb2.Request()
	pd_req.cmd_type = pdpb_pb2.GetStore
	pd_req.header.MergeFrom(pd_header)
	pd_req.get_store.MergeFrom(get_store_req)

	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.PdReq
	message.pd_req.MergeFrom(pd_req)
	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)

	pd_sk.sendall(header_bytes)
	pd_sk.sendall(body_bytes)

	server_reply = pd_sk.recv(1024)
	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	resp_body_bytes = server_reply[16:]

	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def send_raft_put_req(sk, region_info, key, value):
	header = raft_cmdpb_pb2.RaftRequestHeader()
	header.region_id = region_info.region.id
	header.peer.MergeFrom(region_info.leader)
	header.uuid = uuid.uuid1().bytes
	header.region_epoch.MergeFrom(region_info.region.region_epoch)
	header.term = region_info.region.term

	put_req = raft_cmdpb_pb2.PutRequest()
	put_req.key = key
	put_req.value = value

	raft_cmd_req = raft_cmdpb_pb2.Request()
	raft_cmd_req.cmd_type = raft_cmdpb_pb2.Put
 	raft_cmd_req.put.MergeFrom(put_req)

	cmd = raft_cmdpb_pb2.RaftCmdRequest()
 	cmd.header.MergeFrom(header)
 	cmd_req = cmd.requests.add()
 	cmd_req.MergeFrom(raft_cmd_req)

 	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.Cmd
	message.cmd_req.MergeFrom(cmd)

	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)
	sk.sendall(header_bytes)
	sk.sendall(body_bytes)

	server_reply = ''
	while True:
		server_reply += sk.recv(1024)
		if len(server_reply) >= 16:
			break

	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	while True:
		if resp_msg_len + 16 == len(server_reply):
			break
		server_reply += sk.recv(1024)

	resp_body_bytes = server_reply[16:]
	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def send_raft_get_req(sk, region_info, key):
	header = raft_cmdpb_pb2.RaftRequestHeader()
	header.region_id = region_info.region.id
	header.peer.MergeFrom(region_info.leader)
	header.uuid = uuid.uuid1().bytes
	header.region_epoch.MergeFrom(region_info.region.region_epoch)
	header.term = region_info.region.term

	get_req = raft_cmdpb_pb2.GetRequest()
	get_req.key = key

	raft_cmd_req = raft_cmdpb_pb2.Request()
	raft_cmd_req.cmd_type = raft_cmdpb_pb2.Get
 	raft_cmd_req.get.MergeFrom(get_req)

	cmd = raft_cmdpb_pb2.RaftCmdRequest()
 	cmd.header.MergeFrom(header)
 	cmd_req = cmd.requests.add()
 	cmd_req.MergeFrom(raft_cmd_req)

 	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.Cmd
	message.cmd_req.MergeFrom(cmd)

	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)
	sk.sendall(header_bytes)
	sk.sendall(body_bytes)

	server_reply = ''
	while True:
		server_reply += sk.recv(1024)
		if len(server_reply) >= 16:
			break

	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	while True:
		if resp_msg_len + 16 == len(server_reply):
			break
		server_reply += sk.recv(1024)

	resp_body_bytes = server_reply[16:]
	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def send_volume_add_req(sk):
	add_req = volume_cmdpb_pb2.AddRequest()
	add_req.count = 1

	cmd = volume_cmdpb_pb2.Request()
 	cmd.add.MergeFrom(add_req)

 	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.VolumeReq
	message.volume_req.MergeFrom(cmd)

	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)
	sk.sendall(header_bytes)
	sk.sendall(body_bytes)

	server_reply = ''
	while True:
		server_reply += sk.recv(1024)
		if len(server_reply) >= 16:
			break

	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	while True:
		if resp_msg_len + 16 == len(server_reply):
			break
		server_reply += sk.recv(1024)

	resp_body_bytes = server_reply[16:]
	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def send_get_req(sk, region_info):
	header = raft_cmdpb_pb2.RaftRequestHeader()
	header.region_id = 2
	header.peer.MergeFrom(region_info.leader)
	header.uuid = uuid.uuid1().bytes
	header.region_epoch.MergeFrom(region_info.region.region_epoch)
	header.term = region_info.region.term

	get_req = raft_cmdpb_pb2.GetRequest()
 	get_req.key = i

	raft_cmd_req = raft_cmdpb_pb2.Request()
 	raft_cmd_req.cmd_type = raft_cmdpb_pb2.Get
 	raft_cmd_req.get.MergeFrom(get_req)

	cmd = raft_cmdpb_pb2.RaftCmdRequest()
 	cmd.header.MergeFrom(header)
 	cmd_req = cmd.requests.add()
 	cmd_req.MergeFrom(raft_cmd_req)

 	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.Cmd
	message.cmd_req.MergeFrom(cmd)

	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)
	sk.sendall(header_bytes)
	sk.sendall(body_bytes)

	server_reply = ''
	while True:
		server_reply += sk.recv(1024)
		if len(server_reply) >= 16:
			break

	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	while True:
		if resp_msg_len + 16 == len(server_reply):
			break
		server_reply += sk.recv(1024)

	resp_body_bytes = server_reply[16:]

	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def send_delete_req(sk, region_info):
	header = raft_cmdpb_pb2.RaftRequestHeader()
	header.region_id = 2
	header.peer.MergeFrom(region_info.leader)
	header.uuid = uuid.uuid1().bytes
	header.region_epoch.MergeFrom(region_info.region.region_epoch)
	header.term = region_info.region.term

	del_req = raft_cmdpb_pb2.DeleteRequest()
 	del_req.key = 1

	raft_cmd_req = raft_cmdpb_pb2.Request()
 	raft_cmd_req.cmd_type = raft_cmdpb_pb2.Delete
 	raft_cmd_reqreq.delete.MergeFrom(del_req)

	cmd = raft_cmdpb_pb2.RaftCmdRequest()
 	cmd.header.MergeFrom(header)
 	cmd_req = cmd.requests.add()
 	cmd_req.MergeFrom(raft_cmd_req)

 	message = msgpb_pb2.Message()
	message.msg_type = msgpb_pb2.Cmd
	message.cmd_req.MergeFrom(cmd)

	body_bytes = message.SerializeToString()

	magic_num = 56052
	version = 0x01
	msg_len = message.ByteSize()
	msg_id = 0x1
	header_bytes = struct.pack('!2HIq', magic_num, version, msg_len, msg_id)
	sk.sendall(header_bytes)
	sk.sendall(body_bytes)

	server_reply = ''
	while True:
		server_reply += sk.recv(1024)
		if len(server_reply) >= 16:
			break

	resp_header_bytes = server_reply[0:16]
	resp_magic_num, resp_version, resp_msg_len, resp_msg_id = struct.unpack('!2HIq', resp_header_bytes)
	while True:
		if resp_msg_len + 16 == len(server_reply):
			break
		server_reply += sk.recv(1024)

	resp_body_bytes = server_reply[16:]

	resp_message = msgpb_pb2.Message()
	resp_message.ParseFromString(resp_body_bytes)
	return resp_message

def test_volume_put(region_id, key, value):
	pd_sk = socket.socket()
	pd_sk.connect(pd_ip_port)
	pd_sk.sendall("GET /pd/rpc HTTP/1.0\r\n\r\n")
	get_region_msg = get_region(pd_sk, region_id)
	#获取Region信息
	region_info = get_region_msg.pd_resp.get_region_by_id

	#获取Region对应的leader的地址
	get_store_msg = get_store(pd_sk, region_info.leader.store_id)
	get_store_resp = get_store_msg.pd_resp.get_store

	store_ip_port_str = get_store_resp.store.address
	store_ip_port = (store_ip_port_str.split(":")[0], int(store_ip_port_str.split(":")[1]))

	store_sk = socket.socket()
	store_sk.connect(store_ip_port)

	put_resp_message = send_raft_put_req(store_sk, region_info, key, value)
	pd_sk.close()
	store_sk.close()

def test_volume_get(region_id, key):
	pd_sk = socket.socket()
	pd_sk.connect(pd_ip_port)
	pd_sk.sendall("GET /pd/rpc HTTP/1.0\r\n\r\n")
	get_region_msg = get_region(pd_sk, region_id)
	#获取Region信息
	region_info = get_region_msg.pd_resp.get_region_by_id

	#获取Region对应的leader的地址
	get_store_msg = get_store(pd_sk, region_info.leader.store_id)
	get_store_resp = get_store_msg.pd_resp.get_store

	store_ip_port_str = get_store_resp.store.address
	store_ip_port = (store_ip_port_str.split(":")[0], int(store_ip_port_str.split(":")[1]))

	store_sk = socket.socket()
	store_sk.connect(store_ip_port)

	put_resp_message = send_raft_get_req(store_sk, region_info, key)
	pd_sk.close()
	store_sk.close()
	return put_resp_message


def test_volume_add():
	store_ip_port_str = "127.0.0.1:1234"
	store_ip_port = (store_ip_port_str.split(":")[0], int(store_ip_port_str.split(":")[1]))

	store_sk = socket.socket()
	store_sk.connect(store_ip_port)

	add_volume_resp_message = send_volume_add_req(store_sk)
	store_sk.close()
	return add_volume_resp_message

if __name__ == "__main__":
	 for i in range(0, 10):
	 	test_volume_add()

	#f = open("1.jpg", "r")
	#data = f.read()
	#f.close()
	#print len(data), type(data)
	#
	##volume_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
	#volume_list = [1]
	#for volume in volume_list:
	#	for i in range(0, 1):
	#		test_volume_put(volume, i, data)
	#		get_resp = test_volume_get(volume, i)
	#		value = get_resp.cmd_resp.responses[0].get.value
	#		if len(value) == len(data):
	#			print "Success"
