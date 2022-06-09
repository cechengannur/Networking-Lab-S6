#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<unistd.h>

#define W 5
char a[10];
char b[10];
void alpha9(int);
int main()
{
    int s,f,wl,c=1,x,i=0,j,n,p=0,e=0;
    struct sockaddr_in ser;
    s=socket(AF_INET,SOCK_STREAM,0);
    ser.sin_family=AF_INET;
    ser.sin_port=6500;
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");
    connect(s,(struct sockaddr *) &ser, sizeof(ser));
    printf("\nTCP Connection Established.\n");
    printf("\nEnter the number of Frames: ");
    scanf("%d",&f);
    alpha9(f);
    send(s,a,sizeof(a),0);
    strcpy(b,"Time Out ");
    while(1)
    {
        for(i=0;i<W;i++)
        {
            alpha9(c);
            send(s,a,sizeof(a),0);
            if(c<=f)
            {
                printf("\nFrame %d Sent",c);
                c++;
            }
        }
        i=0;
        wl=W;
        while(i<W)
        {
            recv(s,a,sizeof(a),0);
            p=atoi(a);
            if(strcmp(a,b)==0)
            {
                e=c-wl;
                if(e<f)
                {
                    printf("\nTime Out, Resent Frame %d onwards",e);
                }
                break;
            }
            else
            {
                if(p<=f)
                {
                    printf("\nFrame %s Acknowledged",a);
                    wl--;
                }
                else
                {
                    break;
                }
            }
            if(p>f)
            {
                break;
            }
            i++;
        }
        if(wl==0 && c>f)
        {
            send(s,b,sizeof(b),0);
            break;
        }
        else
        {
            c=c-wl;
            wl=W;
        }
    }
    close(s);
    return 0;
}

void alpha9(int z)
{
    int k,i=0,j,g;
    k=z;
    while(k>0)
    {
        i++;
        k=k/10;
    }
    g=i;
    i--;
    while(z>0)
    {
        k=z%10;
        a[i]=k+48;
        i--;
        z=z/10;
    }
    a[g]='\0';
}