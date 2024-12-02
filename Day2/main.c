/*
 * The engineers are trying to figure out which reports are safe. The Red-Nosed reactor safety systems can only tolerate levels that are either gradually 
 * increasing or gradually decreasing. So, a report only counts as safe if both of the following are true:
 * The levels are either all increasing or all decreasing.
 * Any two adjacent levels differ by at least one and at most three.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LENGTH 1024

int parse(FILE *fp){
   int ret = 0;
   char line[MAX_LENGTH];
   while(fgets(line, sizeof[line], fp)){
      line[strcspn(line, "\n")] = '\0'; // Subs the NL with NULL
      // Tokenize the line
      char *token = strtok(line, " ");
      while(token){
         char *endptr;
         errno = 0;
         long val = strtol(token, &endptr, 10);
         // Check for any errors
         if (errno != 0 || *endptr != '\0'){
            fprintf(stderr, "Invalid input: %s\n", token);
            return;
         }
         token = strtok(NULL, " ");
      }
   }

   return ret;
}

int main(int argc, char **argv){
   if(argc < 2){
      fprintf(stderr, "Usage: %s <input_filename>\n", argv[0]);
      return;
   }
   FILE *fp = fopen(argv[1], "r");
   if (fp == NULL)
   {
      printf("Error opening file %s\n", argv[1]);
      return;
   }

   fprintf(stdout, "%d\n", parse(fp));

   fclose(fp);
}