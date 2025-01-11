#include "cpu/exec.h"

make_EHelper(jal){
    rtl_sr(id_dest->reg,&decinfo.seq_pc,4);
    rtl_j(id_src->imm + decinfo.seq_pc);

    print_asm_template2(jal);
}

make_EHelper(jalr){
    s0 = decinfo.seq_pc;
    rtl_sr(id_dest->reg,&s0,4);
    decinfo.jmp_pc = (id_src->val+id_src2->val)&(~1);

    rtl_j(decinfo_set_jmp);

    print_asm_template3(jalr);
}