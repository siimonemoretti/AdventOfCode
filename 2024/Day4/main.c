/* Part 1:
 * We need to detect all the "XMAS" words present in input.txt
 * They can be in all possible positions: horizontal, vertical and diagonal.
 * Also, they can be reversed.
 * Part 2:
 * Now we'll look for both of these patterns:
 * M . S | S . M
 * . A . | . A .
 * S . M | M . S
 * Where the dots can be any character.
 * This is an "X" MAS pattern.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2

/**
 * @brief This function takes the fp to the input file and reads the file line by line, saving it in a matrix
 * that will be returned.
 */
char **read_file(FILE *fp, int *rows, int *cols)
{
   char **matrix = NULL;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   int i = 0;
   while ((read = getline(&line, &len, fp)) != -1)
   {
      matrix = realloc(matrix, (i + 1) * sizeof(char *));
      matrix[i] = malloc((read + 1) * sizeof(char));
      strcpy(matrix[i], line);
      i++;
   }
   *rows = i;
   *cols = len; // Len should be always the same...there should be a check
   free(line);
   return matrix;
}

#ifndef PART_2
int search_xmas(char **matrix, int rows, int cols)
{
   int cnt = 0;
   for (int i = 0; i < rows; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         if (matrix[i][j] == 'X')
         {
            // Horizontal checks
            if (j < (cols - 3) && (matrix[i][j + 1] == 'M' && matrix[i][j + 2] == 'A' && matrix[i][j + 3] == 'S'))
               cnt++;
            if (j > 2 && (matrix[i][j - 1] == 'M' && matrix[i][j - 2] == 'A' && matrix[i][j - 3] == 'S'))
               cnt++;
            // Vertical checks
            if (i < (rows - 3) && (matrix[i + 1][j] == 'M' && matrix[i + 2][j] == 'A' && matrix[i + 3][j] == 'S'))
               cnt++;
            if (i > 2 && (matrix[i - 1][j] == 'M' && matrix[i - 2][j] == 'A' && matrix[i - 3][j] == 'S'))
               cnt++;
            // Diagonal checks
            if (i > 2 && j > 2 && (matrix[i - 1][j - 1] == 'M' && matrix[i - 2][j - 2] == 'A' && matrix[i - 3][j - 3] == 'S'))
               cnt++;
            if (i < (rows - 3) && j < (cols - 3) && (matrix[i + 1][j + 1] == 'M' && matrix[i + 2][j + 2] == 'A' && matrix[i + 3][j + 3] == 'S'))
               cnt++;
            if (i < (rows - 3) && j > 2 && (matrix[i + 1][j - 1] == 'M' && matrix[i + 2][j - 2] == 'A' && matrix[i + 3][j - 3] == 'S'))
               cnt++;
            if (i > 2 && j < (cols - 3) && (matrix[i - 1][j + 1] == 'M' && matrix[i - 2][j + 2] == 'A' && matrix[i - 3][j + 3] == 'S'))
               cnt++;
         }
         // If we find the '\0' character, we stop the search for the current line
         if (matrix[i][j] == '\0')
         {
            break;
         }
      }
   }
   return cnt;
}
#else
int check_diagonal(char c1, char c2)
{
   return (c1 == 'M' && c2 ==  'S') || (c1 == 'S' && c2 == 'M');
}

int search_xmas(char **matrix, int rows, int cols)
{
   int cnt = 0;
   // This time, i and j start from 1 and terminate at rows - 1 and cols - 1 (the latter isn't really useful)
   // This way we don't need "if" statements to check if we are at the borders
   for (int i = 1; i < rows - 1; i++)
   {
      for (int j = 1; j < cols; j++)
      {
         if (matrix[i][j] == '\0')
            break;
         if (matrix[i][j] == 'A')
         {
            if(check_diagonal(matrix[i-1][j-1], matrix[i+1][j+1]) && check_diagonal(matrix[i-1][j+1], matrix[i+1][j-1]))
               cnt++;
         }
      }
   }
   return cnt;
}
#endif

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      printf("Usage: %s <filename>\n", argv[0]);
      return EXIT_FAILURE;
   }
   FILE *fp = fopen(argv[1], "r");
   if (fp == NULL)
   {
      perror("Error opening file");
      return EXIT_FAILURE;
   }
   int rows, cols;
   char **matrix = read_file(fp, &rows, &cols);

   int res = search_xmas(matrix, rows, cols);
   printf("XMAS count: %d\n", res);

   // Free memory
   for (int i = 0; i < rows; i++)
   {
      free(matrix[i]);
   }
   free(matrix);
   fclose(fp);

   return EXIT_SUCCESS;
}