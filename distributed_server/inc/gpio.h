#ifndef GPIO_H
#define GPIO_H

#include "bcm2835.h"

#define LAMP_01 RPI_GPIO_P1_11
#define LAMP_02 RPI_GPIO_P1_12
#define LAMP_03 RPI_V2_GPIO_P1_13
#define LAMP_04 RPI_GPIO_P1_15
#define AC_01 RPI_GPIO_P1_16
#define AC_02 RPI_GPIO_P1_18

#define LIVING_ROOM RPI_GPIO_P1_22
#define KITCHEN RPI_V2_GPIO_P1_37
#define KITCHEN_DOOR RPI_V2_GPIO_P1_29
#define KITCHEN_WINDOW RPI_V2_GPIO_P1_31
#define LIVING_ROOM_DOOR RPI_V2_GPIO_P1_32
#define LIVING_ROOM_WINDOW RPI_V2_GPIO_P1_36
#define BEDROOM_WINDOW_01 RPI_V2_GPIO_P1_38
#define BEDROOM_WINDOW_02 RPI_GPIO_P1_13

struct input_sensors {
    int living_room;
    int kitchen;
    int kitchen_door;
    int kitchen_window;
    int living_room_door;
    int living_room_window;
    int bedroom_window_01;
    int bedroom_window_02;
    int activate_alarm;
};

struct output_devices {
    int lamp_01;
    int lamp_02;
    int lamp_03;
    int lamp_04;
    int ac_01;
    int ac_02;
};

void initialize_gpio(struct output_devices * devices);

void set_input_sensors(struct input_sensors *security_sensors);

void set_output_devices(struct output_devices *devices, char *command);

int check_activate_alarm(struct input_sensors *security_sensors);

void close_gpio();
#endif
