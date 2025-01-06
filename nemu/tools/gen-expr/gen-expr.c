#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define MAXOP 24
static char buf[65536];
int m_index;
int op_num;
static inline int choose(unsigned int i) {
  return rand()%i;
}
static inline void gen_num() {
  int num = choose(65536)+1;
  sprintf(buf+m_index,"%d",num);
}
static inline void gen(char c) {
  sprintf(buf+m_index,"%c",c);  
}
static inline void gen_rand_op(){
  switch(choose(4)){
    case 0:
      buf[m_index] = '+';
      break;
    case 1:
      buf[m_index] = '-';
      break;
    case 2:
      buf[m_index] = '*';
      break;
    case 3:
      buf[m_index] = '/';
      break;
  }
  m_index++;
}

static inline void gen_rand_expr() {
  int choice = choose(3);
  if(MAXOP - op_num <= 2) {
    choice = 0;
  }
  switch (choice) {
    case 0: 
      gen_num();
      while(buf[m_index]) {
        m_index = m_index + 1;
      }
      op_num = op_num + 1;
      break;
    case 1: 
      op_num += 2;
      buf[m_index] = '(';
      m_index++;
      gen_rand_expr();
      buf[m_index] = ')';
      m_index++;
      buf[m_index] = 0;
      break;
    default:
      op_num += 2;
      gen_rand_expr();
      gen_rand_op();
      gen_rand_expr();
      break;
  }

}

static char code_buf[65536*2];
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
    m_index = 0;
    op_num = 0;
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
    int warn = fscanf(fp, "%d", &result);
    if(!warn) {;
    }

    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
