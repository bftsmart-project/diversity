/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: Request.proto */

#ifndef PROTOBUF_C_Request_2eproto__INCLUDED
#define PROTOBUF_C_Request_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1001001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Bftbench__Request Bftbench__Request;


/* --- enums --- */

typedef enum _Bftbench__Request__RequestType {
  BFTBENCH__REQUEST__REQUEST_TYPE__ADD = 0,
  BFTBENCH__REQUEST__REQUEST_TYPE__REMOVE = 1,
  BFTBENCH__REQUEST__REQUEST_TYPE__SIZE = 2,
  BFTBENCH__REQUEST__REQUEST_TYPE__CONTAINS = 3,
  BFTBENCH__REQUEST__REQUEST_TYPE__GET = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(BFTBENCH__REQUEST__REQUEST_TYPE)
} Bftbench__Request__RequestType;

/* --- messages --- */

struct  _Bftbench__Request
{
  ProtobufCMessage base;
  Bftbench__Request__RequestType action;
  protobuf_c_boolean has_value;
  int32_t value;
};
#define BFTBENCH__REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&bftbench__request__descriptor) \
    , 0, 0,0 }


/* Bftbench__Request methods */
void   bftbench__request__init
                     (Bftbench__Request         *message);
size_t bftbench__request__get_packed_size
                     (const Bftbench__Request   *message);
size_t bftbench__request__pack
                     (const Bftbench__Request   *message,
                      uint8_t             *out);
size_t bftbench__request__pack_to_buffer
                     (const Bftbench__Request   *message,
                      ProtobufCBuffer     *buffer);
Bftbench__Request *
       bftbench__request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   bftbench__request__free_unpacked
                     (Bftbench__Request *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Bftbench__Request_Closure)
                 (const Bftbench__Request *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor bftbench__request__descriptor;
extern const ProtobufCEnumDescriptor    bftbench__request__request_type__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_Request_2eproto__INCLUDED */
