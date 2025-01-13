#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);


/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static int cmd_si(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "info","Output information about somthing",cmd_info},
  { "x","Scan memory and output them",cmd_x },
  { "p","Calculate expression",cmd_p }
  ,{ "w","add watchpoint",cmd_w},
  { "d","delete watchpoint",cmd_d},
  {"si","one step go",cmd_si}
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
static int cmd_si(char *args){
  int step = 1;
  if(args != NULL && strcmp(args,"") != 0){
    if (sscanf(args,"%d",&step) != 1){
      printf("illegal format!\n");
      return 0;
    }
  }
  cpu_exec(step);
  return 0;
}

void ui_mainloop(int is_batch_mode) {
#ifdef  MY_DEBUG
  is_batch_mode = true;
#endif
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }
  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif
    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
static int cmd_info(char *args) {
  char *arg = strtok(args," ");
  if(NULL==arg) {
    printf("Please enter \"r\" or \"w\" r for register w for monitor point \n" ); return 0;
  } else {
    if(strcmp(arg,"w")==0) {
      watchpoint_display();
      return 0;
    } else if(strcmp(arg,"r")==0) {
      isa_reg_display();
    } else {
      printf("wrong\n");
    }
  }
  return 0;
}
static int cmd_x(char *args) {
  int num,index;
  if(2 != sscanf(args,"%d 0x%x",&num,&index)) {
    printf("wrong\n");
    return 0;
  }
  if(num<=0) {
    printf("wrong\n");
    return 0;
  }
  for(int i=0;i<num;i++) {
    printf("0x%-10x:",index + i*4);    

    printf("0x%-10x",isa_vaddr_read(index+i*4,4));
    printf("\n");
  }
  printf("\n");
  return 0;
}

static int cmd_p(char *args) {
  bool flag;
  int ans = expr(args,&flag);
  if(flag==false) {
    printf("wrong expression\n");
  } else {
    printf("0x%-10x %d\n",ans,ans);
  }
  return 0;
}
static int cmd_w(char *args) {
  bool success;
  uint32_t value = expr(args,&success);
  if(!success) {
    printf("bad expr for watchpoint\n");
    return 0;
  }
  WP *tmp = new_wp();
  tmp->value = value;
  strcpy(tmp->expr,args);
  
  watchpoint_display();
  return 0;
}
static int cmd_d(char *args){
  int num;
  if(args == NULL || sscanf(args,"%d",&num) != 1 ) {
    printf("Wrong input for delete watchpoint\n");
    return 0;
  }
  free_wp(NO_wp(num));
  return 0;
  
}