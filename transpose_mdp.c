#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions_mdp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Parameter error!!!\n");
        return 1;
    }
    if (strcmp(argv[1], "-d") == 0)
    {
        // Get the password and its length
        int n_d = strlen(argv[2]);
        char *mdp_d = (char *)malloc((n_d + 1) * sizeof(char));
        if (mdp_d == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }
        strcpy(mdp_d, argv[2]);

        // Convert password to uppercase
        for (int i = 0; i < n_d; i++)
        {
            mdp_d[i] = TO_UPPER(mdp_d[i]);
        }
        mdp_d[n_d] = '\0';

        // Generate the order to read and the original order
        int *order_to_read = (int *)malloc(n_d * sizeof(int));
        if (order_to_read == NULL)
        {
            printf("Memory allocation failed\n");
            free(mdp_d);
            return 1;
        }
        convert(mdp_d, order_to_read, n_d);

        int *original_order = (int *)malloc(n_d * sizeof(int));
        if (original_order == NULL)
        {
            printf("Memory allocation failed\n");
            free(mdp_d);
            free(order_to_read);
            return 1;
        }
        org_order(order_to_read, original_order, n_d);

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

        for (int i = 0; i < n_d; i++)
        {
            filenames[i] = (char *)malloc(20 * sizeof(char)); // Allocate sufficient space for filenames
            if (filenames[i] == NULL)
            {
                printf("Memory allocation failed\n");
                for (int j = 0; j < i; j++)
                {
                    free(filenames[j]);
                }
                free(filenames);
                free(mdp_d);
                free(order_to_read);
                free(original_order);
                return 1;
            }
            sprintf(filenames[i], "file%d.txt", i + 1);
        }

        // Rename files back to original order
        rename_files_d(filenames, original_order, n_d);
        // Open files for reading
        FILE **input_files = (FILE **)malloc(n_d * sizeof(FILE *));
        if (input_files == NULL)
        {
            printf("Memory allocation failed\n");
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

        for (int i = 0; i < n_d; i++)
        {
            input_files[i] = fopen(filenames[i], "r");
            if (input_files[i] == NULL)
            {
                printf("Error: Cannot open file %s\n", filenames[i]);
                for (int j = 0; j < i; j++)
                {
                    fclose(input_files[j]);
                }
                free(input_files);
                for (int j = 0; j < n_d; j++)
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

        // Create output file
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

        // Reconstruct the original file
        int ch;
        int files_done = 0;
        while (files_done < n_d)
        {
            files_done = 0;
            for (int i = 0; i < n_d; i++)
            {
                if ((ch = fgetc(input_files[i])) != EOF)
                {
                    fputc(ch, output_file);
                }
                else
                {
                    files_done++;
                }
            }
        }

        // Clean up
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
        char *file_name = argv[1];
        int n = strlen(argv[2]);
        char *mdp = (char *)malloc((n + 1) * sizeof(char));
        if (mdp == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }
        strcpy(mdp, argv[2]);
        // line9-17 save the inputs

        for (int i = 0; i < n; i++)
        {
            mdp[i] = TO_UPPER(mdp[i]);
        }
        mdp[n] = '\0';
        // line 20-24 clean the pw to upper case

        int *new_order = (int *)malloc(n * sizeof(int));
        convert(mdp, new_order, n);
        // new_order has the order the files should be created
        printf("filename is %s, column number is %d, pw is: %s\n", file_name, n, mdp);
        printf("The new order of the files is:");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", new_order[i]);
        }
        process_file_mdp(file_name, n);
        char **filenames = (char **)malloc(n * sizeof(char *));
        for (int i = 0; i < n; i++)
        {
            filenames[i] = (char *)malloc(sizeof(char));
        }

        for (int i = 0; i < n; i++)
        {
            sprintf(filenames[i], "file%d.txt", i + 1);
        }

        int new_position[n];
        convert_again(new_order, new_position, n);
        rename_files(filenames, new_position, n);

        for (int i = 0; i < n; i++)
        {
            free(filenames[i]);
        }
        free(filenames);
        free(mdp);
    }

    return 0;
}