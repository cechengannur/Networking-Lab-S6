#include<stdio.h>

int main(){
    int incoming, outgoing, buck_size, n, store = 0;
    //store is used to store the number of packets that are removed from the buffer
    //buck_size is the size of the buffer in bytes
    //n is the number of packets that are to be transmitted
    //incoming is the size of the incoming packet in bytes
    //outgoing is the size of the outgoing packet in bytes
    printf("Enter bucket size, outgoing rate and no of inputs: ");
    scanf("%d %d %d", &buck_size, &outgoing, &n);

    while (n != 0) {
        printf("Enter the incoming packet size : ");
        scanf("%d", &incoming);
        printf("Incoming packet size %d\n", incoming);
        //if the incoming packet size is greater than the bucket size
        if (incoming <= (buck_size - store)){
            store += incoming; //add the incoming packet size to the buffer
            printf("Bucket buffer size %d out of %d\n", store, buck_size);
        } else {
            printf("Dropped %d no of packets\n", incoming - (buck_size - store));
            printf("Bucket buffer size %d out of %d\n", store, buck_size);
            //if the incoming packet size is greater than the bucket size
            //then the packets are dropped
            store = buck_size;
        }
        store = store - outgoing; //subtract the outgoing packet size from the buffer
        if(store < 0)
            store = 0; //if the buffer size is less than 0 then set it to 0
        printf("After outgoing %d packets left out of %d in buffer\n", store, buck_size);
        n--;
    }
}
