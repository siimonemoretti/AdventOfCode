/* Reading from "input.txt", we need to detect instructions of multiplication.
 * An instruction is valid if is in the form of "mul(X,Y)" where X and Y are at most three digits.
 * We need to sum the results of all valid instructions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int funct(FILE *fp)
{
   int res;
   int i = 0;

   // Let's define the regular expression
   regex_t regex;
   int reti = regcomp(&regex, "mul\\(([0-9]{1,3}),([0-9]{1,3})\\)", REG_EXTENDED);
   if(reti){
      fprintf(stderr, "Could not compile regex\n");
      return 1;
   }

   char ch;
   while ((ch = fgetc(fp)) != EOF)
   {
      if (ch == 'm')
      {
         // If we read an 'm' we obtain a string that is 12 characters long
         fseek(fp, -1, SEEK_CUR);
         char str[13];
         fgets(str, 13, fp);
         // We check if we match the regular expression
         reti = regexec(&regex, str, 0, NULL, 0);
         if (!reti)
         {
            // If we match the regular expression, we extract the two numbers
            int x, y;
            sscanf(str, "mul(%d,%d)", &x, &y);
            res += x * y;
            i++;
         }
         // Restore the file pointer
         fseek(fp, -11, SEEK_CUR);
      }
   }
   printf("Number of valid instructions: %d\n", i);
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