#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<strings.h>

int main()
{
    int number;//creating a variable to store the number 
    int clientsocket,port; //clientsocket is the socket descriptor , port is the port number
    struct sockaddr_in serveraddr; //creating a structure of type sockaddr_in for server
    socklen_t len; //creating a variable to store the length of the server address
    struct hostent *server; //creating a structure of type hostent for server
    char message[50]; //creating a char array to store the message
    
    clientsocket=socket(AF_INET,SOCK_DGRAM,0);//creating a socket
    //steps involved in the server address creation.
    bzero((char*)&serveraddr,sizeof(serveraddr));//initializing the server address to zero
    len=sizeof(serveraddr);//storing the length of the server address in len
    serveraddr.sin_family=AF_INET;//setting the family of the server address to AF_INET

    printf("Enter the port number ");
    scanf("%d",&port);
    serveraddr.sin_port=htons(port);//setting the port number of the server address to port , htons is used to convert the port number to network byte order
    fgets(message,2,stdin);//fgets is used to read the message from the user and storing it in message 2 is the size of the message
    

    if(connect(clientsocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
    {
        printf("\nConnection failed\n");
        exit(0);
    }
    else
    {
        printf("\nConnection successful\n");
    }

    printf("Enter a number to be sent to the server : ");

    //get number
    scanf("%d",&number);

    //send number
    sendto(clientsocket,&number,sizeof(number),0,(struct sockaddr*)&serveraddr,sizeof(serveraddr));//sendto is used to send the message to the server
    
    //recieve message
    recvfrom(clientsocket,message,sizeof(message),0,(struct sockaddr*)&serveraddr,&len);//recvfrom is used to receive the message from the server
    printf("\n%s",message);

    close(clientsocket);//closing the socket
}

