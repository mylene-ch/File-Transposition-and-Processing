#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions_mdp.h"

// Rename files based on a new order while preventing overwriting by using temporary names
void rename_files(char *files[], const int new_order[], int num_files)
{
    char temp_name[100];
    
    // Step 1: Rename each file to a temporary name to avoid overwriting
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "%s.tmp", files[i]); // Add a `.tmp` suffix to prevent conflicts
        if (rename(files[i], temp_name) != 0)
        {
            perror("Error renaming to temporary name");
            return; // Exit if renaming fails
        }
    }

    // Step 2: Rename temporary files to final names based on the new order
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "%s.tmp", files[new_order[i] - 1]); // Map old names to the new order
        if (rename(temp_name, files[i]) != 0)
        {
            perror("Error renaming to final name");
            return; // Exit if renaming fails
        }
    }
}

// Process an input file, splitting its content into n output files
void process_file_mdp(const char *filename, int n)
{
    FILE *input_file;
    FILE **output_files;
    char *line;
    int i, j;
    long file_size;

    // Open the input file for reading
    input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        printf("Error: Cannot open input file %s\n", filename);
        exit(1);
    }

    // Calculate the file size to allocate sufficient memory
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    rewind(input_file);

    // Allocate memory for storing the file content
    line = (char *)malloc(file_size + 1);
    if (line == NULL)
    {
        printf("Error: Memory allocation failed\n");
        fclose(input_file);
        exit(1);
    }

    // Read the entire file into memory
    fread(line, 1, file_size, input_file);
    line[file_size] = '\0'; // Null-terminate the string

    // Allocate memory for file pointers for the output files
    output_files = (FILE **)malloc(n * sizeof(FILE *));
    if (output_files == NULL)
    {
        printf("Error: Memory allocation failed\n");
        free(line);
        fclose(input_file);
        exit(1);
    }

    // Open n output files for writing
    for (i = 0; i < n; i++)
    {
        char output_filename[20];
        sprintf(output_filename, "file%d.txt", i + 1); // Generate filenames like file1.txt
        output_files[i] = fopen(output_filename, "w");
        if (output_files[i] == NULL)
        {
            printf("Error creating file %s\n", output_filename);

            // Close previously opened files and free resources on error
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

    // Distribute the content of the input file across the n output files
    for (i = 0; i < strlen(line); i += n)
    {
        for (j = 0; j < n && (i + j) < strlen(line); j++)
        {
            fputc(line[i + j], output_files[j]); // Write one character to each file in round-robin order
        }
    }

    // Close all output files and free allocated memory
    for (i = 0; i < n; i++)
    {
        fclose(output_files[i]);
    }

    free(output_files);
    free(line);
    fclose(input_file);
}

// Create a new order for processing based on the minimum values in the input array
void convert(const char *mdp, int *new_order, int size)
{
    int file_num = 1;
    int saved[size];

    // Initialize the saved array to track processed indices
    for (int i = 0; i < size; i++)
    {
        saved[i] = 0;
    }

    // Find the smallest unsaved character and assign a new order
    for (int k = 0; k < size; k++)
    {
        int min_index = -1;
        char min_char = 127; // ASCII maximum

        for (int i = 0; i < size; i++)
        {
            if (!saved[i] && mdp[i] < min_char) // Check unsaved characters only
            {
                min_char = mdp[i];
                min_index = i;
            }
        }

        if (min_index != -1)
        {
            new_order[min_index] = file_num++; // Assign the order
            saved[min_index] = 1;             // Mark as saved
        }
    }
}

// Adjust file order based on the password
void convert_again(int *pw, int *new_order, int size)
{
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

// Reverse the order of file renaming to the original state
void rename_files_d(char *files[], int new_order[], int num_files)
{
    char temp_name[100];
    char final_name[100];

    // Step 1: Rename each file to a temporary name
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "file%d.tmp", i + 1); // Use a temporary name
        if (rename(files[i], temp_name) != 0)
        {
            perror("Error renaming to temporary name");
            return;
        }
    }

    // Step 2: Rename temporary files back to the original positions
    for (int i = 0; i < num_files; i++)
    {
        sprintf(temp_name, "file%d.tmp", i + 1);
        sprintf(final_name, "file%d.txt", new_order[i]);
        if (rename(temp_name, final_name) != 0)
        {
            perror("Error renaming to final name");
            return;
        }
    }
}

// Reconstruct the original order of files based on their initial positions
void org_order(int *pw, int *or_order, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (pw[j] == i + 1)
            {
                or_order[i] = j + 1; // Assign the original order
                break;
            }
        }
    }
}
