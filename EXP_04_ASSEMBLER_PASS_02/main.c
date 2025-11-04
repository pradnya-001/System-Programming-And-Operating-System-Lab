#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct symbol {
    char name[20];
    int address;
} sy[20];

struct literal {
    char name;
    int address;
} lit[20];

int main() {
    FILE *fir, *fout;
    char op[3], type;
    int operand, address, ns = 0, nl = 0, lc = 0;

    fir = fopen("ir.dat", "r");
    if (fir == NULL) {
        printf("Error: Cannot open ir.dat\n");
        return 1;
    }

    fout = fopen("output.txt", "w");
    if (fout == NULL) {
        printf("Error: Cannot create output.txt\n");
        fclose(fir);
        return 1;
    }

    // Example initialization of symbol table (from Pass 1 output)
    sy[0].address = 105; // S0
    sy[1].address = 106; // S1
    ns = 2;

    // Example initialization of literal table (from Pass 1 output)
    lit[0].address = 108; // L0
    lit[1].address = 109; // L1
    nl = 2;

    printf("PASS 2 OUTPUT:\n");

    while (fscanf(fir, "%2s%d", op, &operand) != EOF) {
        if (strcmp(op, "AD") == 0) { 
            if (operand == 1) { // START
                fscanf(fir, "%c%d", &type, &address);
                lc = address; // set LC from START
            } else if (operand == 2) { // END
                // END doesn’t produce machine code
            }
        }

        else if (strcmp(op, "IS") == 0) { // Imperative statement
            int reg = 0;
            fscanf(fir, "%d%c%d", &reg, &type, &address);

            int mem_addr = 0;
            if (type == 'S')
                mem_addr = sy[address].address;
            else if (type == 'L')
                mem_addr = lit[address].address;
            else if (type == 'C')
                mem_addr = address;

            fprintf(fout, "%d\t(%d)\t%d\t%d\n", lc, operand, reg, mem_addr);
            printf("%d\t(%d)\t%d\t%d\n", lc, operand, reg, mem_addr);
            lc++;
        }

        else if (strcmp(op, "DL") == 0) { // Declarative statement
            fscanf(fir, "%d%d", &operand, &address);
            if (operand == 1) { // DC
                fprintf(fout, "%d\t--\t--\t%d\n", lc, address);
                printf("%d\t--\t--\t%d\n", lc, address);
                lc++;
            } else if (operand == 2) { // DS
                lc += address;
            }
        }
    }

    fclose(fir);
    fclose(fout);
    printf("\nPass 2 completed successfully. Output stored in output.txt\n");
    return 0;
}
