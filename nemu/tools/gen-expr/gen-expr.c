#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
#define MAXOP 64
static char buf[65536];
int index;
int op_num;
static inline int choose(unsigned int i) {
  return rand()%i;
}
static inline void gen_num() {
  int num = choose(65536);
  sprintf(buf+index,"%d",num);
}
static inline void gen(char c) {
  sprintf(buf+index,"%c",c);  
}
static inline void gen_rand_op(){
  switch(choose(4)){
    case 0:
      buf[index] = '+';
      break;
    case 1:
      buf[index] = '-';
      break;
    case 2:
      buf[index] = '*';
      break;
    case 3:
      buf[index] = '/';
      break;
  }
  index++;
}

static inline void gen_rand_expr() {
  int choice = choose(3);
  if(MAXOP - op_num <= 2) {
    choice = 0;
  }
  switch (choice) {
    case 0: 
      gen_num();
      while(buf[index]) {
        index = index + 1;
      }
      op_num = op_num + 1;
      break;
    case 1: 
      op_num += 2;
      buf[index] = '(';
      index++;
      gen_rand_expr();
      buf[index] = ')';
      index++;
      break;
    default:
      op_num += 2;
      gen_rand_expr();
      gen_rand_op();
      gen_rand_expr();
      break;
  }

}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
