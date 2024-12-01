#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void parse_file(int **v1, int **v2, int *size, FILE *fp)
{
   int capacity = 10; // Initial capacity
   *v1 = malloc(capacity * sizeof(int));
   *v2 = malloc(capacity * sizeof(int));

   if (*v1 == NULL || *v2 == NULL)
   {
      printf("Memory allocation failed\n");
      exit(1);
   }

   int x, y, i = 0;
   while (fscanf(fp, "%d %d", &x, &y) == 2)
   {
      if (i >= capacity)
      {
         capacity *= 2;
         *v1 = realloc(*v1, capacity * sizeof(int));
         *v2 = realloc(*v2, capacity * sizeof(int));
         if (*v1 == NULL || *v2 == NULL)
         {
            printf("Memory reallocation failed\n");
            exit(1);
         }
      }
      (*v1)[i] = x;
      (*v2)[i] = y;
      i++;
   }
   *size = i; // Update the size of the arrays
}

void swap(int *x, int *y)
{
   int tmp = *x;
   *x = *y;
   *y = tmp;
}

void bubble_sort(int vec[], int dim)
{
   // Sort the vector in ascending order
   bool swapped;
   for (int i = 0; i < dim - 1; i++)
   {
      swapped = false;
      for (int j = 0; j < dim - 1 - i; j++)
      {
         if (vec[j] > vec[j + 1])
         {
            swap(&vec[j], &vec[j + 1]);
            swapped = true;
         }
      }
      if (!swapped)
         return;
   }
}

int main()
{
   FILE *fp = fopen("input1.txt", "r");
   if (fp == NULL)
   {
      printf("Error opening file\n");
      return 1;
   }

   int *v1 = NULL, *v2 = NULL;
   int size = 0;

   parse_file(&v1, &v2, &size, fp);
   fclose(fp);

   bubble_sort(v1, size);
   bubble_sort(v2, size);

   int distance = 0;
   for (int i = 0; i < size; i++)
   {
      distance += abs(v1[i] - v2[i]);
   }
   printf("Distance is: %d\n", distance);

   // Now calculate the "Similarity score": for each value in v1, search how many times it appears in v2
   // and sum to the total the value itself multiplied by the number of times it appears in v2

   int similarity_score = 0;
   for (int i = 0; i < size; i++)
   {
      // Since vectors are sorted I'll check how many times the value in v1 is present, and then in v2!
      int val = v1[i];
      int factor = 1;
      while (1)
      {
         if (v1[i + 1] == val)
         {
            i++;
            factor++;
         }
         else
         {
            break;
         }
      }
      for (int k = 0; k < size; k++){
         if (v2[k] == val)
            similarity_score += (val * factor);
      }
   }

   printf("Similarity score: %d\n", similarity_score);

   free(v1);
   free(v2);

   return 0;
}
