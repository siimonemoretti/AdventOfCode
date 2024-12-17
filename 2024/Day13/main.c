/* Day13 Part1:
 * Costs 3 tokens to push the A button and 1 token to push the B button.
 * With a little experimentation, you figure out that each machine's buttons are configured to move the claw a specific amount to the right (along the X axis) and a specific amount forward (along the Y axis) each time that button is pressed.
 * Each machine contains one prize; to win the prize, the claw must be positioned exactly above the prize on both the X and Y axes.
 * Input format:
 * """
 * Button A: X+94, Y+34
 * Button B: X+22, Y+67
 * Prize: X=8400, Y=5400
 *         (EMPTY LINE)
 * """
 * Part2:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PART_2

int solve_linear_system(double C, double D, double E, double F, double Gx, double Gy, double *a, double *b)
{
   // Compute the determinant
   double det = C * F - E * D;
   if (det == 0)
   {
      printf("The system is singular (determinant = 0). No unique solution exists.\n");
      return -1;
   }

   // Compute inverse matrix and solutions
   *a = (F * Gx - E * Gy) / det;
   *b = (-D * Gx + C * Gy) / det;

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

   double C, D, E, F, Gx, Gy;
   char buf[100];
#ifdef DEBUG
   printf("Start reading file\n");
#endif
   while (fgets(buf, sizeof(buf), fp) != NULL)
   {
      if (strncmp(buf, "Button A:", 9) == 0)
      {
         sscanf(buf, "Button A: X+%lf, Y+%lf", &C, &D);
      }
      else if (strncmp(buf, "Button B:", 9) == 0)
      {
         sscanf(buf, "Button B: X+%lf, Y+%lf", &E, &F);
      }
      else if (strncmp(buf, "Prize:", 6) == 0)
      {
         sscanf(buf, "Prize: X=%lf, Y=%lf", &Gx, &Gy);
      }
      else if (strcmp(buf, "\n") == 0)
      {
         break;
      }
#ifdef DEBUG
         printf("C=%lf, D=%lf, E=%lf, F=%lf, Gx=%lf, Gy=%lf\n", C, D, E, F, Gx, Gy);
#endif
   }

   fclose(fp);

   return EXIT_SUCCESS;
}