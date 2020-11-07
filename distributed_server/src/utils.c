#include "utils.h"
#include <stdio.h>

void format_json(char *json, struct system * system_state){
    sprintf(
        json,
        "{"
        "   \"temperature\": %.2f,"
        "   \"humidity\": %.2f,"
        "   \"living_room\": %d,"
        "   \"kitchen\": %d,"
        "   \"kitchen_door\": %d,"
        "   \"kitchen_window\": %d,"
        "   \"living_room_door\": %d,"
        "   \"living_room_window\": %d,"
        "   \"bedroom_window_01\": %d,"
        "   \"bedroom_window_02\": %d,"
        "   \"activate_alarm\": %d,"
        "   \"lamp_01\": %d,"
        "   \"lamp_02\": %d,"
        "   \"lamp_03\": %d,"
        "   \"lamp_04\": %d,"
        "   \"ac_01\": %d,"
        "   \"ac_02\": %d,"
        "   \"desired_temperature\": %.2f"
        "}",
        system_state->bme280_sensor.temperature,
        system_state->bme280_sensor.humidity,
        system_state->gpio_input.living_room,
        system_state->gpio_input.kitchen,
        system_state->gpio_input.kitchen_door,
        system_state->gpio_input.kitchen_window,
        system_state->gpio_input.living_room_door,
        system_state->gpio_input.living_room_window,
        system_state->gpio_input.bedroom_window_01,
        system_state->gpio_input.bedroom_window_02,
        system_state->gpio_input.activate_alarm,
        system_state->gpio_output.lamp_01,
        system_state->gpio_output.lamp_02,
        system_state->gpio_output.lamp_03,
        system_state->gpio_output.lamp_04,
        system_state->gpio_output.ac_01,
        system_state->gpio_output.ac_02,
	system_state->desired_temperature
    );
}
