from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def printMatrix(Matrix):
    """Print matrix, allowing for tuple indices"""

    for row in Matrix:
        for column in row:
            if type(column) is tuple:
                print(column[0], end="")    # (column[0], column[1]) = (cost, operation)
            else:       # eg. ints or chars
                print(column, end="")
        print(end="\n")


def distances(a, b):
    """Calculate edit distance from a to b"""

    # Record original lengths for later use
    originalLenA = len(a)
    originalLenB = len(b)

    # Prepend a space and update lengths
    a = " " + a
    b = " " + b
    currentLenA = originalLenA + 1
    currentLenB = originalLenB + 1

    # Pad words with spaces to make lengths equal to achieve square matrix (simplifies loop)
    if currentLenA > currentLenB:
        for i in range(currentLenA - currentLenB):
            b = b + " "
        currentLenB = currentLenA
    elif currentLenA < currentLenB:
        for i in range(currentLenB - currentLenA):
            a = a + " "
        currentLenA = currentLenB

    # Matrix = [[], [], ..., []] is a list of lists,
    # so append to matrix (initially an empty list) a series of rows (also initially empty sub-lists) and append to each sub-list a tuple of (cost, operation)
    #   THIS ALGORITHM FAILS ONE CHECK, IT'S IS PROBABLY VERY WRONG
    Matrix = []
    for i in range(currentLenA):

        # Note preceding and leading spaces in strings
        # eg. string1 =  abc
        #     string2 =  123

        # Append a new row (sub-list)
        Matrix.append([])
        cost = 0

        # Compare increasingly longer strings together
        # Eg,  " " vs " ",  " " vs " 1",  " " vs " 12", ...,
        #     " a" vs " ", " a" vs " 1", " a" vs " 12",
        #     etc. until ... " abc  " vs " 123  "
        string1temp = a[0:i+1]                      # each i loop will create one of string1temp = [" ", " a", " ab", " abc", " abc ", " abc  ", etc.]
        print("string1temp = " + string1temp)
        for j in range(currentLenB):                # each j loop will create one of string1temp = [" ", " 1", " 12", " 123", " 123 ", " 123  ", etc.]
            string2temp = b[0:j+1]
            print("string2temp = " + string2temp)

            # Use sets to compare (helps with predicting future incidences of a letter?)
            set1 = set(string1temp)
            set2 = set(string2temp)
            set1orig = set(a)
            set2orig = set(b)

            print("set1.issubset(set2) = " + str(set1.issubset(set2)))
            print("set2.issubset(set1) = " + str(set2.issubset(set1)))
            print("set1orig.issubset(set2orig) = " + str(set1orig.issubset(set2orig)))
            print("set2orig.issubset(set1orig) = " + str(set2orig.issubset(set1orig)))

            # If substrings don't match, figure out cost and operation
            if string1temp != string2temp:

                # Cost
                cost = sum(1 for k,m in zip(string1temp,string2temp) if k!=m and set1.issubset(set2orig) == False and set2.issubset(set1orig) == False and (k not in set2 or m not in set1))
                print('cost1 = ' + str(cost))
                cost = cost + abs(len(string1temp)-len(string2temp))
                print('cost2 = ' + str(cost))

                # Operation
                if len(string1temp) > len(string2temp):
                    opCode = 1 # deletion
                elif len(string1temp) < len(string2temp):
                    opCode = 2 #insertion
                elif len(string1temp) == len(string2temp):
                    opCode = 3 # substitution

                operation = Operation(opCode)
                print("operation = " + str(operation))

                # Combine cost and operation into tuple
                tup = (cost, operation)
                print("i = " + str(i))
                print("j = " + str(j))
                Matrix[i].append(tup)

            # If substrings match, no cost or operation necessary
            else:
                tup = (0,0)
                print("i = " + str(i))
                print("j = " + str(j))
                Matrix[i].append(tup)

    print("Matrix =")
    printMatrix(Matrix)


    # Trim matrix from square matrix to an irregular matrix according to word lengths
    newMatrix = [[0 for x in range(originalLenB+1)] for y in range(originalLenA+1)]
    print("newMatrix =")
    printMatrix(newMatrix)

    for i in range(originalLenB+1):
        for j in range(originalLenA+1):
            newMatrix[j][i] = Matrix[j][i]

    print("newMatrix =")
    printMatrix(newMatrix)

    return newMatrix

# Add headings to the matrix of (cost, operation)'s'
def addHeadings(Matrix, a, b):

    # print("a = " + str(a))
    # for i in range(len(a)+3):
    #     if i < len(a):
    #         print("a["+str(i)+"] = " + str(a[i]))

    # print("b = " + str(b))
    # for i in range(len(b)+3):
    #     if i < len(b):
    #         print("b["+str(i)+"] = " + str(b[i]))


    # Expand non-heading table to include two extra rows and two extra columns
    newLengA = len(a) + 1 + 2       # +1 for index and +2 for two extra columns
    newLengB = len(b) + 1 + 2       # +1 for index and +2 for two extra rows
    MatrixWithHeadings = [[0 for x in range(newLengB)] for y in range(newLengA)]
    print("MatrixWithHeadings =")
    printMatrix(MatrixWithHeadings)

    # Add headings and place the original matrix after the first two columns and rows
    for i in range(newLengB):
        for j in range(newLengA):
            if i < 2 or j < 2:
                if i+j <= 2:                                                # empty spaces in top-left corner
                    MatrixWithHeadings[j][i] = (" ", " ")
                else:
                    if i == 0 and j < newLengA:                             # vertical heading word
                        MatrixWithHeadings[j][i] = (str(a[j-3]), " ")
                    elif i == 1:                                            # vertical heading indices
                        MatrixWithHeadings[j][i] = (str(j-2), " ")
                    elif j == 0 and i < newLengB:                           # horizontal heading word
                        MatrixWithHeadings[j][i] = (str(b[i-3]), " ")
                    elif j == 1:                                            # horizontal heading indices
                        MatrixWithHeadings[j][i] = (str(i-2), " ")

            else:                                                           # embed original matrix
                MatrixWithHeadings[j][i] = Matrix[j-2][i-2]


    print("MatrixWithHeadings =")
    printMatrix(MatrixWithHeadings)

    return MatrixWithHeadings



# a = "Harvard"
# b = "Yale"
# Matrix = distances(a,b)
# printMatrix(Matrix)
