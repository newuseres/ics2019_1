#include "cpu/exec.h"

make_EHelper(jal) { 
    rtl_sr(id_dest->reg, &decinfo.seq_pc, 4);
    rtl_add(&id_src->val,&decinfo.seq_pc,&id_src->val);
    rtl_subi(&id_src->val,&id_src->val,4);
//    Log("tz 0x%x",id_src->val);
    rtl_j(id_src->val);
    print_asm_template2(jal);
}
make_EHelper(jalr){
    rtl_sr(id_dest->reg,&decinfo.seq_pc,4);
    rtl_add(&id_src->val,&id_src->val,&id_src2->val);
    rtl_andi(&id_src->val,&id_src->val,(~1));
    rtl_j(id_src->val);
    print_asm_template3(jalr);
}

make_EHelper(branch_go){
    rtl_add(&decinfo.jmp_pc,&decinfo.seq_pc,&id_dest->val);
    rtl_subi(&decinfo.jmp_pc,&decinfo.jmp_pc,4);
    switch(decinfo.isa.instr.funct3){
        case 0x0:
            rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, decinfo.jmp_pc);
            print_asm_template3(beq);
            break;
        case 0x1:
            rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
            print_asm_template3(bne);
            break;
        case 0x4:
            rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, decinfo.jmp_pc);
            print_asm_template3(blt);
            break;
        case 0x5:
            rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
            print_asm_template3(bge);
            break;
        case 0x6:
            rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
            print_asm_template3(bltu);
            break;
        case 0x7:
            rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
            print_asm_template3(bgeu);
            break;
        default:
            assert(0 && "wrong BRANCH code");
    }
}