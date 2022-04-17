#include <stdint.h>
#include <stddef.h>

#include "riscv_core.h"

#include "xprintf.h"


#ifdef __cplusplus
 extern "C" {
#endif



int main(void);



static inline void __init_data(const uint32_t *load, uint32_t *start, uint32_t *stop)
{
    while(start != stop) {
        *start++ = *load++;
    }
}



static inline void __init_bss(uint32_t *start, uint32_t *stop)
{
    while(start != stop) {
        *start++ = 0x00;
    }
}



#ifdef STD_LIBC_INIT_ARRAY
    void __libc_init_array(void);
    __WEAK void _init(void){}
#else

    typedef void (*__init_array_func)(void);

    #define LIBC_INIT_SYMBOL(symbol) \
        extern __init_array_func symbol []

    LIBC_INIT_SYMBOL(__preinit_array_start);
    LIBC_INIT_SYMBOL(__preinit_array_end  );
    LIBC_INIT_SYMBOL(__init_array_start   );
    LIBC_INIT_SYMBOL(__init_array_end     );

    static inline void __libc_init_array_call(__init_array_func *array_start,
                                              __init_array_func *array_end)
    {
        while(array_start != array_end) {
            (*array_start)();
            array_start++;
        }
    }

    //add LDFLAGS += -nostartfiles. in make
    static inline void __libc_init_array(void)
    {
        __libc_init_array_call(__preinit_array_start, __preinit_array_end);
        __libc_init_array_call(__init_array_start,    __init_array_end);
    }

#endif //STD_LIBC_INIT_ARRAY


void print_chr(int chr)
{
    *((int*)0x20000000) = chr;
}



void startup_init()
{
    extern uint32_t __data_source_start;
    extern uint32_t __data_target_start;
    extern uint32_t __data_target_end;

    extern uint32_t __bss_target_start;
    extern uint32_t __bss_target_end;

    __init_data(&__data_source_start, &__data_target_start, &__data_target_end);
    __init_bss(&__bss_target_start, &__bss_target_end);
    __libc_init_array();
    
    xdev_out(print_chr);

    main();

    __ECALL();
}



__IRQ_M_WEAK(default_handler)
{
    __ECALL();
    
    while(1) {
        __NOP();
    }
}



__IRQ_M_WEAK_DEFAULT(trap_handler);
__IRQ_M_WEAK_DEFAULT(irq_m_software);
__IRQ_M_WEAK_DEFAULT(irq_m_timer);
__IRQ_M_WEAK_DEFAULT(irq_m_external);
__IRQ_M_WEAK_DEFAULT(irq_fast_0);
__IRQ_M_WEAK_DEFAULT(irq_fast_1);
__IRQ_M_WEAK_DEFAULT(irq_fast_2);
__IRQ_M_WEAK_DEFAULT(irq_fast_3);
__IRQ_M_WEAK_DEFAULT(irq_fast_4);
__IRQ_M_WEAK_DEFAULT(irq_fast_5);
__IRQ_M_WEAK_DEFAULT(irq_fast_6);
__IRQ_M_WEAK_DEFAULT(irq_fast_7);
__IRQ_M_WEAK_DEFAULT(irq_fast_8);
__IRQ_M_WEAK_DEFAULT(irq_fast_9);
__IRQ_M_WEAK_DEFAULT(irq_fast_10);
__IRQ_M_WEAK_DEFAULT(irq_fast_11);
__IRQ_M_WEAK_DEFAULT(irq_fast_12);
__IRQ_M_WEAK_DEFAULT(irq_fast_13);
__IRQ_M_WEAK_DEFAULT(irq_fast_14);
__IRQ_M_WEAK_DEFAULT(irq_fast_15);



#ifdef __cplusplus
}
#endif
