/* main.c

   This file written 2015 by F Lundevall, David Broman and Artur Podobas

   Latest update 2024-07-27 by Artur Podobas

   For copyright and licensing, see file COPYING */

extern void print(const char*);
extern void print_dec(unsigned int);
extern void print_hex32 ( unsigned int x);

void handle_interrupt(void) {}

void print_word (const char *str, void *ptr)
{
  print(str);
  print(" : address=");
  print_hex32((unsigned int) ptr);
  print(" : value=");
  print_dec(*((unsigned int*)ptr));
  print("\n");
}

void print_byte (const char *str, char *ptr)
{
  print(str);
  print(" : address=");
  print_hex32((unsigned int) ptr);
  print(" : value=");
  print_dec(*((unsigned char*)ptr));
  print("\n");
}



int gv; /* Global variable. */
int in = 3; /* Global variable, initialized to 3. */

void fun(int param)
{
  param++;
  print_word( "AF1: param", &param );
  gv = param; /* Change the value of a global variable. */
}

/* This is the main function */
int main()
{
  /* Local variables. */
  int m;
  int * p; /* Declare p as pointer, so that p can hold an address. */
  char cs[ 9 ] = "Bonjour!";
  char * cp = cs; /* Declare cp as pointer, initialise cp to point to cs */
                                                                                                                                                                                             
  /* Do some calculation. */                                                                                                                                                                
  gv = 4;                                                                                                                                                                                    
  m = gv + in;
  
  /* Check the addresses and values of some variables and stuff */                                                                                                                           
  print_word( "AM1: gv", &gv );                                                                                                                                                                
  print_word( "AM2: in", &in );
  print_word( "AM3: fun", &fun );
  print_word( "AM4: main", &main );

  p = &m;

  /* Check p and m */
  print_word( "AM5: p", &p );
  print_word( "AM6: m", &m );

  /* Change *p */

  *p = *p + 1;

  /* Check p and m */
  print_word( "AM7: p", &p );
  print_word( "AM8: m", &m );

  p = (int*)cp;   /* Casting a char pointer to an integer pointer */

  print_word( "AM9: p", &p );

  print_byte( "AM10: cs[0]", &cs[0] );
  print_byte( "AM11: cs[1]", &cs[1] );
  print_byte( "AM12: cs[2]", &cs[2] );
  print_byte( "AM13: cs[3]", &cs[3] );

  *p = 0x1234abcd; /* It starts to get interesting... */

  print_byte( "AM14: cs[0]", &cs[0] );
  print_byte( "AM15: cs[1]", &cs[1] ); 
  print_byte( "AM16: cs[2]", &cs[2] );
  print_byte( "AM17: cs[3]", &cs[3] );

  fun(m);

  /* Re-check the addresses and values of m and gv */
  print_word( "AM18: m", &m );
  print_word( "AM19: gv", &gv );

}
