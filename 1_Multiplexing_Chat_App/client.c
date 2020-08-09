#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <pthread.h>
	
#define BUF_SIZE 100
#define NAME_SIZE 20

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];
	
void * sendMsg(void * arg);
void * rcvMsg(void * arg);
void errorHandling(char * msg);
	
	
int main(int argc, char *argv[]) {
	pthread_t snd_t, rcv_t;
	struct sockaddr_in serv_addr;
	void * return_t;
	int sock;

	//input error
	if (argc != 4) {
		printf("Wrong input. Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	 }
	
	sprintf(name, "[%s]", argv[3]);
	//create socket
	sock=socket(PF_INET, SOCK_STREAM, 0);	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	//error
	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1) {
		errorHandling("connecting error occurred");
	}
	
	//create threads
	pthread_create(&snd_t, NULL, sendMsg, (void*)&sock);
	pthread_create(&rcv_t, NULL, rcvMsg, (void*)&sock);
	pthread_join(snd_t, &return_t);
	pthread_join(rcv_t, &return_t);

	close(sock);  
	return 0;
}
	
// sending thread
void * sendMsg(void * arg) {
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];

	while(1) {
		fgets(msg, BUF_SIZE, stdin);
		//exit
		if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")) {
			close(sock);
			exit(0);
		}

		sprintf(name_msg,"%s %s", name, msg);
		//send message to server
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

//reading thread
void* rcvMsg(void * arg) {
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;

	while(1) {
		//get message from server
		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);

		if (str_len == -1) {
			return (void*)-1;
		}

		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}

	return NULL;
}

//error handling
void errorHandling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
