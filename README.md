# File Transposition and Processing Project

This project consists of C programs designed for file transposition and secure password-based manipulation of file orders. It includes functionality for transposing files based on column arrangements and reordering them using a password as input.

## Features

- **File Transposition**: Split and transpose a file's contents into multiple files based on a specified number of columns.
- **Password-Based Reordering**: Change the order of files according to a password-derived sequence.
- **Original Order Restoration**: Restore files back to their original order using password mapping.
- **Efficient Memory Management**: Uses dynamic memory allocation to handle file processing.

## Files in the Project

### 1. `functions_mdp.c` and `functions_mdp.h`

Contains utility functions for file manipulation and password processing.

#### Key Functions:

- `rename_files(char *files[], const int new_order[], int num_files)`: Renames files based on a new order.
- `process_file_mdp(const char *filename, int n)`: Processes an input file and splits its contents across `n` output files.
- `convert(const char *mdp, int *new_order, int size)`: Generates a new order for files based on a password.
- `convert_again(int *pw, int *new_order, int size)`: Converts a password-based order into another representation.
- `rename_files_d(char *files[], int new_order[], int num_files)`: Restores files to their original order.
- `org_order(int *pw, int *or_order, int size)`: Calculates the original order of files from a password.

### 2. `transpose_file.c`

Implements basic file transposition functionality.

#### Key Functionality:

- `process_file`: Reads an input file and splits its contents into `n` files based on columns.
- Command-line interface for specifying the number of columns and the input file.

### 3. `transpose_mdp.c`

Provides password-based file transposition and reordering.

#### Key Functionality:

- Supports two modes:
  - **Transposition Mode**: Transposes files based on a password sequence.
  - **Detransposition Mode**: Restores the original file from transposed parts using the password.
- Command-line interface for specifying password and operation mode.
- Uses `functions_mdp.c` for core functionality.

## How to Use

### 1. Compile the Programs

Use a C compiler like `gcc` to compile the project files:

```bash
gcc functions_mdp.c transpose_file.c -o transpose_file
gcc functions_mdp.c transpose_mdp.c -o transpose_mdp
```

### 2. Transpose a File

Run the `transpose_file` program:

```bash
./transpose_file <number_of_columns> <input_file>
```

This will split the input file into parts, one for each column.

### 3. Transpose Files Using a Password

Run the `transpose_mdp` program in transposition mode:

```bash
./transpose_mdp <input_file> <password>
```

This uses the password to reorder the transposed files.

### 4. Restore Original File Using a Password

Run the `transpose_mdp` program in detransposition mode:

```bash
./transpose_mdp -d <password>
```

This restores the original file from the transposed files using the password.

## Requirements

- C Compiler (e.g., GCC)
- Basic knowledge of command-line operations

## Limitations

- The program assumes filenames follow a strict naming convention (`file1.txt`, `file2.txt`, etc.).
- Requires sufficient memory for dynamic allocations during processing.



This project demonstrates secure file manipulation techniques using password-based algorithms and efficient memory management.

---

Feel free to explore and extend the functionality for your specific needs!

