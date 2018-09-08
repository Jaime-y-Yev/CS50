import sys


# User may only input 2 command-line arguments, the 2nd being a word
try:
    if len(sys.argv) != 2:
        print("Usage: ./vigenere key")
        sys.exit(1)

    key = sys.argv[1]

    if key.isalpha() == False:
        print("Enter only letters for the key")
        sys.exit(1)

except ValueError:
    print("Enter a number for the key")
    sys.exit(1)


# Prompt for string to encrypt
plainText = input("plaintext: ")

# Only encrypt A-Z and a-z characters, break into upper and lowercase
encryptableLettersUp = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
encryptableLettersDown = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']
encryptableLetters = encryptableLettersUp + encryptableLettersDown

# Loop through plainText and (conditionally) the key
cipherText = []
keyIndex = 0
keyLength = len(key)
for plainLetter in plainText:

    # If not a special character, encrypt plainLetter] and loop through key
    if plainLetter in encryptableLetters:

        currentKeyChar = ord(key[keyIndex % keyLength])     # loop through key

        # Conditionally adjust key according to upper or lower case eg. key=a or key=A means shift=0, key=B or key=B means shift=1
        if chr(currentKeyChar).isupper():
            shift = currentKeyChar % ord('A')
        elif chr(currentKeyChar).islower():
            shift = currentKeyChar % ord('a')

        cipherLetter = ord(plainLetter) + shift


        # If plainText case and cipherText case don't match
        if (plainLetter.isupper() != chr(cipherLetter).isupper()) or (plainLetter.islower() and chr(cipherLetter).islower() == False):

            cipherLetter = cipherLetter - 26

            # Some characters may land beyond 'z' but will loop around differently depending on where they lan
            if cipherLetter > ord('z') and cipherLetter < 150:
                cipherLetter = (cipherLetter % 122) + 96
            elif cipherLetter >= 150:
                cipherLetter = (cipherLetter % 122) + 70

        cipherLetter = chr(cipherLetter)

        keyIndex = keyIndex + 1     # pick the next key character to be used in the following loop

    # Do not encrypt special characters, do not loop through key
    else:
        cipherLetter = plainLetter


    cipherText.append(cipherLetter)


print("ciphertext: ", end="")
for cipherLetter in cipherText:
    print(cipherLetter, end="")
print()

