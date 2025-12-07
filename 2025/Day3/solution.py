# Day 3

def part1():
   with open("input.txt", 'r') as f:
      banks = f.readlines()
      joltage = 0
      for b in banks:
         b = b.strip()
         max1 = max(b[:len(b)-1])
         idx1 = b.index(max1)
         # print(f"{b}'s first max is {max1} @ pos {idx1}")
         max2 = max(b[idx1+1:])
         idx2 = b.index(max2, idx1+1)
         # print(f"{b}'s second max is {max2} @ pos {idx2}")
         joltage += int(max1)*10 + int(max2)
      print(f"Part 1: {joltage}")
   return

def part2():
   # We must find the largest 12 digit string
   with open("input.txt", 'r') as f:
      banks = f.readlines()
      joltage = 0
      for b in banks:
         b = b.strip()
         last = 0
         str = ''
         for i in range(12):
            substr = b[last:len(b)-11+i]
            max_i = max(substr)
            str += max_i
            idx = substr.index(max_i)
            last += (idx+1)
         joltage += int(str)
      print(f"Part 2: {joltage}")
   return

if __name__ == "__main__":
   #part1()
   part2()
