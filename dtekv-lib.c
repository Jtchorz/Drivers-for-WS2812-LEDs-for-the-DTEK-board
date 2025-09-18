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

/*
 * nextprime
 * 
 * Return the first prime number larger than the integer
 * given as a parameter. The integer must be positive.
 */
#define PRIME_FALSE   0     /* Constant to help readability. */
#define PRIME_TRUE    1     /* Constant to help readability. */
int nextprime( int inval )
{
   register int perhapsprime = 0; /* Holds a tentative prime while we check it. */
   register int testfactor; /* Holds various factors for which we test perhapsprime. */
   register int found;      /* Flag, false until we find a prime. */

   if (inval < 3 )          /* Initial sanity check of parameter. */
   {
     if(inval <= 0) return(1);  /* Return 1 for zero or negative input. */
     if(inval == 1) return(2);  /* Easy special case. */
     if(inval == 2) return(3);  /* Easy special case. */
   }
   else
   {
     /* Testing an even number for primeness is pointless, since
      * all even numbers are divisible by 2. Therefore, we make sure
      * that perhapsprime is larger than the parameter, and odd. */
     perhapsprime = ( inval + 1 ) | 1 ;
   }
   /* While prime not found, loop. */
   for( found = PRIME_FALSE; found != PRIME_TRUE; perhapsprime += 2 )
   {
     /* Check factors from 3 up to perhapsprime/2. */
     for( testfactor = 3; testfactor <= (perhapsprime >> 1) + 1; testfactor += 1 )
     {
       found = PRIME_TRUE;      /* Assume we will find a prime. */
       if( (perhapsprime % testfactor) == 0 ) /* If testfactor divides perhapsprime... */
       {
         found = PRIME_FALSE;   /* ...then, perhapsprime was non-prime. */
         goto check_next_prime; /* Break the inner loop, go test a new perhapsprime. */
       }
     }
     check_next_prime:;         /* This label is used to break the inner loop. */
     if( found == PRIME_TRUE )  /* If the loop ended normally, we found a prime. */
     {
       return( perhapsprime );  /* Return the prime we found. */
     } 
   }
   return( perhapsprime );      /* When the loop ends, perhapsprime is a real prime. */
} 
