while True:
    height = int(input("Height: "))    #convert the string input to an int
    
    if height >= 0 and height <= 23:    #if height is not in bounds 0 to 23, ask again
        break

for row in range(height):
    print(" " * (height-(row+1)), end="")   #the end="" prevents the auto newline
    print("#" * (row+1), end="")
    print("  ", end="")
    print("#" * (row+1))
    