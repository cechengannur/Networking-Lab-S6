#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>

void finfFibanocci(int numrecieved,char fib[]);

int main()
{
    int serversocket,clientsocket,port; 
    struct sockaddr_in serveraddr,clientaddr; 
    socklen_t len;  
    char message[50]; 
    struct serveraddr;

    // empty string fib
    char fib[500] = "";

    int numrecieved;

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
    clientsocket=accept(serversocket,(struct sockaddr*)&clientaddr,&len);//accepting the client connection

    printf("\nClient connectivity received.\n");
    
    write(clientsocket,"Enter an integer:",sizeof("Enter an integer:")); 
    read(clientsocket,&numrecieved,sizeof(numrecieved));
    printf("\nNumber send from server : %d",numrecieved);

    //calculatina a string fib to send back to client that contains the fibonacci series
    finfFibanocci(numrecieved,fib);
    write(clientsocket,fib,sizeof(fib));

    close(serversocket);
    close(clientsocket);
    return 0;

}

void finfFibanocci(int numrecieved,char fib[])
{
    //finding each number in the fibonacci series and strncat it to the fib string
    int i;
    int first = 0;
    int second = 1;
    int next = 0;
    for (i = 0; i < numrecieved; i++)
    {
        if (i <= 1)
        {
            next = i;
        }
        else
        {
            next = first + second;
            first = second;
            second = next;
        }
        //converting the number to string
        char str[10];
        sprintf(str, "%d", next);
        //concatenating the string to the fib string
        strcat(fib, str);
        strcat(fib, " ");
    }
}
