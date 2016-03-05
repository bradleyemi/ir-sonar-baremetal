#include <stdio.h>
#include <gpio.h>
#include <uart.h>
#include "circular.h"

typedef unsigned color;
const color WHITE = 0xffffffff;
const color BLUE = 0xffff0000;
const color RED = 0xff0000ff;
const color GREEN = 0xff00ff00;
const unsigned WIDTH = 640;
const unsigned HEIGHT = 480;
const unsigned DEPTH = 4;

// This function should not be called.
void impossible_vector(unsigned pc) {
  printf("impossible exception at pc=%x\n", pc);
  reboot();
}

void interrupt_vector(unsigned pc) {
  printf("Interrupt!\n");
}

void main(void) {
  //Wire the sensor to GPIO23 and an LED to GPIO24. 
  gpio_init();
  sensor_init();
  gpio_set_output(GPIO_PIN24);
  while (1) {
  	int out = gpio_read(GPIO_PIN23);
    printf("pin 23 output: %d\n", out);
    if (out == 1) {
    	gpio_write(GPIO_PIN24,1);
    }
    else {
    	gpio_write(GPIO_PIN24,0);
    }
  }
}
