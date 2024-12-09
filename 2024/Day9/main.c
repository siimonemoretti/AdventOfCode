/* Day9 Part1:
 *
 *
 * Part2:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PART_2

typedef struct
{
   unsigned char file_len;
   unsigned char space_len;
} Block;

typedef struct
{
   Block *blocks;
   int block_size;
   int block_cnt;
} MemoryLayout;

MemoryLayout *parse(FILE *fp)
{
   MemoryLayout *layout = malloc(sizeof(MemoryLayout));
   layout->block_size = 50;
   layout->block_cnt = 0;
   layout->blocks = malloc(layout->block_size * sizeof(Block));

   char ch;
   while ((ch = fgetc(fp)) != EOF)
   {
      // Check if it is a digit
      if (ch < '0' || ch > '9')
      {
         printf("Invalid character: %c \n", ch);
         return NULL;
      }
      // Check if we need to resize the array
      if (layout->block_cnt == layout->block_size)
      {
         layout->block_size *= 2;
         layout->blocks = realloc(layout->blocks, layout->block_size * sizeof(Block));
      }

      layout->blocks[layout->block_cnt].file_len = (unsigned char)(ch - '0');

      ch = fgetc(fp);
      if (ch == EOF)
      {
         // Insert 0 space
         layout->blocks[layout->block_cnt].space_len = 0;
      }
      else
      {
         // Check if it is a digit
         if (ch < '0' || ch > '9')
         {
            printf("Invalid character: %c\n", ch);
            return NULL;
         }
         layout->blocks[layout->block_cnt].space_len = (unsigned char)(ch - '0');
      }

      layout->block_cnt++;
   }

   return layout;
}

unsigned long long compute_checksum(MemoryLayout *ml)
{
   // Calculate the maximum possible memory size needed
   int max_memory_size = ml->block_cnt * 9;
   int *memory = malloc(max_memory_size * sizeof(int));
   if (!memory)
   {
      fprintf(stderr, "Memory allocation failed\n");
      return 0;
   }

   int mem_index = 0;
   int ascending_cnt = 0;
   int descending_cnt = ml->block_cnt - 1;

   while (ascending_cnt <= descending_cnt)
   {
      // Ascending phase: Fill memory with ascending blocks
      for (int i = 0; i < ml->blocks[ascending_cnt].file_len && mem_index < max_memory_size; i++)
      {
         memory[mem_index++] = ascending_cnt;
      }
      ml->blocks[ascending_cnt].file_len = 0;

      // Descending phase: Fill the remaining space in ascending block with descending blocks
      int free_space = ml->blocks[ascending_cnt].space_len;
      while (free_space > 0 && descending_cnt >= ascending_cnt)
      {
         if (ml->blocks[descending_cnt].file_len > 0)
         {
            memory[mem_index++] = descending_cnt;
            ml->blocks[descending_cnt].file_len--;
            free_space--;
         }
         else
         {
            descending_cnt--; // Move to the next descending block
         }
      }

      ascending_cnt++;
   }

   // Compute checksum
   unsigned long long checksum = 0;
   for (int i = 0; i < mem_index; i++)
   {
#ifdef DEBUG
      printf("%d", memory[i]);
#endif
      checksum += memory[i] * i;
   }
#ifdef DEBUG
   printf("\n");
#endif

   free(memory);
   return checksum;
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

   // Parse file into MemoryLayout
   MemoryLayout *ml = parse(fp);
   if (ml == NULL)
      return EXIT_FAILURE;
#ifdef DEBUG
   printf("Block count: %d\n", ml->block_cnt);
   for (int i = 0; i < ml->block_cnt; i++)
   {
      printf("Block %d: file_len: %d, space_len: %d\n", i, ml->blocks[i].file_len, ml->blocks[i].space_len);
   }
#endif
   unsigned long long checksum = compute_checksum(ml);
   printf("Checksum: %llu\n", checksum);

   // Free memory
   free(ml->blocks);
   free(ml);
   fclose(fp);

   return EXIT_SUCCESS;
}