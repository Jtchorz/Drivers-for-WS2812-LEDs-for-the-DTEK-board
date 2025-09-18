/* main.c

   This file written 2015 by F Lundevall, David Broman and Artur Podobas

   Latest update 2024-07-27 by Artur Podobas

   For copyright and licensing, see file COPYING */
#include <stdint.h>
#include <stdlib.h>
#define NUM_LEDS 3
extern void init_all(void);
extern void colour_it(uint8_t BUFFER[][3]);
extern void print(char *);

void handle_interrupt(unsigned cause) 
{}


uint8_t BUFFER_LEDS[NUM_LEDS][3];
/* This is the main function */
int main()
{
  init_all();
  int cnt = 0;
  BUFFER_LEDS[0][0] = 25;
  BUFFER_LEDS[0][1] = 0;
  BUFFER_LEDS[0][2] = 0;
  BUFFER_LEDS[1][0] = 0;
  BUFFER_LEDS[1][1] = 25;
  BUFFER_LEDS[1][2] = 0;

  BUFFER_LEDS[2][0] = 0;
  BUFFER_LEDS[2][1] = 0;
  BUFFER_LEDS[2][2] = 25;
  while(1){
    //BUFFER_LEDS[0][0] = cnt;
    //BUFFER_LEDS[1][1] = cnt;
    //cnt++;
    colour_it(BUFFER_LEDS);
    //if(cnt>50)
     // cnt = 0;
    
  }

}
