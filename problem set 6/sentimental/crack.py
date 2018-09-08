import sys
from crypt import crypt
import string


def compare(key, salt, hash):

    key = "".join(key)
    print("key = " + str(key))

    encryptedPassword = crypt(str(key), salt)

    if encryptedPassword == hash:
        print("The password is: " + str(key))
        sys.exit(0)


def loopNonRecursive(keyLength, key, options, salt, hash):

    for option in options:
        key[0] = option

        if keyLength < 1:
            compare(key, salt, hash)
        else:

            for option in options:

                key[1] = option

                if keyLength < 2:
                    compare(key, salt, hash)
                else:

                    for option in options:

                        key[2] = option

                        if keyLength < 3:
                            compare(key, salt, hash)
                        else:

                            for option in options:

                                key[3] = option

                                if keyLength < 4:
                                    compare(key, salt, hash)
                                else:

                                    for option in options:

                                        key[4] = option

                                        compare(key, salt, hash)


def loopRecursive(keyLength, keyIndex, key, options, salt, hash):


    for option in options:
        key[keyIndex] = option

        if keyLength < keyIndex+1:
            compare(key, salt, hash)
        else:
            loopRecursive(keyLength, keyIndex+1, key, options, salt, hash)


# User may only input 2 command-line arguments
if len(sys.argv) != 2:
    print("Usage: ./crack hash")
    sys.exit(1)

hash = sys.argv[1]

salt = hash[:2]

options = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
           'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']


key = ['' for i in range(5)]

for keyLength in range(5):
    #loopNonRecursive(keyLength, key, options, salt, hash)
    loopRecursive(keyLength, 0, key, options, salt, hash)
