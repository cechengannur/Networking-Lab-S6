#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define S_PORT 43454
#define C_PORT 43455
#define ERROR -1
#define IP_STR "127.0.0.1"

int main(int argc, char const *argv[]) {
	int sfd;
	int num = 1;
	time_t start_time, rtt, current_time;
	struct sockaddr_in servaddr, clientaddr;
	socklen_t addrlen;
	sfd = socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
	if (sfd == ERROR) {
		perror("Could not open a socket");
		return 1;
	}
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(IP_STR);
	servaddr.sin_port=htons(S_PORT);

	memset((char *) &clientaddr, 0, sizeof(clientaddr));
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_addr.s_addr=inet_addr(IP_STR);
	clientaddr.sin_port=htons(C_PORT);

	if((bind(sfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr)))!=0) {
		perror("Could not bind socket");
		return 2;
	}

	printf("Client is running on %s:%d\n", IP_STR, C_PORT);
	start_time = time(NULL);
	sendto(sfd, &num, sizeof(num), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	addrlen = sizeof(clientaddr);
	recvfrom(sfd, &current_time, sizeof(current_time), 0, (struct sockaddr *)&clientaddr, &addrlen);
	rtt = time(NULL) - start_time;
	current_time += rtt / 2;
	printf("Server's Time: %s\n", ctime(&current_time));

	return 0;
}