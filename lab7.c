
#define RPI_INT_ENABLE_1             0x2000B210
#define RPI_INT_ENABLE_2             0x2000B214
#define RPI_INT_DISABLE_1            0x2000B21C
#define RPI_INT_DISABLE_2            0x2000B220

/*
 * Disable all interrupts except  GPIO interrupt 0, which is for pins 0-31. This
 * interrupt is IRQ 49, so bit 17 of the second register (BCM2835 ARM
 * Peripherals Manual, top of page 113).
 */
void kdriver_init() {
  gpio_init();

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

void int_handler(unsigned pc) {
  gpio_check_and_clear_event(GPIO_PIN23);
}
