#ifndef RISCV_CORE_H
#define RISCV_CORE_H



#include "compiler_def.h"



#define __NOP()                     __ASM volatile ("nop")


#define __ECALL()                   __ASM volatile ("ecall")


#define __EBREAK()                  __ASM volatile ("ebreak")



#endif // RISCV_CORE_H
