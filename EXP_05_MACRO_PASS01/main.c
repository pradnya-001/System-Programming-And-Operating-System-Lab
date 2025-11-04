#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *fip, *fmnt, *fmdt, *fir;
    char word[100], macroName[50];
    int sno = 1, mdno = 1;

    // Open input and output files
    fip = fopen("input.dat", "r");     // Input Assembly File
    fmnt = fopen("mnt.dat", "w");      // Macro Name Table
    fmdt = fopen("mdt.dat", "w");      // Macro Definition Table
    fir = fopen("ir.dat", "w");        // Intermediate Representation

    // Check if any file failed to open
    if (fip == NULL) {
        printf("Error: Could not open input.dat. Make sure it exists in the same folder.\n");
        return 1;
    }
    if (!fmnt || !fmdt || !fir) {
        printf("Error: Could not create one of the output files.\n");
        fclose(fip);
        return 1;
    }

    // Process input file line by line
    while (fscanf(fip, "%s", word) != EOF) {
        if (strcmp(word, "MACRO") == 0) {
            // Found start of macro definition
            fscanf(fip, "%s", macroName);

            // Write entry to MNT (Macro Name Table)
            fprintf(fmnt, "%d\t%s\t%d\n", sno, macroName, mdno);
            sno++;

            // First line of MDT (Macro header)
            fprintf(fmdt, "%d\t%s\n", mdno, macroName);
            mdno++;

            // Read and store macro body in MDT until "MEND"
            while (fgets(word, sizeof(word), fip)) {
                if (strncmp(word, "MEND", 4) == 0) {
                    fprintf(fmdt, "%d\tMEND\n", mdno);
                    mdno++;
                    break;
                } else {
                    fprintf(fmdt, "%d\t%s", mdno, word);
                    mdno++;
                }
            }
        } 
        else if (strcmp(word, "END") == 0) {
            fprintf(fir, "END\n");
            break;
        } 
        else {
            // Normal assembly instruction → goes to IR
            fprintf(fir, "%s", word);

            char line[100];
            fgets(line, sizeof(line), fip);
            fprintf(fir, "%s", line);
        }
    }

    // Close all files
    fclose(fip);
    fclose(fmnt);
    fclose(fmdt);
    fclose(fir);

    printf("\n✅ Macro Pass 1 completed successfully!");
    printf("\nGenerated files:\n - mnt.dat\n - mdt.dat\n - ir.dat\n");

    return 0;
}
