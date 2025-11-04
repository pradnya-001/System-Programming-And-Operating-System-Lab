#include <stdio.h>
#include <string.h>

char mot[6][6] = {"STOP", "ADD", "SUB", "MUL", "MOVER", "MOVEM"}; // Mnemonics
char r[4][6] = {"AREG", "BREG", "CREG", "DREG"};                 // Registers

struct Literal {
    char name; // Literal name
    int add;   // Literal address
} lit[20];

struct Symbol {
    char name[20]; // Symbol name
    int add;       // Symbol address
} sy[20];

int main() {
    char st[10];
    int lc = 0, n1 = 0, ns = 0, i, j = 0, k = 0;
    int litflag = 0, m = 0, tempb, flag = 0;
    FILE *fip, *fir;

    fip = fopen("input.asm", "r");  // Assembler input file
    fir = fopen("ir.dat", "w");     // Intermediate Representation output

    if (!fip) {
        printf("Error: Could not open input.asm\n");
        return 1;
    }

    fscanf(fip, "%s", st);
    while (!feof(fip)) {
    up:
        if (!strcmp(st, "START")) { // Copy starting address
            fscanf(fip, "%d", &lc);
            fprintf(fir, "AD%d C%d\n", 1, lc);
        }

        fscanf(fip, "%s", st); // Read next token

        for (i = 0; i < 6; i++) { // Check mnemonic table
            if (!strcmp(st, mot[i])) {
                fprintf(fir, "IS%d", i + 1); // Found in MOT
                fscanf(fip, "%s", st);

                // Check for register
                for (j = 0; j < 4; j++)
                    if (!strcmp(st, r[j]))
                        fprintf(fir, " %d", j + 1);

                fscanf(fip, "%s", st);
                if (!strcmp(st, "=")) { // If literal
                    k = 1;
                    fscanf(fip, "%s", st);
                    litflag = 0;

                    // Check if literal already exists
                    for (j = 0; j < n1; j++) {
                        if (lit[j].name == st[1]) {
                            litflag = 1;
                            fprintf(fir, " L%d\n", j);
                        }
                    }

                    // If not present, add new literal
                    if (litflag == 0) {
                        lit[n1].name = st[1];
                        fprintf(fir, " L%d\n", n1);
                        n1++;
                    }
                    break;
                } else { // Otherwise symbol
                    flag = 0;
                    for (j = 0; j < ns; j++) {
                        if (!strcmp(sy[j].name, st)) {
                            flag = 1;
                            fprintf(fir, " S%d\n", j);
                            lc++;
                            goto up;
                        }
                    }

                    if (flag == 0) { // Add new symbol
                        strcpy(sy[ns].name, st);
                        fprintf(fir, " S%d\n", ns);
                        ns++;
                        break;
                    }
                }
            }

            // Check for symbol definitions DC / DS
            for (m = 0; m < ns; m++) {
                if (!strcmp(st, sy[m].name)) {
                    fscanf(fip, "%s", st);
                    if (!strcmp(st, "DS")) {
                        fscanf(fip, "%d", &tempb);
                        fprintf(fir, " DL%d %d\n", 2, tempb);
                        sy[m].add = lc;
                        lc += tempb;
                        goto up;
                    } else if (!strcmp(st, "DC")) {
                        fscanf(fip, "%d", &tempb);
                        fprintf(fir, " DL%d %d\n", 1, tempb);
                        sy[m].add = lc;
                        goto up;
                    }
                }
            }
        }

        // Check for END of assembly statements
        if (!strcmp(st, "END")) {
            for (i = 0; i < n1; i++) {
                fprintf(fir, "AD%d C%c\n", 2, lit[i].name);
                lit[i].add = lc;
                lc++;
            }
            break;
        }

        lc++;
        fprintf(fir, "\n");
    }

    // Print tables
    printf("\nDATABASE:\n");
    printf("\nSYMBOL TABLE\nEntry\tSymbol\tAddress\n");
    for (i = 0; i < ns; i++)
        printf("%d\t%s\t%d\n", i, sy[i].name, sy[i].add);

    printf("\nLITERAL TABLE\nEntry\tLiteral\tAddress\n");
    for (i = 0; i < n1; i++)
        printf("%d\t%c\t%d\n", i, lit[i].name, lit[i].add);

    fclose(fir);
    fclose(fip);
    return 0;
}
