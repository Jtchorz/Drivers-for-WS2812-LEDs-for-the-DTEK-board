.data
.align 2
welcome_msg: .asciz "================================================\n===== RISC-V Boot-Up Process Now Complete ======\n================================================\n"
	
.section .text
.align 2
.globl _start
	
_isr_handler:
	j _isr_routine	   /* ISR service routine here */
	j _start  	   /* This is the address that a "hard reset" will go to */
	
_isr_routine:
	// Reserve some space on the stack
 	addi sp, sp, -4*32
	
	// Push all registers on the stack (except SP)
	sw x1, 0(sp)	
	sw x3, 8(sp)
	sw x4, 12(sp)
	sw x5, 16(sp)
	sw x6, 20(sp)
	sw x7, 24(sp)
	sw x8, 28(sp)
	sw x9, 32(sp)
	sw x10, 36(sp)
	sw x11, 40(sp)
	sw x12, 44(sp)
	sw x13, 48(sp)
	sw x14, 52(sp)
	sw x15, 56(sp)
	sw x16, 60(sp)
	sw x17, 64(sp)
	sw x18, 68(sp)
	sw x19, 72(sp)
	sw x20, 76(sp)
	sw x21, 80(sp)
	sw x22, 84(sp)
	sw x23, 88(sp)
	sw x24, 92(sp)
	sw x25, 96(sp)
	sw x26, 100(sp)
	sw x27, 104(sp)
	sw x28, 108(sp)
	sw x29, 112(sp)
	sw x30, 116(sp)
	sw x31, 120(sp)
	
	// Find out the cause of this instruction
	csrr t0, mcause
	// Was this an external interrupt? (that is, msb='1')
	li t1, 0x80000000
	// If so, then jump to the external interrupt handler
	bgtu t0,t1, external_irq
	add a6, t0, zero
	// Check if its a ecall -- if so, skip setting a0=mepc and a6=mcause
	addi t1, zero, 11
	beq t0, t1, skip_init_args
	csrr a0, mepc
skip_init_args:
	jal handle_exception	
	// Read the mepc
	csrr t0, mepc
	// Increase it with 4 (otherwise we have an endless loop)	
	addi t0,t0,4
	// Update mepc
	csrw mepc, t0
	// Jump to the place where we go back to where we were interrupted
	j restore

external_irq:
	li t0, 0x7fffffff
	csrr t1, mcause
	and a0, t0, t1
	jal handle_interrupt

restore:
	/* Restore registers from the stack */
	lw x1, 0(sp)
	lw x3, 8(sp)
	lw x4, 12(sp)
	lw x5, 16(sp)
	lw x6, 20(sp)
	lw x7, 24(sp)
	lw x8, 28(sp)
	lw x9, 32(sp)
	lw x10, 36(sp) 
	lw x11, 40(sp)
	lw x12, 44(sp)
	lw x13, 48(sp)
	lw x14, 52(sp)
	lw x15, 56(sp)
	lw x16, 60(sp)
	lw x17, 64(sp)
	lw x18, 68(sp)
	lw x19, 72(sp)
	lw x20, 76(sp)
	lw x21, 80(sp)
	lw x22, 84(sp)
	lw x23, 88(sp)
	lw x24, 92(sp)
	lw x25, 96(sp)
	lw x26, 100(sp)
	lw x27, 104(sp)
	lw x28, 108(sp)
	lw x29, 112(sp)
	lw x30, 116(sp)
	lw x31, 120(sp)
	// Reclaim the space we used
 	addi sp, sp, 4*32

	// Return from interrupt
	mret

	/* This is where the application starts */
_start: 
	// Set the stack point to somewhere free in the main memory
	csrw mie, x0
	la sp, _stack_end
	la gp, __global_pointer
	la a0, welcome_msg
	li a7,4
	ecall
	// Jump to main
	jal main
	
loop:	j loop
