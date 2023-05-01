#include "../src/protocol.h"
#include <assert.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  char *buffer = (char *)calloc(2048, sizeof(char));
  pack_msg(buffer, "msg", "ftb", "hello there");
  assert(strcmp(buffer + 2, "msg") == 0);
  assert(strcmp(buffer + 6, "ftb") == 0);
  assert(strcmp(buffer + 38, "hello there") == 0);
  uint16_t msg_size = unpack_uint16(buffer);
  assert(msg_size == 12); // msg_size include null byte
  proto_msg *unpacked_msg =
      unpack_msg(buffer + 2, msg_size); // size will be read first so the buffer
                                        // to unpack doesn't include it
  assert(strcmp(unpacked_msg->msg_type, "msg") == 0);
  assert(strcmp(unpacked_msg->sender_uname, "ftb") == 0);
  assert(strcmp(unpacked_msg->content, "hello there") == 0);
  return EXIT_SUCCESS;
}
