# Day 1

def part1():
   tot = 0
   with open("input.txt", "r") as f:
      line = f.readline().strip()
      # File should only be on one line
      if not line:
         raise AssertionError("Input file is empty")
      ranges = line.split(',')
      for r in ranges:
         try:
            start, end = r.split('-')
         except ValueError:
            raise AssertionError(f"Invalid range format: {r!r}")
         
         for i in range(int(start), int(end)+1):
            if len(str(i)) % 2:
               continue
            # Check if the two parts match
            lh = str(i)[:len(str(i))//2] # Lower half
            uh = str(i)[len(str(i))//2:] # Upper half
            if lh == uh:
               # print(i)
               tot += i
   print(f"Part1: {tot}")

def divisors(n):
   result = []
   for i in range(1, int(n**0.5) + 1):
      if n % i == 0:
         result.append(i)
         if i != n // i:
            result.append(n // i)
   return sorted(result)

def part2():
   tot = 0
   with open("input.txt", "r") as f:
      line = f.readline().strip()
      # File should only be on one line
      if not line:
         raise AssertionError("Input file is empty")
      ranges = line.split(',')
      for r in ranges:
         try:
            start, end = r.split('-')
         except ValueError:
            raise AssertionError(f"Invalid range format: {r!r}")
         
         for i in range(int(start), int(end)+1):
            # Now we check more than 2 repetitions
            s = str(i)
            for d in divisors(len(s)):
               # Divide string in parts
               substr = [s[d*j:d*j+d] for j in range(len(s)//d)]
               # Check if all substrings match
               if all(x == substr[0] for x in substr) and len(substr) != 1:
                  tot += i
                  break
               
   print(f"Part2: {tot}")

if __name__ == "__main__":
   #part1()
   part2()
