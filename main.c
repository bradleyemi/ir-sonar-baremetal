#include <stdio.h>
#include <gpio.h>
#include <uart.h>
#include "circular.h"
#include "sensor.h"
#include "gl.h"

typedef unsigned color;
const color WHITE = 0xffffffff;
const color BLUE = 0xffff0000;
const color RED = 0xff0000ff;
const color GREEN = 0xff00ff00;
const unsigned WIDTH = 640;
const unsigned HEIGHT = 480;
const unsigned DEPTH = 4;

int ir_sensors[3] = {GPIO_PIN20, GPIO_PIN21, GPIO_PIN22};

// This function should not be called.
void impossible_vector(unsigned pc) {
  printf("impossible exception at pc=%x\n", pc);
  reboot();
}

void interrupt_vector(unsigned pc) {
  sensor_int_handler();
}

void main(void) {
  delay(3);
  sensor_init();
  gl_init(WIDTH, HEIGHT, GL_DOUBLEBUFFER);
  gl_draw_string(10,10,"Infrared Motion Detection System", WHITE);
  gl_swap_buffer();
  while (1) {
    int ir_sensor_reads[3];
    for (int i = 0; i < 3; i++) {
      int r = gpio_read(ir_sensors[i]);
      ir_sensor_reads[i] = r;
      //printf("Sensor %d = %d   ", i, r);
    }
    printf("\n");
  }
}
