#include <stdio.h>
#include <gpio.h>
#include <uart.h>
#include "circular.h"
#include "sensor.h"
#include "gl.h"
#include "sonar.h"

typedef unsigned color;
const color WHITE = 0xffffffff;
const color BLUE = 0xffff0000;
const color RED = 0xff0000ff;
const color GREEN = 0xff00ff00;
const color BLACK = 0x00000000;
const unsigned WIDTH = 640;
const unsigned HEIGHT = 480;
const unsigned DEPTH = 4;

int ir_sensors[8] = {GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN17, GPIO_PIN27, GPIO_PIN22, GPIO_PIN10, GPIO_PIN9};
int ir_sensor_reads[8] = {0,0,0,0,0,0,0,0};

int sonar_trig[8] = {GPIO_PIN11, GPIO_PIN6, GPIO_PIN19, GPIO_PIN21, GPIO_PIN16, GPIO_PIN7, GPIO_PIN25, GPIO_PIN23};
int sonar_echo[8] = {GPIO_PIN5, GPIO_PIN13, GPIO_PIN26, GPIO_PIN20, GPIO_PIN12, GPIO_PIN8, GPIO_PIN24, GPIO_PIN18};

int sonar_distance_reads[8] = {0,0,0,0,0,0,0,0};
int sonar_n_accept[8] = {0,0,0,0,0,0,0,0};
int sonar_trigger_start = 0;

int detection_flag = 0;

int disable_sonar_interrupts = 0;
int disable_ir_interrupts = 0;

// This function should not be called.
void impossible_vector(unsigned pc) {
  printf("impossible exception at pc=%x\n", pc);
  reboot();
}

void interrupt_vector(unsigned pc) {
  int sonar_int = 0;
  for (int i = 0; i < 8; i++) {
    if ((gpio_check_event(sonar_echo[i]) == 1)) {
      sonar_int = 1;
      if (disable_sonar_interrupts == 0) sonar_int_handler();
    }
  }
  if (sonar_int == 0)  {
    if (disable_ir_interrupts == 0) sensor_int_handler();
  }
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

  //top sensors
  if (n==0) gl_draw_rect(xl+(4*15), yt+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==1) gl_draw_rect(xl+(4*35), yt+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==2) gl_draw_rect(xl+(4*55), yt+S_OFFSET, S_SIZE, S_SIZE, c);
  if (n==3) gl_draw_rect(xl+(4*75),  yt+S_OFFSET, S_SIZE, S_SIZE, c);
  
  //bottom sensors
  if (n==4) gl_draw_rect(xl+(4*75), yb-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
  if (n==5) gl_draw_rect(xl+(4*55), yb-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
  if (n==6) gl_draw_rect(xl+(4*35), yb-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
  if (n==7) gl_draw_rect(xl+(4*15), yb-S_OFFSET-S_SIZE, S_SIZE, S_SIZE, c);
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

void sonar_draw(void) {
  color c = GREEN;
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

  gl_clear(BLACK);
  gl_draw_string(10,10,"Infrared Motion Detection System", WHITE);
  gl_draw_string(10,10 + 2*gl_get_char_height(), "INTRUDER DETECTED!", RED);
  draw_grid();

  for (int i = 0; i < 8; i++) {
    if (ir_sensor_reads[i] == 1) draw_sensor(i, RED);
  }

  int array_size = sizeof(sonar_distance_reads)/sizeof(sonar_distance_reads[0]);
  for (int i = 0; i < array_size; i++) {
    if (sonar_distance_reads[i] == 0) {}
    else {
      gl_draw_rect(xl+(40*(i+1)), yt+S_OFFSET+sonar_distance_reads[i], S_SIZE, S_SIZE, c);
    }
  }
  gl_swap_buffer();
}

//swapping
void screen_init(void) {
  gl_init(WIDTH, HEIGHT, GL_DOUBLEBUFFER);
  gl_draw_string(10,10,"IR and Sonar Detection System", WHITE);
  draw_grid();
  gl_swap_buffer();
}

//swapping
void intruder_detected(void) {
  gl_clear(BLACK);
  gl_draw_string(10,10,"IR and Sonar Detection System", WHITE);
  gl_draw_string(10,10 + 2*gl_get_char_height(), "INTRUDER DETECTED!", RED);
  draw_grid();
  for (int i = 0; i < 8; i++) {
    if (ir_sensor_reads[i] == 1) draw_sensor(i, RED);
  }
  gl_swap_buffer();
}

//swapping
void intruder_undetected(void) {
  gl_clear(BLACK);
  gl_draw_string(10,10,"IR and Sonar Detection System", WHITE);
  draw_grid();
  gl_swap_buffer();
}

void sensor_read_clear(void) {
  for (int i = 0; i < 8; i++) {
    ir_sensor_reads[i] = 0;
    sonar_distance_reads[i] = 0;
  }
}

void wait_for_rising_edge(int pin) {
  while (gpio_read(pin) == 1) {} 
  while (gpio_read(pin) == 0) {}
}

void wait_for_falling_edge(unsigned pin) {
  while (gpio_read(pin) == 0) {}
  while (gpio_read(pin) == 1) {}
}

void main(void) {
  delay(3);
  sensor_init();
  screen_init();
  sonar_init();
  while (1) {
    if (detection_flag == 1) {
      system_disable_interrupts();
      for (int n_iter = 0; n_iter < 60; n_iter++) {
        //wait for IR
        system_enable_interrupts();
        disable_sonar_interrupts = 1;
        delay(1);
        disable_sonar_interrupts = 0;
        system_disable_interrupts();
        //draw IR
        intruder_detected();
        //wait for sonar
        sonar_trigger_all();
        system_enable_interrupts();
        disable_ir_interrupts = 1;
        delay_us(17400);
        disable_ir_interrupts = 0;
        system_disable_interrupts();
        sonar_draw();
      }
      detection_flag = 0;
      intruder_undetected();
      sensor_read_clear();
      system_enable_interrupts();
    } 
  } 
}
