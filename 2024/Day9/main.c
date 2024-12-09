/* Day9 Part1:
 *
 *
 * Part2:
 * We want to compute again the checksum but implementing a different ordering algorithm.
 * Now, we start from the highest ID and we try to move it to the first space that contains it all. If no such space exists, we leave it there.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2

typedef struct
{
   unsigned char file_len;
   unsigned char space_len;
#ifdef PART_2
   unsigned char offset;
#endif
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

      // Write file len
      layout->blocks[layout->block_cnt].file_len = (unsigned char)(ch - '0');
#ifdef PART_2
      // Init offset
      layout->blocks[layout->block_cnt].offset = 0;
#endif
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

#ifndef PART_2
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
#else
unsigned long long compute_checksum(MemoryLayout *ml)
{
   int max_memory_size = ml->block_cnt * 9;
   int *memory = malloc(max_memory_size * sizeof(int));
   // Init memory to all 0
   memset(memory, 0, max_memory_size * sizeof(int));
   unsigned long long checksum = 0;
   int cnt = ml->block_cnt - 1;
   while (cnt > 0)
   {
      // Read its size and search for a non occupied space
      unsigned char size_needed = ml->blocks[cnt].file_len;
      int tmp_offset = 0, found = 0;
      for (int ii = 0; ii < cnt; ii++)
      {
         if (size_needed <= ml->blocks[ii].space_len)
         {
            tmp_offset += ml->blocks[ii].file_len;
            tmp_offset += ml->blocks[ii].offset;
#ifdef DEBUG
            printf("Space found for %d @ %d\n", cnt, tmp_offset + ml->blocks[ii].offset);
#endif
            ml->blocks[ii].space_len -= size_needed;
            ml->blocks[ii].offset += size_needed;
            for (int kk = 0; kk < size_needed; kk++)
            {
               memory[tmp_offset + kk] = cnt;
            }
            found = 1;
            break;
         }
#ifdef DEBUG
         else
         {
            printf("%d space is not enough for %d, he needs %d\n", ml->blocks[ii].space_len, cnt, size_needed);
         }
#endif
         tmp_offset += (ml->blocks[ii].file_len + ml->blocks[ii].space_len + ml->blocks[ii].offset);
      }
      if (!found) // No space was found, so write it to its current position
      {
#ifdef DEBUG
         printf("No space found for %d, writing it @ %d\n", cnt, tmp_offset);
#endif
         for (int kk = 0; kk < size_needed; kk++)
         {
            memory[tmp_offset + kk] = cnt;
         }
      }
      cnt--;
   }

   // Compute checksum
   for (int i = 0; i < max_memory_size; i++)
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