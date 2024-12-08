/* Day8 Part1:
 * We have a map (2D array of characters) of antennas that are represented by a digit, a lower case letter or an upper case letter.
 * We want to know how many unique locations within the bounds of the map contain an antinode.
 * An antinode occurs at any point that is in line with two antennas and has a distance of X from one antenna and 2X from the other.
 *
 * Part2:
 * Actually, now an antinode occours at any point that is in line with two antennas, regardless of the distance.
 * We want to know how many unique locations within the bounds of the map contain an antinode.
 * For example (antinodes are represented by 'X'):
 * T....#....
 * ...T......
 * .T....#...
 * .........#
 * ..#.......
 * ..........
 * ...#......
 * ..........
 * ....#.....
 * ..........
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2

typedef struct
{
   int x;
   int y;
} Coordinate;

typedef struct
{
   Coordinate *locations;
   int num_antennas;
   char symbol;
} Antennas;

void init_antenna(Antennas *a, int size)
{
   a->locations = malloc(size * sizeof(Coordinate));
   a->num_antennas = 0;
   a->symbol = '+';
}

Antennas *search(Antennas *ant, int cnt, char ch)
{
   for (int i = 0; i < cnt; i++)
   {
      if (ant[i].symbol == ch)
         return &ant[i];
   }
   return NULL;
}

Antennas *parse_antennas(FILE *fp, int *count, int *x_dim, int *y_dim)
{
   int cnt = 0, size = 5, loc_size = 10;
   // Allocate 5 different antennas, to start
   Antennas *ant = malloc(size * sizeof(Antennas));
   // In each antenna allocate 10 coordinates
   for (int i = 0; i < 5; i++)
      init_antenna(&ant[i], loc_size);
   // Loop through each char
   char ch;
   int x = 0, y = 0;
   while ((ch = fgetc(fp)) != EOF)
   {
      if (ch == '.')
      {
         x++;
         continue;
      }
      else if (ch == '\n')
      {
         x = 0;
         y++;
         continue;
      }
      // Search if we already have antennas of this type
      Antennas *tmp = search(ant, cnt, ch);
      if (tmp == NULL)
      {
         if (cnt == size)
         {
            size *= 2;
            ant = realloc(ant, size * sizeof(Antennas));
            for (int i = cnt; i < size; i++)
               init_antenna(&ant[i], loc_size);
         }
         ant[cnt].symbol = ch;
         tmp = &ant[cnt];
         cnt++;
         // Add the location to the antenna
         tmp->locations[tmp->num_antennas].x = x;
         tmp->locations[tmp->num_antennas].y = y;
      }
      else if (tmp != NULL)
      {
         tmp->locations[tmp->num_antennas].x = x;
         tmp->locations[tmp->num_antennas].y = y;
      }
      tmp->num_antennas++;
      x++;
   }
   *x_dim = x;
   *y_dim = y + 1;
   *count = cnt;
   return ant;
}

Coordinate compute_antinode(Coordinate a1, Coordinate a2)
{
   Coordinate antinode;
   int delta_x = a2.x - a1.x;
   antinode.x = a2.x + delta_x;
   int delta_y = a2.y - a1.y;
   antinode.y = a2.y + delta_y;
   return antinode;
}

int fits_in_map(Coordinate a, int x, int y)
{
   return a.x >= 0 && a.x < x && a.y >= 0 && a.y < y;
}

Coordinate *compute_valid_antinodes(Coordinate a1, Coordinate a2, int x, int y, int *tot)
{
   // We start from a dynamic array of coordinates of 50 elements, and we will resize it if needed (Adding 25 elements each time)
   // We compute the delta (both x and y) between the two points and then we compute all the possible antinodes.
   int size = 50;
   Coordinate *antinodes = malloc(size * sizeof(Coordinate));
   *tot = 0;
   int delta_x = a2.x - a1.x;
   int delta_y = a2.y - a1.y;
   // First we compute 
   int go_on = 1, i = 1;
   while (go_on == 1)
   {
      Coordinate a;
      a.x = a1.x + delta_x * i;
      a.y = a1.y + delta_y * i;
      if (fits_in_map(a, x, y))
      {
         if (*tot == size)
         {
            size += 25;
            antinodes = realloc(antinodes, size * sizeof(Coordinate));
         }
         antinodes[*tot] = a;
         (*tot)++;
         i++;
      }
      else
         go_on = 0;
   }
   // Then we compute the antinodes that "start" from a2 and go after a1
   go_on = 1;
   i = 1;
   while (go_on == 1)
   {
      Coordinate a;
      a.x = a2.x - delta_x * i;
      a.y = a2.y - delta_y * i;
      if (fits_in_map(a, x, y))
      {
         if (*tot == size)
         {
            size += 25;
            antinodes = realloc(antinodes, size * sizeof(Coordinate));
         }
         antinodes[*tot] = a;
         (*tot)++;
         i++;
      }
      else
         go_on = 0;
   }
   return antinodes;
}

int is_present(Coordinate *antinodes, int total, Coordinate a)
{
   for (int i = 0; i < total; i++)
   {
      if (antinodes[i].x == a.x && antinodes[i].y == a.y)
         return 1;
   }
   return 0;
}

int count_antinodes(Antennas *antennas, int antenna_cnt, int x, int y)
{
   // We know the size of the matrix: it's given by x and y.
   // For each couple of points, we can compute their antinode and then check whether they are inside the area or not.
   int total = 0;
   // Coordinates of antinodes: dynamic array
   int size = 50;
   Coordinate *antinodes = malloc(size * sizeof(Coordinate));
   for (int i = 0; i < antenna_cnt; i++)
   {
      for (int j = 0; j < antennas[i].num_antennas; j++)
      {
         // For each point, choose a second point and check if the antinodes are inside the map.
         // If they fit inside, check that the point is not already present. If so, add it.
         for (int k = j; k < antennas[i].num_antennas; k++)
         {
            // If the two points are the same, skip
            if (j == k)
               continue;
#ifndef PART_2
            Coordinate a1 = compute_antinode(antennas[i].locations[j], antennas[i].locations[k]);
            if (fits_in_map(a1, x, y))
            {
               if (!is_present(antinodes, total, a1))
               {
                  if (total == size)
                  {
                     size *= 2;
                     antinodes = realloc(antinodes, size * sizeof(Coordinate));
                  }
                  antinodes[total] = a1;
                  total++;
               }
            }
            Coordinate a2 = compute_antinode(antennas[i].locations[k], antennas[i].locations[j]);
            if (fits_in_map(a2, x, y))
            {
               if (!is_present(antinodes, total, a2))
               {
                  if (total == size)
                  {
                     size *= 2;
                     antinodes = realloc(antinodes, size * sizeof(Coordinate));
                  }
                  antinodes[total] = a2;
                  total++;
               }
            }
#else
            // Compute all the antinodes between the two points
            int tot = 0;
            Coordinate *tmp = compute_valid_antinodes(antennas[i].locations[j], antennas[i].locations[k], x, y, &tot);
            // For each antinode, check if it's already present
            for (int l = 0; l < tot; l++)
            {
               if (!is_present(antinodes, total, tmp[l]))
               {
                  if (total == size)
                  {
                     size *= 2;
                     antinodes = realloc(antinodes, size * sizeof(Coordinate));
                  }
                  antinodes[total] = tmp[l];
                  total++;
               }
            }
            free(tmp);
#endif
         }
      }
   }
   #ifdef DEBUG
   // Print a map with the antinodes
   for (int i = 0; i < y; i++)
   {
      for (int j = 0; j < x; j++)
      {
         Coordinate a;
         a.x = j;
         a.y = i;
         if (is_present(antinodes, total, a))
            printf("X");
         else
            printf(".");
      }
      printf("\n");
   }
   #endif
   free(antinodes);
   return total;
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

   // Parse all the "Antennas" from the file
   int antenna_cnt = 0, x = 0, y = 0;
   Antennas *antennas = parse_antennas(fp, &antenna_cnt, &x, &y);
   printf("Dimensions: %d x %d\n", x, y);
   int tot = count_antinodes(antennas, antenna_cnt, x, y);
   printf("Total antinodes: %d\n", tot);

   fclose(fp);

   return EXIT_SUCCESS;
}