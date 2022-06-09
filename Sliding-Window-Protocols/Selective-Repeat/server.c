/*
!important
First compile server.c by gcc server.c -o server
run ./server
Compile client.c by gcc client.c -o client
run ./client
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<time.h>
#include<unistd.h>

#define MAXBUFLEN 10000
#define PKT_SIZE 128
#define PORT 8882
#define PDR 0.2

void die(char *s)
{
	perror(s);
	exit(1);
}
//Acknowledgement Packet
typedef struct packet1{
	int ack_no;
}ACK_PKT;

//Data Packet
typedef struct packet2{
	int packet_no;
	int packet_size;
	char data[PKT_SIZE];
}DATA_PKT;

int main()
{
	struct sockaddr_in si_me, si_client;
	int s, i, slen = sizeof(si_client) , recv_len, Window_Size,BASE = 0,N;
	int mode = 0;
	printf("Select Mode: \n");
	printf("0 : Loss Less Scenario.\n");
	printf("1 : Packet Loss Scenario.\n");
	scanf("%d",&mode);
	char dest[MAXBUFLEN];				//Buffer for Received Packets
	DATA_PKT rcv_pkt;
	ACK_PKT ack_pkt;
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		die("socket");
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
 
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1){
		die("bind");
	}
	if ((recv_len = recvfrom(s, &Window_Size, sizeof(int), 0, (struct sockaddr *) &si_client, &slen)) == -1){
		die("recvfrom()");
	}
	else{
		printf("Received Window Size: %d\n",Window_Size);
	}
	if ((recv_len = recvfrom(s, &N, sizeof(int), 0, (struct sockaddr *) &si_client, &slen)) == -1){
		die("recvfrom()");
	}
	else{
		printf("Number of packets: %d\n",N);
	}
	int count = 0;

	/* Array to mark the received packets */
	int *rcvd = calloc(N,sizeof(int));
	while(1){
		if(BASE == N) break;
		for(int i = BASE ; i < BASE + Window_Size && i < N ; i++){
			if(rcvd[i] == 0 ){
				if ((recv_len = recvfrom(s, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *) &si_client, &slen)) == -1){
					die("recvfrom()");
				}else{	
					srand48(time(NULL)+i);
					if(mode == 1)				// To Decide the mode of implementation
						if(drand48() < PDR){	// Drop packet with random value below PDR
							printf("RECEIVE PACKET %d : DROP : BASE %d\n",rcv_pkt.packet_no,BASE);
							continue;	
						}				
					//if(rcv_pkt.packet_no == 3 && count == 0){if(rcv_pkt.packet_no == 3) count++;continue;}
					//if(rcv_pkt.packet_no == 11 && count == 1){if(rcv_pkt.packet_no == 11) count++;continue;}
					printf("RECEIVE PACKET %d : ACCEPT : BASE %d\n",rcv_pkt.packet_no,BASE);

					//Store the packet data in buffer.
					for(int j = 0 ; j < PKT_SIZE; j++){
						int position = i * PKT_SIZE + j;
						dest[position] = rcv_pkt.data[j];
					}
					ack_pkt.ack_no = rcv_pkt.packet_no;
					rcvd[rcv_pkt.packet_no] = 1;
					sendto(s, &ack_pkt, sizeof(ack_pkt), 0, (struct sockaddr *) &si_client, slen);
					printf("SEND ACK %d\n",ack_pkt.ack_no);
					//printf("%s %d\n",rcv_pkt.data,rcv_pkt.packet_no);
					if(rcv_pkt.packet_no == BASE){
						while(rcvd[BASE] == 1 && BASE < N)
							BASE++;			//Sliding the Window
					}
					if(BASE == N)
						break;
				}
			}
		}
	}
	//Buffer is stored in the file
	FILE *file = fopen("out.txt","w");
	int result = fputs(dest,file);
	if(result == EOF)
		die("fputs()");
	fclose(file);
	close(s);
	return 0;
}
