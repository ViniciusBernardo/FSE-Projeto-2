// Client side C/C++ program to demonstrate Socket programming 
#include "bme280/sensor_bme280.c"
#include "gpio.c"
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>

#define HOST "192.168.0.53"
#define PORT 8042 

pthread_mutex_t mutex;
pthread_mutex_t mutex_bme280;
pthread_mutex_t mutex_input;
pthread_cond_t condition;
pthread_cond_t condition_bme280;
pthread_cond_t condition_input;
unsigned int run = 0;
unsigned int run_bme280 = 0;
unsigned int run_input = 0;
int execute = 1;

struct communication {
    int sock;
    struct external_measurement bme280_sensor;
    struct input_sensors gpio_input;
};

void *send_info(void *param){
    pthread_mutex_lock(&mutex);
    struct communication * conn_obj = (struct communication *)param;
    char buffer[1024] = {0};
    while(!run){
        pthread_cond_wait(&condition, &mutex);
        char *json = malloc(300*sizeof(char));
        sprintf(
            json,
            "{"
            "   \"temperature\": %.2f,"
            "   \"living_room\": %d,"
            "   \"kitchen\": %d,"
            "   \"kitchen_door\": %d,"
            "   \"kitchen_window\": %d,"
            "   \"living_room_door\": %d,"
            "   \"living_room_window\": %d,"
            "   \"bedroom_window_01\": %d,"
            "   \"bedroom_window_02\": %d"
            "}",
            conn_obj->bme280_sensor.temperature,
            conn_obj->gpio_input.living_room,
            conn_obj->gpio_input.kitchen,
            conn_obj->gpio_input.kitchen_door,
            conn_obj->gpio_input.kitchen_window,
            conn_obj->gpio_input.living_room_door,
            conn_obj->gpio_input.living_room_window,
            conn_obj->gpio_input.bedroom_window_01,
            conn_obj->gpio_input.bedroom_window_02
        );
        printf("SENDING: %s\n", json);
        send(conn_obj->sock , json , strlen(json) , 0 );
        run = 0;
    }
    pthread_mutex_unlock(&mutex);
}

void *read_bme280_sensor(void *param){
    pthread_mutex_lock(&mutex_bme280);
    struct external_measurement * measurements = (struct external_measurement *)param;
    while(!run_bme280){
        pthread_cond_wait(&condition_bme280, &mutex_bme280);
        measurements->temperature = get_external_temperature(measurements->sensor_bme280);
        run_bme280 = 0;
    }
    pthread_mutex_unlock(&mutex_bme280);
}

void *read_input_sensors(void *param){
    pthread_mutex_lock(&mutex_input);
    struct input_sensors * security_sensors = (struct input_sensors *)param;
    while(!run_input){
        pthread_cond_wait(&condition_input, &mutex_input);
        set_input_sensors(security_sensors);
        run_input = 0;
    }
    pthread_mutex_unlock(&mutex_input);
}

void exit_program(int signal){
    execute = 0;
};

void sig_handler(int signum){
    pthread_mutex_lock(&mutex_bme280);
    if(run_bme280 == 0){
        run_bme280 = 1;
        pthread_cond_signal(&condition_bme280);
    }
    pthread_mutex_unlock(&mutex_bme280);

    pthread_mutex_lock(&mutex_input);
    if(run_input == 0){
        run_input = 1;
        pthread_cond_signal(&condition_input);
    }
    pthread_mutex_unlock(&mutex_input);

    pthread_mutex_lock(&mutex);
    if(run == 0){
        run = 1;
        pthread_cond_signal(&condition);
    }
    pthread_mutex_unlock(&mutex);
    alarm(1);
}

int main(int argc, char const *argv[]) { 
	int sock = 0; 
	struct sockaddr_in serv_addr; 
	struct communication conn_obj;
	conn_obj.bme280_sensor.sensor_bme280 = create_sensor("/dev/i2c-1");
	conn_obj.bme280_sensor.temperature = 22.15;
	if ((conn_obj.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, HOST, &serv_addr.sin_addr)<=0) { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(conn_obj.sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

    initialize_gpio();

    signal(SIGALRM, sig_handler);
    signal(SIGINT, &exit_program);
    alarm(1);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_bme280, NULL);
    pthread_mutex_init(&mutex_input, NULL);
    pthread_cond_init(&condition, NULL);
    pthread_cond_init(&condition_bme280, NULL);
    pthread_cond_init(&condition_input, NULL);

    pthread_t thread_id[3];
    pthread_create(&thread_id[0], NULL, send_info, (void *)&conn_obj);
    pthread_create(&thread_id[1], NULL, read_bme280_sensor, (void *)&conn_obj.bme280_sensor);
    pthread_create(&thread_id[2], NULL, read_input_sensors, (void *)&conn_obj.gpio_input);

    while(execute){sleep(1);}

    close_gpio();
    pthread_join(&thread_id[0], NULL);
    pthread_join(&thread_id[1], NULL);
    pthread_join(&thread_id[2], NULL);
	return 0; 
} 
