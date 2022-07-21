#include <stdio.h>

int costMatrix[20][20], n;

struct routers
{
    int distance[20];
    int adjNodes[20];
} node[20];

// function to read the cost matrix
void readCostMatrix()
{
    int i, j;
    printf("\nEnter cost matrix\n");
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            scanf("%d", &costMatrix[i][j]);
            // distance from X to X is 0
            costMatrix[i][i] = 0;
            node[i].distance[j] = costMatrix[i][j]; 
            node[i].adjNodes[j] = j;
        }
    }
}

void calcRoutingTable()
{
    int i, j, k;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            for (k = 0; k < n; ++k)
            {   
                // if the cost of the path from X to Y is less than the cost of the path from X to Z
                if (node[i].distance[j] > costMatrix[i][k] + node[k].distance[j])
                {
                    // substitute with minimum distance
                    node[i].distance[j] = node[i].distance[k] + node[k].distance[j];
                    // substitute with minimum path
                    node[i].adjNodes[j] = k;
                }
            }
        }
    }
}

void displayRoutes()
{
    int i, j;
    for (i = 0; i < n; ++i)
    {
        printf("\nRouter %d\n", i + 1);
        for (j = 0; j < n; ++j)
        {
            printf("Node %d via %d : Distance %d\n", j + 1, node[i].adjNodes[j] + 1, node[i].distance[j]);
        }
        printf("\n");
    }
}

int main()
{
    int i, j;
    printf("Number of nodes: ");
    scanf("%d", &n);
    readCostMatrix();
    calcRoutingTable();
    displayRoutes();

    return 0;
}