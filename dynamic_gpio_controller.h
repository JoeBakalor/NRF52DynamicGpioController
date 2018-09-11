#include <stdint.h>
#include <stddef.h>


void deinit(void);

void set_gpio_as_output(uint8_t GPIO);

void set_gpio_as_input(uint8_t GPIO);

void saadc_init_channel(void);

void saadc_sampling_event_init(void);

void saadc_sampling_event_enable(void);
