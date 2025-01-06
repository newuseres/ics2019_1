#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[256];
  uint32_t value;

} WP;
WP * new_wp();
void free_wp(WP *wp);
void watchpoint_display();
WP* NO_wp(int num);
void init_wp_pool();

#endif
