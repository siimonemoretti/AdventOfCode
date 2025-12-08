# Day 5

def part1():
   ranges = True
   range_list = []
   num_valid = 0
   with open(f"input.txt", 'r') as f:
      for l in f:
         # Check if line is empty:
         l = l.strip()
         if l == "":
            ranges = False
            continue

         if ranges:
            try: 
               a, b = l.split('-')
            except:
               raise AssertionError(f"Invalid range line: {l!r}")
            range_list.append( (int(a), int(b)) )
         else:
            num = int(l)
            for r in range_list:
               if num >= r[0] and num <= r[1]:
                  num_valid += 1
                  break
   print(f"Part1: {num_valid}")

def part2():
   unique_fresh = 0
   range_list = []
   with open(f"input.txt", 'r') as f:
      for l in f:
         l = l.strip()
         if l == "":
            break
         try:
            a, b = l.split('-')
         except:
            raise AssertionError(f"Invalid range line: {l!r}")
         range_list.append( (int(a), int(b)) )
   # Sort range list by start value
   range_list.sort()
   # Count unique ranges
   curr_start, curr_end = range_list[0]
   unique_fresh += curr_end - curr_start + 1
   for a,b in range_list[1:]:
      if curr_end < a:
         unique_fresh += b - a + 1
         curr_start, curr_end = a, b
      elif curr_end == a:
         unique_fresh += b - a
         curr_start, curr_end = a, b
      else:
         if curr_end >= b:
            continue
         else:
            unique_fresh += b - curr_end
            curr_end = b

   print(f"Part2: {unique_fresh}")

if __name__ == "__main__":
   #part1()
   part2()
