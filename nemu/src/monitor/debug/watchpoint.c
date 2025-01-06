#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  WP_head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP * new_wp() { 
  if(NULL==free_) {
    printf("wp_free is null\n");
    assert(0); 
  }
  WP *tmp = free_;
  free_ = free_->next;
  tmp->next = WP_head;
  WP_head = tmp;
  return tmp;
}
void free_wp(WP *wp) {
  if(wp==NULL) {
    return;
  }
  bool flag = false;
  if(WP_head==wp) {
    WP_head = wp->next;
  } else {
    for(WP *p = WP_head; p->next!=NULL;p=p->next) {
      if(p->next == wp) {
        p->next = p->next->next;
        flag = true;
        break;
      }
    }
    if(flag) {
      wp->next = free_;
      free_ = wp;
    }
  }
}
void watchpoint_display() {
  for(WP* wp = WP_head;wp!=NULL;wp=wp->next) {
    printf("WatchPoint NO:%d EXP:%s  Result:%d\n",wp->NO,wp->expr,wp->value);
  }
}
WP* NO_wp(int num) {
  for(WP* wp = WP_head;wp!=NULL;wp=wp->next) {
    if(wp->NO == num) {
      return wp;
    }
  }
  return NULL;
}