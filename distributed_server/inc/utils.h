#ifndef UTILS_H
#define UTILS_H

#include "gpio.h"
#include "sensor_bme280.h"

struct system {
    int sock;
    float desired_temperature;
    struct bme280_system bme280_sensor;
    struct input_sensors gpio_input;
    struct output_devices gpio_output;
};

void format_json(char *json, struct system * system_state);

#endif
