# Day 7 - 2025

def part1():
   tot = 0
   with open(f"input.txt", 'r') as f:
      l = f.readline().strip()
      positions = {l.find('S')}
      for line in f:
         for el in range(len(line)):
            if line[el] == '^' and el in positions:
               positions.remove(el)
               positions.add(el+1)
               positions.add(el-1)
               tot += 1
   print(f"Part1: {tot}")

def part2():
   f = open(f"input.txt", 'r').read().splitlines()
   values = [0] * len(f[0])
   values[f[0].find('S')] = 1
   for l in f[1:]:
      for e in range(len(l)):
         if l[e] == '^':
            values[e + 1] += values[e]
            values[e - 1] += values[e]
            values[e] = 0

   tot = sum(values)
   print(f"Part2: {tot}")
   return

if __name__ == "__main__":
   # part1()
   part2()
