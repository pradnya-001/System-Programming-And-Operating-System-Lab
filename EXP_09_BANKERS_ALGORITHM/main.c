#include <stdio.h>
#include <stdlib.h>

int Pmax[3], avl, avltemp, maxres, running[3], safe = 0, Pcurr[3];

// Function prototypes
void input();
void requestresource();
void viewresources();
int checksafe();
int checkcompletion(int proc);

int main()
{
    int i;
    for (i = 0; i < 3; i++)
        running[i] = 1; // initialize processes as running

    input();

    while (1)
    {
        system("cls"); // clear screen (use clrscr() in Turbo C)

        // Check for completion
        if (!(running[0] + running[1] + running[2]))
        {
            printf("\n\nCongratulations! All processes completed successfully without deadlock!\n");
            getchar();
            break;
        }
        else
        {
            printf("\n==== Deadlock Prevention using Banker's Algorithm ====\n");
            viewresources();
            requestresource();
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    return 0;
}

void input()
{
    int i;
    printf("Enter total maximum resources available in system: ");
    scanf("%d", &maxres);

    for (i = 0; i < 3; i++)
    {
        printf("\nEnter maximum resources required for P%d: ", i + 1);
        scanf("%d", &Pmax[i]);
        printf("Enter resources currently allocated to P%d: ", i + 1);
        scanf("%d", &Pcurr[i]);
    }

    avl = maxres - (Pcurr[0] + Pcurr[1] + Pcurr[2]);
}

void requestresource()
{
    int proc, req;

    printf("\nEnter Process Number to request resources (1-3): ");
    scanf("%d", &proc);
    proc--;

    if (proc < 0 || proc > 2 || !running[proc])
    {
        printf("\nInvalid process number or process already completed!\n");
        getchar();
        return;
    }

    printf("Currently P%d needs %d more resources.\n", proc + 1, Pmax[proc] - Pcurr[proc]);

    while (1)
    {
        printf("Enter number of resources to allocate to P%d: ", proc + 1);
        scanf("%d", &req);

        if ((req > (Pmax[proc] - Pcurr[proc])) || (req > avl))
        {
            printf("Invalid request! Try again.\n");
        }
        else
        {
            break;
        }
    }

    // Tentatively allocate
    Pcurr[proc] += req;
    avl -= req;

    if (checksafe())
    {
        printf("\nResources successfully allocated.\n");
        if (checkcompletion(proc))
            printf("Process P%d has completed and released its resources.\n", proc + 1);
    }
    else
    {
        // Rollback allocation
        Pcurr[proc] -= req;
        avl += req;
        printf("\nUnsafe allocation! Resources not allocated to avoid deadlock.\n");
    }
}

int checkcompletion(int proc)
{
    if (Pcurr[proc] == Pmax[proc])
    {
        avl += Pmax[proc];
        running[proc] = 0;
        return 1;
    }
    return 0;
}

void viewresources()
{
    int i;
    printf("\nTotal resources in system: %d\n", maxres);
    printf("Currently available resources: %d\n", avl);

    printf("\nMax. resources required by each process:\n");
    for (i = 0; i < 3; i++)
        if (running[i])
            printf("P%d -> %d\n", i + 1, Pmax[i]);

    printf("\nCurrently allocated resources:\n");
    for (i = 0; i < 3; i++)
        if (running[i])
            printf("P%d -> %d\n", i + 1, Pcurr[i]);
}

int checksafe()
{
    int i;
    safe = 0;
    avltemp = avl;

    for (i = 0; i < 3; i++)
    {
        if (running[i])
        {
            if (Pmax[i] - Pcurr[i] <= avltemp)
            {
                avltemp += Pcurr[i];
                safe = 1;
            }
        }
    }
    return safe;
}
