#include "sensor.h"
#include "gpio.h"

#define RPI_INT_ENABLE_1             0x2000B210
#define RPI_INT_ENABLE_2             0x2000B214
#define RPI_INT_DISABLE_1            0x2000B21C
#define RPI_INT_DISABLE_2            0x2000B220

const unsigned SENS1 = GPIO_PIN23;

void sensor_init(void) {
	gpio_set_input(SENS1);
    //gpio_set_pullup(SENS1);
}

char sensor_read(void) {
	/* implement */
	return 'a';
}

void sensor_int_handler(void) {
    /* implement */
}