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

make_EHelper(calc_i){
  switch (decinfo.isa.instr.funct3){
    case 0x00:
        rtl_addi(&id_dest->val, &id_src->val, id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(addi);
      break;
    case 0x04:
        rtl_xori(&id_dest->val, &id_src->val, id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(xori);      
      break;
    case 0x06:
        rtl_ori(&id_dest->val, &id_src->val, id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(ori);
      break;
    case 0x07:
        rtl_andi(&id_dest->val, &id_src->val, id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(andi);
      break; 
    case 0x01:
        rtl_shli(&id_dest->val, &id_src->val, id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(slli);
      break;
    case 0x05:
        if(decinfo.isa.instr.funct7 == 0) {
          rtl_shri(&id_dest->val, &id_src->val, id_src2->val);
          rtl_sr(id_dest->reg, &id_dest->val, 4);
          print_asm_template3(srli);
        } else if(decinfo.isa.instr.funct7 == 0x20) {
          rtl_sari(&id_dest->val, &id_src->val, id_src2->val);
          rtl_sr(id_dest->reg, &id_dest->val, 4);
          print_asm_template3(srai);          
        } else {
          assert(0&&"WRONG CALC_I");
        }
      break;
    case 0x02:
        id_dest->val = (signed)id_src->val < (signed)id_src2->val;
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(slti);        
      break;    
    case 0x03:
        id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(sltiu);      
      break;
    default:
      assert(0&&"WRONG CALC_I");
  }
}

make_EHelper(calc_r){
  switch (decinfo.isa.instr.funct3){
    case 0x00:
      if(decinfo.isa.instr.funct7 == 0x00) {
        rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(add);
      } else if(decinfo.isa.instr.funct7 == 0x20 ) {
        rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(sub);
      } else if(decinfo.isa.instr.funct7 == 0x01) {
        rtl_imul_lo(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(mul);
      } else {
        assert(0&&"WRONG CALC_R");
      }
      break;
    case 0x04:
      if(decinfo.isa.instr.funct7 == 0x00){
        rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(xor);
      } else if(decinfo.isa.instr.funct7 == 0x01) {
        rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(div);  
      } else {
        assert(0&&"WRONG CALC_R");
      }
      break;
    case 0x06:
      if(decinfo.isa.instr.funct7 == 0x00){       
        rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(or);
      }
      else if(decinfo.isa.instr.funct7 == 0x01) {                                       
        rtl_idiv_r(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(rem);
      } else {
        assert(0&&"WRONG CALC_R");
      }
    break;
      break;
    case 0x07:
      if(decinfo.isa.instr.funct7 == 0x00){   
        rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(and);
      }
      else if(decinfo.isa.instr.funct7 == 0x01){                                 
        rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(remu);
      } else {
        assert(0&&"WRONG CALC_R");
      }
      break; 
    case 0x01:
      if(decinfo.isa.instr.funct7 == 0x00){
        rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(sll);
      }
      else if(decinfo.isa.instr.funct7 == 0x01) {
        rtl_imul_hi(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(mulh);
      } else {
        assert(0&&"WRONG CALC_R");
      }
      break;
    case 0x05:
      if(decinfo.isa.instr.funct7 == 0x00){
        rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(srl);
      }
      else if(decinfo.isa.instr.funct7 == 0x20){
        rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(sra);
      }
      else if(decinfo.isa.instr.funct7 == 0x01) {
        rtl_div_q(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(divu);
      } else {
        assert(0&&"WRONG CALC_R");
      }
      break;
    case 0x02: 
      if(decinfo.isa.instr.funct7 == 0x00){
        id_dest->val = (signed)id_src->val < (signed)id_src2->val;
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template3(slt);
      }
      else if(decinfo.isa.instr.funct7 == 0x01){
        //not real mulhsu
        rtl_imul_hi(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);        
        print_asm_template2(mulhsu);
      } else {
        assert(0&&"WRONG CALC_R");
      }
      break;
    case 0x03:
      if(decinfo.isa.instr.funct7 == 0b1){
        rtl_mul_hi(&id_dest->val, &id_src->val, &id_src2->val);
        rtl_sr(id_dest->reg, &id_dest->val, 4);        
        print_asm_template2(mulhu);
      }
      else if(decinfo.isa.instr.funct7 == 0){
        id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
        rtl_sr(id_dest->reg, &id_dest->val, 4);
        print_asm_template2(sltu);
      }
      else{
          assert(0&&"WRONG CLAC_R");
      }    
      break;
    default:
      assert(0&&"WRONG CALC_I");
  }
}

