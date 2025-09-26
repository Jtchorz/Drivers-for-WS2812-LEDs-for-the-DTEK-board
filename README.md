# Drivers-for-WS2812-LEDs-for-the-DTEK-board
This repo attempts to create a c library and an example c file that will allow the user to use GPIO pins on the DTEK board to control a strip of WS2812 LEDs

This is for DTECH course, no randomness or anything, just sends colors in sequence.


ok, So finished version it is.

To make this work, basically get library.h and library.c
put them in the directory where you are compiling for riscv
and add #include "library.h"
note the ""