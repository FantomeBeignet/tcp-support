#ifndef __RELATIONSHIP_H__
#define __RELATIONSHIP_H__

#include <stdio.h>
#include <stdlib.h>

#define MAX_USERS 24

int relationship[MAX_USERS];

void init_relationship();

void add_connection(int i, int j);

void remove_connection(int i, int j);

void update_connection(int i, int j);

#endif // __RELATIONSHIP_H__
