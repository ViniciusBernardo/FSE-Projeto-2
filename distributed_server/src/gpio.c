// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include "gpio.h"

void initialize_gpio(struct output_devices * devices){
    if (!bcm2835_init())
        return;

    bcm2835_gpio_fsel(LIVING_ROOM, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(KITCHEN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(KITCHEN_DOOR, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(KITCHEN_WINDOW, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(LIVING_ROOM_DOOR, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(LIVING_ROOM_WINDOW, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(BEDROOM_WINDOW_01, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(BEDROOM_WINDOW_02, BCM2835_GPIO_FSEL_INPT);

    bcm2835_gpio_fsel(LAMP_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_03, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_04, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AC_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AC_02, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_write(LAMP_01, LOW);
    bcm2835_gpio_write(LAMP_02, LOW);
    bcm2835_gpio_write(LAMP_03, LOW);
    bcm2835_gpio_write(LAMP_04, LOW);
    bcm2835_gpio_write(AC_01, LOW);
    bcm2835_gpio_write(AC_02, LOW);

    devices->lamp_01 = 0;
    devices->lamp_02 = 0;
    devices->lamp_03 = 0;
    devices->lamp_04 = 0;
    devices->ac_01 = 0;
    devices->ac_02 = 0;
}

void set_input_sensors(struct input_sensors *security_sensors){
    security_sensors->living_room = bcm2835_gpio_lev(LIVING_ROOM);
    security_sensors->kitchen = bcm2835_gpio_lev(KITCHEN);
    security_sensors->kitchen_door = bcm2835_gpio_lev(KITCHEN_DOOR);
    security_sensors->kitchen_window = bcm2835_gpio_lev(KITCHEN_WINDOW);
    security_sensors->living_room_door = bcm2835_gpio_lev(LIVING_ROOM_DOOR);
    security_sensors->living_room_window = bcm2835_gpio_lev(LIVING_ROOM_WINDOW);
    security_sensors->bedroom_window_01 = bcm2835_gpio_lev(BEDROOM_WINDOW_01);
    security_sensors->bedroom_window_02 = bcm2835_gpio_lev(BEDROOM_WINDOW_02);
}

void set_output_devices(struct output_devices *devices, char *command){
    if(strcmp(command, "lamp_01") == 0){
        bcm2835_delay(50);
        bcm2835_gpio_write(LAMP_01, !devices->lamp_01);
        devices->lamp_01 = !devices->lamp_01;
    } else if(strcmp(command, "lamp_02") == 0) {
        bcm2835_delay(50);
        bcm2835_gpio_write(LAMP_02, !devices->lamp_02);
        devices->lamp_02 = !devices->lamp_02;
    } else if(strcmp(command, "lamp_03") == 0) {
        bcm2835_delay(50);
        bcm2835_gpio_write(LAMP_03, !devices->lamp_03);
        devices->lamp_03 = !devices->lamp_03;
    } else if(strcmp(command, "lamp_04") == 0) {
        bcm2835_delay(50);
        bcm2835_gpio_write(LAMP_04, !devices->lamp_04);
        devices->lamp_04 = !devices->lamp_04;
    } else if(strcmp(command, "ac_01") == 0) {
        bcm2835_delay(50);
        bcm2835_gpio_write(AC_01, !devices->ac_01);
        devices->ac_01 = !devices->ac_01;
    } else if(strcmp(command, "ac_02") == 0) {
        bcm2835_delay(50);
        bcm2835_gpio_write(AC_02, !devices->ac_02);
        devices->ac_02 = !devices->ac_02;
    }
}

int check_activate_alarm(struct input_sensors *security_sensors) {
    int activate_alarm = 0;
    activate_alarm =
        security_sensors->living_room ||
        security_sensors->kitchen ||
        security_sensors->kitchen_door ||
        security_sensors->kitchen_window ||
        security_sensors->living_room_door ||
        security_sensors->living_room_window ||
        security_sensors->bedroom_window_01 ||
        security_sensors->bedroom_window_02;
    return activate_alarm;
}

void close_gpio(){
    bcm2835_gpio_write(LAMP_01, LOW);
    bcm2835_gpio_write(LAMP_02, LOW);
    bcm2835_gpio_write(LAMP_03, LOW);
    bcm2835_gpio_write(LAMP_04, LOW);
    bcm2835_gpio_write(AC_01, LOW);
    bcm2835_gpio_write(AC_02, LOW);
    bcm2835_close();
}
