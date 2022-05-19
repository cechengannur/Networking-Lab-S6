/*
First run server.c by gcc server.c -o server
then run server by ./server
After that start client by  gcc client.c -o client
then run client by ./client
*/
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>

int main()
{
    int serversocket,clientsocket,port;
    struct sockaddr_in serveraddr,clientaddr;
    socklen_t len;
    char message[50];
    struct serveraddr;


    serversocket=socket(AF_INET,SOCK_STREAM,0);
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    bzero((char*)&clientaddr,sizeof(clientaddr));
    len=sizeof(clientaddr);
    listen(serversocket,5);
    printf("\nWaiting for client connection\n");
    printf("\nhai:");
    clientsocket=accept(serversocket,(struct sockaddr*)&clientaddr,&len);
    printf("\nClient connectivity received.\n");
    printf("\nReading message from the client.\n");
    read(clientsocket,message,sizeof(message));
    printf("\nThe client has sent.%s",message);
    printf("\nSending message to the client.\n");
    write(clientsocket,"YOUR MESSAGE RECEIVED.",sizeof("YOUR MESSAGE RECEIVED."));
    close(clientsocket);
    close(serversocket);


}

/*
OUTPUT
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/TCP$ gcc server.c -o server
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/TCP$ ./server
Enter the port number 6000

Waiting for client connection

hai:
Client connectivity received.

Reading message from the client.

The client has sent.HI,IAM CLIENT...
Sending message to the client.
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/TCP$ ^C
*/