#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int frame_kind; //ACK:0, SEQ:1 FIN:2
    int sq_no;
    int ack;
    Packet packet;
}Frame;

int main(int argc, char** argv){

	if (argc != 2){
		printf("Usage: %s <port>", argv[0]);
		exit(0);	
	}

	int port = atoi(argv[1]);
	int sockfd;
	struct sockaddr_in serverAddr, newAddr;
	char buffer[1024];
	socklen_t addr_size;

	int frame_id=0;
	Frame frame_recv;
	Frame frame_send;	

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	addr_size = sizeof(newAddr);

	while(1){
		int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&newAddr, &addr_size);
		if (f_recv_size > 0 && frame_recv.frame_kind == 1 && frame_recv.sq_no == frame_id){
			printf("[+]Frame Received: %s\n", frame_recv.packet.data);
			
			frame_send.sq_no = 0;
			frame_send.frame_kind = 0;
			frame_send.ack = frame_recv.sq_no + 1;
			sendto(sockfd, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&newAddr, addr_size);
			printf("[+]Ack Send\n");
		}else{
			printf("[+]Frame Not Received\n");
		}
		frame_id++;	
	}
	
	close(sockfd);
	return 0;
}
