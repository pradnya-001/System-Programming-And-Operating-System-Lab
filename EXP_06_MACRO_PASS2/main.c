#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char word[50], mname[50], temp[50];
    int sno, found;
    FILE *fir, *fmnt, *fmdt, *fop;

    // Open all files
    fir = fopen("ir.dat", "r");     // Intermediate code
    fmnt = fopen("mnt.dat", "r");   // Macro Name Table
    fmdt = fopen("mdt.dat", "r");   // Macro Definition Table
    fop  = fopen("output.dat", "w"); // Final expanded output

    // Check file opening
    if (!fir || !fmnt || !fmdt || !fop)
    {
        printf("❌ Error: Could not open one or more files.\n");
        return 1;
    }

    // Process each line from intermediate code
    while (fscanf(fir, "%s", word) != EOF)
    {
        found = 0;
        rewind(fmnt);  // Reset MNT pointer to start

        // Search in MNT for macro name
        while (fscanf(fmnt, "%d%s", &sno, mname) != EOF)
        {
            if (strcmp(word, mname) == 0)
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            // If macro call found, print its definition from MDT
            rewind(fmdt);

            // Find macro start in MDT
            while (fscanf(fmdt, "%d%s", &sno, temp) != EOF)
            {
                if (strcmp(temp, mname) == 0)
                    break;
            }

            // Now copy instructions until MEND
            while (fscanf(fmdt, "%d%s", &sno, temp) != EOF)
            {
                if (strcmp(temp, "MEND") == 0)
                    break;
                fprintf(fop, "%s\n", temp);
            }
        }
        else
        {
            // If not a macro name, copy line as-is
            fprintf(fop, "%s\n", word);
        }
    }

    printf("✅ Macro Pass 2 completed successfully!\n");
    printf("Output saved in 'output.dat'.\n");

    fclose(fir);
    fclose(fmnt);
    fclose(fmdt);
    fclose(fop);
    return 0;
}
