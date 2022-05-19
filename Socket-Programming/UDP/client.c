#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>

int main()
{
    //variable to store the socket_id.
    int clientsocket,port;
    //variable to store the address.
    struct sockaddr_in serveraddr;
    //variableto store the address length.
    socklen_t len;
    //variable to store the network byte order address.
    struct hostent *server;
    char message[50];
    //socket creation.
    clientsocket=socket(AF_INET,SOCK_DGRAM,0);
    //steps involved in the server address creation.
    bzero((char*)&serveraddr,sizeof(serveraddr));
    len=sizeof(serveraddr);
    serveraddr.sin_family=AF_INET;
    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);
    fgets(message,2,stdin);
    printf("\nSending message for server connection\n");
    //sending message.
    sendto(clientsocket,"HI I AM CLIENT...",sizeof("HI I AM CLIENT...."),0,(struct
    sockaddr*)&serveraddr,sizeof(serveraddr));
    printf("\nReceiving message from server.\n");
    //receiving messages.
    recvfrom(clientsocket,message,sizeof(message),0,(struct sockaddr*)&serveraddr,&len);
    printf("\nMessage received:\t%s\n",message);
    close(clientsocket);
}

/*
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/UDP$ gcc client.c -o client
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/UDP$ ./client
Enter the port number 6000

Sending message for server connection

Receiving message from server.

Message received:       YOUR MESSAGE RECEIVED.
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/UDP$ 
*/