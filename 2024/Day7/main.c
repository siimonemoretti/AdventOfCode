/* Day7 Part1:
 * We're given a file with many lines with this format:
 * X: A B ... C
 * Where X is the final result of the operation between A, B, ..., C
 * The only possible operations are: sum and multiplication
 * The order of the operations is from left to right, not respecting the priority of the operations
 * Eg: 1 + 2 * 3 = 9 because 1 + 2 = 3 and then 3 * 3 = 9
 * We need to determine wheter the result can be computed or not with the given operations
 * We want to compute the sum of all the operations that can be computed
 *
 * Part2:
 * Now, there's a third operator to be considered: the concatenation ('||')
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PART_2

typedef struct
{
   unsigned long long result;
   int n_operands;
   int *operands;
} Operation;

int parse_line(char *line, Operation *op)
{
   char delimiter[] = " ";
   char *ptr = strtok(line, delimiter);
   // Get the result
   if (ptr == NULL)
   {
      printf("Error tokenizing line\n");
      return 0;
   }
   op->result = strtoull(ptr, NULL, 10);
   ptr = strtok(NULL, delimiter);
   // Get the operands
   int size = 8;
   op->operands = (int *)malloc(size * sizeof(int));
   op->n_operands = 0;
   while (ptr)
   {
      if (op->n_operands == size)
      {
         size *= 2;
         op->operands = (int *)realloc(op->operands, size * sizeof(int));
      }
      op->operands[op->n_operands] = atoi(ptr);
      op->n_operands++;
      ptr = strtok(NULL, delimiter);
   }
   return 1;
}

int is_computable(Operation op)
{
   // We need to compute all the 2^op.n_operands possible operations
   // We can use a bitmask to determine which operations to perform
   // 0 -> sum, 1 -> multiplication
   int n_operations = 1 << op.n_operands;
   for (int i = 0; i < n_operations; i++)
   {
      unsigned long long result = op.operands[0];
      for (int j = 0; j < op.n_operands; j++)
      {
         if (i & (1 << j))
         {
            result *= op.operands[j + 1];
         }
         else
         {
            result += op.operands[j + 1];
         }
      }
      if (result == op.result)
      {
         return result;
      }
   }
   return 0;
}

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

   // Parse the file casting the line to an Operation struct
   char *line = NULL;
   size_t len = 0;
   Operation op;
   unsigned long long sum = 0;
   while (getline(&line, &len, fp) != -1)
   {
      // Reset operation struct
      op.n_operands = -1;
      op.result = -1;
      op.operands = NULL;

      if (!parse_line(line, &op))
      {
         printf("Error parsing line: %s\n", line);
         return EXIT_FAILURE;
      }
#ifdef DEBUG
      printf("%llu: ", op.result);
      for (int i = 0; i < op.n_operands; i++)
      {
         printf("%d ", op.operands[i]);
      }
      printf("\n");
#endif
      if (is_computable(op))
      {
         sum += op.result;
      }
   }
   printf("Total sum: %llu\n", sum);

   fclose(fp);

   return EXIT_SUCCESS;
}