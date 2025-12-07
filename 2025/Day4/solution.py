# Day 4

def part1():
   access_points = 0
   matrix = open(f"input.txt", 'r').read().splitlines()
   for i in range(len(matrix)):
      for j in range(len(matrix[0])):
         if matrix[i][j] == '@':
            cnt = 0
            for di in range(-1, 2):
               for dj in range(-1, 2):
                  if di == 0 and dj == 0: continue
                  ii = i + di
                  jj = j + dj
                  if ii in range(len(matrix)) and jj in range(len(matrix[ii])) and matrix[ii][jj] == '@':
                     cnt += 1
            if cnt < 4:
               access_points += 1
   print(f"Part1: {access_points}")
   return

def part2():
   matrix = [list(r) for r in open(f"input.txt", 'r').read().splitlines()]
   def remove():
      access_points = 0
      for i in range(len(matrix)):
         for j in range(len(matrix[0])):
            if matrix[i][j] == '@':
               cnt = 0
               for di in range(-1, 2):
                  for dj in range(-1, 2):
                     if di == 0 and dj == 0: continue
                     ii = i + di
                     jj = j + dj
                     if ii in range(len(matrix)) and jj in range(len(matrix[ii])) and matrix[ii][jj] == '@':
                        cnt += 1
               if cnt < 4:
                  matrix[i][j] = 'x'
                  access_points += 1
      return access_points
   
   tot_removed = just_removed = remove()
   while just_removed != 0:
      tot_removed += (just_removed := remove())
   
   print(f"Part2: {tot_removed}")
   return

if __name__ == "__main__":
   #part1()
   part2()
