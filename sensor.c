#include "sensor.h"
#include "gpio.h"

#define RPI_INT_ENABLE_1             0x2000B210
#define RPI_INT_ENABLE_2             0x2000B214
#define RPI_INT_DISABLE_1            0x2000B21C
#define RPI_INT_DISABLE_2            0x2000B220

const unsigned SENS1 = GPIO_PIN23;
const unsigned SENS2 = GPIO_PIN25;

const unsigned OUT1 = GPIO_PIN24;
const unsigned OUT2 = GPIO_PIN26;

void sensor_init(void) {
	gpio_init();


    // Set signal receiving pins
	gpio_set_input(SENS1);
    gpio_set_pullup(SENS1);
    gpio_detect_rising_edge(SENS1);

    gpio_set_input(SENS2);
    gpio_set_pullup(SENS2);
    gpio_detect_rising_edge(SENS2);

    // Setup LED outputs
    gpio_set_output(OUT1);
    gpio_set_output(OUT2);

    // Make sure everything is cleared
    PUT32(RPI_INT_DISABLE_1, 0xFFFFFFFF);
    PUT32(RPI_INT_DISABLE_2, 0xFFFFFFFF);

    // Enable GPIO interrupts
    PUT32(RPI_INT_ENABLE_1, 0x00100000);
    PUT32(RPI_INT_ENABLE_2, 0x00100000);

    // Enable interrupts
    system_enable_interrupts();
}

void sensor_int_handler(void) {
	gpio_check_and_clear_event(SENS1);
	system_disable_interrupts();

	//check which interrupt was triggered
	if (gpio_read(SENS1) == 1) {
		gpio_write(OUT1, 1);
	}
	if (gpio_read(SENS2) == 1) {
		gpio_write(OUT2, 1);
	}

    delay(2);

    //turn off the LEDs
    gpio_write(OUT1, 0);
    gpio_write(OUT2, 0);

    system_enable_interrupts();
}