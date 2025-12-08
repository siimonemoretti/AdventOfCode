# Day 1
import re

pattern = re.compile(r'^[RL]\d+$')
def part1():
   # Counts how many times we land on index 0
   idx = 50
   cnt = 0
   with open("./input.txt", 'r') as f:
      for l in f:
         if not pattern.match(l.strip()):
            raise AssertionError(f"Invalid line: {l!r}")
         else:
            n = int(l[1:])
            idx = ( (idx + n) % 100 ) if l[0] == 'R' else ( (idx + 100 - n) % 100 )
            # Increment counter if idx is 0
            cnt += 1 if idx == 0 else 0
   print(f"Part 1: {cnt}")

def part2():
   # Same as part 1 but now also count when idx moves past 0
   idx = 50
   cnt = 0
   with open("./input.txt", 'r') as f:
      for l in f:
         if not pattern.match(l.strip()):
            raise AssertionError(f"Invalid line: {l!r}")
         else:
            n = int(l[1:])
            q, r = divmod(n, 100)
            cnt += q
            if l[0] == 'R':
               idx += r
               if idx >= 100:
                  idx -= 100
                  cnt += 1
            elif l[0] == 'L':
               if idx != 0:
                  idx -= r
                  if idx < 0:
                     idx += 100
                     cnt += 1
                  elif idx == 0:
                     cnt += 1
               else:
                  idx = 100 - r
            print(f"idx: {idx:02}, cnt: {cnt}")
   print(f"Part 2: {cnt}")

if __name__ == "__main__":
   part1()
   part2()
