OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv",
	      "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY(_start)
STARTUP(boot.o)

MEMORY
{
    RAM (xrw)   : ORIGIN = 0x00000000, LENGTH = 32M
}

SECTIONS
{
   __stack_size = DEFINED(__stack_size) ? __stack_size : 0x100000;
   PROVIDE(__stack_size = __stack_size);
   __heap_size = DEFINED(__heap_size) ? __heap_size : 0x800;

   . = 0x0;
   .text : {*(.text*); }

   .data : { *(.data*)
             PROVIDE( __global_pointer = . + 0x800 );
             *(.sdata*)}

   .bss : { *(.bss) }
   .rodata : { *(.rodata) }
   .comment : { *(.comment) }
   .stack :  {
   PROVIDE(_stack_begin = .);
   . = ALIGN(4);
   . += __stack_size;
   PROVIDE(_stack_end = .);
    }
}
