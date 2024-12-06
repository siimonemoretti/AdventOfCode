/* Day6 Part1:
 * We're given a file representing a map, composed of: ".", "#", and an arrow (">" || "<" || "^" || "v").
 * A dot represents an empty space and the "#" represents an obstacle.
 * The arrow represents the person's direction: he will move until he has an obstacle in front of him. In that case, he will turn right.
 * We want to know how many *distint* position the person will visit before getting out of the map.
 *
 * Part2:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PART_2

char **parse_file(FILE *fp, int *row, int *cols)
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

void find_starting_position(char **map, int *x, int *y, int row, int cols)
{
   for (int i = 0; i < row; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         if (map[i][j] == '>' || map[i][j] == '<' || map[i][j] == '^' || map[i][j] == 'v')
         {
            *x = i;
            *y = j;
            return;
         }
      }
   }
}

int count_marks(char **map, int row, int cols)
{
   int count = 0;
   for (int i = 0; i < row; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         if (map[i][j] == 'X')
         {
            count++;
         }
      }
   }
   return count;
}

int count_distinct_positions(char **map, int row, int cols)
{
   int x = 0, y = 0;

   find_starting_position(map, &x, &y, row, cols);
   printf("Starting position: (%d, %d)\n", x, y);

   // We'll mark the positions we've visited with a "X"
   int finished = 0;
   while (1)
   {
      switch (map[x][y])
      {
      case '>':
         if (map[x][y + 1] == '\n' || map[x][y + 1] == '\0')
         {
            finished = 1;
            map[x][y] = 'X';
         }
         else if (map[x][y + 1] == '.' || map[x][y + 1] == 'X')
         {
            map[x][y] = 'X';
            y++;
            map[x][y] = '>';
         }
         else if (map[x][y + 1] == '#')
         {
            map[x][y] = 'v';
         }
         break;
      case '<':
         if (y == 0)
         {
            finished = 1;
            map[x][y] = 'X';
         }
         else if (map[x][y - 1] == '.' || map[x][y - 1] == 'X')
         {
            map[x][y] = 'X';
            y--;
            map[x][y] = '<';
         }
         else if (map[x][y - 1] == '#')
         {
            map[x][y] = '^';
         }
         break;
      case '^':
         if (x == 0)
         {
            finished = 1;
            map[x][y] = 'X';
         }
         else if (map[x - 1][y] == '.' || map[x - 1][y] == 'X')
         {
            map[x][y] = 'X';
            x--;
            map[x][y] = '^';
         }
         else if (map[x - 1][y] == '#')
         {
            map[x][y] = '>';
         }
         break;
      case 'v':
         if (x == row - 1)
         {
            finished = 1;
            map[x][y] = 'X';
         }
         else if (map[x + 1][y] == '.' || map[x + 1][y] == 'X')
         {
            map[x][y] = 'X';
            x++;
            map[x][y] = 'v';
         }
         else if (map[x + 1][y] == '#')
         {
            map[x][y] = '<';
         }
         break;
      default:
         printf("Invalid char @ (%d, %d): %c\n", x, y, map[x][y]);
         break;
      }
      if (finished)
         break;
   }

   return count_marks(map, row, cols);
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

   char **map = NULL;
   int row, cols, tot;
   map = parse_file(fp, &row, &cols);
   printf("Map size: %d, %d\n", row, cols);

   tot = count_distinct_positions(map, row, cols);
   printf("Total distinct positions visited: %d\n", tot);

   for (int i = 0; i < row; i++)
   {
      free(map[i]);
   }
   free(map);

   fclose(fp);

   return EXIT_SUCCESS;
}