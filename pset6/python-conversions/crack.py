# Pseudocode for the Python version of Crack.

# 1 Check the command line argument has a valid password.
# 2 Create "hashed" passwords for all iterations of strings with
#   up to 4 alpha characters.
# 3 Compare each "hashed" password against the one given in the
#   command line argument. Matches? ---> Success!

import sys
import crypt
import string

# STEP 1
# check that two command line arguments (crack.py + password) were entered
if not len(sys.argv)==2:
    print("Error, you must enter one command line argument.")
    exit(1)
    
cla_password = (sys.argv[1])

# STEP 2
# iterate over all the letters (upper & lower) in a one-char string
for c1 in string.ascii_letters: 
    hashed_password = crypt.crypt(c1,"50")
    # STEP 3
    if hashed_password == cla_password:
        print("password is... {}".format(c1))
        exit(0)

# if password was not found, iterate over two-letter passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        hashed_password = crypt.crypt((c1+c2),"50")
        if hashed_password == cla_password:
            print("password is... {}".format(c1+c2))
            exit(0)

# and 3-letter passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        for c3 in string.ascii_letters:
            hashed_password = crypt.crypt((c1+c2+c3),"50")
            if hashed_password == cla_password:
                print("password is... {}".format(c1+c2+c3))
                exit(0)
                
# and finally 4-letter passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        for c3 in string.ascii_letters:
            for c4 in string.ascii_letters:
                hashed_password = crypt.crypt((c1+c2+c3+c4),"50")
                if hashed_password == cla_password:
                    print("password is... {}".format(c1+c2+c3+c4))
                    exit(0)

# if the password was not in any of those strings, we cannot find it
print("password was not identified")
exit(0)

