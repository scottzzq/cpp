// Code generated by protoc-gen-gogo.
// source: volume_cmdpb.proto
// DO NOT EDIT!

/*
	Package volume_cmdpb is a generated protocol buffer package.

	It is generated from these files:
		volume_cmdpb.proto

	It has these top-level messages:
		AddRequest
		AddResponse
		Request
		Response
*/
package volume_cmdpb

import (
	"fmt"
	"io"
	"math"

	proto "github.com/golang/protobuf/proto"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion2 // please upgrade the proto package

type CmdType int32

const (
	CmdType_Invalid CmdType = 0
	CmdType_Add     CmdType = 1
)

var CmdType_name = map[int32]string{
	0: "Invalid",
	1: "Add",
}
var CmdType_value = map[string]int32{
	"Invalid": 0,
	"Add":     1,
}

func (x CmdType) Enum() *CmdType {
	p := new(CmdType)
	*p = x
	return p
}
func (x CmdType) String() string {
	return proto.EnumName(CmdType_name, int32(x))
}
func (x *CmdType) UnmarshalJSON(data []byte) error {
	value, err := proto.UnmarshalJSONEnum(CmdType_value, data, "CmdType")
	if err != nil {
		return err
	}
	*x = CmdType(value)
	return nil
}
func (CmdType) EnumDescriptor() ([]byte, []int) { return fileDescriptorVolumeCmdpb, []int{0} }

type AddRequest struct {
	Count            *uint64 `protobuf:"varint,1,opt,name=count" json:"count,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *AddRequest) Reset()                    { *m = AddRequest{} }
func (m *AddRequest) String() string            { return proto.CompactTextString(m) }
func (*AddRequest) ProtoMessage()               {}
func (*AddRequest) Descriptor() ([]byte, []int) { return fileDescriptorVolumeCmdpb, []int{0} }

func (m *AddRequest) GetCount() uint64 {
	if m != nil && m.Count != nil {
		return *m.Count
	}
	return 0
}

type AddResponse struct {
	VolumeIds        []uint64 `protobuf:"varint,1,rep,name=volume_ids,json=volumeIds" json:"volume_ids,omitempty"`
	XXX_unrecognized []byte   `json:"-"`
}

func (m *AddResponse) Reset()                    { *m = AddResponse{} }
func (m *AddResponse) String() string            { return proto.CompactTextString(m) }
func (*AddResponse) ProtoMessage()               {}
func (*AddResponse) Descriptor() ([]byte, []int) { return fileDescriptorVolumeCmdpb, []int{1} }

func (m *AddResponse) GetVolumeIds() []uint64 {
	if m != nil {
		return m.VolumeIds
	}
	return nil
}

type Request struct {
	CmdType          *CmdType    `protobuf:"varint,1,opt,name=cmd_type,json=cmdType,enum=volume_cmdpb.CmdType" json:"cmd_type,omitempty"`
	Add              *AddRequest `protobuf:"bytes,2,opt,name=add" json:"add,omitempty"`
	XXX_unrecognized []byte      `json:"-"`
}

func (m *Request) Reset()                    { *m = Request{} }
func (m *Request) String() string            { return proto.CompactTextString(m) }
func (*Request) ProtoMessage()               {}
func (*Request) Descriptor() ([]byte, []int) { return fileDescriptorVolumeCmdpb, []int{2} }

func (m *Request) GetCmdType() CmdType {
	if m != nil && m.CmdType != nil {
		return *m.CmdType
	}
	return CmdType_Invalid
}

func (m *Request) GetAdd() *AddRequest {
	if m != nil {
		return m.Add
	}
	return nil
}

type Response struct {
	CmdType          *CmdType     `protobuf:"varint,1,opt,name=cmd_type,json=cmdType,enum=volume_cmdpb.CmdType" json:"cmd_type,omitempty"`
	Add              *AddResponse `protobuf:"bytes,2,opt,name=add" json:"add,omitempty"`
	XXX_unrecognized []byte       `json:"-"`
}

func (m *Response) Reset()                    { *m = Response{} }
func (m *Response) String() string            { return proto.CompactTextString(m) }
func (*Response) ProtoMessage()               {}
func (*Response) Descriptor() ([]byte, []int) { return fileDescriptorVolumeCmdpb, []int{3} }

func (m *Response) GetCmdType() CmdType {
	if m != nil && m.CmdType != nil {
		return *m.CmdType
	}
	return CmdType_Invalid
}

func (m *Response) GetAdd() *AddResponse {
	if m != nil {
		return m.Add
	}
	return nil
}

func init() {
	proto.RegisterType((*AddRequest)(nil), "volume_cmdpb.AddRequest")
	proto.RegisterType((*AddResponse)(nil), "volume_cmdpb.AddResponse")
	proto.RegisterType((*Request)(nil), "volume_cmdpb.Request")
	proto.RegisterType((*Response)(nil), "volume_cmdpb.Response")
	proto.RegisterEnum("volume_cmdpb.CmdType", CmdType_name, CmdType_value)
}
func (m *AddRequest) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalTo(dAtA)
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *AddRequest) MarshalTo(dAtA []byte) (int, error) {
	var i int
	_ = i
	var l int
	_ = l
	if m.Count != nil {
		dAtA[i] = 0x8
		i++
		i = encodeVarintVolumeCmdpb(dAtA, i, uint64(*m.Count))
	}
	if m.XXX_unrecognized != nil {
		i += copy(dAtA[i:], m.XXX_unrecognized)
	}
	return i, nil
}

func (m *AddResponse) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalTo(dAtA)
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *AddResponse) MarshalTo(dAtA []byte) (int, error) {
	var i int
	_ = i
	var l int
	_ = l
	if len(m.VolumeIds) > 0 {
		for _, num := range m.VolumeIds {
			dAtA[i] = 0x8
			i++
			i = encodeVarintVolumeCmdpb(dAtA, i, uint64(num))
		}
	}
	if m.XXX_unrecognized != nil {
		i += copy(dAtA[i:], m.XXX_unrecognized)
	}
	return i, nil
}

func (m *Request) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalTo(dAtA)
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *Request) MarshalTo(dAtA []byte) (int, error) {
	var i int
	_ = i
	var l int
	_ = l
	if m.CmdType != nil {
		dAtA[i] = 0x8
		i++
		i = encodeVarintVolumeCmdpb(dAtA, i, uint64(*m.CmdType))
	}
	if m.Add != nil {
		dAtA[i] = 0x12
		i++
		i = encodeVarintVolumeCmdpb(dAtA, i, uint64(m.Add.Size()))
		n1, err := m.Add.MarshalTo(dAtA[i:])
		if err != nil {
			return 0, err
		}
		i += n1
	}
	if m.XXX_unrecognized != nil {
		i += copy(dAtA[i:], m.XXX_unrecognized)
	}
	return i, nil
}

func (m *Response) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalTo(dAtA)
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *Response) MarshalTo(dAtA []byte) (int, error) {
	var i int
	_ = i
	var l int
	_ = l
	if m.CmdType != nil {
		dAtA[i] = 0x8
		i++
		i = encodeVarintVolumeCmdpb(dAtA, i, uint64(*m.CmdType))
	}
	if m.Add != nil {
		dAtA[i] = 0x12
		i++
		i = encodeVarintVolumeCmdpb(dAtA, i, uint64(m.Add.Size()))
		n2, err := m.Add.MarshalTo(dAtA[i:])
		if err != nil {
			return 0, err
		}
		i += n2
	}
	if m.XXX_unrecognized != nil {
		i += copy(dAtA[i:], m.XXX_unrecognized)
	}
	return i, nil
}

func encodeFixed64VolumeCmdpb(dAtA []byte, offset int, v uint64) int {
	dAtA[offset] = uint8(v)
	dAtA[offset+1] = uint8(v >> 8)
	dAtA[offset+2] = uint8(v >> 16)
	dAtA[offset+3] = uint8(v >> 24)
	dAtA[offset+4] = uint8(v >> 32)
	dAtA[offset+5] = uint8(v >> 40)
	dAtA[offset+6] = uint8(v >> 48)
	dAtA[offset+7] = uint8(v >> 56)
	return offset + 8
}
func encodeFixed32VolumeCmdpb(dAtA []byte, offset int, v uint32) int {
	dAtA[offset] = uint8(v)
	dAtA[offset+1] = uint8(v >> 8)
	dAtA[offset+2] = uint8(v >> 16)
	dAtA[offset+3] = uint8(v >> 24)
	return offset + 4
}
func encodeVarintVolumeCmdpb(dAtA []byte, offset int, v uint64) int {
	for v >= 1<<7 {
		dAtA[offset] = uint8(v&0x7f | 0x80)
		v >>= 7
		offset++
	}
	dAtA[offset] = uint8(v)
	return offset + 1
}
func (m *AddRequest) Size() (n int) {
	var l int
	_ = l
	if m.Count != nil {
		n += 1 + sovVolumeCmdpb(uint64(*m.Count))
	}
	if m.XXX_unrecognized != nil {
		n += len(m.XXX_unrecognized)
	}
	return n
}

func (m *AddResponse) Size() (n int) {
	var l int
	_ = l
	if len(m.VolumeIds) > 0 {
		for _, e := range m.VolumeIds {
			n += 1 + sovVolumeCmdpb(uint64(e))
		}
	}
	if m.XXX_unrecognized != nil {
		n += len(m.XXX_unrecognized)
	}
	return n
}

func (m *Request) Size() (n int) {
	var l int
	_ = l
	if m.CmdType != nil {
		n += 1 + sovVolumeCmdpb(uint64(*m.CmdType))
	}
	if m.Add != nil {
		l = m.Add.Size()
		n += 1 + l + sovVolumeCmdpb(uint64(l))
	}
	if m.XXX_unrecognized != nil {
		n += len(m.XXX_unrecognized)
	}
	return n
}

func (m *Response) Size() (n int) {
	var l int
	_ = l
	if m.CmdType != nil {
		n += 1 + sovVolumeCmdpb(uint64(*m.CmdType))
	}
	if m.Add != nil {
		l = m.Add.Size()
		n += 1 + l + sovVolumeCmdpb(uint64(l))
	}
	if m.XXX_unrecognized != nil {
		n += len(m.XXX_unrecognized)
	}
	return n
}

func sovVolumeCmdpb(x uint64) (n int) {
	for {
		n++
		x >>= 7
		if x == 0 {
			break
		}
	}
	return n
}
func sozVolumeCmdpb(x uint64) (n int) {
	return sovVolumeCmdpb(uint64((x << 1) ^ uint64((int64(x) >> 63))))
}
func (m *AddRequest) Unmarshal(dAtA []byte) error {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowVolumeCmdpb
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: AddRequest: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: AddRequest: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field Count", wireType)
			}
			var v uint64
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				v |= (uint64(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			m.Count = &v
		default:
			iNdEx = preIndex
			skippy, err := skipVolumeCmdpb(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if skippy < 0 {
				return ErrInvalidLengthVolumeCmdpb
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			m.XXX_unrecognized = append(m.XXX_unrecognized, dAtA[iNdEx:iNdEx+skippy]...)
			iNdEx += skippy
		}
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func (m *AddResponse) Unmarshal(dAtA []byte) error {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowVolumeCmdpb
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: AddResponse: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: AddResponse: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field VolumeIds", wireType)
			}
			var v uint64
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				v |= (uint64(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			m.VolumeIds = append(m.VolumeIds, v)
		default:
			iNdEx = preIndex
			skippy, err := skipVolumeCmdpb(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if skippy < 0 {
				return ErrInvalidLengthVolumeCmdpb
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			m.XXX_unrecognized = append(m.XXX_unrecognized, dAtA[iNdEx:iNdEx+skippy]...)
			iNdEx += skippy
		}
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func (m *Request) Unmarshal(dAtA []byte) error {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowVolumeCmdpb
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: Request: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: Request: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field CmdType", wireType)
			}
			var v CmdType
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				v |= (CmdType(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			m.CmdType = &v
		case 2:
			if wireType != 2 {
				return fmt.Errorf("proto: wrong wireType = %d for field Add", wireType)
			}
			var msglen int
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				msglen |= (int(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			if msglen < 0 {
				return ErrInvalidLengthVolumeCmdpb
			}
			postIndex := iNdEx + msglen
			if postIndex > l {
				return io.ErrUnexpectedEOF
			}
			if m.Add == nil {
				m.Add = &AddRequest{}
			}
			if err := m.Add.Unmarshal(dAtA[iNdEx:postIndex]); err != nil {
				return err
			}
			iNdEx = postIndex
		default:
			iNdEx = preIndex
			skippy, err := skipVolumeCmdpb(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if skippy < 0 {
				return ErrInvalidLengthVolumeCmdpb
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			m.XXX_unrecognized = append(m.XXX_unrecognized, dAtA[iNdEx:iNdEx+skippy]...)
			iNdEx += skippy
		}
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func (m *Response) Unmarshal(dAtA []byte) error {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowVolumeCmdpb
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: Response: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: Response: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field CmdType", wireType)
			}
			var v CmdType
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				v |= (CmdType(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			m.CmdType = &v
		case 2:
			if wireType != 2 {
				return fmt.Errorf("proto: wrong wireType = %d for field Add", wireType)
			}
			var msglen int
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				msglen |= (int(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			if msglen < 0 {
				return ErrInvalidLengthVolumeCmdpb
			}
			postIndex := iNdEx + msglen
			if postIndex > l {
				return io.ErrUnexpectedEOF
			}
			if m.Add == nil {
				m.Add = &AddResponse{}
			}
			if err := m.Add.Unmarshal(dAtA[iNdEx:postIndex]); err != nil {
				return err
			}
			iNdEx = postIndex
		default:
			iNdEx = preIndex
			skippy, err := skipVolumeCmdpb(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if skippy < 0 {
				return ErrInvalidLengthVolumeCmdpb
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			m.XXX_unrecognized = append(m.XXX_unrecognized, dAtA[iNdEx:iNdEx+skippy]...)
			iNdEx += skippy
		}
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func skipVolumeCmdpb(dAtA []byte) (n int, err error) {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return 0, ErrIntOverflowVolumeCmdpb
			}
			if iNdEx >= l {
				return 0, io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		wireType := int(wire & 0x7)
		switch wireType {
		case 0:
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return 0, ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return 0, io.ErrUnexpectedEOF
				}
				iNdEx++
				if dAtA[iNdEx-1] < 0x80 {
					break
				}
			}
			return iNdEx, nil
		case 1:
			iNdEx += 8
			return iNdEx, nil
		case 2:
			var length int
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return 0, ErrIntOverflowVolumeCmdpb
				}
				if iNdEx >= l {
					return 0, io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				length |= (int(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			iNdEx += length
			if length < 0 {
				return 0, ErrInvalidLengthVolumeCmdpb
			}
			return iNdEx, nil
		case 3:
			for {
				var innerWire uint64
				var start int = iNdEx
				for shift := uint(0); ; shift += 7 {
					if shift >= 64 {
						return 0, ErrIntOverflowVolumeCmdpb
					}
					if iNdEx >= l {
						return 0, io.ErrUnexpectedEOF
					}
					b := dAtA[iNdEx]
					iNdEx++
					innerWire |= (uint64(b) & 0x7F) << shift
					if b < 0x80 {
						break
					}
				}
				innerWireType := int(innerWire & 0x7)
				if innerWireType == 4 {
					break
				}
				next, err := skipVolumeCmdpb(dAtA[start:])
				if err != nil {
					return 0, err
				}
				iNdEx = start + next
			}
			return iNdEx, nil
		case 4:
			return iNdEx, nil
		case 5:
			iNdEx += 4
			return iNdEx, nil
		default:
			return 0, fmt.Errorf("proto: illegal wireType %d", wireType)
		}
	}
	panic("unreachable")
}

var (
	ErrInvalidLengthVolumeCmdpb = fmt.Errorf("proto: negative length found during unmarshaling")
	ErrIntOverflowVolumeCmdpb   = fmt.Errorf("proto: integer overflow")
)

func init() { proto.RegisterFile("volume_cmdpb.proto", fileDescriptorVolumeCmdpb) }

var fileDescriptorVolumeCmdpb = []byte{
	// 231 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x09, 0x6e, 0x88, 0x02, 0xff, 0xe2, 0x12, 0x2a, 0xcb, 0xcf, 0x29,
	0xcd, 0x4d, 0x8d, 0x4f, 0xce, 0x4d, 0x29, 0x48, 0xd2, 0x2b, 0x28, 0xca, 0x2f, 0xc9, 0x17, 0xe2,
	0x41, 0x16, 0x53, 0x52, 0xe2, 0xe2, 0x72, 0x4c, 0x49, 0x09, 0x4a, 0x2d, 0x2c, 0x4d, 0x2d, 0x2e,
	0x11, 0x12, 0xe1, 0x62, 0x4d, 0xce, 0x2f, 0xcd, 0x2b, 0x91, 0x60, 0x54, 0x60, 0xd4, 0x60, 0x09,
	0x82, 0x70, 0x94, 0x74, 0xb8, 0xb8, 0xc1, 0x6a, 0x8a, 0x0b, 0xf2, 0xf3, 0x8a, 0x53, 0x85, 0x64,
	0xb9, 0xb8, 0xa0, 0x46, 0x64, 0xa6, 0x14, 0x03, 0x55, 0x32, 0x03, 0x55, 0x72, 0x42, 0x44, 0x3c,
	0x53, 0x8a, 0x95, 0xd2, 0xb9, 0xd8, 0x61, 0xc6, 0x19, 0x70, 0x71, 0x00, 0x6d, 0x89, 0x2f, 0xa9,
	0x2c, 0x48, 0x05, 0x9b, 0xc8, 0x67, 0x24, 0xaa, 0x87, 0xe2, 0x22, 0xe7, 0xdc, 0x94, 0x10, 0xa0,
	0x64, 0x10, 0x7b, 0x32, 0x84, 0x21, 0xa4, 0xc5, 0xc5, 0x9c, 0x98, 0x92, 0x22, 0xc1, 0x04, 0x54,
	0xcc, 0x6d, 0x24, 0x81, 0xaa, 0x18, 0xe1, 0xce, 0x20, 0x90, 0x22, 0xa5, 0x4c, 0x2e, 0x0e, 0xb8,
	0x9b, 0x48, 0xb7, 0x49, 0x1b, 0xd9, 0x26, 0x49, 0x2c, 0x36, 0x41, 0x4c, 0x06, 0x5b, 0xa5, 0x25,
	0xcf, 0xc5, 0x0e, 0x35, 0x40, 0x88, 0x9b, 0x8b, 0xdd, 0x33, 0xaf, 0x2c, 0x31, 0x27, 0x33, 0x45,
	0x80, 0x41, 0x88, 0x9d, 0x8b, 0x19, 0xa8, 0x56, 0x80, 0xd1, 0x49, 0xe0, 0xc4, 0x23, 0x39, 0xc6,
	0x0b, 0x40, 0xfc, 0x00, 0x88, 0x67, 0x3c, 0x96, 0x63, 0x00, 0x04, 0x00, 0x00, 0xff, 0xff, 0x76,
	0x78, 0xf6, 0x22, 0x7b, 0x01, 0x00, 0x00,
}
