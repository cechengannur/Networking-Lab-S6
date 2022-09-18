#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>

int isPrime(int n);


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
        strncat(fib, str, 10);
        strncat(fib, " ", 1);
    }
}

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

//function to check whether the number is amstrong or not
int isAmstrong(int n)
{
    int temp,rem,sum=0;
    temp=n;
    while(temp!=0)
    {
        rem=temp%10;
        sum=sum+rem*rem*rem;
        temp=temp/10;
    }
    if(sum==n)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//function to check whether the number is palindrome or not
int isPalindrome(int n)
{
    int temp,rem,sum=0;
    temp=n;
    while(temp!=0)
    {
        rem=temp%10;
        sum=sum*10+rem;
        temp=temp/10;
    }
    if(sum==n)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//srtring palindrome
int isStringPalindrome(char str[])
{
    int i,j;
    for(i=0,j=strlen(str)-1;i<j;i++,j--)
    {
        if(str[i]!=str[j])
        {
            return 0;
        }
    }
    return 1;
}


//fibanocci series
int fibanocci(int n)
{
    int a=0,b=1,c,i;
    if(n==1)
    {
        printf("%d",a);
    }
    else if(n==2)
    {
        printf("%d %d",a,b);
    }
    else
    {
        printf("%d %d",a,b);
        for(i=3;i<=n;i++)
        {
            c=a+b;
            printf(" %d",c);
            a=b;
            b=c;
        }
    }
}


