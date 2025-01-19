#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TO_UPPER(c) ((c) >= 'a' && (c) <= 'z' ? (c) + 'A' - 'a' : (c))

void rename_files(char *files[], const int new_order[], int num_files);
void process_file_mdp(const char *filename, int n);
void convert(const char *mdp, int *new_order, int size);
void convert_again(int *pw, int *new_order, int size);
void rename_files_d(char *files[], int new_order[], int num_files);
void org_order(int *pw, int *or_order, int size);