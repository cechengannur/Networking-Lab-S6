/*
First run server.c by gcc server.c -o server
then run server by ./server
After that start client by  gcc client.c -o client
then run client by ./client
*/

#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>
#include<unistd.h>
int main()
{
    int clientsocket,port; //clientsocket is the socket descriptor , port is the port number
    struct sockaddr_in serveraddr; //creating a structure of type sockaddr_in for server
    socklen_t len; //creating a variable to store the length of the server address
    char message[50];


    clientsocket=socket(AF_INET,SOCK_STREAM,0);//creating a socket
    bzero((char*)&serveraddr,sizeof(serveraddr));//initializing the server address to zero
    len=sizeof(serveraddr); //storing the length of the server address in len
    serveraddr.sin_family=AF_INET; //setting the family of the server address to AF_INET

    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);//setting the port number of the server address to port
    printf("\nTrying to connect to the server.\n");


    connect(clientsocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));//connecting to the server
    printf("\nConnected to the server.\n");
    printf("\nSending message for server connection");
    
    send(clientsocket,"HI,IAM CLIENT...",sizeof("HI,IAM CLIENT..."),0);//sending the message to the server
    printf("\nReceiving message from server.\n");
    recv(clientsocket,message,sizeof(message),0);//receiving the message from the server
    printf("\nMessage received.\t%s\n",message);
    close(clientsocket);//closing the socket
}

/*

s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/TCP$ gcc client.c -o client
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/TCP$ ./client
Enter the port number 6000

Trying to connect to the server.

Connected to the server.

Sending message for server connection
Receiving message from server.

Message received.       YOUR MESSAGE RECEIVED.
s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/Socket-Programming/TCP$ 

*/