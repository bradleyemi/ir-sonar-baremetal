#include "sonar.h"
#include "gpio.h"

extern int sonar_trig[8];
extern int sonar_echo[8];

extern int detection_flag;
extern int timer_get_time(void);


void sonar_init(void) {
    for (int i = 0; i < 8; i++) {
    	int sensor = sonar_echo[i];
    	gpio_set_input(sensor);
    }

    for (int i = 0; i < 8; i++) {
        int sensor = sonar_trig[i];
        gpio_set_input(sensor);
    }
}

int sonar_trigger_and_read(int trig, int echo) {
    int wait_time = 38000;
    int start_pulse = timer_get_time();
    while (timer_get_time() - start_pulse < 11) {
      gpio_write(trig, 1);
    }
    gpio_write(trig, 0);
    int start_wait_rise = timer_get_time();
    int rising_edge = 0;
    while (timer_get_time() - start_wait_rise < wait_time) {
        if (gpio_read(echo) == 1) {
            rising_edge = 1;
            break;
        }
    }
    if (rising_edge != 1) return -1;
    int dist = 0;
    int start_wait = timer_get_time();
    while (timer_get_time() - start_wait < wait_time) {
      if (gpio_read(echo) == 0) {
        dist = (timer_get_time() - start_wait)/58;
        return dist;
      }
    }
    return dist;
}