// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>
#include <bcm2835.h>

#define SENSOR_ABERTURA_03 RPI_V2_GPIO_P1_32

pthread_mutex_t mutex;
pthread_cond_t condition;
unsigned int run = 0;
int execute = 1;

void *detect_event(){
    pthread_mutex_lock(&mutex);
    bcm2835_gpio_set_eds(SENSOR_ABERTURA_03);
    while(!run){
        pthread_cond_wait(&condition, &mutex);
	if(bcm2835_gpio_lev(SENSOR_ABERTURA_03)){
	    printf("SENSOR ATIVADO!\n");
	} else {
	    printf("SENSOR NÃO ATIVADO\n");
	}
	fflush(stdout);
        run = 0;
    }
    pthread_mutex_unlock(&mutex);
}

void initialize_gpio(){
    if (!bcm2835_init())
        return;

    bcm2835_gpio_fsel(SENSOR_ABERTURA_03, BCM2835_GPIO_FSEL_INPT);
}

void close_gpio(){
    bcm2835_close();
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
    alarm(2);
}

int main(int argc, char const *argv[]) { 

    initialize_gpio();

    signal(SIGALRM, sig_handler);
    signal(SIGINT, &exit_program);
    alarm(2);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, detect_event, NULL);

    while(execute){sleep(1);}

    pthread_join(&thread_id, NULL);
    close_gpio();
	return 0; 
} 
