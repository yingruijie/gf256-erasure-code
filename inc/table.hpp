#ifndef _TABLE_HPP_
#define _TABLE_HPP_

#include <string.h>
#include <iomanip>
#include <iostream>

extern int *P;
extern int *L;
extern bool inited_table;

void get_table_256(void);

void destory_table_256(void);

void print_table_256(void);

#endif