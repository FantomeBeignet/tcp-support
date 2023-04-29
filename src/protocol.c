#include "protocol.h"

void pack_uint16(char *buffer, uint16_t i) {
  uint16_t net_i = htons(i);
  *buffer++ = net_i >> 8;
  *buffer++ = net_i;
}

uint16_t unpack_uint16(char *buffer) {
  return ((uint16_t)buffer[0] << 8) | buffer[1];
}

int rightpad(char *buffer, int l) {
	size_t buflen = strlen(buffer);
	if (buflen > l) return -1;
	if (buflen ==  l) return 0;
	for (int i = buflen; i < l; i++) {
		buffer[i] = '\0';
	}
	return (l - buflen);
}
