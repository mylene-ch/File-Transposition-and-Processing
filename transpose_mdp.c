#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions_mdp.h"

int main(int argc, char *argv[])
{
    // Check for correct number of arguments
    if (argc != 3)
    {
        printf("Parameter error!!!\n"); // Ensure user provides exactly 2 arguments
        return 1; // Exit with error
    }

    // Check if the first argument is `-d` for detransposition mode
    if (strcmp(argv[1], "-d") == 0)
    {
        // Extract the password from the second argument and calculate its length
        int n_d = strlen(argv[2]);
        char *mdp_d = (char *)malloc((n_d + 1) * sizeof(char)); // Allocate memory for password
        if (mdp_d == NULL)
        {
            printf("Memory allocation failed\n");
            return 1; // Exit if memory allocation fails
        }
        strcpy(mdp_d, argv[2]); // Copy password to `mdp_d`

        // Convert password to uppercase
        for (int i = 0; i < n_d; i++)
        {
            mdp_d[i] = TO_UPPER(mdp_d[i]); // Convert each character to uppercase
        }
        mdp_d[n_d] = '\0'; // Null-terminate the string

        // Generate the order to read files based on the password
        int *order_to_read = (int *)malloc(n_d * sizeof(int)); // Order for reading files
        if (order_to_read == NULL)
        {
            printf("Memory allocation failed\n");
            free(mdp_d); // Free previously allocated memory
            return 1;
        }
        convert(mdp_d, order_to_read, n_d); // Generate the order from the password

        // Generate the original order of files
        int *original_order = (int *)malloc(n_d * sizeof(int));
        if (original_order == NULL)
        {
            printf("Memory allocation failed\n");
            free(mdp_d);
            free(order_to_read);
            return 1;
        }
        org_order(order_to_read, original_order, n_d); // Generate original order based on reading order

        // Print the new and original orders for debugging
        printf("The new order of the files is: ");
        for (int i = 0; i < n_d; i++)
        {
            printf("%d ", order_to_read[i]);
        }
        printf("\n");

        printf("The original order of the files is: ");
        for (int i = 0; i < n_d; i++)
        {
            printf("%d ", original_order[i]);
        }
        printf("\n");

        // Allocate memory for filenames
        char **filenames = (char **)malloc(n_d * sizeof(char *));
        if (filenames == NULL)
        {
            printf("Memory allocation failed\n");
            free(mdp_d);
            free(order_to_read);
            free(original_order);
            return 1;
        }

        // Generate filenames (e.g., file1.txt, file2.txt)
        for (int i = 0; i < n_d; i++)
        {
            filenames[i] = (char *)malloc(20 * sizeof(char)); // Allocate sufficient space for filenames
            if (filenames[i] == NULL)
            {
                printf("Memory allocation failed\n");
                for (int j = 0; j < i; j++) // Free previously allocated filenames
                {
                    free(filenames[j]);
                }
                free(filenames);
                free(mdp_d);
                free(order_to_read);
                free(original_order);
                return 1;
            }
            sprintf(filenames[i], "file%d.txt", i + 1); // Create filenames like file1.txt
        }

        // Rename files back to their original order
        rename_files_d(filenames, original_order, n_d);

        // Open the renamed files for reading
        FILE **input_files = (FILE **)malloc(n_d * sizeof(FILE *));
        if (input_files == NULL)
        {
            printf("Memory allocation failed\n");
            for (int i = 0; i < n_d; i++) // Free filenames
            {
                free(filenames[i]);
            }
            free(filenames);
            free(mdp_d);
            free(order_to_read);
            free(original_order);
            return 1;
        }

        for (int i = 0; i < n_d; i++)
        {
            input_files[i] = fopen(filenames[i], "r"); // Open each file for reading
            if (input_files[i] == NULL)
            {
                printf("Error: Cannot open file %s\n", filenames[i]);
                for (int j = 0; j < i; j++) // Close already opened files
                {
                    fclose(input_files[j]);
                }
                free(input_files);
                for (int j = 0; j < n_d; j++) // Free filenames
                {
                    free(filenames[j]);
                }
                free(filenames);
                free(mdp_d);
                free(order_to_read);
                free(original_order);
                return 1;
            }
        }

        // Create an output file for the detransposed data
        FILE *output_file = fopen("detransposition.txt", "w");
        if (output_file == NULL)
        {
            printf("Error: Cannot create output file\n");
            for (int i = 0; i < n_d; i++)
            {
                fclose(input_files[i]);
            }
            free(input_files);
            for (int i = 0; i < n_d; i++)
            {
                free(filenames[i]);
            }
            free(filenames);
            free(mdp_d);
            free(order_to_read);
            free(original_order);
            return 1;
        }

        // Reconstruct the original file from the input files
        int ch;
        int files_done = 0;
        while (files_done < n_d) // Continue until all files are processed
        {
            files_done = 0;
            for (int i = 0; i < n_d; i++) // Read character by character from each file
            {
                if ((ch = fgetc(input_files[i])) != EOF) // If not EOF, write to output
                {
                    fputc(ch, output_file);
                }
                else
                {
                    files_done++; // Count files that have reached EOF
                }
            }
        }

        // Cleanup: Close files and free memory
        fclose(output_file);
        for (int i = 0; i < n_d; i++)
        {
            fclose(input_files[i]);
        }
        free(input_files);
        for (int i = 0; i < n_d; i++)
        {
            free(filenames[i]);
        }
        free(filenames);
        free(mdp_d);
        free(order_to_read);
        free(original_order);

        printf("File successfully detransposed to 'detransposition.txt'.\n");
    }
    else
    {
        // Process for regular transposition
        char *file_name = argv[1]; // Input file name
        int n = strlen(argv[2]);  // Length of the password
        char *mdp = (char *)malloc((n + 1) * sizeof(char)); // Allocate memory for the password
        if (mdp == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }
        strcpy(mdp, argv[2]); // Copy password

        // Convert password to uppercase
        for (int i = 0; i < n; i++)
        {
            mdp[i] = TO_UPPER(mdp[i]);
        }
        mdp[n] = '\0';

        int *new_order = (int *)malloc(n * sizeof(int)); // Allocate memory for file order
        convert(mdp, new_order, n); // Generate new order

        // Debug output
        printf("filename is %s, column number is %d, pw is: %s\n", file_name, n, mdp);
        printf("The new order of the files is:");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", new_order[i]);
        }

        process_file_mdp(file_name, n); // Process the file into n parts

        // Memory cleanup
        free(new_order);
        free(mdp);
    }

    return 0;
}
