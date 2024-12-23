/* Day17 Part1:
 * We have a three bit computer, with three registers and 8 (2^3) instructions.
 * Each instruction is identified by a 3-bit opcode and followed by a 3-bit operand.
 * A number called the instruction pointer identifies the position in the program from which the next opcode will be read; it starts at 0, pointing at the first 3-bit number in the program. Except for jump instructions, the instruction pointer increases by 2 after each instruction is processed (to move past the instruction's opcode and its operand). If the computer tries to read an opcode past the end of the program, it instead halts.
 * There are two types of operands; each instruction specifies the type of its operand. The value of a literal operand is the operand itself. For example, the value of the literal operand 7 is the number 7. The value of a combo operand can be found as follows:
 * Combo operands 0 through 3 represent literal values 0 through 3.
 * Combo operand 4 represents the value of register A.
 * Combo operand 5 represents the value of register B.
 * Combo operand 6 represents the value of register C.
 * Combo operand 7 is reserved and will not appear in valid programs.
 *
 * The adv instruction (opcode 0) performs division. The numerator is the value in the A register. The denominator is found by raising 2 to the power of the instruction's combo operand. (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.) The result of the division operation is truncated to an integer and then written to the A register.
 * The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand, then stores the result in register B.
 * The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest 3 bits), then writes that value to the B register.
 * The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register is not zero, it jumps by setting the instruction pointer to the value of its literal operand; if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
 * The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then stores the result in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
 * The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. (If a program outputs multiple values, they are separated by commas.)
 * The bdv instruction (opcode 6) works exactly like the adv instruction except that the result is stored in the B register. (The numerator is still read from the A register.)
 * The cdv instruction (opcode 7) works exactly like the adv instruction except that the result is stored in the C register. (The numerator is still read from the A register.)
 *
 * Input is in this format:
 * Register A: 729
 * Register B: 0
 * Register C: 0
 *                (blank line)
 * Program: 0,1,5,4,3,0
 *
 * We have to determine what the program is trying to output.
 * 
 * Part2:
 * Find the lowest integer value for register A in order to obtain the output equal to the input program.
 * For the example above, we have to find the lowest value of register A that will output 0,1,5,4,3,0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2

typedef struct
{
   uint8_t opcode;
   uint8_t operand;
} Instruction;

typedef struct
{
   #ifndef PART_2
   int A;
   #else
   unsigned long long A;
   #endif
   int B;
   int C;
   Instruction *program;
   size_t program_size;
   int *output;
   size_t output_size;
} Computer;

void parse(FILE *fp, Computer *pc)
{
   char *line = NULL;
   size_t len = 0;
   int i = 0;
   // Get registers
   getline(&line, &len, fp);
   #ifndef PART_2
   sscanf(line, "Register A: %d", &pc->A);
   #else
   sscanf(line, "Register A: %llu", &pc->A);
   #endif
   getline(&line, &len, fp);
   sscanf(line, "Register B: %d", &pc->B);
   getline(&line, &len, fp);
   sscanf(line, "Register C: %d", &pc->C);
   // Skip blank line
   getline(&line, &len, fp);
   // Get program
   getline(&line, &len, fp);
   // Consume "Program: "
   line += 9;
   char *token = strtok(line, ",");
   while (token != NULL)
   {
      pc->program = (Instruction *)realloc(pc->program, (i + 1) * sizeof(Instruction));
      sscanf(token, "%hhu", &pc->program[i].opcode);
      token = strtok(NULL, ",");
      sscanf(token, "%hhu", &pc->program[i].operand);
      token = strtok(NULL, ",");
      i++;
   }
   pc->program_size = i;
   line -= 9;
   free(line);
}

int combo_operand(Computer *pc, uint8_t operand)
{
   switch (operand)
   {
   case 0:
      return 0;
   case 1:
      return 1;
   case 2:
      return 2;
   case 3:
      return 3;
   case 4:
      return pc->A;
   case 5:
      return pc->B;
   case 6:
      return pc->C;
   default:
      return -1;
   }
}

void compute(Computer *pc)
{
   uint16_t ip = 0;
   while (ip < pc->program_size)
   {
      #ifdef DEBUG
      printf("A: %d, B: %d, C: %d\n", pc->A, pc->B, pc->C);
      #endif
      switch (pc->program[ip].opcode)
      {
      case 0: // Divide A by 2^COMBO operand and store in A
         pc->A /= (1 << combo_operand(pc, pc->program[ip].operand));
         ip++;
         break;
      case 1: // Bitwise XOR B with operand and store in B
         pc->B ^= pc->program[ip].operand;
         ip++;
         break;
      case 2: // Compute COMBO operand % 8 and store in B
         pc->B = combo_operand(pc, pc->program[ip].operand) % 8;
         ip++;
         break;
      case 3: // Jump to operand if A != 0
         if (pc->A != 0)
         {
            ip = pc->program[ip].operand / 2;
         }
         else
         {
            ip++;
         }
         break;
      case 4: // Bitwise XOR B with C and store in B
         pc->B ^= pc->C;
         ip++;
         break;
      case 5: // Outputs COMBO operand % 8 [Writes into *output]
         pc->output = (int *)realloc(pc->output, (pc->output_size + 1) * sizeof(int));
         pc->output[pc->output_size++] = combo_operand(pc, pc->program[ip].operand) % 8;
         ip++;
         break;
      case 6: // Divide A by 2^COMBO operand and store in B
         pc->B = pc->A / (1 << combo_operand(pc, pc->program[ip].operand));
         ip++;
         break;
      case 7: // Divide A by 2^COMBO operand and store in C
         pc->C = pc->A / (1 << combo_operand(pc, pc->program[ip].operand));
         ip++;
         break;
      default:
         printf("Invalid opcode %d\n", pc->program[ip].opcode);
         return;
      }
   }
}

int cmp(int *a, int *b, size_t size)
{
   for (size_t i = 0; i < size; i++)
   {
      if (a[i] != b[i])
      {
         return 0;
      }
   }
   return 1;
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

   // Initialize computer
   Computer *pc = (Computer *)malloc(sizeof(Computer));
   pc->A = 0;
   pc->B = 0;
   pc->C = 0;
   pc->program = NULL;
   pc->program_size = 0;
   pc->output = NULL;
   pc->output_size = 0;
   // Parse program and registers
   parse(fp, pc);
#ifdef DEBUG
   printf("Register A: %d\n", pc->A);
   printf("Register B: %d\n", pc->B);
   printf("Register C: %d\n", pc->C);
   for (int i = 0; i < pc->program_size; i++)
   {
      printf("Instruction %d: %d %d\n", i, pc->program[i].opcode, pc->program[i].operand);
   }
#endif
   // Execute program
   compute(pc);
   // Print output
   printf("[PART1] Output: ");
   for (size_t i = 0; i < pc->output_size - 1; i++)
   {
      printf("%d,", pc->output[i]);
   }
   printf("%d\n", pc->output[pc->output_size - 1]);


   /* PART 2 */
   // Loop through all possible values of register A, and at the end check whether the output matches the program
   // Decompose the Instruction array into an integer array
   int *program = (int *)malloc(pc->program_size * 2 * sizeof(int));
   for (size_t i = 0; i < pc->program_size; i++)
   {
      program[i * 2] = pc->program[i].opcode;
      program[i * 2 + 1] = pc->program[i].operand;
   }
   for(unsigned long long i = 3337283687; i < 3337283689; i++)
   {
      pc->A = i;
      pc->B = 0;
      pc->C = 0;
      pc->output = NULL;
      pc->output_size = 0;
      compute(pc);
      // Print output
      printf("%llu, Output: ", i);
      for (size_t j = 0; j < pc->output_size - 1; j++)
      {
         printf("%d,", pc->output[j]);
      }
      printf("%d\n", pc->output[pc->output_size - 1]);
      // Compare output array with program array
      if (pc->output_size != (pc->program_size*2))
         continue;
      else if (cmp(program, pc->output, pc->output_size))
      {
         printf("[PART2] Register A: %llu\n", i);
         break;
      }
   }
   
   // Free memory
   free(pc->program);
   free(pc);
   fclose(fp);

   return EXIT_SUCCESS;
}