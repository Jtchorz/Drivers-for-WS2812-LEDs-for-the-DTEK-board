//thic code is for now in C for testing, when I'm finished and figure out how to do .h this will change.
#include <stdio.h>
#include <stdint.h>

#define num_leds 125
#define BRIGHT 200
#define GPIO_address 0x000000
#define wait 11  //how long to wait with the signal ~.4us because we initialise timer once, I will just double this value for longer wait period
                //11 instead of 12 cuz clock is one cycle more

//this is to guarantee 8 bits for RGB good representation
uint8_t BUFFER_LEDS[num_leds][3] = {0};

volatile int* TMR1_CTRL = (volatile int*) 0x04000024; //for starting and stopping timer

volatile int* TMR1_flag = (volatile int*) 0x04000020;  //for checking if timer did done

//this function maps a value from ange A into a corresponding value in range B, useful when we want 10 brightness levels etc
int Map_values(int value, int A_min, int A_max, int B_min, int B_max)
{
    if (value < A_min)
        value = A_min;
    if (value > A_max)
        value = A_max;
    return (B_min + (value - A_min)/(A_max - A_min)  *   (B_max - B_min));  //normalise value to [0,1] then scale it to B range 
}


//they are written in macros to not mess with timings
#define pin_high(){
    //write code here that will set the appropriate pin to 1
}

#define pin_low(){
    //write code here that will set the appropriate pin to 0
}

#define wait(int cycles){
    //tell processor to wait a number of cycles, which is stupid cuz for loops take a shit ton of time, 
    //imma just say nop enough times and is gonna be ok
    //meybe this useful for the 50us as it doesnt have to be as precise

}

//how to send one singular bit, this process is very timing dependent

////thi function is IMPORTANT if the timing is off stuff breaks ##################################################################################
//#################################################################################################################################
#define SendBit(bool bit){
    if(bit){
        while(!(TMR1_flag[0] & 0x1));  //this is just waiting to make sure we are in sync with timers
        pin_high();   //executing this immediately to minimize drift
        TMR1_flag[0] = 0;

        while(!(TMR1_flag[0] & 0x1));
        TMR1_flag[0] = 0;
        while(!(TMR1_flag[0] & 0x1));   //do two cycles, so that it is ~0.8us and by running it in cont it doesnt matter how many instructions do I use
                                    //to reset the flag
        pin_low();                  //do it first, so that the gpio is set down exactly where it needs, then reset the flag, but the tmr is still running
        TMR1_flag[0] = 0; 
        //while(!TMR1_flag[0] & 0x1);  //I will on purpose not wait here, as the next if will at least wait 400ns to finish the timer
    }
    else
    {
        //this is copy, but just in reverse, first high for one cycle, then high for two cycles
        while(!(TMR1_flag[0] & 0x1));  //this is just waiting to make sure we are in sync with timers
        pin_high();   //executing this immediately to minimize drift
        TMR1_flag[0] = 0;

        while(!(TMR1_flag[0] & 0x1));         //just one cycle high
        pin_low();                  
        TMR1_flag[0] = 0;
        //and now we have to wait ~0.8ns, so we will execute one wait, and then follow the for normally
        while(!(TMR1_flag[0] & 0x1));
        TMR1_flag[0] = 0;
    }
}


void singleLed_sendColor(uint8_t Red, uint8_t Gren, uint8_t Blue, uint8_t brightness){
    uint8_t green = Map_Range(Green, 0, 255, 0, brightness); // -100 adjusting for red led low forward voltage
    uint8_t red = Map_Range(Red, 0, 255, 0, brightness);
    uint8_t blue = Map_Range(Blue, 0, 255, 0, brightness);
    //ok, so I have to rethink the logic a bit in here
    //how do I time 24 bits perfectly?
    int i = 0;
    TMR1_CTRL[0] = 0x6;  //start the timer in continous mode for better accuracy
    //and now what? I wait for one edge when in the waiting bit to make sure we are on timer, tolerance is 600ns so should be fine with the 400 extra
     // Send the green component first (MSB)
    for (i = 7; i >= 0; i--) {
        SendBit((green >> i) & 1);
    }
    // Send the red component next
    for (int i = 7; i >= 0; i--) {
        SendBit((red >> i) & 1);
    }
    // Send the blue component last (LSB)
    for (int i = 7; i >= 0; i--) {
        SendBit((blue >> i) & 1);
    }

}

void colout_it(uint8_t BUFFER_LEDS[num_leds][3])
{
    for (int i = 0; i < num_leds; i++) {
        singleLed_sendColor(BUFFER_LEDS[i][0], BUFFER_LEDS[i][1], BUFFER_LEDS[i][2], BRIGHT);
    }
    //we can delay as long as we want >50us here, the leds will reset
    Delay_Us(50);
}
void init_timer(void)
{
  volatile int* TMR1_PLow = (volatile int*) 0x04000028;
  volatile int* TMR1_PHigh = (volatile int*) 0x0400002C;
  int timeout = wait;
  TMR1_PLow[0] = timeout & 0xFFFF;
  TMR1_PHigh[0] = (timeout >> 16) & 0xFFFF;

  volatile int* TMR1_CTRL = (volatile int*) 0x04000024;
  TMR1_CTRL[0] = 0x6;  //start the timer in continous mode for better accuracy

  return;
}

int main(){
    printf("Hello World\n");

    //I will test this stuff here
}