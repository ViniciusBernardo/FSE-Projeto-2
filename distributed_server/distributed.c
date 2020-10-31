// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h>
#include <pthread.h>

#define PORT 8042 

pthread_mutex_t mutex;
pthread_cond_t condition;
unsigned int run = 0;
int execute = 1;

void *send_temperature(void *param){
    pthread_mutex_lock(&mutex);
    int valread;
    int * sock = (int *)param;
    char buffer[1024] = {0};
    char *hello = "Temperature 50 °C"; 
    while(!run){
        pthread_cond_wait(&condition, &mutex);
        printf("SENDING: %s\n", hello);
        send(*sock , hello , strlen(hello) , 0 ); 
        valread = read(*sock , buffer, 1024);
        printf("%s\n",buffer );
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
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

    signal(SIGALRM, sig_handler);
    signal(SIGINT, &exit_program);
    alarm(1);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, send_temperature, (void *)&sock);

    while(execute){sleep(1);}

    pthread_join(&thread_id, NULL);
	return 0; 
} 
