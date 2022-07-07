/*
    First compile server.c by gcc server.c -o server
    run server by ./server <PORT NO.>
    Eg : ./server 6500

    Then compile client.c by gcc client.c -o client
    run client by ./client <PORT NO.>
    Eg : ./client 127.0.0.1 6500
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 100
int main(int argc, char *argv[])
{
    int n, sock_fd;
    struct sockaddr_in servaddr, cliaddr;
    char mesg[MAXLINE + 1];
    socklen_t len;
    char *str_ptr, *buf_ptr, *str;
    len = sizeof(cliaddr);
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("cannot create socket\n");
        exit(1);
    }
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed:");
        exit(1);
    }

    if ((n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
    {
        perror("size not received:");
        exit(1);
    }
    mesg[n] = '\0';
    printf("mesg:%s\n", mesg);
    sprintf(mesg, "220 name_of_server_mail_server\n");
    sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
    mesg[n] = '\0';
    printf("C:%s\n", mesg);
    str_ptr = strdup(mesg);
    buf_ptr = strsep(&str_ptr, " ");
    sprintf(mesg, "250 Hello %s", str_ptr);
    free(buf_ptr);
    sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
    mesg[n] = '\0';
    printf("C:%s", mesg);
    str_ptr = strdup(mesg);
    buf_ptr = strsep(&str_ptr, ":");
    str_ptr[strlen(str_ptr) - 1] = '\0';

    sprintf(mesg, "250 Hello %s.......sender ok\n", str_ptr);
    free(buf_ptr);
    sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
    mesg[n] = '\0';
    printf("C:%s", mesg);
    str_ptr = strdup(mesg);
    buf_ptr = strsep(&str_ptr, ":");
    str_ptr[strlen(str_ptr) - 1] = '\0';
    sprintf(mesg, "250 Hello %s.......Recepient ok\n", str_ptr);
    free(buf_ptr);
    sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
    mesg[n] = '\0';
    printf("C:%s\n", mesg);
    sprintf(mesg, "354 Enter mail,end with \".\" on a line by itself \n");
    sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    while (1)
    {
        n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        mesg[n] = '\0';
        printf("C:%s\n", mesg);
        mesg[strlen(mesg) - 1] = '\0';

        str = mesg;
        while (isspace(*str++))
            ;
        if (strcmp(--str, ".") == 0)
            break;
        sprintf(mesg, "250 messages accepted for delivery \n");
        sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        n = recvfrom(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        mesg[n] = '\0';
        printf("C:%s\n", mesg);
        sprintf(mesg, "221 servers mail server closing connection\n");
        sendto(sock_fd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    }
}


/*
OUTPUT :

s6d2@user-HP-280-G3-MT:~/Networking-Lab-S6/SMTP$ ./server 6500
mesg:SMTP REQUEST FROM CLIENT

C:HELLO name_of_client_mail_server

C:MAIL FROM :<aromal@katha.today>
C:RCPT TO : <ashishpeter@gmail.com>
C:DATA

C:Hello Ashish! How Are You ?

C:.

C:QUIT

^C
s6d2@user-HP-280-G3-MT:~/gitgub-lab/Networking-Lab-S6/SMTP$ 

*/