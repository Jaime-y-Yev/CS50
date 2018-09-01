#include <stdio.h>
#include <cs50.h>
#include <math.h>

string getCardType();
int cardSum();


int main(void)
{
    // Requess card number from user
    printf("Number: ");
    long long cardNum = get_long_long();

    if (cardSum(cardNum) % 10 == 0)
        printf("%s\n", getCardType(cardNum));
    else
        printf("INVALID\n");

    return 0;
}

// Return card company name based on card number input
string getCardType(long long cardnum)
{
    if (cardnum >= 5100000000000000 && cardnum < 5600000000000000)
        return "MASTERCARD\n";

    else if ((cardnum >= 4000000000000 && cardnum < 5000000000000) || (cardnum >= 4000000000000000 && cardnum < 5000000000000000))
        return "VISA\n";

    else if ((cardnum >= 340000000000000 && cardnum < 350000000000000) || (cardnum >= 370000000000000 && cardnum < 380000000000000))
        return "AMEX\n";
    else
        return "INVALID\n";
}

// Returns a processed total based on the card number input
int cardSum(int cardNum)
{
    int sumOdd = 0;
    int sumEven = 0;
    int x1, x2;

    // Odd positions summed up in sumOdd
    for (int i = 1; i <= 16; i = i + 2)
    {
        long long cardNumNew = cardNum / pow(10,16-i);

        long long cardNumNew1 = cardNumNew * 10;

        long long cardNumNew2 = cardNum / pow(10,15-i);

        long long y = cardNumNew2 - cardNumNew1;

        sumOdd = sumOdd + y;
    }

    /*
        Even positions are first multiplied by 2,
        any resulting double digits will be further split into two digits,
        the resulting single digits are summed up in sumEven
    */
    for (int i = 0; i <= 15; i = i + 2)
    {
        long long cardNumNew = cardNum / pow(10,16-i);

        long long cardNumNew1 = cardNumNew * 10;

        long long cardNumNew2 = cardNum / pow(10,15-i);

        long long x = 2 * (cardNumNew2 - cardNumNew1);

        //any double digits are separated into their two constituent digits
        if (x >= 10)
        {
            x1 = x / 10;
            x2 = x - 10;
            x = x1 + x2;
        }

        sumEven = sumEven + x;
    }

    return sumOdd + sumEven;
}
