import sys
from crypt import crypt


def compare(key, salt, hash):
    """If the encrypted key equals the given hash, that key is the password (so terminate the program)"""

    key = "".join(key)
    print("key = " + str(key))

    encryptedPassword = crypt(str(key), salt)

    if encryptedPassword == hash:
        print("The password is: " + str(key))
        sys.exit(0)     # if the key is found, terminate the program with exit code 0


def loopNonRecursive(keyLength, key, options, salt, hash):
    """Non-recursively loop first through all one-letter combinations, then all two-letter combos, and so forth until all five-letter combos"""

    for option in options:
        key[0] = option

        if keyLength < 1:               # if in the one-letter loop, compare each and every ONE-LETTER combo
            compare(key, salt, hash)
        else:                           # else allow for all two-letter combos

            for option in options:

                key[1] = option

                if keyLength < 2:               # if in the two-letter loop, compare each and every TWO-LETTER combo
                    compare(key, salt, hash)
                else:                           # else allow for all three-letter combos

                    for option in options:

                        key[2] = option

                        if keyLength < 3:                   # THREE-LETTER combos
                            compare(key, salt, hash)
                        else:

                            for option in options:

                                key[3] = option

                                if keyLength < 4:                   # FOUR-LETTER combos
                                    compare(key, salt, hash)
                                else:                               # FIVE-LETTER combos

                                    for option in options:

                                        key[4] = option

                                        compare(key, salt, hash)


def loopRecursive(keyLength, keyIndex, key, options, salt, hash):
    """Recursively loop first through all one-letter combinations, then all two-letter combos, and so forth until all five-letter combos"""

    for option in options:
        key[keyIndex] = option

        if keyLength < keyIndex+1:              # if in the one-letter loop, compare each and every ONE-LETTER combo; if in the second-letter loop, compare each TWO-LETTER combo, etc.
            compare(key, salt, hash)
        else:
            loopRecursive(keyLength, keyIndex+1, key, options, salt, hash)


# User may only input 2 command-line arguments
if len(sys.argv) != 2:
    print("Usage: ./crack hash")
    sys.exit(1)

hash = sys.argv[1]

salt = hash[:2]         # salt is 2 characters + 1 for null byte

options = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
           'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']


key = ['' for i in range(5)]    # key is 5 characters + 1 for null byte


# Loop through all possible combinations,
# first through all one-letter combinations (i=0), then through all two-letter passwords (i=1),
# and so forth until looping finally through all five-letter passwords (i=4)
for keyLength in range(5):
    #loopNonRecursive(keyLength, key, options, salt, hash)
    loopRecursive(keyLength, 0, key, options, salt, hash)
