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

//fn definition for isPrime
int isPrime(int n);


int main()
{
    int serversocket,clientsocket,port; //clientsocket is the socket descriptor , port is the port number
    struct sockaddr_in serveraddr,clientaddr; //creating a structure of type sockaddr_in for server
    socklen_t len; //creating a variable to store the length of the server address 
    char message[50]; //
    struct serveraddr; //creating a structure of type sockaddr_in for server

    int numrecieved;

    serversocket=socket(AF_INET,SOCK_STREAM,0); //creating a socket
    bzero((char*)&serveraddr,sizeof(serveraddr));//initializing the server address to zero
    serveraddr.sin_family=AF_INET;//setting the family of the server address to AF_INET

    printf("Enter the port number ");
    scanf("%d",&port);

    serveraddr.sin_port=htons(port); //setting the port number of the server address to port
    serveraddr.sin_addr.s_addr=INADDR_ANY; //setting the IP address of the server address to INADDR_ANY
    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr)); //binding the server address to the socket
    bzero((char*)&clientaddr,sizeof(clientaddr)); //initializing the client address to zero
    len=sizeof(clientaddr); //storing the length of the client address in len
    listen(serversocket,5); //listening to the socket, 5 is the number of clients that can connect to the server

    printf("\nWaiting for client connection\n"); 
    printf("\nhai:");
    clientsocket=accept(serversocket,(struct sockaddr*)&clientaddr,&len);//accepting the client connection

    printf("\nClient connectivity received.\n");
    
    //take an integer input from client
    write(clientsocket,"Enter an integer:",sizeof("Enter an integer:")); 
    //get the integer from client
    read(clientsocket,&numrecieved,sizeof(numrecieved));
    //print the integer recieved from client
    printf("\nNumber send from server : %d",numrecieved);

    //check if the number is prime or not using the function isprime()
    if(isPrime(numrecieved))
    {
        write(clientsocket,"Number is prime",sizeof("Number is prime"));
    }
    else
    {
        write(clientsocket,"Number is not prime",sizeof("Number is not prime"));
    }

    close(clientsocket);//closing the client socket
    close(serversocket);//closing the server socket


}

//function to check if number is prime or not
int isPrime(int n)
{
    int i;
    for(i=2;i<=n/2;i++)
    {
        if(n%i==0)
        {
            return 0;
        }
    }
    return 1;
}

