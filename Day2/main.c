/*
 * The engineers are trying to figure out which reports are safe. The Red-Nosed reactor safety systems can only tolerate levels that are either gradually
 * increasing or gradually decreasing. So, a report only counts as safe if both of the following are true:
 *    1. The levels are either all increasing or all decreasing.
 *    2. Any two adjacent levels differ by at least one and at most three.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_LENGTH 512

int parse(FILE *fp)
{
   int ret = 0;
   char line[MAX_LENGTH];
   // Ascending represents the order of the values
   bool ascending, first_round, valid;

   while (fgets(line, sizeof(line), fp))
   {
      line[strcspn(line, "\n")] = '\0'; // Subs the NL with NULL
      // Tokenize the line
      char *token = strtok(line, " ");
      // Read first value
      char *endptr;
      errno = 0;
      long old = strtol(token, &endptr, 10);
      // Check for any errors
      if (errno != 0 || *endptr != '\0')
      {
         fprintf(stderr, "Invalid input: %s\n", token);
         return 0;
      }
      token = strtok(NULL, " ");
      first_round = true;
      valid = true;

      while (token)
      {
         errno = 0;
         long val = strtol(token, &endptr, 10);
         // Check for any errors
         if (errno != 0 || *endptr != '\0')
         {
            fprintf(stderr, "Invalid input: %s\n", token);
            return 0;
         }
         token = strtok(NULL, " ");
         if (abs(old - val) < 1 || abs(old - val) > 3)
            valid = false;
         if (first_round)
         {
            first_round = false;
            if (old > val)
               ascending = false;
            else
               ascending = true;
         }
         else
         {
            if (ascending && old > val || !ascending && old < val)
               valid = false;
         }
         old = val;
      }
      if (valid)
         ret++;
   }

   return ret;
}

int main(int argc, char **argv)
{
   if (argc < 2)
   {
      fprintf(stderr, "Usage: %s <input_filename>\n", argv[0]);
      return EXIT_FAILURE;
   }
   FILE *fp = fopen(argv[1], "r");
   if (fp == NULL)
   {
      printf("Error opening file %s\n", argv[1]);
      return EXIT_FAILURE;
   }

   fprintf(stdout, "Result: %d\n", parse(fp));

   fclose(fp);

   return EXIT_SUCCESS;
}
