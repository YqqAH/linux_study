//chat_client.c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define NAME_SIZE 20
#define MSG_SIZE 100

char name[NAME_SIZE];
char msg[MSG_SIZE];

int exitFlag;

char SERVER_IP[20];
char SERVER_PORT[6];

int client_sock;

void interrupt(int arg) {
	printf("\nYou typped Ctrl + C\n");
	printf("Bye\n");

	close(client_sock);
	exit(1);
}

int main() {
	char comm[100] = { 0 };
	char send_buf[NAME_SIZE + MSG_SIZE + 1];
	char recv_buf[NAME_SIZE + MSG_SIZE];
	signal(SIGINT, interrupt);

	while (1) {
		printf("ssafy >> ");
		memset(comm, 0, sizeof(comm));
		gets(comm);

		if (strcmp(comm, "close") == 0) {
			printf("INFO :: Disconnected with Server\n");
			write(client_sock, comm, strlen(comm));
			close(client_sock);
			client_sock = 0;
			continue;
		}
		else if (strcmp(comm, "exit") == 0) {
			printf("INFO :: Good Bye\n");
			write(client_sock, comm, strlen(comm));
			close(client_sock);
			client_sock = 0;
			break;
		}

		char* p = strtok(comm, " ");
		if (strcmp(p, "connect") == 0) {
			if (client_sock) {
				printf("ERROR :: 3_Already Connected with Server\n");
				continue;
			}

			p = strtok(NULL, " ");
			strcpy(SERVER_IP, p);
			p = strtok(NULL, " ");
			strcpy(SERVER_PORT, p);

			client_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (client_sock == -1) {
				printf("ERROR :: 1_Socket Create Error\n");
				exit(1);
			}
			//printf("Socket Create!\n");

			struct sockaddr_in server_addr = { 0 };
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
			server_addr.sin_port = htons(atoi(SERVER_PORT));
			socklen_t server_addr_len = sizeof(server_addr);

			if (connect(client_sock, (struct sockaddr*)&server_addr, server_addr_len) == -1) {
				printf("ERROR :: 2_Connect Error\n");
				exit(1);
			}
			//printf("Connect Success!\n");

			memset(recv_buf, 0, NAME_SIZE + MSG_SIZE);
			int len = read(client_sock, recv_buf, NAME_SIZE + MSG_SIZE - 1);
			printf("%s\n", recv_buf);
		}
		else {
			if (!client_sock) {
				printf("ERROR :: 4_No Connected Server\n");
				continue;
			}

			memset(send_buf, 0, NAME_SIZE + MSG_SIZE);
			strcat(send_buf, p);
			strcat(send_buf, " ");
			p = strtok(NULL, "\n");
			if (p != NULL)strcat(send_buf, p);
			//sprintf(send_buf, "%s", p);
			write(client_sock, send_buf, strlen(send_buf));

			memset(recv_buf, 0, NAME_SIZE + MSG_SIZE);
			int len = read(client_sock, recv_buf, NAME_SIZE + MSG_SIZE - 1);
			if (len == 0) {
				printf("INFO :: Server Disconnected\n");
				kill(0, SIGINT);
				exitFlag = 1;
				break;
			}
			printf("%s\n", recv_buf);
		}
	}

	return 0;
}