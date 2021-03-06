# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: coprocessor.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import errorpb_pb2
import kvrpcpb_pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='coprocessor.proto',
  package='coprocessor',
  serialized_pb=_b('\n\x11\x63oprocessor.proto\x12\x0b\x63oprocessor\x1a\rerrorpb.proto\x1a\rkvrpcpb.proto\"&\n\x08KeyRange\x12\r\n\x05start\x18\x01 \x01(\x0c\x12\x0b\n\x03\x65nd\x18\x02 \x01(\x0c\"m\n\x07Request\x12!\n\x07\x63ontext\x18\x01 \x01(\x0b\x32\x10.kvrpcpb.Context\x12\n\n\x02tp\x18\x02 \x01(\x03\x12\x0c\n\x04\x64\x61ta\x18\x03 \x01(\x0c\x12%\n\x06ranges\x18\x04 \x03(\x0b\x32\x15.coprocessor.KeyRange\"v\n\x08Response\x12\x0c\n\x04\x64\x61ta\x18\x01 \x01(\x0c\x12$\n\x0cregion_error\x18\x02 \x01(\x0b\x32\x0e.errorpb.Error\x12!\n\x06locked\x18\x03 \x01(\x0b\x32\x11.kvrpcpb.LockInfo\x12\x13\n\x0bother_error\x18\x04 \x01(\t')
  ,
  dependencies=[errorpb_pb2.DESCRIPTOR,kvrpcpb_pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_KEYRANGE = _descriptor.Descriptor(
  name='KeyRange',
  full_name='coprocessor.KeyRange',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='start', full_name='coprocessor.KeyRange.start', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='end', full_name='coprocessor.KeyRange.end', index=1,
      number=2, type=12, cpp_type=9, label=1,
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
  serialized_start=64,
  serialized_end=102,
)


_REQUEST = _descriptor.Descriptor(
  name='Request',
  full_name='coprocessor.Request',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='context', full_name='coprocessor.Request.context', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='tp', full_name='coprocessor.Request.tp', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='data', full_name='coprocessor.Request.data', index=2,
      number=3, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='ranges', full_name='coprocessor.Request.ranges', index=3,
      number=4, type=11, cpp_type=10, label=3,
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
  serialized_start=104,
  serialized_end=213,
)


_RESPONSE = _descriptor.Descriptor(
  name='Response',
  full_name='coprocessor.Response',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='data', full_name='coprocessor.Response.data', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='region_error', full_name='coprocessor.Response.region_error', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='locked', full_name='coprocessor.Response.locked', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='other_error', full_name='coprocessor.Response.other_error', index=3,
      number=4, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
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
  serialized_start=215,
  serialized_end=333,
)

_REQUEST.fields_by_name['context'].message_type = kvrpcpb_pb2._CONTEXT
_REQUEST.fields_by_name['ranges'].message_type = _KEYRANGE
_RESPONSE.fields_by_name['region_error'].message_type = errorpb_pb2._ERROR
_RESPONSE.fields_by_name['locked'].message_type = kvrpcpb_pb2._LOCKINFO
DESCRIPTOR.message_types_by_name['KeyRange'] = _KEYRANGE
DESCRIPTOR.message_types_by_name['Request'] = _REQUEST
DESCRIPTOR.message_types_by_name['Response'] = _RESPONSE

KeyRange = _reflection.GeneratedProtocolMessageType('KeyRange', (_message.Message,), dict(
  DESCRIPTOR = _KEYRANGE,
  __module__ = 'coprocessor_pb2'
  # @@protoc_insertion_point(class_scope:coprocessor.KeyRange)
  ))
_sym_db.RegisterMessage(KeyRange)

Request = _reflection.GeneratedProtocolMessageType('Request', (_message.Message,), dict(
  DESCRIPTOR = _REQUEST,
  __module__ = 'coprocessor_pb2'
  # @@protoc_insertion_point(class_scope:coprocessor.Request)
  ))
_sym_db.RegisterMessage(Request)

Response = _reflection.GeneratedProtocolMessageType('Response', (_message.Message,), dict(
  DESCRIPTOR = _RESPONSE,
  __module__ = 'coprocessor_pb2'
  # @@protoc_insertion_point(class_scope:coprocessor.Response)
  ))
_sym_db.RegisterMessage(Response)


# @@protoc_insertion_point(module_scope)
