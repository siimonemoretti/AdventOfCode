/* Day11 Part1:
 * As you observe them for a while, you find that the stones have a consistent behavior. Every time you blink, the stones each simultaneously change according to the first applicable rule in this list:
 *    If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
 *    If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. The left half of the digits  are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
 *    If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
 *
 * Part2:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 65
// #define PART_2

inline int is_digit(char c)
{
   return (c >= '0' && c <= '9');
}

int count_stones(char *buf, size_t size)
{
   int n_stones = 0;
   for (int i = 0; i <= size; i++)
   {
      if (buf[i] == ' ' || buf[i] == '\0')
         n_stones++;
   }
   return n_stones;
}

int blink(char *buf, size_t size, int iterations)
{
   if (iterations <= 0)
   {
      return count_stones(buf, size);
   }

   char new_buf[BUF_SIZE];
   char tmp_word[65];
   int tmp_cnt = 0;
   int total_stones = 0;

   for (int i = 0; i <= size; i++)
   {
      if (buf[i] == ' ' || buf[i] == '\0')
      {
         tmp_word[tmp_cnt] = '\0';
         i++;
#ifdef DEBUG
         printf("tmp_word: %s\n", tmp_word);
#endif
         if (tmp_cnt == 1 && atoi(tmp_word) == 0)
         {
            total_stones += blink("1", 1, iterations - 1);
#ifdef DEBUG
#endif
         }
         else if (tmp_cnt & 1)
         { // Odd case
            unsigned long long num = atoll(tmp_word);
            num *= 2024;
            // Convert to string
            sprintf(new_buf, "%llu", num);
            total_stones += blink(new_buf, strlen(new_buf), iterations - 1);
         }
         else
         { // Even case
            int half = tmp_cnt / 2;
            char left[33];
            char right[33];
            for (int j = 0; j < half; j++)
               left[j] = tmp_word[j];
            left[half] = '\0';
            for (int j = half; j < tmp_cnt; j++)
               right[j - half] = tmp_word[j];
            right[tmp_cnt - half] = '\0';
            // Convert them to numbers because if we have 0s at the beginning, we want to remove them
            unsigned long long left_num = atoll(left);
            unsigned long long right_num = atoll(right);
            // Convert to strings
            sprintf(new_buf, "%llu", left_num);
            total_stones += blink(new_buf, strlen(new_buf), iterations - 1);
            sprintf(new_buf, "%llu", right_num);
            total_stones += blink(new_buf, strlen(new_buf), iterations - 1);
         }
         tmp_cnt = 0;
      }
      tmp_word[tmp_cnt++] = buf[i];
   }
   return total_stones;
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

   char buf[BUF_SIZE];
   char word[BUF_SIZE];
   int word_index = 0;
   fgets(buf, BUF_SIZE, fp);
   int n_stones = 0;

   // For each word we find, we loop 25 times and sum the number of stones that we'd obtain
   clock_t begin = clock();
   for (int i = 0; i <= strlen(buf); i++)
   {
      if (buf[i] == ' ' || buf[i] == '\0')
      {
         word[word_index] = '\0';
         word_index = 0;
#ifdef DEBUG
         printf("Word: %s.\n", word);
#endif
         n_stones += blink(word, strlen(word), 25);
      }
      else
      {
         word[word_index++] = buf[i];
      }
   }
   printf("@ END %d stones\n", n_stones);
   clock_t end = clock();
   double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("Time spent: %f\n", time_spent);

   fclose(fp);

   return EXIT_SUCCESS;
}