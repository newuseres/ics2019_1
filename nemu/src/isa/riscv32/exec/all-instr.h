#include "cpu/exec.h"

make_EHelper(lui);
make_EHelper(auipc);

make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(calc_i);

make_EHelper(jal);
make_EHelper(jalr);