#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    wp_pool[i].value = 0;
    memset(wp_pool[i].expr,0,sizeof(wp_pool[i].expr));
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
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
  tmp->next = head;
  head = tmp;
  return tmp;
}
void free_wp(WP *wp) {
  if(wp==NULL) {
    return;
  }
  bool flag = false;
  if(head==wp) {
    head = wp->next;
    flag = true;
  } else {
    for(WP *p = head; p->next!=NULL;p=p->next) {
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
  for(WP* wp = head;wp!=NULL;wp=wp->next) {
    printf("WatchPoint NO:%d EXP:%s  Result:%d\n",wp->expr,wp->value);
  }
}
WP* NO_wp(int num) {
  for(WP* wp = head;wp!=NULL;wp=wp->next) {
    if(wp->NO == num) {
      return wp;
    }
  }
  return NULL;
}