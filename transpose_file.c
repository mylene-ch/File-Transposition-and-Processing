#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_file(const char *filename, int n)
{
    FILE *input_file;
    FILE **output_files;
    char *line;
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
    for (int i = 0; i < n; i++)
    {
        char output_filename[20];
        sprintf(output_filename, "file%d.txt", i + 1);
        output_files[i] = fopen(output_filename, "w");
        if (output_files[i] == NULL)
        {
            printf("Error of creating file%s\n", output_filename);
            // Close the previously opened files in case of  error
            // and also end the program
            for (int j = 0; j < i; j++)
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
    for (int i = 0; i < strlen(line); i += n)
    {
        for (int j = 0; j < n && (i + j) < strlen(line); j++)
        {
            fputc(line[i + j], output_files[j]);
        }
    }

    // Close the output files and free the memory allocation
    for (int i = 0; i < n; i++)
    {
        fclose(output_files[i]);
    }
    free(output_files);
    free(line);
    fclose(input_file);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error de arguments!!");
        return 1;
    }

    int n = atoi(argv[1]); // to convert the char to int
    if (n <= 0)
    {
        printf("Error: n must be a positive integer\n");
        return 1;
    }

    process_file(argv[2], n);
    return 0;
}