#include <stdio.h>
int cost[10][10];
int dist[10];
int arr[20];
void djikstra(int);
int search(int);
int length_of(int *);
void print_route(int, int);
int prev[20];
int order_arr[20];
int src;
int main()
{
    int num_nodes, i, j, d;
    printf("Enter number of nodes:");
    scanf("%d", &num_nodes);
    printf("Enter the source node:");
    scanf("%d", &src);
    printf("Enter the cost matrix, For infinity enter 999\n");
    for (i = 0; i < num_nodes; i++)
        for (j = 0; j < num_nodes; j++)
            scanf("%d", &cost[i][j]);
    djikstra(num_nodes);

    return 0;
}
void djikstra(int _num_nodes)
{
    int i, min_val, l;
    int k = 0;
    int m = 0;
    int min = 999;
    int last;
    int neighbour[20];

    for (i = 0; i < 20; i++)
    {
        arr[i] = -1;
        neighbour[i] = -1;
        order_arr[i] = -1;
        prev[i] = -1;
    }
    arr[0] = src;
    last = 0;
    for (i = 0; i < _num_nodes; i++)
    {
        if (i != src)
        {
            if (cost[src][i] < 999)
            {
                dist[i] = cost[src][i];
                prev[i] = src;
            }
            else
                dist[i] = 999;
        }
        else
        {
            dist[i] = 0;
        }
    }
    do
    {
        for (i = 0; i < _num_nodes; i++)
        {
            if (search(i) == 0)
            {
                if (dist[i] < min)
                {
                    min = dist[i];
                    min_val = i;
                }
            }
        }
        last++;
        arr[last] = min_val;
        for (i = 0; i < _num_nodes; i++)
        {
            if (search(i) == 0)
            {
                if (cost[min_val][i] < 999)
                {
                    neighbour[m] = i;
                    m++;
                }
            }
        }
        m = 0;
        while (neighbour[m] != -1)
        {
            if (dist[min_val] + cost[min_val][neighbour[m]] < dist[neighbour[m]])
            {
                dist[neighbour[m]] = dist[min_val] + cost[min_val][neighbour[m]];
                prev[neighbour[m]] = min_val;
            }
            m++;
        }
        m = 0;
        for (i = 0; i < _num_nodes; i++)
            neighbour[i] = -1;
        min = 999;
        min_val = -1;
    } while (length_of(arr) != _num_nodes);
    i = 1;
    l = 1;
    while (i < _num_nodes)
    {
        print_route(i, l);
        printf("[ distance = %d]", dist[i]);
        printf("\n");
        i++;
        l++;
        for (k = 0; k < 20; k++)
            order_arr[k] = -1;
    }
}

void print_route(int _i, int _l)
{
    int begin;
    int *ptr;
    int h, len, temp;
    static int inc[20];
    if (_i == src)
    {
        ptr = order_arr;
        while (*ptr != -1)
            ptr++;
        len = ptr - order_arr;
        for (h = 0; h < len / 2; h++)
        {
            temp = order_arr[h];
            order_arr[h] = order_arr[len - h - 1];
            order_arr[len - h - 1] = temp;
        }
        ptr = order_arr;
        printf("%d", src);
        while (*ptr != -1)
        {
            printf("->%d ", *ptr);
            ptr++;
        }
        return;
    }
    else
    {
        order_arr[inc[_l]] = _i;
        inc[_l]++;
        print_route(prev[_i], _l);
    }
}


int search(int _i)
{
    int i = 0;
    while (arr[i] != -1)
    {
        if (_i == arr[i])
            break;
        else
            i++;
    }
    if (arr[i] == -1)
        return 0;
    else
        return 1;
}


int length_of(int _arr[])
{
    int i = 0;
    while (_arr[i] != -1)
        i++;
    return i;
}



// Sample Output
// aromal@NitroAN51555:~/cnlab/Networking-Lab-S6/Link-State-Routing$ ./a.out
// Enter number of nodes:4
// Enter the source node:0
// Enter the cost matrix, For infinity enter 999
// 0 5 8 999
// 5 0 9 2
// 8 9 0 6
// 999 2 6 0
// 0->1 [ distance = 5]
// 0->2 [ distance = 8]
// 0->1 ->3 [ distance = 7]
// image for input graph https://www.freecodecamp.org/news/content/images/2020/06/image-126.png
