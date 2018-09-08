import sys

# User may only input 2 command-line arguments, the 2nd being a positive number
try:
    if len(sys.argv) != 2:
        print("Usage: ./caesar key")
        sys.exit(1)
    if int(sys.argv[1]) < 0:
        print("Enter a positive number for the key")
        sys.exit(1)

    key = int(sys.argv[1])

except ValueError:
    print("Enter a number for the key")
    sys.exit(1)

# Prompt for string to encrypt
plainText = input("plaintext: ")

# Only encrypt A-Z and a-z characters
encryptableLetters = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

# Rotate every character a value of key up the alphabet, ignores special characters
cipherText = []
for plainLetter in plainText:

    # Encrypt characters between A-Z or a-z only
    if plainLetter in encryptableLetters:
        cipherLetter = (ord(plainLetter) + key) % ord('z')  # wrap around 'z'

        # If encrypted character lands on special character, shift into the accepted range
        if cipherLetter < ord('A'):
            cipherLetter = cipherLetter + 96

            # Some characters may land beyond 'z' but will loop around differently depending on where they land
            if cipherLetter > ord('z') and cipherLetter < 150:
                cipherLetter = (cipherLetter % ord('z')) + 96
            elif cipherLetter >= 150:
                cipherLetter = (cipherLetter % ord('z')) + 70

        cipherLetter = chr(cipherLetter)

    else:
        cipherLetter = plainLetter


    cipherText.append(cipherLetter)


print("ciphertext: ", end="")
for cipherLetter in cipherText:
    print(cipherLetter, end="")
print()
