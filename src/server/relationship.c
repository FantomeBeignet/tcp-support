#include "relationship.h"

void init_relationship(int *relationship) {
  for (int i = 0; i < MAX_USERS; i++) {
    relationship[i] = -1; // -1 means no connection
  }
}

void add_connection(int *relationship, int i, int j) {
  relationship[i] = j;
  relationship[j] = i;
}

void remove_connection(int *relationship, int i, int j) {
  relationship[i] = -1;
  relationship[j] = -1;
}

void update_connection(int *relationship, int i, int j) {
  int old = relationship[i];
  relationship[i] = j;
  relationship[old] = -1;
  relationship[j] = i;
}
