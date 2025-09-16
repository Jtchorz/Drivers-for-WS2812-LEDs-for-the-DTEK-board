//thic code is for now in C for testing, when I'm finished and figure out how to do .h this will change.
#include <stdio.h>

#define num_leds 125
#define BRIGHT 200
#define GPIO_address 0x000000
#deifne wait_long 27  //how long to wait with the signal ~.9us
#define wait_short 11   //how long to wait with the signal ~.35us

//this is to guarantee 8 bits for RGB good representation
uint8_t BUFFER_LEDS[num_leds][3] = {0};

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
        pin_high();
        wait(wait_long);
        pin_low();
        //wait(wait_short);
        //apparently I shouldn't wait here, because the other functions take about 400ns
    }
    else
    {
        pin_high();
        wait(wait_short);
        pin_low();
        //wait(wait_long);
        //apparently should only wait a smidge, cuz other functions take up a lot of time
        wait(wait_short);
    }
}


void singleLed_sendColor(uint8_t Red, uint8_t Gren, uint8_t Blue, uint8_t brightness){
    //this function is pretty much copied from: https://github.com/Blakesands/CH32V003/blob/main/WS2812_driver_CH32V003/GD_WS2812_DRIVER.h
    uint8_t green = Map_Range(Green, 0, 255, 0, (brightness-100)); // -100 adjusting for red led low forward voltage
    uint8_t red = Map_Range(Red, 0, 255, 0, brightness);
    uint8_t blue = Map_Range(Blue, 0, 255, 0, (brightness-100));

     // Send the green component first (MSB)
    for (int i = 7; i >= 0; i--) {
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

int main(){
    printf("Hello World\n");

    //I will test this stuff here
}