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
const color BLACK = 0x00000000;
const unsigned WIDTH = 640;
const unsigned HEIGHT = 480;
const unsigned DEPTH = 4;

int ir_sensors[3] = {GPIO_PIN20, GPIO_PIN21, GPIO_PIN22};
int ir_sensor_reads[3] = {0,0,0};
int detection_flag = 0;

// This function should not be called.
void impossible_vector(unsigned pc) {
  printf("impossible exception at pc=%x\n", pc);
  reboot();
}

void interrupt_vector(unsigned pc) {
  sensor_int_handler();
}

void draw_sensor(int n, color c) {
  //adjustable grid parameters
  int V_OFFSET = 10+4*gl_get_char_height();
  int H_OFFSET = 10;
  int BOX_SIZE = 180;

  int S_OFFSET = 20;
  int S_SIZE = 10;
  
  //corners (left, mid, right, top, mid, bottom)
  int xl = H_OFFSET;
  int xm = H_OFFSET + BOX_SIZE;
  int xr = H_OFFSET + 2*BOX_SIZE;
  int yt = V_OFFSET;
  int ym = V_OFFSET + BOX_SIZE;
  int yb = V_OFFSET + 2*BOX_SIZE;

  //left sensors
  if (n==0) gl_draw_rect(xm-S_OFFSET-S_SIZE, yt+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==1) gl_draw_rect(xl+S_OFFSET, ym-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
  if (n==2) gl_draw_rect(xl+S_OFFSET, ym+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==3) gl_draw_rect(xm-S_OFFSET-S_SIZE, yb-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
  
  //right sensors
  if (n==4) gl_draw_rect(xm+S_OFFSET, yt+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==5) gl_draw_rect(xr-S_OFFSET-S_SIZE, ym-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
  if (n==6) gl_draw_rect(xr-S_OFFSET-S_SIZE, ym+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==7) gl_draw_rect(xm+S_OFFSET, yb-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
}

//non-swapping
void draw_grid(void) {
  //adjustable grid parameters
  int V_OFFSET = 10+4*gl_get_char_height();
  int H_OFFSET = 10;
  int BOX_SIZE = 180;

  int S_OFFSET = 20;
  int S_SIZE = 10;
  color S_COLOR = BLUE;
  
  //corners (left, mid, right, top, mid, bottom)
  int xl = H_OFFSET;
  int xm = H_OFFSET + BOX_SIZE;
  int xr = H_OFFSET + 2*BOX_SIZE;
  int yt = V_OFFSET;
  int ym = V_OFFSET + BOX_SIZE;
  int yb = V_OFFSET + 2*BOX_SIZE;
  
  //gridlines
  gl_draw_box(xl, xm, yt, ym, WHITE);
  gl_draw_box(xm, xr, yt, ym, WHITE);
  gl_draw_box(xl, xm, ym, yb, WHITE);
  gl_draw_box(xm, xr, ym, yb, WHITE);

  for (int i = 0; i < 8; i++) {
    draw_sensor(i, S_COLOR);
  }
}

//swapping
void screen_init(void) {
  gl_init(WIDTH, HEIGHT, GL_DOUBLEBUFFER);
  gl_draw_string(10,10,"Infrared Motion Detection System", WHITE);
  draw_grid();
  gl_swap_buffer();
}

//swapping
void intruder_detected(void) {
  gl_clear(BLACK);
  gl_draw_string(10,10,"Infrared Motion Detection System", WHITE);
  gl_draw_string(10,10 + 2*gl_get_char_height(), "INTRUDER DETECTED!", RED);
  draw_grid();
  for (int i = 0; i < 3; i++) {
    if (ir_sensor_reads[i] == 1) draw_sensor(i, RED);
  }
  gl_swap_buffer();
}

//swapping
void intruder_undetected(void) {
  gl_clear(BLACK);
  gl_draw_string(10,10,"Infrared Motion Detection System", WHITE);
  draw_grid();
  gl_swap_buffer();
}

void sensor_read_clear(void) {
  for (int i = 0; i < 3; i++) {
    ir_sensor_reads[i] = 0;
  }
}

void main(void) {
  delay(3);
  sensor_init();
  screen_init();
  while (1) {
    printf("Detection Flag %d\n", detection_flag);
    if (detection_flag == 1) {
      detection_flag = 0;
      intruder_detected();
      sensor_read_clear();
      delay(5);
      intruder_undetected();
    }
    /*int ir_sensor_reads[3];
    for (int i = 0; i < 3; i++) {
      int r = gpio_read(ir_sensors[i]);
      ir_sensor_reads[i] = r;
    }
    printf("\n");*/
  }
}
