#ifndef __RELATIONSHIP_H__
#define __RELATIONSHIP_H__

#include <stdio.h>
#include <stdlib.h>

#define MAX_USERS 24

void init_relationship(int *relationship);

void add_connection(int *relationship, int i, int j);

void remove_connection(int *relationship, int i, int j);

void update_connection(int *relationship, int i, int j);

#endif // __RELATIONSHIP_H__
