#include "cpu/exec.h"

make_EHelper(jal){
  rtl_addi(&reg_l(id_dest->reg), pc , 4);
  printf("PC PC PC,0x%x",id_src->imm);
  print_asm_template2(jal);
  rtl_j(id_src->imm+*pc);
}

make_EHelper(jalr){
    s0 = decinfo.seq_pc;
    rtl_sr(id_dest->reg,&s0,4);
    decinfo.jmp_pc = (id_src->val+id_src2->val)&(~1);

    rtl_j(decinfo_set_jmp);

    print_asm_template3(jalr);
}