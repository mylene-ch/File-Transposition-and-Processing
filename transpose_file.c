#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to process an input file and distribute its content across n output files
void process_file(const char *filename, int n)
{
    FILE *input_file;      // Pointer to the input file
    FILE **output_files;   // Array of pointers for output files
    char *line;            // Buffer to hold the file content
    long file_size;        // Size of the input file

    // Open the input file in read mode
    input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        printf("Error: Cannot open input file %s\n", filename);
        exit(1);
    }

    // Determine the size of the input file
    fseek(input_file, 0, SEEK_END);  // Move the file pointer to the end
    file_size = ftell(input_file);  // Get the file's size
    rewind(input_file);             // Reset the file pointer to the beginning

    // Allocate memory for the entire file content
    line = (char *)malloc(file_size + 1);  // +1 for null terminator
    if (line == NULL)
    {
        printf("Error: Memory allocation failed\n");
        fclose(input_file);
        exit(1);
    }

    // Read the file content into the buffer
    fread(line, 1, file_size, input_file);
    line[file_size] = '\0';  // Null-terminate the string

    // Allocate memory for output file pointers
    output_files = (FILE **)malloc(n * sizeof(FILE *));
    if (output_files == NULL)
    {
        printf("Error: Memory allocation failed\n");
        free(line);
        fclose(input_file);
        exit(1);
    }

    // Create and open n output files
    for (int i = 0; i < n; i++)
    {
        char output_filename[20];
        sprintf(output_filename, "file%d.txt", i + 1);  // Name files as file1.txt, file2.txt, etc.
        output_files[i] = fopen(output_filename, "w");
        if (output_files[i] == NULL)
        {
            printf("Error: Unable to create file %s\n", output_filename);

            // Cleanup previously opened files to prevent resource leaks
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

    // Distribute the content of the input file into n output files in a round-robin manner
    for (int i = 0; i < strlen(line); i += n)
    {
        for (int j = 0; j < n && (i + j) < strlen(line); j++)
        {
            fputc(line[i + j], output_files[j]);  // Write one character to the corresponding output file
        }
    }

    // Close all output files
    for (int i = 0; i < n; i++)
    {
        fclose(output_files[i]);
    }

    // Free allocated memory and close the input file
    free(output_files);
    free(line);
    fclose(input_file);
}

int main(int argc, char *argv[])
{
    // Validate the number of command-line arguments
    if (argc != 3)
    {
        printf("Error: Incorrect number of arguments. Usage: ./program <n> <filename>\n");
        return 1;
    }

    // Parse the value of n (number of output files) from the first argument
    int n = atoi(argv[1]);
    if (n <= 0)
    {
        printf("Error: n must be a positive integer\n");
        return 1;
    }

    // Call the process_file function with the filename provided in the second argument
    process_file(argv[2], n);

    return 0;  // Indicate successful program termination
}
