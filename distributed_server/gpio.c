// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>
#include <bcm2835.h>

#define LIVING_ROOM RPI_V2_GPIO_P1_32
#define BEDROOM_WINDOW_01 RPI_V2_GPIO_P1_38

struct input_sensors {
    int living_room;
    int bedroom_window_01;
};

void initialize_gpio(){
    if (!bcm2835_init())
        return;

    bcm2835_gpio_fsel(LIVING_ROOM, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(BEDROOM_WINDOW_01, BCM2835_GPIO_FSEL_INPT);
}

void set_input_sensors(struct input_sensors *security_sensors){
    security_sensors->living_room = bcm2835_gpio_lev(LIVING_ROOM);
    security_sensors->bedroom_window_01 = bcm2835_gpio_lev(BEDROOM_WINDOW_01);
}

void close_gpio(){
    bcm2835_close();
}
