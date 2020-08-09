#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
#define MAX_CLNT 20

void errorHandling(char *msg);

int main(int argc, char *argv[]) {
	struct sockaddr_in serv_addr, clnt_addr;
	struct timeval timeout;
	int serv_sock, clnt_sock;
	fd_set fds, cpy_fds;
	
	int clnt_socks[MAX_CLNT];
	int clnt_cnt = 0;

	char buf[BUF_SIZE];
	int fd_max, str_len, fd_num;
	socklen_t addr_size;
	
	//input error
	if(argc != 2) {
		printf("Wrong input. Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	//create socket
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	//error
	if (bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1) {
		errorHandling("binding error occurred");
	}
	if (listen(serv_sock, 5) == -1) {
		errorHandling("listening error occurred");
	}

	//set up io multiplexing
	FD_ZERO(&fds);
	//serv_sock is changed if client call connect function
	FD_SET(serv_sock, &fds);
	fd_max = serv_sock;

	while(1) {
		cpy_fds = fds;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		//error case
		if((fd_num = select(fd_max + 1, &cpy_fds, 0, 0, &timeout)) == -1){
			break;
		}
		//no change
		if(fd_num == 0) {
			continue;
		}
		
		//check every fd
		for(int i = 0; i < fd_max + 1; i++) {
			if (!FD_ISSET(i, &cpy_fds)) {
				continue;
			}

			// connection request
			if(serv_sock == i) {		
				addr_size = sizeof(clnt_addr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
					
				clnt_socks[clnt_cnt++] = clnt_sock;
				FD_SET(clnt_sock, &fds);
				if(fd_max < clnt_sock)
					fd_max = clnt_sock;
				printf("connected client: %d\n", clnt_sock);
			}
			// read message
			else {
				str_len = read(i, buf, BUF_SIZE);
				// close request
				if(str_len == 0) {
					FD_CLR(i, &fds);
					close(i);
					printf("closed client: %d\n", i);
				}
				//send message to clients
				else {  
					for(int j = 0; j < clnt_cnt; j++) {
						write(clnt_socks[j], buf, str_len);
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

//error handling
void errorHandling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}