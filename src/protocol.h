#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct proto_msg {
  char *msg_type;
  char *sender_uname;
  char *content;
} proto_msg;

void pack_uint16(char *buffer, uint16_t i);

uint16_t unpack_uint16(char *buffer);

char *rightpad(const char *s, size_t l);

void pack_msg(char *buffer, const char *msg_type, const char *sender_uname,
              const char *content);
proto_msg *unpack_msg(char *buffer, uint16_t msg_size);

#endif
