// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>
#include <bcm2835.h>

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
};

void initialize_gpio(){
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

void close_gpio(){
    bcm2835_close();
}
