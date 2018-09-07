
# Keeps requesting a positive dollar amount (X.XX or X.X)
while True:
    try:
        dollarCents = -1     # force the first while loop
        while (int(dollarCents) < 0):
            dollarCents = input("Input a dollar amount eg. X.XX or X.X: ")
            dollarCents = float(dollarCents)

            if dollarCents < 0.00:
                dollarCents = input("Input a dollar amount eg. X.XX or X.X: ")
                dollarCents = float(dollarCents)
        break
    except ValueError:
        print("Enter a number, no letters")

cents = round(dollarCents * 100)


# Find number of quarters, dimes, and nickels (in that order)
numCoins = 0
remain = cents
totalCoins = 0


# Each loop extracts amount of quarters, dimes, and nickels (pennies processed after the loop)
for i in range(3):

    coinValue = 25 - i * (20 - i * 5)

    numCoins = int(remain / coinValue)

    remain = remain - numCoins * coinValue

    totalCoins = totalCoins + numCoins

# Adds final remainder (pennies) to coin counter and prints final total coin count
totalCoins = totalCoins + remain
print(totalCoins)
