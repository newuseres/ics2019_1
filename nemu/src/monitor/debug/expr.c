#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256,
  TK_EQ,
  TK_NEQ,
  TK_NUM,
  TK_HEX,
  TK_REG,
  TK_AND,
  TK_PTR, // pointer
  TK_REV // negative

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-",'-'},//subduction \-
  {"\\*",'*'},//multiple
  {"/",'/'},//divide
  {"==", TK_EQ},         // equal
  {"!=",TK_NEQ},
  {"&&",TK_AND},
  {"\\(",'('},
  {"\\)",')'},
  {"0x[0-9a-fA-F]+",TK_HEX},
  {"[0-9]+",TK_NUM},
  {"\\$[\\$0-9a-zA-Z]+",TK_REG}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
        regexec(&re[i], e + position, 1, &pmatch, 0);
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {

        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case '-':
           if(nr_token==0 || (tokens[nr_token-1].type == '+')|| (tokens[nr_token-1].type == '-')|| (tokens[nr_token-1].type == '(')) {
            tokens[nr_token].type = TK_REV;
           } else { 
            tokens[nr_token].type = rules[i].token_type; 
           }          
          strncpy(tokens[nr_token].str, e+position, substr_len);
          tokens[nr_token].str[substr_len] = 0;
            nr_token++;
          break;
          case '*':
           if(nr_token==0 || (tokens[nr_token-1].type == '+')|| (tokens[nr_token-1].type == '-')|| (tokens[nr_token-1].type == '(') || (tokens[nr_token-1].type == '*')) {
            tokens[nr_token].type = TK_PTR;
           } else {
            tokens[nr_token].type = rules[i].token_type; 
           }
          strncpy(tokens[nr_token].str, e+position, substr_len);
           tokens[nr_token].str[substr_len] = 0;
            nr_token++;
          break;
          break;
          case TK_REG:
          case TK_NUM:
          case TK_HEX:
            if(substr_len>=32) {
              printf("num too long!");
              return false;
            }
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].type = rules[i].token_type;
            tokens[nr_token].str[substr_len] = 0;
            nr_token++;
            break;

          default:
            strncpy(tokens[nr_token].str, e+position, substr_len);
            tokens[nr_token].type = rules[i].token_type;
            tokens[nr_token].str[substr_len] = 0;
            nr_token++;
          case TK_NOTYPE:
            break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


  /* TODO: Insert codes to evaluate the expre */
bool check_parentheses(int i,int j);
int priority_op(int type) {
  switch (type){
    
  case TK_REV:
    return 6;
  case TK_PTR:
    return 5;
  case TK_AND:
    return 4;
  case TK_EQ:
  case TK_NEQ:
    return 3;
  case '*':
  case '/':
    return 2;
  case '+':
  case '-':
    return 1;
  default:
  return 7;
  }
  return 7;
}
uint32_t calc(int i, int j, bool *success){
  if(i > j){
    *success = false;
    return 0;
  }
  if(i == j){
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if(tokens[i].type != TK_NUM && tokens[i].type != TK_HEX && tokens[i].type != TK_REG){
      *success = false;
      return 0;
    }
    *success = true;
    uint32_t ans;
    switch(tokens[i].type) {
      case TK_REG:
        ans = isa_reg_str2val(tokens[i].str+1,success);
      break;
      case TK_NUM:
        sscanf(tokens[i].str,"%u",&ans);
      break;
      case TK_HEX:
        sscanf(tokens[i].str+2,"%x",&ans);
      break;
    }
    return ans;
  }

  if(check_parentheses(i,j)){
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return calc(i+1, j-1, success);
  }
  /* We should do more things here. */
  int brack_num = 0;
  int pri_op = -1; // op is the main position for operator
  int now_pri = 6; // 1 --> 6 show main priority
  for(int k=i;k<=j;k++) {
    if(tokens[k].type=='(') {
      brack_num = brack_num + 1;
    } else if(tokens[k].type == ')') {
      brack_num = brack_num - 1;
    } else if(brack_num == 0) {
      int op = priority_op(tokens[k].type);
      if( now_pri >= op  ) {
        now_pri = op;
        pri_op = k;
      }
    }
  }
  if(pri_op == -1) {
      *success = false;
      return 0;
  }
  if(6 == now_pri) { //addr
      uint32_t addr = calc(i+1,j,success);
      if(! (*success) ) {
        return 0;
      }
      Log("START CALC,0x%x",addr);
      Log("WHY");
      return paddr_read(addr,4);
  } else if(5  == now_pri) {
      uint32_t num = calc(i+1,j,success);
      if(!(*success)) {
        return 0;
      }
      return -num;
  }
  uint32_t left_val = calc(i,pri_op-1,success);
  if(!success) {
    return 0;
  }
  uint32_t right_val = calc(pri_op+1,j,success);
  if(!success) {
    return 0;
  }
  switch (tokens[pri_op].type) {
    case '+': return left_val + right_val;
    case '-': return left_val - right_val;
    case '*': return left_val * right_val;
    case '/':
      if(right_val == 0) {
        printf("divide 0\n");
        return 0;
      }
      return left_val / right_val;
    case TK_EQ: return (left_val == right_val);
    case TK_NEQ: return (left_val != right_val);
    case TK_AND: return (left_val && right_val);
    default:
      printf("wrong\n");
      assert(0);
  }
}
bool check_parentheses(int i,int j) {
  if( ! (tokens[i].type == '(' && tokens[j].type == ')') ) {
    return false;
  }
  int num = 0;
  for(i=i+1;i<j;i++) {
    if(tokens[i].type == '(') {
      num = num + 1;
    }
    if(tokens[i].type == ')' ) {
      num = num - 1;
    }
    if(num < 0 ) {
      return false;
    }
  }
  if(num) {
    return false;
  }
  return true;
}

uint32_t expr(char *e, bool *success) {
  if(NULL==e) {
    success = false;
    return 0;
  }
  if (!make_token(e)) {
    *success = false;
    printf("false expr");
    return 0;
  }
  return calc(0,nr_token-1,success);
}