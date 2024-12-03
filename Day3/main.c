/* Reading from "input.txt", we need to detect instructions of multiplication.
 * An instruction is valid if is in the form of "mul(X,Y)" where X and Y are at most three digits.
 * We need to sum the results of all valid instructions.
 * As for part 2, we need to check also for "do()" and "don't()" instructions.
 * The most recent directive specifies whether a multiplication should be performed or not.
 */
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>

#define DONT_STR_LEN 7
#define DO_STR_LEN 4
#define MUL_STR_LEN 12

int find_char_index(char *str, char target)
{
   for (int i = 0; str[i] != '\0'; i++)
   {
      if (str[i] == target)
         return i;
   }
   return -1;
}

int funct(FILE *fp)
{
   int res = 0, reti;
   bool enabled = true;

   regex_t regex, reg_do, reg_dont;
   if (regcomp(&regex, "mul\\(([0-9]{1,3}),([0-9]{1,3})\\)", REG_EXTENDED) ||
       regcomp(&reg_do, "do\\(\\)", REG_EXTENDED) ||
       regcomp(&reg_dont, "don't\\(\\)", REG_EXTENDED))
   {
      fprintf(stderr, "Could not compile regex\n");
      return 1;
   }

   int do_cnt = 0;
   int dont_cnt = 0;
   char ch;
   while ((ch = fgetc(fp)) != EOF)
   {
      long pos = ftell(fp); // Store the current file position
      if (ch == 'd')
      {
         if (pos >= 1)
            fseek(fp, -1, SEEK_CUR); // Rewind safely
         char str[8];                // 7 chars + null terminator
         if (fgets(str, sizeof(str), fp) == NULL)
         {
            fprintf(stderr, "Error reading file\n");
            return EXIT_FAILURE;
         }

         // Check for "do()"
         reti = regexec(&reg_do, str, 0, NULL, 0);
         if (!reti)
         {
            enabled = true;
            do_cnt++;
         }

         // Check for "don't()"
         reti = regexec(&reg_dont, str, 0, NULL, 0);
         if (!reti)
         {
            enabled = false;
            dont_cnt++;
         }

         // Rewind file pointer safely
         fseek(fp, pos - ftell(fp) + 1, SEEK_CUR);
      }
      else if (ch == 'm' && enabled)
      {
         if (pos >= 1)
            fseek(fp, -1, SEEK_CUR); // Rewind safely

         char str[MUL_STR_LEN + 1]; // 12 chars + null terminator
         if (fgets(str, sizeof(str), fp) == NULL)
         {
            fprintf(stderr, "Error reading file\n");
            return EXIT_FAILURE;
         }

         // Check for "mul(%d,%d)"
         reti = regexec(&regex, str, 0, NULL, 0);
         if (!reti)
         {
            int x, y;
            if (sscanf(str, "mul(%d,%d)", &x, &y) == 2)
               res += x * y;
            // Move after the closing parenthesis
            int index = find_char_index(str, ')');
            if (index != -1)
               fseek(fp, index - MUL_STR_LEN + 1, SEEK_CUR);
         }
         else
            fseek(fp, pos - ftell(fp) + 1, SEEK_CUR);
      }
   }

   printf("Do count: %d\n", do_cnt);
   printf("Don't count: %d\n", dont_cnt);
   regfree(&regex);
   return res;
}

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      printf("Usage: %s <filename>\n", argv[0]);
      return 1;
   }
   FILE *fp = fopen(argv[1], "r");
   int result = funct(fp);
   printf("Result is: %d\n", result);
   return EXIT_SUCCESS;
}