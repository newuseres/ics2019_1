#include "nemu.h"

const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  printf("REGISTER INFO:\n");
  printf("%-4s 0x%-10x %u\n","pc",cpu.pc,cpu.pc);
  for(int i=0;i<32;i++) {
    rtlreg_t reg = reg_l(i);
    printf("%-4s 0x%-10x %d\n",reg_name(i,4),reg,reg);
  }
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
  if(strcmp(s, "pc") == 0){
    *success = true;
    return cpu.pc;
  }
  for(int i = 0; i < 32; i++){
    if(strcmp(s, regsl[i]) == 0){
      *success = true;
      return reg_l(i);
    }
  }
  *success = false;
  return 0;
}
