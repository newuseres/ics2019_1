#include "cpu/exec.h"

make_EHelper(jal) { 
    rtl_sr(id_dest->reg, &decinfo.seq_pc, 4);
    rtl_add(&id_src->val,&decinfo.seq_pc,&id_src->val);
    rtl_subi(&id_src->val,&id_src->val,4);
    rtl_j(id_src->val);
    print_asm_template2(jal);
}
make_EHelper(jalr){
    s0 = decinfo.seq_pc;
    rtl_sr(id_dest->reg,&decinfo.seq_pc,4);
    rtl_add(&id_src->val,&id_src->val,&id_src2->val);
    rtl_andi(&id_src->val,&id_src->val,(~1));
    rtl_j(id_src->val);
    print_asm_template3(jalr);
}