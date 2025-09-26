#include <stdint.h>
#include "library.h"

extern void print(char *);
extern void print_dec(unsigned int);

#define BRIGHT 255
#define GPIO_address 0x040000E0 //gpio no1
#define wait 8  //how long to wait with the signal ~.4us because we initialise timer once, I will just double this value for longer wait period
                //11 instead of 12 cuz clock is one cycle more
                //we aim for 8 instructions, so that we have a lot of space for overshoot

volatile int* TMR1_flag = (volatile int*) 0x04000020;  //for checking if timer did done

volatile int* gpio1_data = (volatile int*) GPIO_address;

volatile uint8_t* flagbit = (volatile uint8_t*) 0x04000020;

//they are written in macros to not mess with timings
#define pin_high() ((*gpio1_data) = 0x1)   //this is clobbering other pins, I know. I will care with testing
    
#define pin_low() ((*gpio1_data) = 0x0)  //this is clobbering other pins, I know.

#define flag_reset() ((*flagbit) = 0)

#define wait_250() ({while(!(*flagbit)){}})   // ==while(!(*flagbit));

//how to send one singular bit, this process is very timing dependent

////thi function is IMPORTANT if the timing is off stuff breaks ##################################################################################
//#################################################################################################################################
void SendBit(bool bit){
    
  flag_reset();   //this is sure that inside the bits, we are actually in sync, not just seeing, oh the flagh is 1 and the timer is whenever
    if(bit){
        wait_250();  //this actually is tripped when the timer flag is set, thanks to setup
        flag_reset(); 

        pin_high(); 

        wait_250();
        flag_reset();
        wait_250();   //do three cycles, so that it is ~0.750us
        flag_reset();                            
        wait_250();

        pin_low();                  //do it first, so that the gpio is set down exactly where it needs, then reset the flag, but the tmr is still running
        
        flag_reset();
        //I will on purpose not wait here, as the next will take 250 at least, propably 500. so just not wait, it will be fine
    }
    else
    {
        
        //this is copy, but just in reverse, first high for one cycle, then high for two cycles

        wait_250();  //this actually is tripped when the timer flag is set, thanks to setup
         
        flag_reset();  //we have a bigger tolarance between diodes than inside, here is better

        pin_high();  

        wait_250();  //just one cycle high
                        
        pin_low();

        //and now we have to wait ~0.8ns, so we will execute two waits ~500ns which then makes at least 
        //one instruction left to start next bit propably two
        flag_reset();
        wait_250();
        flag_reset();
        wait_250();
        flag_reset();
    }
}

void singleLed_sendColor(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t brightness){
    int i = 0;  

    for (i = 7; i >= 0; i--) {
        SendBit((Green >> i) & 1);
    }

    for (i = 7; i >= 0; i--) {
        SendBit((Red >> i) & 1);
    }

    for (i = 7; i >= 0; i--) {
        SendBit((Blue >> i) & 1);
    }

}

void colour_it(uint8_t BUFFER[][3], int num_leds)
{
    for (int i = 0; i < num_leds; i++) {
        singleLed_sendColor(BUFFER[i][0], BUFFER[i][1], BUFFER[i][2], BRIGHT);
    }
    //we can delay as long as we want >50us here, the leds will reset,  but for lowest we did experimentally found 140 clock counts
    int cnt = 0;
    
    while (cnt < 143)  //wait enough so it resets reliably
    {
        while(!(TMR1_flag[0] & 0x1));   //check this later, see if you can use your macros here
        TMR1_flag[0] = 0;
        cnt++;

    }
    
    //print("sent");
}
void init_timer(void)
{
  volatile int* TMR1_PLow = (volatile int*) 0x04000028;
  volatile int* TMR1_PHigh = (volatile int*) 0x0400002C;
  int timeout = wait;
  TMR1_PLow[0] = timeout & 0xFFFF;
  TMR1_PHigh[0] = (timeout >> 16) & 0xFFFF;

  volatile int* TMR1_CTRL = (volatile int*) 0x04000024; //for starting and stopping timer
  TMR1_CTRL[0] = 0x6;  //start the timer in continous mode for better accuracy

  return;
}
void init_gpio(void)
{
    volatile int* GPIO_dir = (volatile int*) (GPIO_address + 0x4);
    GPIO_dir[0] = GPIO_dir[0] | 0x1; //set it as output 

}

void init_all(void)
{
    init_timer();
    init_gpio();
    print("initialised");
}
/*int main(){
    init_timer();
    init_gpio();
    BUFFER_LEDS[0][0] = 50;
    BUFFER_LEDS[0][1] = 50;
    BUFFER_LEDS[0][2] = 50;

    colour_it(BUFFER_LEDS);

}*/