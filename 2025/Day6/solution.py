# Day 6
import math

def part1():
   operands = []
   operations = []
   with open(f"input.txt", 'r') as f:
      for l in f:
         l = l.split()
         if l[0].isdigit():
            # Append all elements as new sub-list of operands
            operands.append( [int(x) for x in l] )
         else:
            operations.extend(l)
   results = []
   for op in range(len(operations)):
      if operations[op] == '+':
         results.append(sum(operands[k][op] for k in range(len(operands))))
      elif operations[op] == '*':
         results.append(math.prod(operands[k][op] for k in range(len(operands))))
      else:
         raise AssertionError(f"Unknown operation: {operations[op]!r}")
   # print(f"---\n{operands}\n{operations}\n{results}")
   print(f"Part1: {sum(results)}")

def part2():
   operands = []
   operations = []
   lines = open(f"input.txt", 'r').readlines()

   idx = len(lines[-1]) - 1
   Empty = False
   operands.append( [] )
   while idx >= 0:
      if Empty: # May need to skip the line if we just finished reading
         Empty = False
         idx -= 1
         operands.append( [] )
         continue

      tmp_opd = ''
      for k in range(len(lines)-1):
         if lines[k][idx] != ' ':
            tmp_opd += lines[k][idx]
      operands[-1].append(int(tmp_opd))
      
      if lines[-1][idx] != ' ':
         Empty = True
         operations.append(lines[-1][idx])
      idx -= 1
   
   results = []
   for op in range(len(operations)):
      if operations[op] == '+':
         results.append(sum(operands[op]))
      elif operations[op] == '*':
         results.append(math.prod(operands[op]))
      else:
         raise AssertionError(f"Unknown operation: {operations[op]!r}")
   print(f"Part2: {sum(results)}")

if __name__ == "__main__":
   #part1()
   part2()
