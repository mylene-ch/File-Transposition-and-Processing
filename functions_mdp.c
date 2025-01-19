#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions_mdp.h"

void rename_files(char *files[], const int new_order[], int num_files)
{
    char temp_name[100];
    // Step 1: Rename each file to a temporary name to avoid overwriting
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "%s.tmp", files[i]);
        if (rename(files[i], temp_name) != 0)
        {
            perror("Error renaming to temporary name");
            return;
        }
    }
    // Step 2: Rename from temporary files to final files based on new_order
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "%s.tmp", files[new_order[i] - 1]);
        if (rename(temp_name, files[i]) != 0)
        {
            perror("Error renaming to final name");
            return;
        }
    }
}

void process_file_mdp(const char *filename, int n)
{
    FILE *input_file;
    FILE **output_files;
    char *line;
    int i, j;
    long file_size;

    // Open input file
    input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        printf("Error: Cannot open input file %s\n", filename);
        exit(1);
    }

    // Get file size
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    rewind(input_file);

    // Allocate memory for the line
    line = (char *)malloc(file_size + 1);
    if (line == NULL)
    {
        printf("Error: Memory allocation failed\n");
        fclose(input_file);
        exit(1);
    }

    // Read the entire file
    fread(line, 1, file_size, input_file);
    line[file_size] = '\0';

    // Allocate array for output file pointers
    output_files = (FILE **)malloc(n * sizeof(FILE *));
    if (output_files == NULL)
    {
        printf("Error: Memory allocation failed\n");
        free(line);
        fclose(input_file);
        exit(1);
    }

    // Create(open) output files
    for (i = 0; i < n; i++)
    {
        char output_filename[20];
        sprintf(output_filename, "file%d.txt", i + 1);
        output_files[i] = fopen(output_filename, "w");
        if (output_files[i] == NULL)
        {
            printf("Error of creating file%s\n", output_filename);
            // Close the previously opened files in case of  error
            // and also end the program
            for (j = 0; j < i; j++)
            {
                fclose(output_files[j]);
            }
            free(output_files);
            free(line);
            fclose(input_file);
            exit(1);
        }
    }

    // Process the input and write to output files
    for (i = 0; i < strlen(line); i += n)
    {
        for (j = 0; j < n && (i + j) < strlen(line); j++)
        {
            fputc(line[i + j], output_files[j]);
        }
    }

    // Close the output files and free the memory allocation
    for (i = 0; i < n; i++)
    {
        fclose(output_files[i]);
    }

    free(output_files);
    free(line);
    fclose(input_file);
}

void convert(const char *mdp, int *new_order, int size)
{
    int file_num = 1, j = 0;
    char min = mdp[0];
    int min_index;
    int saved[size];

    for (int i = 0; i < size; i++)
    {
        saved[i] = 0;
    }

    for (int k = 0; k < size; k++)
    {
        int min_index = -1;
        char min_char = 127;

        // Find the smallest unsaved character
        for (int i = 0; i < size; i++)
        {
            if (!saved[i] && mdp[i] < min_char)
            {
                min_char = mdp[i];
                min_index = i;
            }
        }

        // Mark the smallest character's position with the current order and mark it as saved
        if (min_index != -1)
        {
            new_order[min_index] = file_num++;
            saved[min_index] = 1; // Mark as saved
        }
    }
}

void convert_again(int *pw, int *new_order, int size)
{
    // pw = 4, 1, 3, 2
    // new_order = 2, 4, 3, 1
    // file1 in the 2nd position
    // file2 in the 4th...
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (pw[i] == j + 1)
            {
                new_order[j] = i + 1;
            }
        }
    }
}

void rename_files_d(char *files[], int new_order[], int num_files)
{
    char temp_name[100];
    char final_name[100];

    // Step 1: Rename each file to a temporary name to avoid overwriting
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "file%d.tmp", i + 1);
        if (rename(files[i], temp_name) != 0)
        {
            perror("Error renaming to temporary name");
            return;
        }
    }

    // Step 2: Rename from temporary files back to original positions
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "file%d.tmp", i + 1);         // Access temporary name
        sprintf(final_name, "file%d.txt", new_order[i]); // Generate final name
        if (rename(temp_name, final_name) != 0)
        {
            perror("Error renaming to final name");
            return;
        }
    }
}

void org_order(int *pw, int *or_order, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (pw[j] == i + 1)
            {
                or_order[i] = j + 1;
                break;
            }
        }
    }
}