/* Day5 Part1:
 * Read file's lines untill the first empty line.
 * Each line contains "X|Y" where X and Y are integers and specify a rule: in a list, X needs to be before Y.
 * After the blank line, there will be lists that need to be checked if they are in the correct order.
 * List's format is "X,Y,Z,...,N" where X, Y, Z, ..., N are integers. The total number of integers is unknown.
 * If a list is in the correct order, we need to sum the central number of that list to a total sum.
 * We'll parse the first part of the file into an array of linked lists.
 * The first element of each list will represent the X. All the consecutive elements will represent the Y's, since there can be many rules with the same X.
 *
 * Part2:
 * Consider now only the lists that are not in the correct order.
 * We need to reorganize the lists so that they are in the correct order.
 * Finally, again, we need to keep track of the sum of the central numbers of the lists.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define PART_2

/**
 * @brief This function parses the file untill the first empty line (and consumes it).
 * @return An array of pointers to the head of each list.
 */
node_t **parse_file(FILE *fp)
{
   node_t **lists = (node_t **)malloc(sizeof(node_t *)); // Allocate memory for the first list

   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   int i = 0, x, y;
   while ((read = getline(&line, &len, fp)) != -1)
   {
      if (line[0] == '\n')
      {
         break;
      }
      // Parse the line
      sscanf(line, "%d|%d", &x, &y);
      // Search for any list with x as the head
      int found = 0;
      for (int j = 0; lists[j] != NULL; j++)
      {
         if (lists[j]->data == x)
         {
            push(lists[j], y);
            found = 1;
            break;
         }
      }
      if (!found)
      {
         // Create a new list
         lists = (node_t **)realloc(lists, (i + 2) * sizeof(node_t *));
         lists[i] = (node_t *)malloc(sizeof(node_t));
         lists[i]->data = x;
         lists[i]->next = (node_t *)malloc(sizeof(node_t));
         lists[i]->next->data = y;
         lists[i]->next->next = NULL;
         i++;
      }
   }
   return lists;
}

#ifndef PART_2
int check_lists(FILE *fp, node_t **lists)
{
   int sum = 0, num;

   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   while ((read = getline(&line, &len, fp)) != -1)
   {
      int restart = 0;
      // Dynamic array
      int size = 0, capacity = 25;
      int *arr = (int *)malloc(capacity * sizeof(int));
      // Tokenize the line with commas
      char *token = strtok(line, ",");
      while (token != NULL)
      {
         num = atoi(token);
         // Check if number is breaking any rules: find its list of rules and see wheter any of the previous numbers are in the list
         for (int i = 0; lists[i] != NULL; i++)
         {
            if (lists[i]->data == num)
            {
               // We have found the list: check if previous numbers of the array are in the list
               for (int j = 0; j < size; j++)
               {
                  if (search(lists[i], arr[j]))
                  {
                     restart = 1;
                     break;
                  }
               }
               if (restart)
                  break;
            }
         }
         if (restart)
            break;
         arr[size++] = num;
         if (size == capacity)
         {
            capacity *= 2;
            arr = (int *)realloc(arr, capacity * sizeof(int));
         }

         // Go to next token
         token = strtok(NULL, ",");
      }
      // Sum the central number if the list is correct
      if (size % 2 == 0 && !restart)
      { // This case should never happen, it's not contemplated in the problem; thus the print statement
         sum += arr[size / 2 - 1];
         printf("List has an even number of elements, maybe there's an error.\n");
      }
      else if (!restart)
      {
         sum += arr[size / 2];
      }
   }

   return sum;
}
#else
int check_lists(FILE *fp, node_t **lists)
{
   int sum = 0, num;

   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   while ((read = getline(&line, &len, fp)) != -1)
   {
      int reorganized = 0;
      // Dynamic array
      int size = 0, capacity = 25;
      int *arr = (int *)malloc(capacity * sizeof(int));
      // Tokenize the line with commas
      char *token = strtok(line, ",");
      while (token != NULL)
      {
         num = atoi(token);
         // Check if number is breaking any rules: in that case reorganize the list and set a flag
         int already_inserted = 0; // Check if the number would break the rules: in that case,
         for (int i = 0; lists[i] != NULL; i++)
         {
            if (lists[i]->data == num)
            {
               // We have found the list: check if previous numbers of the array are in the list
               for (int j = 0; j < size; j++)
               {
                  if (search(lists[i], arr[j]))
                  {
                     // The number breaks the rules: shift the array to the right and insert it
                     for (int k = size; k > j; k--)
                     {
                        arr[k] = arr[k - 1];
                     }
                     arr[j] = num;
                     size++;
                     reorganized = 1;
                     already_inserted = 1;
                     break;
                  }
               }
               if (already_inserted)
                  break;
            }
         }
         if (!already_inserted)
         {
            arr[size++] = num;
         }

         if (size == capacity)
         {
            capacity *= 2;
            arr = (int *)realloc(arr, capacity * sizeof(int));
         }

         // Go to next token
         token = strtok(NULL, ",");
      }
      // Sum the central number if the list was reorganized
      if (reorganized)
      {
         if (size % 2 == 0)
            sum += arr[size / 2 - 1];
         else
            sum += arr[size / 2];
      }
   }

   return sum;
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

   node_t **lists = parse_file(fp);

   // Check the lists and sum the central numbers
   int total_sum = check_lists(fp, lists);
   printf("Total sum: %d\n", total_sum);

   fclose(fp);

   return EXIT_SUCCESS;
}