/* Day23 Part1:
 * The puzzle input is a map of the local network connections, in the following form:
 * "ab-cd"
 * Meaning that computer "ab" is connected to computer "cd".
 * Start by looking for sets of three computers where each computer in the set is connected to the other two computers.
 * We want to find how many LANs have at least one computer starting with letter "t".
 * Part2:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct {
   node_t *head;
   int size;
} lan_t;

int parse(FILE *fp, lan_t **lans)
{
   // Read the file line by line
   // Search if one of the two computers are into any lan list and if so add the other (if not already present)
   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   while ((read = getline(&line, &len, fp)) != -1)
   {
      char *computer1 = strtok(line, "-");
      char *computer2 = strtok(NULL, "\n");
      
   }
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

   lan_t *lans = (lan_t *)malloc(sizeof(lan_t));
   lans->head = NULL;
   lans->size = 0;
   int lan_count = parse(fp, &lans);

   fclose(fp);

   return EXIT_SUCCESS;
}