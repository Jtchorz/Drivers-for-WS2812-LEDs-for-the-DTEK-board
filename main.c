/* main.c

   This file written 2015 by F Lundevall, David Broman and Artur Podobas

   Latest update 2024-07-27 by Artur Podobas

   For copyright and licensing, see file COPYING */
#include <stdint.h>
#include <stdlib.h>
#define NUM_LEDS 144
/*extern void init_all(void);
extern void colour_it(uint8_t BUFFER[][3], int num_leds);/*/
#include "library.h" 
//extern void print(char *);

void handle_interrupt(unsigned cause) 
{}


uint8_t BUFFER_LEDS[NUM_LEDS][3];
/* This is the main function */
int main()
{
  init_all();
  int cnt = 0;
  BUFFER_LEDS[0][0] = 0;
  BUFFER_LEDS[0][1] = 0;
  BUFFER_LEDS[0][2] = 0;

  /*BUFFER_LEDS[1][0] = 1;
  BUFFER_LEDS[1][1] = 1;
  BUFFER_LEDS[1][2] = 1;

  BUFFER_LEDS[2][0] = 1;
  BUFFER_LEDS[2][1] = 1;
  BUFFER_LEDS[2][2] = 1;*/
  while(1){
    //BUFFER_LEDS[0][0] = cnt;
    //BUFFER_LEDS[1][1] = cnt;
    //cnt++;
   /* for(int i = 0; i < NUM_LEDS; i++)
    { 
      BUFFER_LEDS[i][0] = 1;
      BUFFER_LEDS[i][1] = 1;
      BUFFER_LEDS[i][2] = 1;
      colour_it(BUFFER_LEDS, NUM_LEDS);
      if(i<2)
      {
        BUFFER_LEDS[NUM_LEDS-2+i][0] = 0;
        BUFFER_LEDS[NUM_LEDS-2+i][1] = 0;
        BUFFER_LEDS[NUM_LEDS-2+i][2] = 0;
      }
      else
      {
        BUFFER_LEDS[i-2][0] = 0;
        BUFFER_LEDS[i-2][1] = 0;
        BUFFER_LEDS[i-2][2] = 0;
      }
      colour_it(BUFFER_LEDS, NUM_LEDS);
    }
   */

    if(cnt > 143)
      cnt = 0;

    BUFFER_LEDS[cnt][0] = 1;
    BUFFER_LEDS[cnt][1] = 1;
    BUFFER_LEDS[cnt][2] = 1;
    if(cnt > 0){
    BUFFER_LEDS[cnt-1][0] = 0;
    BUFFER_LEDS[cnt-1][1] = 0;
    BUFFER_LEDS[cnt-1][2] = 0;
    }
    else{
      BUFFER_LEDS[NUM_LEDS-1][0] = 0;
      BUFFER_LEDS[NUM_LEDS-1][1] = 0;
      BUFFER_LEDS[NUM_LEDS-1][2] = 0;
    }
    colour_it(BUFFER_LEDS, NUM_LEDS);

    cnt++;
    //if(cnt>50)
     // cnt = 0;
    
  }

}
