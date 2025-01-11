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

make_EHelper(calc_i) {
  switch (decinfo.isa.instr.funct3)
  {
      case 0:
            rtl_addi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(addi);
    default:
      assert(0 && "UNDEFINED_CALC_U_CODE" );
    break;
  }
    rtl_sr(id_dest->reg, &id_dest->val, 4);
}