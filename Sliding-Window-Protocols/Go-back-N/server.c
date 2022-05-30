#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <math.h>
#include <time.h>


/* ============= PACKET FORMAT ============= 
    NAME                SIZE                REMARKS
    Type :              1 byte      (RRQ = 1; DATA = 2; ACK = 3; ERROR = 4)
    Window Size:        1 byte
    Sequence number:    1 byte
    Filename:           20 byte     null terminated string
    Data:               0-512 bytes depending on the packet size.
    =============== Messages  ===============
    NAME                                FORMAT
    1. Read request (RRQ):          type; window size; file name 
    2. Data (DATA):                 type, sequence number, data
    3. Acknowledgment (ACK):        type; sequence number 
    4. Error (ERROR):               type 
*/

#define IP_PROTOCOL 0 

#define LEN 512 
#define TRUE 1

#define SUCCESS 0
#define FAILURE 1
#define DEBUG   1


#define RRQ     1
#define DATA    2
#define ACK     3
#define ERROR   4

#define TYPE_INDEX          0
#define WINDOW_SIZE_INDEX   1
#define SEQ_NUM_INDEX       1
#define FILE_NAME_INDEX     2

#define TIME_OUT   3
#define FAIL_LIMIT 5

#define RED     printf("\033[0;31m");
#define GREEN   printf("\033[0;32m");
#define YELLOW  printf("\033[0:33m");
#define DEFAULT printf("\033[0m"); 

#define debug(s) if(DEBUG) printf(s);

int WINDOW_SIZE;

// return min element of num1, num2
int min(int num1, int num2){
    return (num1<num2)?num1:num2;
}
// return type of given argument
int getType(char packet[LEN]){
    // value is stored in char so converting to int
    return packet[TYPE_INDEX]-'0';
}
// return window size of given packet
int getWindowSize(char packet[LEN]){
    return packet[WINDOW_SIZE_INDEX];
}
// return sequence number of given packet
char getSeqNum(char packet[LEN]){
    return packet[SEQ_NUM_INDEX];
}
// returning file name of given packet
char* getFileName(char packet[LEN]){
    // assigning 20bytes to filename
    char* file_name = malloc(20);
    // fetching filename 
    for(int i=0;i<20;i++){
        file_name[i] = packet[FILE_NAME_INDEX + i];
    }
    // set last value to escape
    file_name[19]='\0';
    return file_name;
}

// senfing ERROR packet to client-> when client wants to acces file which doesn't exist 
void sendError(int sockfd, struct sockaddr_in my_addr){
    
    // error_packet size is 1 becasue no additional data has to be sent in sending error
    char error_packet[1];
    int error_packet_len = 1;
    
    error_packet[0]='4';
    // sending to client
    int send_bits = sendto(sockfd, error_packet, error_packet_len, 0, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(send_bits <= 0){
        printf("Error in Sending Error Message !\n");
    }else{
        printf("Successfully Sent Error Message !\n");
    }
}
// flushing given packet
void flush_packet(char packet[LEN]){
    int i = 0;
    for(i=0;i<LEN;i++){
        packet[i]='\0';
    }
}
// printing given char array of given len
void print(char* data, int len){
    int i;
    for(i=0; i<len; i++){
        printf("%c", *data);
        data++;
    }
}
// wait untill ACK of given seq_num is received.
int wait_ack(int sockfd, struct sockaddr_in my_addr, int seq_num){
    char packet[LEN];
    int len = sizeof(my_addr);
    
    // ############## Setting TIME OUT ##################
    struct timeval timeout;  

    timeout.tv_sec = 0;
    timeout.tv_usec = TIME_OUT*1000;

    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
        RED printf("setsockopt failed\n"); DEFAULT
        return FAILURE;
    }
    // ##################################################
    
    // wait for any packet to arrive
    int readBits = recvfrom(sockfd, packet, LEN, 0, (struct sockaddr*)&my_addr, &len);
    
    // ############### UN-Setting TIME OUT ##############
    // because setsockopt function resets it's timer every time new data arrives.
    // So, we only want to constraint time for ACK. if we don't unset it then TIMEOUT will be applied to all
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
        RED printf("setsockopt failed\n"); DEFAULT
        return FAILURE;
    }
    // ###################################################

    if(readBits <= 0){
        RED printf("Time out has occured !\n"); DEFAULT        
        return FAILURE;
    }

    // Fetching type from given packet
    int type = getType(packet);
    // checking if arrived packet is ACK or not
    if(type == ACK){
        // fetching seq_num from packet names as received_sequece_number
        int rcv_seq_num = packet[SEQ_NUM_INDEX];
        // checking if rcv_seq_num is same as required seq_num
        if(seq_num == rcv_seq_num){
            GREEN printf("Recieved ACK for seq_num : %d\n", seq_num); DEFAULT
            return SUCCESS; 
        }
        else{
            RED printf("Recieved ACK for seq_num : %d\nBut Required was %d\n", rcv_seq_num, seq_num); DEFAULT
            return FAILURE;
        }
    }
    else{
        RED printf("Recieved some packet of type : %d, not ACK\n", type); DEFAULT
        return FAILURE;
    }

}
void send_go_back_n(FILE *fp, int sockfd, struct sockaddr_in my_addr){
    // for creating random number
    srand(time(0));
    // ####################### GETTING DATA FROM FILE #######################
    char data[256*LEN];
    // get data from file in data variable
    // return if file has some problems
    if(fp == NULL){
        printf("Error in reading file !\n");
        return;
    }
    // for debug purpose
    
    // debug("Data from file :\n");
    // debug("================= START ================\n");
    
    // ind is index for data
    int ind = 0;
    // each readed character from file will be stored in char c
    char c = fgetc(fp);
    // reading the file
    while(c != EOF){
        // for debug purpose 
        // if(DEBUG)
        // printf("%c", c);
        // storing value in data variable
        data[ind]=c;
        c = fgetc(fp);
        ind++;
    }
    // for debug purpose
    // debug("================= END ================\n");
    // after whole file is readed ind will be representing data length
    int data_lengh = ind;
    
    if(DEBUG)
    printf("Length : %d\n", data_lengh); 
    // ################################################################################ 

    // calculating how many packets we have to send
    // because max packet size is 512bytes, so if data is more then more packets
    // here divided by LEN-2 instade of LEN because we are sending additional 2bytes
    // representing type and seq_num.
    int total_packets = (int)ceil(data_lengh*1.0/(LEN-2)); 
    if(DEBUG)
    printf("Total Packets to be send : %d\n", total_packets);

    // if sent_packet[i] = 0 -> i'th packet is not sent yet.
    //    sent_packet[i] = 1 -> i'th packet is sent. so do not send again.
    int sent_packet[total_packets],i;
    // reseting sent_packet 
    for(i=0; i<total_packets; i++)
    sent_packet[i] = 0;

    
   int window_start = 0;
   int failed_count = 0;

   for(window_start = 0; window_start < total_packets; window_start++){
        int window_index;
        for(window_index = window_start; window_index < min(window_start+WINDOW_SIZE, total_packets); window_index++){
            // checking is window_index'th packet is already sent or not
            if(!sent_packet[window_index]){
                
                if(DEBUG){
                    printf("Sending packet : %d\n", window_index);
                }
                // ================ SLICING PACKET FROM DATA ================
                char packet[LEN];
                // geting data for packet which will be stored in packet variable
                // flush_packet for safty, because it can contain some garbge data
                flush_packet(packet);
                // first byte is represting TYPE which is equal to DATA (= 2)
                packet[0]= '2';
                // second byte is representing seq_num 
                packet[1]= window_index;
                
                int i, data_ind;
                // data_ind is index in data, from where packet_num's data will start
                data_ind = window_index*(LEN-2);
                for(i=2;i<LEN;i++){
                    packet[i] = data[data_ind];
                    data_ind++;
                }
                // =================      SLICING DONE     =====================
                
                // generating random number between 0 - 1 
                double random_num = ((rand() % 10000) / 10000.0);
                // random number is for TESTING PURPOSE...we can create such environment 
                // where packets can be lost in network.
                
                // this generated random number will be used as probability of SUCCESSFULLY SENDING packet
                // if generated number is less then 0.4 then we treat this as LOST PACKET
                // i.e. packet was sent from server side but not received by receiver...lost in network
                // here 90% probability of successfully sending packet
                //      10% probability of LOSTing  packet in network
                if(random_num <= 0.1){
                    RED printf("Packet ");DEFAULT
                    printf("%d", window_index);
                    RED printf(" was lost in network...\n"); DEFAULT
                }else{
                    // send packet to receiver
                    // NOTE : this is sent using DGRAM->UDP so it may happen that packets are actually lost between network. 
                    //        but this happen very rarely. so i had to create such environment to test code.
                    int send_bits = sendto(sockfd, packet, LEN, 0, (struct sockaddr*)&my_addr, sizeof(my_addr));
                    if(send_bits <= 0){
                        RED printf("Error in sending packet with seq_num : %d\n", window_index); DEFAULT
                        window_index--;
                    }
                }
                // setting array value to 1. because even if it was lost in  network, it was sent from server
                sent_packet[window_index] = 1;
            }
        }
        // wait for ACK
        if(DEBUG){
            YELLOW printf("Waiting ACK for %d\n", window_start); DEFAULT
        }
        // after window is sent we will get ACK of first packet in window
        // so here we wait for window_start's ACK
        if(wait_ack(sockfd, my_addr, window_start) == FAILURE){
            // it ACK did not arrive then we have to resent whole window.
            // so we have to reset all packets's sent value to 0
            YELLOW printf("Re-Sending : %d -> %d]\n",window_start, min(window_start+WINDOW_SIZE, total_packets)); DEFAULT
            for(window_index = window_start; window_index < min(window_start+WINDOW_SIZE, total_packets); window_index++){
                sent_packet[window_index] = 0;
            }
            failed_count ++;
            if(failed_count == FAIL_LIMIT){
                RED printf("Closing connection because of 5 consecutive TIME_OUT\n"); DEFAULT
                return ;
            }
            window_start --;
        }else{
            failed_count = 0;
        }
        
   }
   printf("Data sent process is finished...\n");
}


int main(int argc, char *argv[]){
    // checking if enough arguments are passed.
    if(argc != 2){
		fprintf(stderr,"Error, no port provided.");
	}

    
    // ################# Initializing ##################
    // Converting string to int
    int PORT_NO = atoi(argv[1]);
	struct sockaddr_in my_addr;
    int my_addr_len = sizeof(my_addr);
    my_addr.sin_family = AF_INET; 
	my_addr.sin_port = htons(PORT_NO); 
	my_addr.sin_addr.s_addr = INADDR_ANY;
    // #################################################



    // ############### Creating Socket #################
    int sockfd;
    // SOCK_DGRAM for udp connection 
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);
    // if socket function returned value less then 0 then
    // there was some problem.
    if(sockfd < 0){
        printf("Error in creating Socket !\n");
        return FAILURE;
    }else{
        printf("Socket Created Successfully !\n");
    }
    // #####################################################


    // ################# Binding Socket ####################
    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr))==0){
        printf("Successfully Binded Socket !\n");
    }else{
        printf("Error in Binding Socket !\n");
        return FAILURE;
    }
    // #####################################################

    // ################ Provideing Service #################
    while(TRUE){
        printf("Waiting for Request...\n");
        // creating a variable to store received data
        char packet[LEN];
        // flushing for igonoring garbage values currently stored in packet var
        flush_packet(packet);
        // waiting untill some packets are fatched.
        int received_len;
        // NOTE : be aware of last argument. we have to pass address of len. i.e. &my_addr_len
        received_len = recvfrom(sockfd, packet, LEN, 0, (struct sockaddr*)&my_addr, &my_addr_len);
        // if received_len is <= 0 then there is some problem, so turminating code
        if(received_len <= 0){
            RED printf("Error while reading...\n"); DEFAULT
            return FAILURE;
        }
        // fatching packet type from received packet
        int type = getType(packet);
        // handling request if packet is file request
        if(type == RRQ){
            printf("Extracted Type          : ");
            YELLOW printf("RRQ\n"); DEFAULT

            // fatching window size from received packet
            WINDOW_SIZE = getWindowSize(packet);
            printf("Extracted Window Size   : ");
            YELLOW printf("%d\n", WINDOW_SIZE); DEFAULT
            
            // fatching file name requested
            char* file_name = getFileName(packet);
            printf("Extracted File Name     : ");
            YELLOW printf("%s\n", file_name); DEFAULT

            // check if that file exist or not
            FILE *fp = fopen(file_name, "r"); 
            // sending ERROR packet when no file found that was reqeusted 
            if (fp == NULL){
                printf("\nFile open failed!\n"); 
                sendError(sockfd, my_addr);
            }
            // if requested file exist then sending the file using GO BACK N protocol
            else{
                printf("\nFile Successfully opened!\n"); 
                send_go_back_n(fp, sockfd, my_addr);
            }
            // closing file pointer
            fclose(fp);
        }
        else{
            // discarding packet which are not RRQ
            printf("\nPacket Type not valid !\n");
        }

    }

    return 0;
}
