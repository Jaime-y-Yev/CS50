import sys
import string

# Get the number of blocks from the user, must be between 0 and 23, both included
#while True:
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
    sys.exit(0)

plaintext = input("plaintext: ")

encryptableLetters = string.ascii_uppercase + string.ascii_lowercase

ciphertext = []
ciphertextChar = []
for i in range(len(plaintext)):
    if plaintext[i] in encryptableLetters:
        ciphertext.append((ord(plaintext[i]) + key) % ord('z'))

        if ciphertext[i] < ord('A'):
            ciphertext[i] = ciphertext[i] + 96

            if ciphertext[i] > ord('z') and ciphertext[i] < 150:
                ciphertext[i] = (ciphertext[i] % ord('z')) + 96
            elif ciphertext[i] >= 150:
                ciphertext[i] = (ciphertext[i] % ord('z')) + 70

    else:
        ciphertext.append(plaintext[i])

    if type(ciphertext[i]) != str:
        ciphertextChar.append(chr(ciphertext[i]))
    else:
        ciphertextChar.append(ciphertext[i])

#print(ciphertext)
print("ciphertext: ", end="")
for char in ciphertextChar:
    print(char, end="")
print()
