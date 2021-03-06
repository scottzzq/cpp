# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: eraftpb.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='eraftpb.proto',
  package='eraftpb',
  serialized_pb=_b('\n\reraftpb.proto\x12\x07\x65raftpb\"Z\n\x05\x45ntry\x12&\n\nentry_type\x18\x01 \x01(\x0e\x32\x12.eraftpb.EntryType\x12\x0c\n\x04term\x18\x02 \x01(\x04\x12\r\n\x05index\x18\x03 \x01(\x04\x12\x0c\n\x04\x64\x61ta\x18\x04 \x01(\x0c\"W\n\x10SnapshotMetadata\x12&\n\nconf_state\x18\x01 \x01(\x0b\x32\x12.eraftpb.ConfState\x12\r\n\x05index\x18\x02 \x01(\x04\x12\x0c\n\x04term\x18\x03 \x01(\x04\"E\n\x08Snapshot\x12\x0c\n\x04\x64\x61ta\x18\x01 \x01(\x0c\x12+\n\x08metadata\x18\x02 \x01(\x0b\x32\x19.eraftpb.SnapshotMetadata\"\x86\x02\n\x07Message\x12&\n\x08msg_type\x18\x01 \x01(\x0e\x32\x14.eraftpb.MessageType\x12\n\n\x02to\x18\x02 \x01(\x04\x12\x0c\n\x04\x66rom\x18\x03 \x01(\x04\x12\x0c\n\x04term\x18\x04 \x01(\x04\x12\x10\n\x08log_term\x18\x05 \x01(\x04\x12\r\n\x05index\x18\x06 \x01(\x04\x12\x1f\n\x07\x65ntries\x18\x07 \x03(\x0b\x32\x0e.eraftpb.Entry\x12\x0e\n\x06\x63ommit\x18\x08 \x01(\x04\x12#\n\x08snapshot\x18\t \x01(\x0b\x32\x11.eraftpb.Snapshot\x12\x0e\n\x06reject\x18\n \x01(\x08\x12\x13\n\x0breject_hint\x18\x0b \x01(\x04\x12\x0f\n\x07\x63ontext\x18\x0c \x01(\x0c\"7\n\tHardState\x12\x0c\n\x04term\x18\x01 \x01(\x04\x12\x0c\n\x04vote\x18\x02 \x01(\x04\x12\x0e\n\x06\x63ommit\x18\x03 \x01(\x04\"\x1a\n\tConfState\x12\r\n\x05nodes\x18\x01 \x03(\x04\"h\n\nConfChange\x12\n\n\x02id\x18\x01 \x01(\x04\x12,\n\x0b\x63hange_type\x18\x02 \x01(\x0e\x32\x17.eraftpb.ConfChangeType\x12\x0f\n\x07node_id\x18\x03 \x01(\x04\x12\x0f\n\x07\x63ontext\x18\x04 \x01(\x0c*1\n\tEntryType\x12\x0f\n\x0b\x45ntryNormal\x10\x00\x12\x13\n\x0f\x45ntryConfChange\x10\x01*\x8c\x03\n\x0bMessageType\x12\n\n\x06MsgHup\x10\x00\x12\x0b\n\x07MsgBeat\x10\x01\x12\x0e\n\nMsgPropose\x10\x02\x12\r\n\tMsgAppend\x10\x03\x12\x15\n\x11MsgAppendResponse\x10\x04\x12\x12\n\x0eMsgRequestVote\x10\x05\x12\x1a\n\x16MsgRequestVoteResponse\x10\x06\x12\x0f\n\x0bMsgSnapshot\x10\x07\x12\x10\n\x0cMsgHeartbeat\x10\x08\x12\x18\n\x14MsgHeartbeatResponse\x10\t\x12\x12\n\x0eMsgUnreachable\x10\n\x12\x11\n\rMsgSnapStatus\x10\x0b\x12\x12\n\x0eMsgCheckQuorum\x10\x0c\x12\x15\n\x11MsgTransferLeader\x10\r\x12\x11\n\rMsgTimeoutNow\x10\x0e\x12\x10\n\x0cMsgReadIndex\x10\x0f\x12\x14\n\x10MsgReadIndexResp\x10\x10\x12\x15\n\x11MsgRequestPreVote\x10\x11\x12\x1d\n\x19MsgRequestPreVoteResponse\x10\x12*-\n\x0e\x43onfChangeType\x12\x0b\n\x07\x41\x64\x64Node\x10\x00\x12\x0e\n\nRemoveNode\x10\x01')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

_ENTRYTYPE = _descriptor.EnumDescriptor(
  name='EntryType',
  full_name='eraftpb.EntryType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='EntryNormal', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='EntryConfChange', index=1, number=1,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=734,
  serialized_end=783,
)
_sym_db.RegisterEnumDescriptor(_ENTRYTYPE)

EntryType = enum_type_wrapper.EnumTypeWrapper(_ENTRYTYPE)
_MESSAGETYPE = _descriptor.EnumDescriptor(
  name='MessageType',
  full_name='eraftpb.MessageType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='MsgHup', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgBeat', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgPropose', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgAppend', index=3, number=3,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgAppendResponse', index=4, number=4,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgRequestVote', index=5, number=5,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgRequestVoteResponse', index=6, number=6,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgSnapshot', index=7, number=7,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgHeartbeat', index=8, number=8,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgHeartbeatResponse', index=9, number=9,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgUnreachable', index=10, number=10,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgSnapStatus', index=11, number=11,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgCheckQuorum', index=12, number=12,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgTransferLeader', index=13, number=13,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgTimeoutNow', index=14, number=14,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgReadIndex', index=15, number=15,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgReadIndexResp', index=16, number=16,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgRequestPreVote', index=17, number=17,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MsgRequestPreVoteResponse', index=18, number=18,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=786,
  serialized_end=1182,
)
_sym_db.RegisterEnumDescriptor(_MESSAGETYPE)

MessageType = enum_type_wrapper.EnumTypeWrapper(_MESSAGETYPE)
_CONFCHANGETYPE = _descriptor.EnumDescriptor(
  name='ConfChangeType',
  full_name='eraftpb.ConfChangeType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='AddNode', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='RemoveNode', index=1, number=1,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=1184,
  serialized_end=1229,
)
_sym_db.RegisterEnumDescriptor(_CONFCHANGETYPE)

ConfChangeType = enum_type_wrapper.EnumTypeWrapper(_CONFCHANGETYPE)
EntryNormal = 0
EntryConfChange = 1
MsgHup = 0
MsgBeat = 1
MsgPropose = 2
MsgAppend = 3
MsgAppendResponse = 4
MsgRequestVote = 5
MsgRequestVoteResponse = 6
MsgSnapshot = 7
MsgHeartbeat = 8
MsgHeartbeatResponse = 9
MsgUnreachable = 10
MsgSnapStatus = 11
MsgCheckQuorum = 12
MsgTransferLeader = 13
MsgTimeoutNow = 14
MsgReadIndex = 15
MsgReadIndexResp = 16
MsgRequestPreVote = 17
MsgRequestPreVoteResponse = 18
AddNode = 0
RemoveNode = 1



_ENTRY = _descriptor.Descriptor(
  name='Entry',
  full_name='eraftpb.Entry',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='entry_type', full_name='eraftpb.Entry.entry_type', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='term', full_name='eraftpb.Entry.term', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='index', full_name='eraftpb.Entry.index', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='data', full_name='eraftpb.Entry.data', index=3,
      number=4, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=26,
  serialized_end=116,
)


_SNAPSHOTMETADATA = _descriptor.Descriptor(
  name='SnapshotMetadata',
  full_name='eraftpb.SnapshotMetadata',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='conf_state', full_name='eraftpb.SnapshotMetadata.conf_state', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='index', full_name='eraftpb.SnapshotMetadata.index', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='term', full_name='eraftpb.SnapshotMetadata.term', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=118,
  serialized_end=205,
)


_SNAPSHOT = _descriptor.Descriptor(
  name='Snapshot',
  full_name='eraftpb.Snapshot',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='data', full_name='eraftpb.Snapshot.data', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='metadata', full_name='eraftpb.Snapshot.metadata', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=207,
  serialized_end=276,
)


_MESSAGE = _descriptor.Descriptor(
  name='Message',
  full_name='eraftpb.Message',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='msg_type', full_name='eraftpb.Message.msg_type', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='to', full_name='eraftpb.Message.to', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='from', full_name='eraftpb.Message.from', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='term', full_name='eraftpb.Message.term', index=3,
      number=4, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='log_term', full_name='eraftpb.Message.log_term', index=4,
      number=5, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='index', full_name='eraftpb.Message.index', index=5,
      number=6, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='entries', full_name='eraftpb.Message.entries', index=6,
      number=7, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='commit', full_name='eraftpb.Message.commit', index=7,
      number=8, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='snapshot', full_name='eraftpb.Message.snapshot', index=8,
      number=9, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='reject', full_name='eraftpb.Message.reject', index=9,
      number=10, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='reject_hint', full_name='eraftpb.Message.reject_hint', index=10,
      number=11, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='context', full_name='eraftpb.Message.context', index=11,
      number=12, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=279,
  serialized_end=541,
)


_HARDSTATE = _descriptor.Descriptor(
  name='HardState',
  full_name='eraftpb.HardState',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='term', full_name='eraftpb.HardState.term', index=0,
      number=1, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='vote', full_name='eraftpb.HardState.vote', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='commit', full_name='eraftpb.HardState.commit', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=543,
  serialized_end=598,
)


_CONFSTATE = _descriptor.Descriptor(
  name='ConfState',
  full_name='eraftpb.ConfState',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='nodes', full_name='eraftpb.ConfState.nodes', index=0,
      number=1, type=4, cpp_type=4, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=600,
  serialized_end=626,
)


_CONFCHANGE = _descriptor.Descriptor(
  name='ConfChange',
  full_name='eraftpb.ConfChange',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='eraftpb.ConfChange.id', index=0,
      number=1, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='change_type', full_name='eraftpb.ConfChange.change_type', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='node_id', full_name='eraftpb.ConfChange.node_id', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='context', full_name='eraftpb.ConfChange.context', index=3,
      number=4, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=628,
  serialized_end=732,
)

_ENTRY.fields_by_name['entry_type'].enum_type = _ENTRYTYPE
_SNAPSHOTMETADATA.fields_by_name['conf_state'].message_type = _CONFSTATE
_SNAPSHOT.fields_by_name['metadata'].message_type = _SNAPSHOTMETADATA
_MESSAGE.fields_by_name['msg_type'].enum_type = _MESSAGETYPE
_MESSAGE.fields_by_name['entries'].message_type = _ENTRY
_MESSAGE.fields_by_name['snapshot'].message_type = _SNAPSHOT
_CONFCHANGE.fields_by_name['change_type'].enum_type = _CONFCHANGETYPE
DESCRIPTOR.message_types_by_name['Entry'] = _ENTRY
DESCRIPTOR.message_types_by_name['SnapshotMetadata'] = _SNAPSHOTMETADATA
DESCRIPTOR.message_types_by_name['Snapshot'] = _SNAPSHOT
DESCRIPTOR.message_types_by_name['Message'] = _MESSAGE
DESCRIPTOR.message_types_by_name['HardState'] = _HARDSTATE
DESCRIPTOR.message_types_by_name['ConfState'] = _CONFSTATE
DESCRIPTOR.message_types_by_name['ConfChange'] = _CONFCHANGE
DESCRIPTOR.enum_types_by_name['EntryType'] = _ENTRYTYPE
DESCRIPTOR.enum_types_by_name['MessageType'] = _MESSAGETYPE
DESCRIPTOR.enum_types_by_name['ConfChangeType'] = _CONFCHANGETYPE

Entry = _reflection.GeneratedProtocolMessageType('Entry', (_message.Message,), dict(
  DESCRIPTOR = _ENTRY,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.Entry)
  ))
_sym_db.RegisterMessage(Entry)

SnapshotMetadata = _reflection.GeneratedProtocolMessageType('SnapshotMetadata', (_message.Message,), dict(
  DESCRIPTOR = _SNAPSHOTMETADATA,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.SnapshotMetadata)
  ))
_sym_db.RegisterMessage(SnapshotMetadata)

Snapshot = _reflection.GeneratedProtocolMessageType('Snapshot', (_message.Message,), dict(
  DESCRIPTOR = _SNAPSHOT,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.Snapshot)
  ))
_sym_db.RegisterMessage(Snapshot)

Message = _reflection.GeneratedProtocolMessageType('Message', (_message.Message,), dict(
  DESCRIPTOR = _MESSAGE,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.Message)
  ))
_sym_db.RegisterMessage(Message)

HardState = _reflection.GeneratedProtocolMessageType('HardState', (_message.Message,), dict(
  DESCRIPTOR = _HARDSTATE,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.HardState)
  ))
_sym_db.RegisterMessage(HardState)

ConfState = _reflection.GeneratedProtocolMessageType('ConfState', (_message.Message,), dict(
  DESCRIPTOR = _CONFSTATE,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.ConfState)
  ))
_sym_db.RegisterMessage(ConfState)

ConfChange = _reflection.GeneratedProtocolMessageType('ConfChange', (_message.Message,), dict(
  DESCRIPTOR = _CONFCHANGE,
  __module__ = 'eraftpb_pb2'
  # @@protoc_insertion_point(class_scope:eraftpb.ConfChange)
  ))
_sym_db.RegisterMessage(ConfChange)


# @@protoc_insertion_point(module_scope)
