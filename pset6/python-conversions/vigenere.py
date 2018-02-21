# Pseudocode for the Python version of Vigenere.

# 1 Check the command line argument has a valid keyword.
# 2 Prompt the user for a plaintext string to encode.
# 3 Using the keyword, encipher the plaintext into ciphertext.
#   THE CALCS IN STEP 3 COULD BE CONSOLIDATED INTO A SINGLE LINE OF CODE BUT I HAVE
#   SPLIT IT UP INTO MANY LINES OF CODE FOR CLARITY
# 4 Print the ciphertext.


from itertools import cycle
import sys

def main():

# STEP 1
# check that two command line arguments (vigenere.py + KEY) were entered
    if not len(sys.argv)==2:
        print("Error, you must enter one command line argument.")
        exit(1)

# check that 'key' contains only alpha characters
# isalpha() returns True if it's good, so I need to invert it with the !=True
# in order to print the error message and exit if it's a bad 'key'.
    if not (sys.argv[1].isalpha()):
        print("Error, your key must contain only letters.")
        exit(1)

# create a list 'key' to store the keyword defined by the user
    key = list((sys.argv[1]).upper())
    # create a list that is a cycle through the key list, because the plaintext might have more characters than the key!
    cycliclist = cycle(key)
    
# STEP 2
    plaintext = input("plaintext: ")
    # create a list to store the ciphertext in
    cipher = []

# STEP 3
    for i in plaintext:
        # if the character is an uppercase letter do this
        if i.isupper():
            a = (ord(next(cycliclist)))     # get the integer value of the next element in the key list
            b = ord(i)                      # get the integer value of the current plaintext letter
            c = (a+b)%65                    # sum them, then modulus 65, to get the letter+key combination as a "value from A"
            d = c%26                        # modulus 26, so if the value is >25 (i.e. goes past Z) it will loop back to A
            e = d+65                        # add 65 to get back to a "value from A"
            cipher.append(chr(e))           # get the Unicode character for the value
        
        # else if its a lowercase letter do this
        elif i.islower():
            a = (ord(next(cycliclist)))     # same as for isupper, except...
            b = ord(i)
            c = (a+b-32)%65                 # need to -32 before the modulus 65 because lower case "a" is 32 from "A" (97;65)
            d = c%26
            e = d+97                        # add 97 to get back to a "value from a"
            cipher.append(chr(e))
        
        # and if its not a letter do not do anything with the key; just append the character to the cipher list
        else:
            cipher.append(i)

# STEP 4
    # to print ipher as a plain string, create a string 'cipher' from the list before printing
    cipher = "".join(cipher)
    print(cipher)
    
    
if __name__ == "__main__":
    main()