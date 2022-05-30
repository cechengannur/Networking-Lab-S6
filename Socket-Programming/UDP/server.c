#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>

int main()
{
    int serversocket,port; //serversocket is the socket descriptor , port is the port number
    struct sockaddr_in serveraddr,clientaddr; //creating a structure of type sockaddr_in for server and client
    socklen_t len; //creating a variable to store the length of the server address
    char message[50];//creating a char array to store the message
    //socket creation.
    serversocket=socket(AF_INET,SOCK_DGRAM,0);//creating a socket
    //steps involved in defining the serveraddress.
    bzero((char*)&serveraddr,sizeof(serveraddr)); //initializing the server address to zero
    serveraddr.sin_family=AF_INET;//setting the family of the server address to AF_INET

    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);//setting the port number of the server address to port , htons is used to convert the port number to network byte order
    serveraddr.sin_addr.s_addr=INADDR_ANY; //setting the address of the server address to INADDR_ANY , INADDR_ANY is used to bind the socket to all the interfaces of the machine
    //binding the socket to the operating system.
    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));//bind is used to bind the socket to the operating system
    printf("\nWaiting for the client connection\n");
    bzero((char*)&clientaddr,sizeof(clientaddr));//initializing the client address to zero
    len=sizeof(clientaddr);//storing the length of the client address in len

    //receiving message from the client.
    recvfrom(serversocket,message,sizeof(message),0,(struct sockaddr*)&clientaddr,&len);//recvfrom is used to receive the message from the client
    printf("\nConnection received from client.\n");
    printf("\nThe client has send:\t%s\n",message);
    printf("\nSending message to the client.\n");
    //sending message to the client.
    sendto(serversocket,"YOUR MESSAGE RECEIVED.",sizeof("YOUR MESSAGERECEIVED."),0,( struct sockaddr*)&clientaddr,sizeof(clientaddr));//sendto is used to send the message to the client
    close(serversocket);
}



/*
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/UDP$ gcc server.c -o server
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/UDP$ ./server
Enter the port number 6000

Waiting for the client connection

Connection received from client.

The client has send:    HI I AM CLIENT...

Sending message to the client.
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/UDP$ 
*/