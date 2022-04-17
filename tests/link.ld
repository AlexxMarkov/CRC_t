


__STACK_SIZE = 32K;
__HEAP_SIZE  = 2M;



ENTRY(reset_handler)



MEMORY
{
    RAM_FW (xr)     : ORIGIN = 0x80000000, LENGTH = 4M
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 4M
}




SECTIONS
{
    .text :
    {
        KEEP(*(.reset_handler))

        . = ALIGN(4);
        KEEP(*(.trap_vectors))

        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        *(.text*)

        . = ALIGN(4);
        *(.rodata*)

        . = ALIGN(4);
        *(.srodata*)
        
        . = ALIGN(4);
    } > RAM_FW


    .data : ALIGN(4)
    {
        KEEP(*(.tohost))

        . = ALIGN(4);
        KEEP(*(.fromhost))
        
        . = ALIGN(4);
        *(.data*)
        *(.gnu.linkonce.d.*)

        . = ALIGN(4);
        PROVIDE( __global_pointer$ = . + 0x800 );
        *(.sdata*)
        *(.gnu.linkonce.s.*)
        
        . = ALIGN(4);
    } > RAM AT > RAM_FW


    .bss : ALIGN(4)
    {
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        
        . = ALIGN(4);
    } > RAM


    .heap : ALIGN(8)
    {
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __HEAP_SIZE;
    } > RAM


    .stack : ALIGN(4)
    {
        . += __STACK_SIZE;
    } > RAM
    



    /* Remove information from the standard libraries */


    .riscv.attributes 0 : { *(.riscv.attributes) }
}



PROVIDE( __trap_vectors         = ADDR(.trap_vectors));


PROVIDE( __stack_limit          = ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE);
PROVIDE( __stack_top            = ORIGIN(RAM) + LENGTH(RAM));


PROVIDE( __data_source_start    = LOADADDR(.data));
PROVIDE( __data_target_start    = ADDR(.data));
PROVIDE( __data_target_end      = ADDR(.data) + SIZEOF(.data));


PROVIDE( __bss_source_start     = LOADADDR(.bss));
PROVIDE( __bss_target_start     = ADDR(.bss));
PROVIDE( __bss_target_end       = ADDR(.bss) + SIZEOF(.bss));


