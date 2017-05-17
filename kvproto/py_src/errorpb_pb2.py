# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: errorpb.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import metapb_pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='errorpb.proto',
  package='errorpb',
  serialized_pb=_b('\n\rerrorpb.proto\x12\x07\x65rrorpb\x1a\x0cmetapb.proto\"<\n\tNotLeader\x12\x11\n\tregion_id\x18\x01 \x01(\x04\x12\x1c\n\x06leader\x18\x02 \x01(\x0b\x32\x0c.metapb.Peer\"\x0f\n\rStoreNotMatch\"#\n\x0eRegionNotFound\x12\x11\n\tregion_id\x18\x01 \x01(\x04\"T\n\x0eKeyNotInRegion\x12\x0b\n\x03key\x18\x01 \x01(\x0c\x12\x11\n\tregion_id\x18\x02 \x01(\x04\x12\x11\n\tstart_key\x18\x03 \x01(\x0c\x12\x0f\n\x07\x65nd_key\x18\x04 \x01(\x0c\"1\n\nStaleEpoch\x12#\n\x0bnew_regions\x18\x01 \x03(\x0b\x32\x0e.metapb.Region\"\x0e\n\x0cServerIsBusy\"\x0e\n\x0cStaleCommand\":\n\x11RaftEntryTooLarge\x12\x11\n\tregion_id\x18\x01 \x01(\x04\x12\x12\n\nentry_size\x18\x02 \x01(\x04\"\x99\x03\n\x05\x45rror\x12\x0f\n\x07message\x18\x01 \x01(\t\x12&\n\nnot_leader\x18\x02 \x01(\x0b\x32\x12.errorpb.NotLeader\x12\x31\n\x10region_not_found\x18\x03 \x01(\x0b\x32\x17.errorpb.RegionNotFound\x12\x32\n\x11key_not_in_region\x18\x04 \x01(\x0b\x32\x17.errorpb.KeyNotInRegion\x12(\n\x0bstale_epoch\x18\x05 \x01(\x0b\x32\x13.errorpb.StaleEpoch\x12-\n\x0eserver_is_busy\x18\x06 \x01(\x0b\x32\x15.errorpb.ServerIsBusy\x12,\n\rstale_command\x18\x07 \x01(\x0b\x32\x15.errorpb.StaleCommand\x12/\n\x0fstore_not_match\x18\x08 \x01(\x0b\x32\x16.errorpb.StoreNotMatch\x12\x38\n\x14raft_entry_too_large\x18\t \x01(\x0b\x32\x1a.errorpb.RaftEntryTooLarge')
  ,
  dependencies=[metapb_pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_NOTLEADER = _descriptor.Descriptor(
  name='NotLeader',
  full_name='errorpb.NotLeader',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='region_id', full_name='errorpb.NotLeader.region_id', index=0,
      number=1, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='leader', full_name='errorpb.NotLeader.leader', index=1,
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
  serialized_start=40,
  serialized_end=100,
)


_STORENOTMATCH = _descriptor.Descriptor(
  name='StoreNotMatch',
  full_name='errorpb.StoreNotMatch',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
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
  serialized_start=102,
  serialized_end=117,
)


_REGIONNOTFOUND = _descriptor.Descriptor(
  name='RegionNotFound',
  full_name='errorpb.RegionNotFound',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='region_id', full_name='errorpb.RegionNotFound.region_id', index=0,
      number=1, type=4, cpp_type=4, label=1,
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
  serialized_start=119,
  serialized_end=154,
)


_KEYNOTINREGION = _descriptor.Descriptor(
  name='KeyNotInRegion',
  full_name='errorpb.KeyNotInRegion',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='key', full_name='errorpb.KeyNotInRegion.key', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='region_id', full_name='errorpb.KeyNotInRegion.region_id', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='start_key', full_name='errorpb.KeyNotInRegion.start_key', index=2,
      number=3, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='end_key', full_name='errorpb.KeyNotInRegion.end_key', index=3,
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
  serialized_start=156,
  serialized_end=240,
)


_STALEEPOCH = _descriptor.Descriptor(
  name='StaleEpoch',
  full_name='errorpb.StaleEpoch',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='new_regions', full_name='errorpb.StaleEpoch.new_regions', index=0,
      number=1, type=11, cpp_type=10, label=3,
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
  serialized_start=242,
  serialized_end=291,
)


_SERVERISBUSY = _descriptor.Descriptor(
  name='ServerIsBusy',
  full_name='errorpb.ServerIsBusy',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
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
  serialized_start=293,
  serialized_end=307,
)


_STALECOMMAND = _descriptor.Descriptor(
  name='StaleCommand',
  full_name='errorpb.StaleCommand',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
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
  serialized_start=309,
  serialized_end=323,
)


_RAFTENTRYTOOLARGE = _descriptor.Descriptor(
  name='RaftEntryTooLarge',
  full_name='errorpb.RaftEntryTooLarge',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='region_id', full_name='errorpb.RaftEntryTooLarge.region_id', index=0,
      number=1, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='entry_size', full_name='errorpb.RaftEntryTooLarge.entry_size', index=1,
      number=2, type=4, cpp_type=4, label=1,
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
  serialized_start=325,
  serialized_end=383,
)


_ERROR = _descriptor.Descriptor(
  name='Error',
  full_name='errorpb.Error',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='message', full_name='errorpb.Error.message', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='not_leader', full_name='errorpb.Error.not_leader', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='region_not_found', full_name='errorpb.Error.region_not_found', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='key_not_in_region', full_name='errorpb.Error.key_not_in_region', index=3,
      number=4, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='stale_epoch', full_name='errorpb.Error.stale_epoch', index=4,
      number=5, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='server_is_busy', full_name='errorpb.Error.server_is_busy', index=5,
      number=6, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='stale_command', full_name='errorpb.Error.stale_command', index=6,
      number=7, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='store_not_match', full_name='errorpb.Error.store_not_match', index=7,
      number=8, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='raft_entry_too_large', full_name='errorpb.Error.raft_entry_too_large', index=8,
      number=9, type=11, cpp_type=10, label=1,
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
  serialized_start=386,
  serialized_end=795,
)

_NOTLEADER.fields_by_name['leader'].message_type = metapb_pb2._PEER
_STALEEPOCH.fields_by_name['new_regions'].message_type = metapb_pb2._REGION
_ERROR.fields_by_name['not_leader'].message_type = _NOTLEADER
_ERROR.fields_by_name['region_not_found'].message_type = _REGIONNOTFOUND
_ERROR.fields_by_name['key_not_in_region'].message_type = _KEYNOTINREGION
_ERROR.fields_by_name['stale_epoch'].message_type = _STALEEPOCH
_ERROR.fields_by_name['server_is_busy'].message_type = _SERVERISBUSY
_ERROR.fields_by_name['stale_command'].message_type = _STALECOMMAND
_ERROR.fields_by_name['store_not_match'].message_type = _STORENOTMATCH
_ERROR.fields_by_name['raft_entry_too_large'].message_type = _RAFTENTRYTOOLARGE
DESCRIPTOR.message_types_by_name['NotLeader'] = _NOTLEADER
DESCRIPTOR.message_types_by_name['StoreNotMatch'] = _STORENOTMATCH
DESCRIPTOR.message_types_by_name['RegionNotFound'] = _REGIONNOTFOUND
DESCRIPTOR.message_types_by_name['KeyNotInRegion'] = _KEYNOTINREGION
DESCRIPTOR.message_types_by_name['StaleEpoch'] = _STALEEPOCH
DESCRIPTOR.message_types_by_name['ServerIsBusy'] = _SERVERISBUSY
DESCRIPTOR.message_types_by_name['StaleCommand'] = _STALECOMMAND
DESCRIPTOR.message_types_by_name['RaftEntryTooLarge'] = _RAFTENTRYTOOLARGE
DESCRIPTOR.message_types_by_name['Error'] = _ERROR

NotLeader = _reflection.GeneratedProtocolMessageType('NotLeader', (_message.Message,), dict(
  DESCRIPTOR = _NOTLEADER,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.NotLeader)
  ))
_sym_db.RegisterMessage(NotLeader)

StoreNotMatch = _reflection.GeneratedProtocolMessageType('StoreNotMatch', (_message.Message,), dict(
  DESCRIPTOR = _STORENOTMATCH,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.StoreNotMatch)
  ))
_sym_db.RegisterMessage(StoreNotMatch)

RegionNotFound = _reflection.GeneratedProtocolMessageType('RegionNotFound', (_message.Message,), dict(
  DESCRIPTOR = _REGIONNOTFOUND,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.RegionNotFound)
  ))
_sym_db.RegisterMessage(RegionNotFound)

KeyNotInRegion = _reflection.GeneratedProtocolMessageType('KeyNotInRegion', (_message.Message,), dict(
  DESCRIPTOR = _KEYNOTINREGION,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.KeyNotInRegion)
  ))
_sym_db.RegisterMessage(KeyNotInRegion)

StaleEpoch = _reflection.GeneratedProtocolMessageType('StaleEpoch', (_message.Message,), dict(
  DESCRIPTOR = _STALEEPOCH,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.StaleEpoch)
  ))
_sym_db.RegisterMessage(StaleEpoch)

ServerIsBusy = _reflection.GeneratedProtocolMessageType('ServerIsBusy', (_message.Message,), dict(
  DESCRIPTOR = _SERVERISBUSY,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.ServerIsBusy)
  ))
_sym_db.RegisterMessage(ServerIsBusy)

StaleCommand = _reflection.GeneratedProtocolMessageType('StaleCommand', (_message.Message,), dict(
  DESCRIPTOR = _STALECOMMAND,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.StaleCommand)
  ))
_sym_db.RegisterMessage(StaleCommand)

RaftEntryTooLarge = _reflection.GeneratedProtocolMessageType('RaftEntryTooLarge', (_message.Message,), dict(
  DESCRIPTOR = _RAFTENTRYTOOLARGE,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.RaftEntryTooLarge)
  ))
_sym_db.RegisterMessage(RaftEntryTooLarge)

Error = _reflection.GeneratedProtocolMessageType('Error', (_message.Message,), dict(
  DESCRIPTOR = _ERROR,
  __module__ = 'errorpb_pb2'
  # @@protoc_insertion_point(class_scope:errorpb.Error)
  ))
_sym_db.RegisterMessage(Error)


# @@protoc_insertion_point(module_scope)
