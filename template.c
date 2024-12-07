/* Day6 Part1:
 *
 *
 * Part2:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PART_2

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

   fclose(fp);

   return EXIT_SUCCESS;
}