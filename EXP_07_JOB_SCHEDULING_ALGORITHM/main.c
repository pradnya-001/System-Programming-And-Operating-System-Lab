#include <stdio.h>

struct process {
    int p, b, q, a, turn, wait;  // priority, burst, ID, arrival, turnaround, waiting
} proc[40], temp;

int n, i, j, k;

void interchange(int i) {
    temp = proc[i];
    proc[i] = proc[i + 1];
    proc[i + 1] = temp;
}

void input() {
    printf("JOB SCHEDULING ALGORITHMS\n");
    printf("\nEnter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("\nEnter following parameters for P%d", i + 1);
        printf("\nBurst time = ");
        scanf("%d", &proc[i].b);
        printf("Priority    = ");
        scanf("%d", &proc[i].p);
        printf("Arrival time = ");
        scanf("%d", &proc[i].a);
        proc[i].q = i + 1; // Assign process number
    }
}

void output() {
    int sum;
    float ta = 0, wa = 0;
    sum = proc[0].a;

    for (i = 0; i < n; i++) {
        proc[i].wait = sum - proc[i].a;
        sum += proc[i].b;
        proc[i].turn = sum - proc[i].a;
        ta += proc[i].turn;
        wa += proc[i].wait;
    }

    ta /= n;
    wa /= n;

    printf("\nProcess\tBurst\tPriority\tArrival\tTurnaround\tWaiting");
    for (j = 1; j <= n; j++)
        for (i = 0; i < n; i++)
            if (proc[i].q == j)
                printf("\nP%d\t%5d\t%8d\t%8d\t%10d\t%8d",
                       j, proc[i].b, proc[i].p, proc[i].a, proc[i].turn, proc[i].wait);

    printf("\n\nAverage Turnaround Time: %.2f", ta);
    printf("\nAverage Waiting Time: %.2f\n", wa);
}

void ps() {  // Priority Scheduling
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1; j++) {
            if (proc[j].p > proc[j + 1].p)
                interchange(j);
            else if (proc[j].p == proc[j + 1].p && proc[j].b > proc[j + 1].b)
                interchange(j);
        }

    printf("\n\n--- RESULT WITH PRIORITY SCHEDULING ---\n");
    output();
}

void fcfs() {  // FCFS Scheduling
    for (i = 0; i < n; i++)
        for (j = 0; j < n - 1; j++) {
            if (proc[j].a > proc[j + 1].a)
                interchange(j);
            else if (proc[j].a == proc[j + 1].a && proc[j].p < proc[j + 1].p)
                interchange(j);
        }

    printf("\n\n--- RESULT WITH FCFS SCHEDULING ---\n");
    output();
}

void sjf() {  // Shortest Job First
    for (i = 0; i < n; i++)
        for (j = 0; j < n - 1; j++) {
            if (proc[j].b > proc[j + 1].b)
                interchange(j);
            else if (proc[j].b == proc[j + 1].b && proc[j].p < proc[j + 1].p)
                interchange(j);
        }

    printf("\n\n--- RESULT WITH SJF SCHEDULING ---\n");
    output();
}

void rr() {  // Round Robin Scheduling (Quantum = 1)
    int temp1[40], total = 0;
    float ta = 0, wa = 0;

    printf("\n\n--- RESULT WITH ROUND ROBIN SCHEDULING ---");
    printf("\nQuantum time = 1 (arrival time ignored)\nSequence: ");

    for (i = 0; i < n; i++) {
        temp1[i] = proc[i].b;  // Copy burst time
        total += proc[i].b;    // Total burst time
        proc[i].turn = 0;
        proc[i].wait = 0;
    }

    for (j = 0; j < total; j++) {
        for (i = 0; i < n; i++) {
            if (temp1[i] != 0) {
                printf("|P%d|", i + 1);

                // Increment waiting and turnaround for all active processes
                for (k = 0; k < n; k++) {
                    if (temp1[k] != 0) {
                        proc[k].turn++;
                        proc[k].wait++;
                    }
                }

                temp1[i]--;      // One time unit used
                proc[i].wait--;  // Process itself doesn't wait
            }
        }
    }

    printf("\n\nProcess\tBurst\tPriority\tArrival\tTurnaround\tWaiting");
    for (j = 1; j <= n; j++)
        for (i = 0; i < n; i++)
            if (proc[i].q == j) {
                printf("\nP%d\t%5d\t%8d\t%8d\t%10d\t%8d",
                       j, proc[i].b, proc[i].p, proc[i].a, proc[i].turn, proc[i].wait);
                ta += proc[i].turn;
                wa += proc[i].wait;
            }

    printf("\n\nAverage Turnaround Time: %.2f", ta / n);
    printf("\nAverage Waiting Time: %.2f\n", wa / n);
}

int main() {
    input();
    ps();
    fcfs();
    sjf();
    rr();
    return 0;
}
