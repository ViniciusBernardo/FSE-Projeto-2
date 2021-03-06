// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "gpio.h"
#include "sensor_bme280.h"
#include "utils.h"

#define CENTRAL_HOST "192.168.0.53"
#define CENTRAL_PORT 10009
#define DISTRIBUTED_PORT 10109

pthread_mutex_t mutex;
pthread_mutex_t mutex_bme280;
pthread_mutex_t mutex_input;
pthread_cond_t condition;
pthread_cond_t condition_bme280;
pthread_cond_t condition_input;
unsigned int run = 0;
unsigned int run_bme280 = 0;
unsigned int run_input = 0;
unsigned int n_executions = 0;
int execute = 1;

void *send_info(void *param){
    pthread_mutex_lock(&mutex);
    struct system * system_state = (struct system *)param;
    char buffer[1024] = {0};
    while(!run){
        pthread_cond_wait(&condition, &mutex);
        char *json = malloc(420*sizeof(char));
        format_json(json, system_state);
        send(system_state->sock , json , strlen(json) , 0 );
        run = 0;
    }
    pthread_mutex_unlock(&mutex);
}

void *read_bme280_sensor(void *param){
    pthread_mutex_lock(&mutex_bme280);
    struct bme280_system * measurements = (struct bme280_system *)param;
    while(!run_bme280){
        pthread_cond_wait(&condition_bme280, &mutex_bme280);
        get_sensor_data(measurements);
        run_bme280 = 0;
    }
    pthread_mutex_unlock(&mutex_bme280);
}

void *read_input_sensors(void *param){
    pthread_mutex_lock(&mutex_input);
    int last_activate_alarm = 0;
    struct system * system_state = (struct system *)param;
    while(!run_input){
        pthread_cond_wait(&condition_input, &mutex_input);
        set_input_sensors(&system_state->gpio_input);
        system_state->gpio_input.activate_alarm = check_activate_alarm(&system_state->gpio_input);
        char *json = malloc(420*sizeof(char));
        if(system_state->gpio_input.activate_alarm && system_state->gpio_input.activate_alarm != last_activate_alarm){
            /* send to central server message to turn on alarm */
            format_json(json, system_state);
            send(system_state->sock , json , strlen(json) , 0 );
            last_activate_alarm = system_state->gpio_input.activate_alarm;
        } else if(!system_state->gpio_input.activate_alarm && system_state->gpio_input.activate_alarm != last_activate_alarm){
            /* send to central server message to turn off alarm */
            format_json(json, system_state);
            send(system_state->sock , json , strlen(json) , 0 );
            last_activate_alarm = system_state->gpio_input.activate_alarm;
        }
        run_input = 0;
    }
    pthread_mutex_unlock(&mutex_input);
}

void *receive_commands(void *params){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( DISTRIBUTED_PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    struct system * system_state = (struct system *)params;

    while(1){
        valread = read(new_socket, buffer, 1024);
	if(strcmp(buffer, "quit") == 0){
		execute = 0;
	} else if(sscanf(buffer, "%f", &system_state->desired_temperature) > 0){
	    printf("Temperatura Atualizada para %.2f!\n", system_state->desired_temperature);
	} else {
            set_output_devices(&system_state->gpio_output, buffer);
	}
        memset(&buffer[0], 0, sizeof(buffer));
    }
}

void exit_program(int signal){
    execute = 0;
};

void sig_handler(int signum){
	n_executions++;

    // check security sensors every 200ms
    pthread_mutex_lock(&mutex_input);
    if(run_input == 0){
        run_input = 1;
        pthread_cond_signal(&condition_input);
    }
    pthread_mutex_unlock(&mutex_input);

    // send system state to central server every 1s
    if(n_executions == 5){
        pthread_mutex_lock(&mutex);
        if(run == 0){
            run = 1;
            pthread_cond_signal(&condition);
        }
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex_bme280);
        if(run_bme280 == 0){
            run_bme280 = 1;
            pthread_cond_signal(&condition_bme280);
        }
        pthread_mutex_unlock(&mutex_bme280);
	n_executions = 0;
    }
    ualarm(2e5, 2e5);
}

int main(int argc, char const *argv[]) { 
	struct sockaddr_in serv_addr;
	struct system system_state;

	if ((system_state.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(CENTRAL_PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, CENTRAL_HOST, &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(system_state.sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	system_state.bme280_sensor.sensor_bme280 = create_sensor("/dev/i2c-1");
	system_state.bme280_sensor.temperature = 22.15;
	system_state.bme280_sensor.humidity = 50.0;
	system_state.desired_temperature = 22.15;

    initialize_gpio(&system_state.gpio_output);

    signal(SIGALRM, sig_handler);
    signal(SIGINT, &exit_program);
    ualarm(2e5, 2e5);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_bme280, NULL);
    pthread_mutex_init(&mutex_input, NULL);
    pthread_cond_init(&condition, NULL);
    pthread_cond_init(&condition_bme280, NULL);
    pthread_cond_init(&condition_input, NULL);

    pthread_t thread_id[4];
    pthread_create(&thread_id[0], NULL, send_info, (void *)&system_state);
    pthread_create(&thread_id[1], NULL, receive_commands, (void *)&system_state);
    pthread_create(&thread_id[2], NULL, read_bme280_sensor, (void *)&system_state.bme280_sensor);
    pthread_create(&thread_id[3], NULL, read_input_sensors, (void *)&system_state);

    while(execute){sleep(1);}

    close_gpio();
    pthread_join(&thread_id[0], NULL);
    pthread_join(&thread_id[1], NULL);
    pthread_join(&thread_id[2], NULL);
    pthread_join(&thread_id[3], NULL);
	return 0; 
} 
