#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_LENGTH 512

// Helper function to check if the levels meet the "safe" criteria
bool is_safe_report(long *levels, size_t count)
{
   if (count < 3)
      return true; // A single level or no levels is always safe.

   bool ascending = levels[1] > levels[0];

   for (size_t i = 1; i < count; i++)
   {
      long diff = levels[i] - levels[i - 1];
      // Check the difference range
      if (abs(diff) < 1 || abs(diff) > 3)
      {
         return false;
      }
      // Check monotonicity
      if ((ascending && diff < 0) || (!ascending && diff > 0))
      {
         return false;
      }
   }
   return true;
}

// Parse a single line of levels and check if it's safe
bool parse_line(const char *line)
{
   long levels[MAX_LENGTH];
   size_t count = 0;

   // Tokenize the line into numbers
   char *token = strtok((char *)line, " ");
   while (token != NULL)
   {
      char *endptr;
      errno = 0;
      long value = strtol(token, &endptr, 10);
      if (errno != 0 || *endptr != '\0')
      {
         fprintf(stderr, "Invalid number in input: %s\n", token);
         return false;
      }
      levels[count++] = value;
      token = strtok(NULL, " ");
   }

   return is_safe_report(levels, count);
}

// Parse the entire file and count safe reports
int parse(FILE *fp)
{
   int safe_count = 0;
   char line[MAX_LENGTH];

   while (fgets(line, sizeof(line), fp))
   {
      // Remove the newline character
      line[strcspn(line, "\n")] = '\0';

      if (strlen(line) == 0)
      {
         continue; // Skip empty lines
      }

      if (parse_line(line))
      {
         safe_count++;
      }
   }

   return safe_count;
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
      perror("Error opening file");
      return EXIT_FAILURE;
   }

   int result = parse(fp);
   printf("Result: %d\n", result);

   fclose(fp);
   return EXIT_SUCCESS;
}
