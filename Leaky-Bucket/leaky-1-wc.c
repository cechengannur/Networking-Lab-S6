#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NOF_PACKETS 10

// function to generate random number between 0 and 100
int randm(int a)
{
    int rn = (random() % 10) % a;
    return rn == 0 ? 1 : rn;
}

int main()
{
    int packet_sz[NOF_PACKETS], i, clk, b_size, o_rate, p_sz_rm = 0, p_sz, p_time, op;
    // b_size = bandwidth in bytes/sec
    // o_rate = output rate in packets/sec
    // p_sz = packet size in bytes
    // p_time = packet time in sec
    // op = operation to be performed on the packet
    // clk = clock in sec
    // p_sz_rm = packet size in bytes removed from the buffer
    // p_sz = packet size in bytes added to the buffer

    for (i = 0; i < NOF_PACKETS; ++i)
        packet_sz[i] = randm(6) * 10; // generate random packet size between 0 and 60 bytes

    for (i = 0; i < NOF_PACKETS; ++i)
        printf("\npacket[%d]:%d bytes\t", i, packet_sz[i]); // print the packet size
    

    printf("\nEnter the Output rate:");
    scanf("%d", &o_rate);
    printf("Enter the Bucket Size:");
    scanf("%d", &b_size);

    for (i = 0; i < NOF_PACKETS; ++i)
    {   
        // calculate the packet time
        if ((packet_sz[i] + p_sz_rm) > b_size)
            //if packet size is greater than the bucket size
            if (packet_sz[i] > b_size) /*compare the packet siz with bucket size*/
                printf("\n\nIncoming packet size (%dbytes) is Greater than bucket capacity (%dbytes)-PACKET REJECTED", packet_sz[i], b_size);
            else
                printf("\n\nBucket capacity exceeded-PACKETS REJECTED!!");
        else
        {
            p_sz_rm += packet_sz[i];
            printf("\n\nIncoming Packet size: %d", packet_sz[i]);
            printf("\nBytes remaining to Transmit: %d", p_sz_rm);
            p_time = randm(4) * 10; // generate random packet time between 0 and 40 secs
            printf("\nTime left for transmission: %d units", p_time);
            for (clk = 10; clk <= p_time; clk += 10)
            {
                sleep(1);
                if (p_sz_rm)
                {   
                    // if the packet size is greater than the bucket size
                    if (p_sz_rm <= o_rate) /*packet size remaining comparing with output rate*/
                        op = p_sz_rm, p_sz_rm = 0; // if the packet size is less than the output rate, then remove the packet size from the buffer
                    else
                        op = o_rate, p_sz_rm -= o_rate; // else remove the output rate from the buffer
                    printf("\nPacket of size %d Transmitted", op);
                    printf("----Bytes Remaining to Transmit: %d", p_sz_rm);
                }
                else
                {
                    printf("\nTime left for transmission: %d units", p_time - clk);
                    printf("\nNo packets to transmit!!");
                }
            }
        }
    }
}
