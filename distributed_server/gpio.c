// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>
#include <bcm2835.h>

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

    bcm2835_gpio_fsel(LAMP_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_03, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_04, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AC_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AC_02, BCM2835_GPIO_FSEL_OUTP);
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
    bcm2835_delay(50);
    switch(command) {
        case "lamp_01" :
            bcm2835_gpio_write(LAMP_01, !devices->lamp_01);
            devices->lamp_01 = !devices->lamp_01;
            break;
        case "lamp_02" :
            bcm2835_gpio_write(LAMP_02, !devices->lamp_02);
            devices->lamp_02 = !devices->lamp_02;
            break;
        case "lamp_03" :
            bcm2835_gpio_write(LAMP_03, !devices->lamp_03);
            devices->lamp_03 = !devices->lamp_03;
            break;
        case "lamp_04" :
            bcm2835_gpio_write(LAMP_04, !devices->lamp_04);
            devices->lamp_04 = !devices->lamp_04;
            break;
        case "ac_01" :
            bcm2835_gpio_write(AC_01, !devices->ac_01);
            devices->ac_01 = !devices->ac_01;
            break;
        case "ac_02" :
            bcm2835_gpio_write(AC_02, !devices->ac_02);
            devices->ac_02 = !devices->ac_02;
            break;
        default :
            printf("Invalid command: %s\n", command);
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
    bcm2835_close();
}
