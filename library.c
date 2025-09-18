//thic code is for now in C for testing, when I'm finished and figure out how to do .h this will change.
#include <stdio.h>
#include <stdint.h>

#define num_leds 1
#define BRIGHT 100
#define GPIO_address 0x040000E0 //gpio no1
#define wait 11  //how long to wait with the signal ~.4us because we initialise timer once, I will just double this value for longer wait period
                //11 instead of 12 cuz clock is one cycle more

//this is to guarantee 8 bits for RGB good representation
uint8_t BUFFER_LEDS[num_leds][3] = {0};

volatile int* TMR1_CTRL = (volatile int*) 0x04000024; //for starting and stopping timer
volatile int* TMR1_flag = (volatile int*) 0x04000020;  //for checking if timer did done

volatile int* gpio1_data = (volatile int*) GPIO_address;

//this function maps a value from ange A into a corresponding value in range B, useful when we want 10 brightness levels etc
int Map_value(uint8_t value, uint8_t A_min, uint8_t A_max, uint8_t B_min, uint8_t B_max)
{
    if (value < A_min)
        value = A_min;
    if (value > A_max)
        value = A_max;
    return (B_min + (value - A_min)/(A_max - A_min)  *   (B_max - B_min));  //normalise value to [0,1] then scale it to B range 
}


//they are written in macros to not mess with timings
#define pin_high() (gpio1_data[0] = 0x1)   //this is clobbering other pins, I know. I don't care
    
#define pin_low() (gpio1_data[0] = 0x0)  //this is clobbering other pins, I know. I don't care

//how to send one singular bit, this process is very timing dependent

////thi function is IMPORTANT if the timing is off stuff breaks ##################################################################################
//#################################################################################################################################
void SendBit(bool bit){
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
        //whileint(!TMR1_flag[0] & 0x1);  //I will on purpose not wait here, as the next if will at least wait 400ns to finish the timer
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


void singleLed_sendColor(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t brightness){
    uint8_t green = Map_value(Green, 0, brightness, 0, 255); 
    uint8_t red = Map_value(Red, 0, brightness, 0, 255);
    uint8_t blue = Map_value(Blue, 0, brightness, 0, 255);

    //ok, so I have to rethink the logic a bit in here
    //how do I time 24 bits perfectly?
    int i = 0;
    TMR1_CTRL[0] = 0x6;  //start the timer in continous mode for better accuracy [[this make or break]]

    //and now what? I wait for one edge when in the waiting bit to make sure we are on timer, tolerance is 600ns so should be fine with the 400 extra
    // Send the green component first (MSB)

    /*this is actually genius, so listen. SendBit always waits for timer to be done, so the first call sends the first rising edge
    and from that moment the timing has to be tight for 24 bits.
    so when we set the GPIO to LOW, we actually dont wait, just return asap and then the next macro wait for the timer to be done,
    so best case scenario we start next rising edge at precisely 1200ns or worst case 1600. which is still in tolerance
    the worst thing rn is to make sure that from end of one function to the beginnign of the next is not more than 800ns but I dont think so
    */ 
    for (i = 7; i >= 0; i--) {
        SendBit((green >> i) & 1);
    }

    for (i = 7; i >= 0; i--) {
        SendBit((red >> i) & 1);
    }

    for (i = 7; i >= 0; i--) {
        SendBit((blue >> i) & 1);
    }

}

void colour_it(uint8_t BUFFER[num_leds][3])
{
    for (int i = 0; i < num_leds; i++) {
        singleLed_sendColor(BUFFER[i][0], BUFFER[i][1], BUFFER[i][2], BRIGHT);
    }
    //we can delay as long as we want >50us here, the leds will reset
    int cnt = 0;
    while (cnt < 2000)  //wait abt 66us
    {
        while(!(TMR1_flag[0] & 0x1));
        TMR1_flag[0] = 0;
        cnt++;

    }
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
void init_gpio(void)
{
    volatile int* GPIO_dir = (volatile int*) (GPIO_address + 0x4);
    GPIO_dir[0] = GPIO_dir[0] | 0x1; //set it as output 

}
int main(){
    init_timer();
    init_gpio();
    BUFFER_LEDS[0][0] = 50;
    BUFFER_LEDS[0][1] = 50;
    BUFFER_LEDS[0][2] = 50;

    colour_it(BUFFER_LEDS);

}