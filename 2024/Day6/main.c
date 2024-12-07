/* Day6 Part1:
 * We're given a file representing a map, composed of: ".", "#", and an arrow (">" || "<" || "^" || "v").
 * A dot represents an empty space and the "#" represents an obstacle.
 * The arrow represents the person's direction: he will move until he has an obstacle in front of him. In that case, he will turn right.
 * We want to know how many *distint* position the person will visit before getting out of the map.
 *
 * Part2:
 * We can now insert 1 obstacle in the map.
 * We want to know how many *distinct* configuration will generate a loop in the person's path.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2

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
   *x = -1;
   *y = -1;
   return;
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

int is_arrow(char c)
{
   return c == '>' || c == '<' || c == '^' || c == 'v';
}

int check_for_loop(char **map, int row, int cols)
{
   // Algorithm: have a matrix of the same size as the map of type unsigned char (8 bits).
   // In this matrix, we store a 1 if in that position we changed the direction of the arrow.
   // If we step in a position that has a 1, we have a loop.
   unsigned char **visited = malloc(row * sizeof(*visited));
   for (int i = 0; i < row; i++)
   {
      visited[i] = malloc(cols * sizeof(**visited));
      memset(visited[i], 0, cols);
   }
   int x = 0, y = 0;
   find_starting_position(map, &x, &y, row, cols);
   if (x == -1 || y == -1)
   {
      printf("Error: starting position not found:\n");
      // Print the map
      for (int i = 0; i < row; i++)
      {
         printf("%s", map[i]);
      }
      getchar();
      return -1;
   }
   int dir; // 0-up, 1-right, 2-down, 3-left
   switch (map[x][y])
   {
   case '>':
      dir = 1;
      break;
   case '<':
      dir = 3;
      break;
   case '^':
      dir = 0;
      break;
   case 'v':
      dir = 2;
      break;
   default:
      printf("Invalid char @ (%d, %d): %c\n", x, y, map[x][y]);
      return -1;
   }
   // We don't want to ruin the original map, so we'll use three variables: x, y and dir
   while (1)
   {
      switch (dir)
      {
      case 0: // Moving upwards
         if (x == 0)
            return 0;
         else if (map[x - 1][y] == '.')
            x--;
         else if (map[x - 1][y] == '#')
         {
            if (visited[x][y] == 1)
               return 1;
            visited[x][y] = 1;
            // If we hit a wall, we want to change direction until we find a free space
            if (map[x][y + 1] == '.')
               dir = 1;
            else if (map[x + 1][y] == '.')
               dir = 2;
            else if (map[x][y - 1] == '.')
               dir = 3;
         }
         else if (is_arrow(map[x - 1][y]))
            x--;
         break;
      case 1: // Moving right
         if (y == cols - 1)
            return 0;
         else if (map[x][y + 1] == '.')
            y++;
         else if (map[x][y + 1] == '#')
         {
            if (visited[x][y] == 1)
               return 1;
            visited[x][y] = 1;
            if (map[x + 1][y] == '.')
               dir = 2;
            else if (map[x][y - 1] == '.')
               dir = 3;
            else if (map[x - 1][y] == '.')
               dir = 0;
         }
         else if (is_arrow(map[x][y + 1]))
            y++;
         break;
      case 2: // Moving down
         if (x == row - 1)
            return 0;
         else if (map[x + 1][y] == '.')
            x++;
         else if (map[x + 1][y] == '#')
         {
            if (visited[x][y] == 1)
               return 1;
            visited[x][y] = 1;
            if (map[x][y - 1] == '.')
               dir = 3;
            else if (map[x - 1][y] == '.')
               dir = 0;
            else if (map[x][y + 1] == '.')
               dir = 1;
         }
         else if (is_arrow(map[x + 1][y]))
            x++;
         break;
      case 3: // Moving left
         if (y == 0)
            return 0;
         else if (map[x][y - 1] == '.')
            y--;
         else if (map[x][y - 1] == '#')
         {
            if (visited[x][y] == 1)
               return 1;
            visited[x][y] = 1;
            if (map[x - 1][y] == '.')
               dir = 0;
            else if (map[x][y + 1] == '.')
               dir = 1;
            else if (map[x + 1][y] == '.')
               dir = 2;
         }
         else if (is_arrow(map[x][y - 1]))
            y--;
         break;
      default:
         printf("Invalid direction: %d\n", dir);
         break;
      }
   }
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

   char **map = NULL;
   int row, cols, tot = 0;
   map = parse_file(fp, &row, &cols);
   printf("Map size: %d, %d\n", row, cols);
#ifndef PART_2
   tot = count_distinct_positions(map, row, cols);
   printf("Total distinct positions visited: %d\n", tot);
#else
   // Loop through the map and insert an obstacle in each empty space
   for (int i = 0; i < row; i++)
   {
      for (int j = 0; j < cols; j++)
      {
         if (map[i][j] == '.')
         {
            // printf("Inserting obstacle @ (%d, %d)\n", i, j);
            map[i][j] = '#';
            tot += check_for_loop(map, row, cols);
            map[i][j] = '.';
         }
      }
   }
   printf("Total distinct configurations that generate a loop: %d\n", tot);
#endif
   for (int i = 0; i < row; i++)
   {
      free(map[i]);
   }
   free(map);

   fclose(fp);

   return EXIT_SUCCESS;
}