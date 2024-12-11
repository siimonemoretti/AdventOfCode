# This program generates a char array for the Day 11 challenge
# Given numbers from 0 to 1000, it generates a char array with specific formatting rules:
# 1. If number == 0, then output "1"
# 2. If the number of digits is even, split the number in half and print "X Y" (removing leading zeros)
# 3. If the number of digits is odd, multiply the number by 2024 and print it

# Open the "map.h" file for writing and creating it if it doesn't exist
with open("map.h", "w") as file:
    # Writing the first array (key array)
    file.write("#ifndef MAP_H\n") 
    file.write("#define MAP_H\n\n")
    file.write("#define MAX_SIZE 10000\n\n")
    file.write("const char key[MAX_SIZE][4] = {\n")
    for i in range(10000):
        file.write(f"\"{i}\"")
        if i != 9999:
            file.write(", ")
    file.write("\n};\n\n")

    # Writing the second array (values array)
    file.write("const char values[MAX_SIZE][100] = {\n")
    for i in range(10000):
            # Rule 1: If number == 0
        if i == 0:
            file.write("\"1\"")
            # Rule 2: Number of digits is odd
        elif len(str(i)) % 2 != 0:
            file.write(f"\"{i * 2024}\"")
        else:
            # Rule 2: Number of digits is even
            str_num = str(i)
            half = len(str_num) // 2
            first = str_num[:half]  # First half of the number
            second = str_num[half:]  # Second half of the number
            # Remove leading zeros
            first = first.lstrip("0") or "0"  # Ensure "0" is retained if the result is empty
            second = second.lstrip("0") or "0"
            file.write(f"\"{first} {second}\"")

        # Add a comma for all but the last element
        if i != 9999:
            file.write(", ")
    file.write("\n};\n")
    #int getIndex(const char *find)
    #{
    #   for (int i = 0; i < MAX_SIZE; i++)
    #   {
    #      if (strcmp(key[i], find) == 0)
    #      {
    #         return i;
    #      }
    #   }
    #   return -1;
    #}
    file.write("int getIndex(const char *find)\n")
    file.write("{\n")
    file.write("   for (int i = 0; i < MAX_SIZE; i++)\n")
    file.write("   {\n")
    file.write("      if (strcmp(key[i], find) == 0)\n")
    file.write("      {\n")
    file.write("         return i;\n")
    file.write("      }\n")
    file.write("   }\n")
    file.write("   return -1;\n")
    file.write("}\n")
    file.write("#endif\n")
