#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<unistd.h>

#define MAXBUFLEN 10000

#define PKT_SIZE 128
#define PORT 8882
#define TIMEOUT 2

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

void die(char *s){
	perror(s);
	exit(1);
}

int main(){
	struct sockaddr_in si_server;
	fd_set readfds,masterfds;    	// To implement Timeout
	int s, i , slen = sizeof(si_server),Window_Size,BASE = 0,ret;
	DATA_PKT send_pkt;
	ACK_PKT rcv_ack;
	FD_ZERO(&masterfds);
	FD_SET(s, &masterfds);
	memcpy(&readfds,&masterfds,sizeof(fd_set));
	if( (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");
	memset((char *)&si_server, 0 , sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(PORT);
	si_server.sin_addr.s_addr = inet_addr("127.0.0.1");
	printf("Enter Window Size: ");
	scanf("%d",&Window_Size);

	if( sendto(s,&Window_Size, sizeof(int), 0, (struct sockaddr *) &si_server,slen) == -1){
		die("send to()");
	}		
	char source[MAXBUFLEN + 1];   				//Buffer to send the data
	int N;
	//Load Buffer with File Data
	FILE *fp = fopen("in.txt", "r");
	if (fp != NULL){
		size_t newLen = fread(source, sizeof(char), MAXBUFLEN, fp);
		if ( ferror( fp ) != 0 ){
			fputs("Error reading file", stderr);
		}else{
			source[newLen++] = '\0';
		}
		printf("Message Size: %d\n",(int)newLen);
		fclose(fp);
		N = (newLen / PKT_SIZE) + 1; 			// Calculating number of Packets
	}
	printf("Number of Packet: %d\n",N);
	if( sendto(s,&N, sizeof(int), 0, (struct sockaddr *) &si_server,slen) == -1){
		die("send to()");
	}
	//Array to mark the Acknowledgement of Packets
	int *send = calloc(N,sizeof(int)), state = 0, *fresh = calloc(N,sizeof(int));
	for(int i = 0 ; i < N; i++)
		fresh[i] = 1;
	while(1){
		if(BASE == N) break;
		switch(state){

		case 0: /* Packet Sending State */
			for(int i = BASE ; i < BASE + Window_Size && i < N; i++){
				send_pkt.packet_no = i;
				strncpy(send_pkt.data,source+(PKT_SIZE * i),PKT_SIZE);  //Extract the chunk of string from buffer to send
				send_pkt.packet_size = sizeof(send_pkt.data);
				if(send[i] == 0 && fresh[i] == 1){			//Send only new packets
					if( sendto(s,&send_pkt, sizeof(send_pkt), 0, (struct sockaddr *) &si_server,slen) == -1){
						die("send to()");
					}else{
						fresh[i]--;				//Mark as not a new packet
						printf("SEND PACKET %d : BASE %d\n",send_pkt.packet_no,BASE);
					}
				}
			}
			state = 1;
			break;

		case 1:	; /* Acknowledgement Receiving and Timeout State */
			struct timeval tv;
			tv.tv_sec = TIMEOUT;
			tv.tv_usec = 0;
			FD_ZERO(&masterfds);
			FD_SET(s, &masterfds);
			memcpy(&readfds,&masterfds,sizeof(fd_set));
			if(ret = select(s+1, &readfds,NULL, NULL,&tv) < 0)
				die("select()");

			/* Acknowledgement can be received */
			if(FD_ISSET(s, &readfds)){		
				if ((recvfrom(s, &rcv_ack, sizeof(rcv_ack), 0, (struct sockaddr *) &si_server, &slen)) == -1){
					die("recvfrom()");
				}else{
					send[rcv_ack.ack_no] = 1;
					if(rcv_ack.ack_no == BASE){
						while(send[BASE] == 1 && BASE < N)
							BASE++;
						state = 0;
					}
					printf("RECEIVE ACK %d : BASE %d\n",rcv_ack.ack_no,BASE);
				}
			}
			/* Timeout Occured */
			else{					
				for(int i = BASE ; i < BASE + Window_Size && i < N; i++){
					send_pkt.packet_no = i;
					strncpy(send_pkt.data,source+(PKT_SIZE * i),PKT_SIZE);
					//Resending The Packet Not Acknowledged
					if(send[i] == 0){	
						if( sendto(s,&send_pkt, sizeof(send_pkt), 0, (struct sockaddr *) &si_server,slen) == -1){
							die("send to()");
						}else{
							printf("TIMEOUT %d\n",i);
							printf("SEND PACKET %d : BASE %d\n",send_pkt.packet_no,BASE);
						}
				}
			}
				
			}
		}
	}
	close(s);	
}
