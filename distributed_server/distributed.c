// Client side C/C++ program to demonstrate Socket programming 
#include "bme280/sensor_bme280.c"
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
pthread_cond_t condition;
unsigned int run = 0;
int execute = 1;

struct communication {
    int sock;
    struct bme280_dev * sensor_bme280;
};

void *send_temperature(void *param){
    pthread_mutex_lock(&mutex);
    struct communication * conn_obj = (struct communication *)param;
    float temperature;
    int valread;
    char buffer[1024] = {0};
    while(!run){
        pthread_cond_wait(&condition, &mutex);
        char *temp = malloc(5*sizeof(char));
        temperature = get_external_temperature(conn_obj->sensor_bme280);
	sprintf(temp, "%.2f", temperature);
        printf("SENDING: %s\n", temp);
        send(conn_obj->sock , temp , strlen(temp) , 0 ); 
        run = 0;
    }
    pthread_mutex_unlock(&mutex);
}

void exit_program(int signal){
    execute = 0;
};

void sig_handler(int signum){
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
	conn_obj.sensor_bme280 = create_sensor("/dev/i2c-1");
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

    signal(SIGALRM, sig_handler);
    signal(SIGINT, &exit_program);
    alarm(1);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, send_temperature, (void *)&conn_obj);

    while(execute){sleep(1);}

    pthread_join(&thread_id, NULL);
	return 0; 
} 
