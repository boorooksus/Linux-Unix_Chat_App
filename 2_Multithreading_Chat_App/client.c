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
	
	write(sock, name, strlen(name));

	while(1) {
		fgets(msg, BUF_SIZE, stdin);
		//exit
		if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")) {
			close(sock);
			exit(0);
		}
		
		int isPos = 0;
		for(int i = 0; i < BUF_SIZE; i++){
			if(msg[i] == '@'){
				isPos = 1;
				break;
			}
		}
		if(isPos == 0){
			printf("Target user not found!\n");
			continue;
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
		
		char cur_name[NAME_SIZE];
		int clnt_name_size = 0;
		int idx = 0;
		
		//get receiver's name
		while(idx < BUF_SIZE){
			if(name_msg[idx] == '@'){
				idx++;
				break;
			}
			idx++;
		}
		for(int j = 0; name_msg[idx + j] != 32; j++){
			cur_name[j] = name_msg[idx + j];
			clnt_name_size++;
		}
		
		//check if this user is receiver
		int isPos = 1;
		for(int j = 0; j < clnt_name_size; j++){
			if(name[j + 1] != cur_name[j]){
				isPos = 0;
				break;
			}
		}
		if(cur_name[0] == 'a' && cur_name[1] == 'l' && cur_name[2] == 'l'){
			isPos = 1;
		}
		
		//print message if this user is rigth receiver
		if(isPos == 1){
			name_msg[str_len] = 0;
			
			char fnl_msg[BUF_SIZE + NAME_SIZE];
			int cur = 0;
			
			//get sender's name and message
			for(int j = 0; j < str_len;j++){
				if(name_msg[j] == '@'){
					while(name_msg[j] != 32){
						j++;
					}
					j++;
				}
				fnl_msg[cur++] = name_msg[j];
			}
			fnl_msg[cur] = 0;
			//print
			fputs(fnl_msg, stdout);
		}
	}

	return NULL;
}

//error handling
void errorHandling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
