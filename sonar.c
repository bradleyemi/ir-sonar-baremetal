#include "sonar.h"
#include "gpio.h"

extern int sonar_trig[8];
extern int sonar_echo[8];
extern int sonar_distance_reads[8];
extern int sonar_n_accept[8];

extern int detection_flag;
extern int timer_get_time(void);

extern int sonar_trigger_start;

extern int disable_sonar_interrupts;
extern int disable_ir_interrupts;

int N_ATT = 4;

void sonar_init(void) {
    system_disable_interrupts();
    for (int i = 0; i < 8; i++) {
    	int sensor = sonar_echo[i];
    	gpio_set_input(sensor);
        gpio_set_pullup(sensor);
        gpio_detect_falling_edge(sensor);
    }

    for (int i = 0; i < 8; i++) {
        int sensor = sonar_trig[i];
        gpio_set_output(sensor);
    }
    system_enable_interrupts();
}
/*
int sonar_trigger_and_read(int trig, int echo) {
    int wait_time = 15000;
    int start_pulse = timer_get_time();
    gpio_write(trig, 0);
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
        system_enable_interrupts();
        return dist;
      }
    }
    return dist;
}

int sonar_get_distance(int trig, int echo, int n_att) {
    int n_accept = 0;
    int distance_sum = 0;
    for (int i = 0; i < n_att; i++) {
        int s = sonar_trigger_and_read(trig, echo);
        if (s != 0 && s != -1 && s < 200) {
            distance_sum += s;
            n_accept++;
        }
    }
    if (n_accept == 0) return 0;
    else {
        return distance_sum / n_accept;
    }
}
*/

void sonar_trigger_all(void) {
    int start_pulse = timer_get_time();
    while (timer_get_time() - start_pulse < 11) {
        for (int i = 0; i < 8; i++) {
            gpio_write(sonar_echo[i], 1);
        }
    }
    int sonar_trigger_start = timer_get_time();
    for (int i = 0; i < 8; i++) {
        gpio_write(sonar_echo[i], 0);
    }
}

void sonar_int_handler(void) {
    system_disable_interrupts();
    int falling_time = timer_get_time();
    for (int i = 0; i < 8; i++) {
        int s = gpio_check_and_clear_event(sonar_echo[i]);
        if (s == 1 && (falling_time - sonar_trigger_start < 17400)) {
            sonar_distance_reads[i] = (falling_time - sonar_trigger_start) / 58;
            sonar_n_accept[i] += 1;
        }
    }
    system_enable_interrupts();
}


