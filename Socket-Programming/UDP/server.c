#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>

int main()
{
    //variable to store the socket_id.
    int serversocket,port;
    //variable to store the network addresses.
    struct sockaddr_in serveraddr,clientaddr;
    //variable to store the address length.
    socklen_t len;
    char message[50];
    //socket creation.
    serversocket=socket(AF_INET,SOCK_DGRAM,0);
    //steps involved in defining the serveraddress.
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    //binding the socket to the operating system.
    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    printf("\nWaiting for the client connection\n");
    bzero((char*)&clientaddr,sizeof(clientaddr));
    len=sizeof(clientaddr);
    //receiving message from the client.
    recvfrom(serversocket,message,sizeof(message),0,(struct sockaddr*)&clientaddr,&len);
    printf("\nConnection received from client.\n");
    printf("\nThe client has send:\t%s\n",message);
    printf("\nSending message to the client.\n");
    //sending message to the client.
    sendto(serversocket,"YOUR MESSAGE RECEIVED.",sizeof("YOUR MESSAGERECEIVED."),0,( struct sockaddr*)&clientaddr,sizeof(clientaddr));
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