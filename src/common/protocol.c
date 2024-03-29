#include "protocol.h"

void pack_uint16(char *buffer, uint16_t i) {
  uint16_t net_i = htons(i);
  *buffer++ = net_i >> 8;
  *buffer++ = net_i;
}

uint16_t unpack_uint16(char *buffer) {
  return ntohs(((uint16_t)buffer[0] << 8) | buffer[1]);
}

char *rightpad(const char *s, size_t l) {
  size_t slen = strlen(s);
  if (slen > l)
    return NULL;
  char *ret = calloc(l, sizeof(char));
  memcpy(ret, s, slen);
  for (size_t i = slen; i < l; i++) {
    ret[i] = '\0';
  }
  return ret;
}

void pack_msg(char *buffer, const char *msg_type, const char *sender_uname,
              const char *content) {
  const char *padded_uname = rightpad(sender_uname, 32);
  if (!padded_uname)
    perror("Erreur lors du padding du nom d'utilisateur");
  size_t msglen = strlen(content) + 1;
  pack_uint16(buffer, msglen);
  memcpy(buffer + 2, msg_type, 4);
  memcpy(buffer + 6, padded_uname, 32);
  memcpy(buffer + 38, content, strlen(content));
  buffer[msglen + 4 + 32 + 1] = '\0';
}

proto_msg *unpack_msg(char *buffer, uint16_t msg_size) {
  proto_msg *ret = calloc(1, sizeof(proto_msg));
  if (!ret)
    perror("Erreur : L'espace mémoire pour unpack le message n'a pas pu être "
           "alloué");
  char *msg_type = calloc(4, sizeof(char));
  char *sender_uname = calloc(32, sizeof(char));
  char *content = calloc(msg_size, sizeof(char));

  strncpy(msg_type, buffer, 4);
  strncpy(sender_uname, buffer + 4, 32);
  strncpy(content, buffer + 36, msg_size);

  ret->msg_type = msg_type;
  ret->sender_uname = sender_uname;
  ret->content = content;

  return ret;
}
