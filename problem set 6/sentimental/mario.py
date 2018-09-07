
# Get the number of blocks from the user, must be between 0 and 23, both included
while True:
    try:
        height = -1     # force the first while loop
        while (int(height) < 0 or int(height) > 23):
            height = input("Height (0-23): ")
            height = int(height)

            if height < 0 or height > 23:
                print("Enter a number between 0 and 23, both included")

        break
    except ValueError:
        print("Enter a number")


# Large loop for each row, nested loops for characters within the row
for i in range(height, 0, -1):

    for j in range(i-1, 0, -1):         # spaces before the hashes before the repeating block
        print(" ", end="")

    for j in range(0, height-i):        # hashes before the repeating block
        print("#", end="")

    print("#  #", end="")               # central, repeating block

    for j in range(0, height-i):        # hashes after the repeating block
        print("#", end="")

    print()                             # newline before starting next tow
