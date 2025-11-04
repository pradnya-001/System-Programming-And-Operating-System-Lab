#include <stdio.h>

int fr[10], cy = 1, frsize, stsize, p[20];  // Frames, counters, and page string

void display() {
    int i;
    printf("\nCycle: %d \tRequested Page: %d\t", cy, p[cy - 1]);
    for (i = 0; i < frsize; i++) {
        if (fr[i] != -1)
            printf("\t%d", fr[i]);
        else
            printf("\t-");
    }
    cy++;
}

int main() {
    int i, j, ch = 0, fs[10];
    int index = 0, k, l, flag1 = 0, flag2 = 0, pf = 0;

    printf("\nEnter frame size (1-10): ");
    scanf("%d", &frsize);
    printf("Enter string length (1-20): ");
    scanf("%d", &stsize);
    printf("Enter %d elements of the string: ", stsize);
    for (i = 0; i < stsize; i++)
        scanf("%d", &p[i]);

    printf("\n--- WITH FIFO ALGORITHM ---\n");

again:
    for (i = 0; i < frsize; i++)
        fr[i] = -1;  // Initialize frames with -1

    for (j = 0; j < stsize; j++) {
        flag1 = 0;
        flag2 = 0;

        // Check if the page is already in the frame
        for (i = 0; i < frsize; i++) {
            if (fr[i] == p[j]) {
                flag1 = 1;
                flag2 = 1;
                break;
            }
        }

        // Check for empty slot in the frame
        if (flag1 == 0) {
            for (i = 0; i < frsize; i++) {
                if (fr[i] == -1) {
                    fr[i] = p[j];
                    flag2 = 1;
                    break;
                }
            }
        }

        // Page fault: page not in frame and no free slot
        if (flag2 == 0) {
            if (ch == 1) {
                // ----- LRU Algorithm -----
                for (i = 0; i < frsize; i++)
                    fs[i] = 0; // Mark all frames as not recently used

                // Check usage pattern of last (frsize - 1) pages
                for (k = j - 1, l = 1; l <= frsize - 1; l++, k--) {
                    for (i = 0; i < frsize; i++) {
                        if (fr[i] == p[k])
                            fs[i] = 1; // Mark recently used
                    }
                }

                // Replace the least recently used (LRU) page
                for (i = 0; i < frsize; i++) {
                    if (fs[i] == 0) {
                        index = i;
                        break;
                    }
                }
                fr[index] = p[j];
            } else {
                // ----- FIFO Algorithm -----
                fr[index] = p[j];
                index++;
                if (index == frsize)
                    index = 0;
            }
            pf++; // Page fault occurred
        }

        display(); // Show current frame status
    }

    printf("\nNumber of page faults: %d\n", pf);

    // After FIFO, toggle to LRU
    if (ch == 0) {
        ch = 1;
        cy = 1;
        pf = 0;
        index = 0;
        printf("\n--- WITH LRU ALGORITHM ---\n");
        goto again;
    }

    return 0;
}
