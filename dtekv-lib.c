#include "dtekv-lib.h"

#define JTAG_UART ((volatile unsigned int*) 0x04000040)
#define JTAG_CTRL ((volatile unsigned int*) 0x04000044)

void printc(char s)
{
    while (((*JTAG_CTRL)&0xffff0000) == 0);
    *JTAG_UART = s;
}

void print(char *s)
{  
  while (*s != '\0') {    
      printc(*s);
      s++;
  }
}

void print_dec(unsigned int x)
{
  unsigned divident = 1000000000;
  char first = 0;
  do {
    int dv = x / divident;
    if (dv != 0) first = 1;
    if (first != 0)
      printc(48+dv);
    x -= dv*divident;
    divident /= 10;
  } while (divident != 0);
    if (first == 0)
	printc(48);
}

void print_hex32 ( unsigned int x)
{
  printc('0');
  printc('x');
  for (int i = 7; i >= 0; i--) {
    char hd = (char) ((x >> (i*4)) & 0xf);
    if (hd < 10)
      hd += '0';
    else
      hd += ('A' - 10);
    printc(hd);
  }   
}

/* function: handle_exception
   Description: This code handles an exception. */
void handle_exception ( unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3, unsigned arg4, unsigned arg5, unsigned mcause, unsigned syscall_num )
{
  switch (mcause)
    {
    case 0:
      print("\n[EXCEPTION] Instruction address misalignment. "); 
      break;
    case 2:
      print("\n[EXCEPTION] Illegal instruction. "); 
      break;
    case 11:
      if (syscall_num == 4)
	print((char*) arg0); 
      if (syscall_num == 11)
	printc(arg0);
      return ;
      break;
    default:
      print("\n[EXCEPTION] Unknown error. ");
      break;
    }
  
  print("Exception Address: ");
  print_hex32(arg0); printc('\n');
  while (1);
}