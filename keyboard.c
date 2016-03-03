#include <stdio.h>
#include <gpio.h>
#include <uart.h>
#include "circular.h"

#include "ps2.h"
#include "keyboard.h"

#define RPI_INT_ENABLE_1             0x2000B210
#define RPI_INT_ENABLE_2             0x2000B214
#define RPI_INT_DISABLE_1            0x2000B21C
#define RPI_INT_DISABLE_2            0x2000B220

const unsigned CLK  = GPIO_PIN23;
const unsigned DATA = GPIO_PIN24;

int release = 0;
unsigned caps = 0;
unsigned shift = 0;
int bytes[16];
int scancode = 0;
int startCheck = 0;
int parityCheck = 0;
int pos = 0;
int bytePos = 0;
int LED = 0;
int nBits = 0;
int releaseFlag = 0;

void wait_for_falling_edge(unsigned pin) {
  while (gpio_read(pin) == 0) {  }
  while (gpio_read(pin) == 1) {  }
}

void keyboard_init(void) {
  gpio_set_function(CLK, GPIO_FUNC_INPUT);
  gpio_set_pullup(CLK);

  gpio_set_function(DATA, GPIO_FUNC_INPUT);
  gpio_set_pullup(DATA);
  
  
   // Ensure all interrupts are disabled.
  PUT32(RPI_INT_DISABLE_1, 0xFFFFFFFF);
  PUT32(RPI_INT_DISABLE_2, 0xFFFFFFFF);

  // We'll be hooking up a button to Pin 23, so set it up appropriately.
  gpio_set_input(GPIO_PIN23);
  gpio_set_pullup(GPIO_PIN23); 
  gpio_detect_falling_edge(GPIO_PIN23);

  // Enable GPIO interrupt 0
  PUT32(RPI_INT_ENABLE_2, 0x00100000);

  // Enable interrupts
  system_enable_interrupts();
  
}

int keyboard_read_byte(void) {
  int scancode = cir_pop();
  if (scancode == KEYBOARD_CODE_RELEASE) {
    int released_scancode = cir_pop();
    if ( (released_scancode == KEYBOARD_CODE_SHIFTL) || (released_scancode ==  KEYBOARD_CODE_SHIFTR) ) {
      if (shift == 1) shift = 0;
    }
    return -1;
  }

  if (scancode == KEYBOARD_CODE_VAR_IGNORE) {
    return -1;
  }

  if (((scancode == KEYBOARD_CODE_SHIFTL) || (scancode ==  KEYBOARD_CODE_SHIFTR))) {
    if (shift == 0) shift = 1;
    return -1;
  }

  if (scancode == KEYBOARD_CODE_CAPS_LOCK) {
    if (caps == 1) {
      caps = 0;
    }
    else if (caps == 0) {
      caps = 1;
    }
    return -1;
  }
  return scancode;
}

char keyboard_read(void) {
  int scancode = keyboard_read_byte();
  if ((scancode > 0) && ((caps == 0 && shift == 0) || (caps == 1 && shift == 1))) {
    return char_from_scan[scancode];
  }
  else if ((scancode > 0) && ((caps == 1 && shift == 0) || (caps == 0 && shift == 1))) {
    return shift_char_from_scan[scancode];
  }
  else {
    return 0;
  }
}

void keyboard_int_handler(void) {
  gpio_check_and_clear_event(GPIO_PIN23);
  system_disable_interrupts();
  if (startCheck == 0) {
    if (gpio_read(DATA) == 0) {
      startCheck = 1;
    }
    else {
      printf("start check failed\n");
    }
  }
  else if (startCheck == 1 && pos < 8) {
    int bit = gpio_read(DATA);
    scancode |= (bit << pos);
    nBits = nBits + bit;
    pos++;
  }
  else if (pos == 8) {
    if (parityCheck == 0) {
      int parityBit = gpio_read(DATA);
      if (parityBit == nBits % 2) {
        printf("parity check failed\n");
        scancode = 0;
        nBits = 0;
        pos = 0;
        startCheck = 0;
        return;
      }
      else {
        parityCheck = 1;
      }
    }
    else {
      int stopBit = gpio_read(DATA);
      if (stopBit != 1) {
        printf("no stop bit\n");
        scancode = 0;
        nBits = 0;
        pos = 0;
        startCheck = 0;
        parityCheck = 0;
        return;
      }
      else {
        cir_push(scancode);
        nBits = 0;
        pos = 0;
        startCheck = 0;
        parityCheck = 0;
        scancode = 0;
      }
    }
  }
  system_enable_interrupts();
}

