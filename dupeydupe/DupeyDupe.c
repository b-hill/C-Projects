#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int isDupeyDupe = 0, i, j;
    char lastDupeWord[100];

    // Init output
    strcpy(lastDupeWord, "no dupey dupe :(");

    // Compare each word pair w/ double loop
    for (i = 1; i < argc ; i++)
    {
        for (j = 1; j < argc; j++)
        {
            // Ignore redundant comparison
            if (i == j)
                continue;

            // Update lastDupeWord if necessary
            if (strcmp(argv[i], argv[j]) == 0)
                strcpy(lastDupeWord, argv[i]);
        }

        // Prevents array out of bounds in next code block
        if (i == argc - 1)
            break;

        // Check for adjacent "dupe" instances
        if ((strcmp(argv[i], argv[i+1]) == 0) && (strcmp(argv[i], "dupe") == 0))
            isDupeyDupe = 1;
    }

    // Print Results
    printf("%s\n", lastDupeWord);
    if (isDupeyDupe)
        printf("dupe dupe!\n");

    return 0;
}
