#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(auipc) {
  rtl_add(&id_dest->val, &cpu.pc, &id_src->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(auipc);
}

make_EHelper(addi) {
    rtl_addi(&id_dest->val, &id_src->val, id_src2->val);
//    Log("0x%x",id_dest->val);
    rtl_sr(id_dest->reg, &id_dest->val, 4);
    print_asm_template3(addi);
}