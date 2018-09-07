def getCardType(cardNum):
    """Returns a string with the name of the card company"""

    if cardNum in range(5100000000000000, 5600000000000000):
        return "MASTERCARD"
    elif cardNum in range(4000000000000, 5000000000000) or cardNum in range(4000000000000000, 5000000000000000):
        return "VISA"
    elif cardNum in range(340000000000000, 350000000000000) or cardNum in range(370000000000000, 380000000000000):
        return "AMEX"
    else:
        return "INVALID"


def getCardSum(cardNum):
    """Returns a processed sum of the card"""

    sumOdd = 0
    sumEven = 0
    x1, x2 = 0, 0

    # Odd positions summed up in sumOdd
    for i in range(0, 16, 2):
        cardNumNew = cardNum / 10 ** (16-i)
        cardNumNew1 = cardNumNew * 10
        cardNumNew2 = cardNum / 10 ** (15-i)
        y = cardNumNew2 - cardNumNew1

        sumOdd = sumOdd + y

    # Even positions summed up according to Luhn algorithm
    for i in range(0, 15, 2):
        cardNumNew = cardNum / 10 ** (16-i)
        cardNumNew1 = cardNumNew * 10
        cardNumNew2 = cardNum / 10 ** (15-i)
        x = 2 * (cardNumNew2 - cardNumNew1)

        # Any resulting double digits are split into two digitsseparated into their two constituent digits
        if x >= 10:
            x1 = x / 10
            x2 = x - 10
            x = x1 + x2

        sumEven = sumEven + x

    return sumOdd + sumEven


# Get the card number from the user
while True:
    try:
        cardNum = ""     # force the first while loop
        while (cardNum == ""):
            cardNum = input("Number: ")
            cardNum = int(cardNum)

        break
    except ValueError:
        print("Enter a card number")


# Calculate the sum based on the card number
cardSum = getCardSum(cardNum)

# The mod10 of the sum of a valid card number is always 0
if (cardSum % 10):
    print(getCardType(cardNum))
else:
    print("INVALID")
