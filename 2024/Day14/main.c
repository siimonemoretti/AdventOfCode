/* Day14 Part1:
 * We're given a set of positions and velocities in this format:
 * "p=X,Y v=vX,vY"
 * We're asked to understand in which quadrant the position will be after 100 seconds have elapsed.
 * To determine the safest area count the number of robots in each quadrant (Robots that are exactly in the middle (horizontally or vertically) don't count as being in any quadrant) and multiply the total of each quadrant to get the "safety factor".
 *
 * Part2:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PART_2

#define TILES_WIDE 101
#define TILES_TALL 103

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

   char line[100];
   int16_t x, y, vx, vy, n_sec = 100;
   uint16_t cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0;
   int tot;
   while (fgets(line, sizeof(line), fp) != NULL)
   {
      // Parse the line
      sscanf(line, "p=%hu,%hu v=%hd,%hd", &x, &y, &vx, &vy);
#ifdef DEBUG
      printf("[0] x=%03hu, y=%03hu, vx=%03hd, vy=%03hd -> ", x, y, vx, vy);
#endif
      // Compute where the position will be after n_sec
      for (int i = 0; i < n_sec; i++)
      {
         if (vx < 0 && abs(vx) > x)
            x = (x + vx + TILES_WIDE) % TILES_WIDE;
         else
            x = (x + vx) % TILES_WIDE;
         if (vy < 0 && abs(vy) > y)
            y = (y + vy + TILES_TALL) % TILES_TALL;
         else
            y = abs(y + vy) % TILES_TALL;
      }
#ifdef DEBUG
      printf("[100] x=%03hu, y=%03hu, vx=%03hd, vy=%03hd\n", x, y, vx, vy);
#endif
      // Count
      if (x < ((TILES_WIDE - 1) / 2) && y < ((TILES_TALL - 1) / 2))
         cnt1++;
      else if (x < ((TILES_WIDE - 1) / 2) && y > ((TILES_TALL - 1) / 2))
         cnt2++;
      else if (x > ((TILES_WIDE - 1) / 2) && y < ((TILES_TALL - 1) / 2))
         cnt3++;
      else if (x > ((TILES_WIDE - 1) / 2) && y > ((TILES_TALL - 1) / 2))
         cnt4++;
   }
   tot = cnt1 * cnt2 * cnt3 * cnt4;
   printf("Safety factor: %d (%d * %d * %d * %d) \n", tot, cnt1, cnt2, cnt3, cnt4);

   fclose(fp);

   return EXIT_SUCCESS;
}