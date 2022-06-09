/*
!important
First compile server.c by gcc server.c -lm -o server
run ./server 5400 in cmd
Compile client.c by gcc client.c -o client
run ./client 127.0.0.1 5400
enter send.txt when enter file name to recieve is prompted
*/



// client code for UDP socket programming 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define IP_PROTOCOL 0 

#define LEN 512 
#define KEY 'H'
#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define FAILURE 1

#define RRQ     1
#define DATA    2
#define ACK     3
#define ERROR   4

#define TYPE_INDEX          0
#define WINDOW_SIZE_INDEX   1
#define SEQ_NUM_INDEX       1
#define FILE_NAME_INDEX     2

#define RED     printf("\033[0;31m");
#define GREEN   printf("\033[0;32m");
#define DEFAULT printf("\033[0m"); 

// please first look at server's code.

int getType(char packet[LEN]){
    return packet[TYPE_INDEX]-'0';
}
int getWindowSize(char packet[LEN]){
    return packet[WINDOW_SIZE_INDEX]-'0';
}
char getSeqNum(char packet[LEN]){
    return packet[SEQ_NUM_INDEX];
}
char* getFileName(char packet[LEN]){
    char* file_name = malloc(20);
    for(int i=0;i<20;i++){
        file_name[i] = packet[FILE_NAME_INDEX + i];
    }
    file_name[19]='\0';
    printf("File in func %s\n", file_name );
    return file_name;
}
void flush_packet(char packet[LEN]){
    int i = 0;
    for(i=0;i<LEN;i++){
        packet[i]='\0';
    }
}
void show_data(char packet[LEN]){
    for(int i=2;i<LEN;i++){
        printf("%c", packet[i]);
    }
    return;
}
// to send ack to server of given seq_num
int send_ack(int sockfd, struct sockaddr_in my_addr, char seq_num){
    char packet[LEN];
    flush_packet(packet);
    // '3' = ACK
    packet[0] = '3';//packet type
    packet[1] = seq_num;
    int len = sizeof(my_addr);
    int sendBits = sendto(sockfd, packet, LEN, 0, (struct sockaddr*)&my_addr, len);
    if(sendBits<=0){
        RED printf("Error while writing...\n"); DEFAULT
        return FAILURE;
    }
    return SUCCESS;
}

void receive_file_go_back_n(int sockfd, struct sockaddr_in my_addr){
    int addrlen = sizeof(my_addr);
    int req_seq_num = 0;
    while(TRUE){
        char packet[LEN];
        int read_bits = recvfrom(sockfd, packet, LEN, 0, (struct sockaddr*)&my_addr, &addrlen); 
        if(read_bits<=0){
            printf("Error in reading...\n");
            return;
        }   
        int type    =   getType(packet);
        if(type == ERROR){
            printf("File Not Found !!!\n");
            return;
        }else if(type != DATA){
            continue;
        }
        int seq_num =   getSeqNum(packet);

        if(seq_num == req_seq_num){
            GREEN printf("Recieved Packet of seq_num : %d\n", seq_num); DEFAULT
            printf("Sending ACK of seq_num : %d\n", req_seq_num);

            if(send_ack(sockfd, my_addr, req_seq_num)==FAILURE){
                RED printf("ACK was not sent successfully...\n"); DEFAULT
            }else{
                printf("ACK Sent Successfully\n");
            }
            req_seq_num++;
        }else{
            RED printf("Recieved Packet of seq_num : %d\n But Required was %d\n", seq_num, req_seq_num); DEFAULT
            
        }
    }
}
void sendRequest(int sockfd, struct sockaddr_in my_addr, char file_name[20]){
    // type + window_size + name 
    //   1   +      1      + 20   = 22
    char packet[22];
    int window_size = 1;
    packet[0]='1';
    packet[1]=window_size;
    int i;
    for(i=2;i<22;i++){
        packet[i] = file_name[i-2];
    }
    int addrlen = sizeof(my_addr);
    int sendBits = sendto(sockfd, packet, 22, 0, (struct sockaddr*)&my_addr, addrlen); 
    if(sendBits <= 0){
        RED printf("Error in sending....\n"); DEFAULT
    }
}

// driver code 
int main(int argc, char *argv[]) 
{   
	if(argc != 3){
		fprintf(stderr,"Usage: %s hostname port\n", argv[0]); //Validating cmd arguments
	}
    // ######################### Initialization #########################
	int PORT_NO = atoi(argv[2]);
	char IP_ADDRESS[50];
	strcpy(IP_ADDRESS, argv[1]);

	struct sockaddr_in my_addr; 
	int addrlen = sizeof(my_addr); 
	my_addr.sin_family = AF_INET; 
	my_addr.sin_port = htons(PORT_NO); 
	my_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
	// ##################################################################
    
    // ###################### Create Socket #############################
	int sockfd = socket(AF_INET, SOCK_DGRAM, 
					IP_PROTOCOL); 
	if (sockfd < 0) 
		printf("\nfile descriptor not received!!\n"); 
	else
		printf("\nfile descriptor %d received\n", sockfd); 
    // ##################################################################


	while (1) { 
        char file_name[20];
		printf("\nPlease enter file name to receive:\n"); 
		scanf("%s", file_name); 
        sendRequest(sockfd, my_addr, file_name);

        receive_file_go_back_n(sockfd, my_addr);
	} 
	return 0; 
} 

