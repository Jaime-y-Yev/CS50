from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def printMatrix(Matrix):
    for row in Matrix:
        for column in row:
            if type(column) is tuple:
                print(column[0], end="")    # index 0 to only print cost (no operation)
            else:       # ints or chars
                print(column, end="")
        print(end="\n")


def distances(a, b):
    """Calculate edit distance from a to b"""

    originalLenA = len(a)
    originalLenB = len(b)

    a = " " + a
    b = " " + b

    currentLenA = originalLenA + 1
    currentLenB = originalLenB + 1

    if currentLenA > currentLenB:
        for i in range(currentLenA - currentLenB):
            b = b + " "
        currentLenB = currentLenA
    elif currentLenA < currentLenB:
        for i in range(currentLenB - currentLenA):
            a = a + " "
        currentLenA = currentLenB

    Matrix = []

    for i in range(currentLenA):
        Matrix.append([])
        cost = 0
        string1temp = a[0:i+1]
        print("string1temp = " + string1temp)
        for j in range(currentLenB):
            string2temp = b[0:j+1]
            print("string2temp = " + string2temp)

            set1 = set(string1temp)
            set2 = set(string2temp)

            set1orig = set(a)
            set2orig = set(b)

            print("set1.issubset(set2) = " + str(set1.issubset(set2)))
            print("set2.issubset(set1) = " + str(set2.issubset(set1)))

            print("set1orig.issubset(set2orig) = " + str(set1orig.issubset(set2orig)))
            print("set2orig.issubset(set1orig) = " + str(set2orig.issubset(set1orig)))

            if string1temp != string2temp:
                cost = sum(1 for k,m in zip(string1temp,string2temp) if k!=m and set1.issubset(set2orig) == False and set2.issubset(set1orig) == False and (k not in set2 or m not in set1))
                print('cost1 = ' + str(cost))

                cost = cost + abs(len(string1temp)-len(string2temp))
                print('cost2 = ' + str(cost))

                if len(string1temp) > len(string2temp):
                    opCode = 1 # deletion
                elif len(string1temp) < len(string2temp):
                    opCode = 2 #insertion
                elif len(string1temp) == len(string2temp):
                    opCode = 3 # substitution

                operation = Operation(opCode)
                print("operation = " + str(operation))

                tup = (cost, operation)
                print("i = " + str(i))
                print("j = " + str(j))
                Matrix[i].append(tup)

            else:
                tup = (0,0)
                print("i = " + str(i))
                print("j = " + str(j))
                Matrix[i].append(tup)


    newMatrix = [[0 for x in range(originalLenB+1)] for y in range(originalLenA+1)]
    print("newMatrix =")
    printMatrix(newMatrix)


    for i in range(originalLenB+1):
        for j in range(originalLenA+1):
            newMatrix[j][i] = Matrix[j][i]

    print("newMatrix =")
    printMatrix(newMatrix)


    # newMatrix2 = [[0 for x in range(originalLenB+3)] for y in range(originalLenA+3)]
    # print("newMatrix2 =")
    # printMatrix(newMatrix2)


    # for i in range(originalLenB+3):
    #     for j in range(originalLenA+3):
    #         if i < 2 or j < 2:
    #             if i+j <= 2:
    #                 newMatrix2[j][i] = (" ", " ")
    #             else:
    #                 if j == 0:
    #                     newMatrix2[j][i] = (str(b[i-2]), " ")
    #                 elif j == 1:
    #                     newMatrix2[j][i] = (str(i-2), " ")
    #                 elif i == 0:
    #                     newMatrix2[j][i] = (str(a[j-2]), " ")
    #                 elif i == 1:
    #                     newMatrix2[j][i] = (str(j-2), " ")
    #         else:
    #             newMatrix2[j][i] = newMatrix[j-2][i-2]


    # print("newMatrix2 =")
    # printMatrix(newMatrix2)

    return newMatrix

def addHeadings(Matrix, a, b):

    print("a = " + str(a))
    for i in range(len(a)+3):
        if i < len(a):
            print("a["+str(i)+"] = " + str(a[i]))

    print("b = " + str(b))
    for i in range(len(b)+3):
        if i < len(b):
            print("b["+str(i)+"] = " + str(b[i]))



    MatrixWithHeadings = [[0 for x in range(len(b)+3)] for y in range(len(a)+3)]
    print("MatrixWithHeadings =")
    printMatrix(MatrixWithHeadings)


    for i in range(len(b)+3):
        for j in range(len(a)+3):
            if i < 2 or j < 2:
                if i+j <= 2:
                    MatrixWithHeadings[j][i] = (" ", " ")
                else:
                    if j == 0 and i < len(b)+3:
                        MatrixWithHeadings[j][i] = (str(b[i-3]), " ")
                    elif j == 1:
                        MatrixWithHeadings[j][i] = (str(i-2), " ")
                    elif i == 0 and j < len(a)+3:
                        MatrixWithHeadings[j][i] = (str(a[j-3]), " ")
                    elif i == 1:
                        MatrixWithHeadings[j][i] = (str(j-2), " ")
            else:
                MatrixWithHeadings[j][i] = Matrix[j-2][i-2]


    print("MatrixWithHeadings =")
    printMatrix(MatrixWithHeadings)

    return MatrixWithHeadings



# a = "Harvard"
# b = "Yale"
# Matrix = distances(a,b)
# printMatrix(Matrix)
