/* Day10 Part1:
 * A good hiking trail is as long as possible and has an even, gradual, uphill slope.
 * For all practical purposes, this means that a hiking trail is any path that starts at height 0, ends at height 9, and always increases by a height of exactly 1 at each step.
 * Hiking trails never include diagonal steps
 * A trailhead is any position that starts one or more hiking trails - here, these positions will always have height 0.
 * A trailhead's score is the number of 9-height positions reachable from that trailhead via a hiking trail.
 * Note that if it is possible to reach a 9-height position from multiple trailheads, it is counted just once.
 * 
 * Part2:
 * Now, instead of counting just once, we want to count the number of 9-height positions reachable from each trailhead.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2

typedef struct
{
   int x;
   int y;
} Position;

char **parse(FILE *fp, int *row, int *cols)
{
   char buffer[1024];
   char **map = NULL;
   int i = 0;
   while (fgets(buffer, 1024, fp) != NULL)
   {
      map = realloc(map, (i + 1) * sizeof(*map));
      map[i] = malloc(strlen(buffer) + 1);
      strcpy(map[i], buffer);
      i++;
   }
   *row = i;
   *cols = strlen(map[0]) - 1; // Remove the newline character
   return map;
}

int is_out_of_map(int x, int y, int x_lim, int y_lim)
{
   return x < 0 || x >= x_lim || y < 0 || y >= y_lim;
}

#ifndef PART_2
int find_trails(char **map, int x_lim, int y_lim, int x_pos, int y_pos, char prev_num, unsigned char **visited)
{
   if (is_out_of_map(x_pos, y_pos, x_lim, y_lim))
      return 0;
   if (map[x_pos][y_pos] == '9' && prev_num == '8' && !visited[x_pos][y_pos])
   {
      visited[x_pos][y_pos] = 1;
      return 1;
   }
   if (map[x_pos][y_pos] != prev_num + 1)
      return 0;

   int score = 0;
   score += find_trails(map, x_lim, y_lim, x_pos + 1, y_pos, map[x_pos][y_pos], visited);
   score += find_trails(map, x_lim, y_lim, x_pos - 1, y_pos, map[x_pos][y_pos], visited);
   score += find_trails(map, x_lim, y_lim, x_pos, y_pos + 1, map[x_pos][y_pos], visited);
   score += find_trails(map, x_lim, y_lim, x_pos, y_pos - 1, map[x_pos][y_pos], visited);
   return score;
}
#else
int find_trails(char **map, int x_lim, int y_lim, int x_pos, int y_pos, char prev_num)
{
   if (is_out_of_map(x_pos, y_pos, x_lim, y_lim))
      return 0;
   if (map[x_pos][y_pos] == '9' && prev_num == '8')
      return 1;
   if (map[x_pos][y_pos] != prev_num + 1)
      return 0;

   int score = 0;
   score += find_trails(map, x_lim, y_lim, x_pos + 1, y_pos, map[x_pos][y_pos]);
   score += find_trails(map, x_lim, y_lim, x_pos - 1, y_pos, map[x_pos][y_pos]);
   score += find_trails(map, x_lim, y_lim, x_pos, y_pos + 1, map[x_pos][y_pos]);
   score += find_trails(map, x_lim, y_lim, x_pos, y_pos - 1, map[x_pos][y_pos]);
   return score;
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

   int x = -1, y = -1;
   char **map = parse(fp, &x, &y);
#ifdef DEBUG
   printf("Map size: %dx%d\n", x, y);
   for (int xx = 0; xx < x; xx++)
   {
      printf("%s", map[xx]);
   }
   printf("\n-----------------\n");
#endif
   // Find the zeros
   int trailhead_score = 0;
   for (int xx = 0; xx < x; xx++)
   {
      for (int yy = 0; yy < y; yy++)
      {
         if (map[xx][yy] == '0') // Call recursive function to find the path to 9 if the current position is a trailhead
         {
#ifndef PART_2
            unsigned char **visited = malloc(x * sizeof(*visited));
            for (int i = 0; i < x; i++)
               visited[i] = calloc(y, sizeof(*visited[i]));
            // Call recursive function passing all the nearby positions
            trailhead_score += find_trails(map, x, y, xx + 1, yy, '0', visited);
            trailhead_score += find_trails(map, x, y, xx - 1, yy, '0', visited);
            trailhead_score += find_trails(map, x, y, xx, yy + 1, '0', visited);
            trailhead_score += find_trails(map, x, y, xx, yy - 1, '0', visited);
#else
            trailhead_score += find_trails(map, x, y, xx + 1, yy, '0');
            trailhead_score += find_trails(map, x, y, xx - 1, yy, '0');
            trailhead_score += find_trails(map, x, y, xx, yy + 1, '0');
            trailhead_score += find_trails(map, x, y, xx, yy - 1, '0');
#endif
#ifdef DEBUG
            printf("Trailhead (%d,%d)'s score: %d\n", xx, yy + 1, trailhead_score);
#endif
         }
      }
   }
   printf("Trailhead score: %d\n", trailhead_score);

   fclose(fp);

   return EXIT_SUCCESS;
}