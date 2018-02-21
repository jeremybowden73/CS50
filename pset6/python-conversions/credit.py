# Pseudocode for the Python version of "credit"

# 1 Get credit card number from user.
# 2 Check the number has a valid number of digits (13, 15, or 16).
# 3 Starting with the penultimate digit, and continuing with every
#   alternate digit, multiply by 2.
# 4 Sum the 'doubled' digits (10 will be 1+0, 12 will be 1+2, etc.)
# 5 Add all the other card number digits that were not doubled.
# 6 Divide by 10. If remainder is 0 then it's a good card number,
#   if remainder is not 0 then it's a bad card number.
# 7 If good, check which bank it is by analysing the specific digits
#   at the start of the card number.


# STEPS #1 and #2

while True:
    cardnumber = input("Card Number: ")
    if len(cardnumber) == 13 or len(cardnumber) == 15 or len(cardnumber) == 16:
        break

# slice the string cardnumber, to get a list "x" containing only the
# penultimate element and every subsequent alternate one.
x = (cardnumber[-2::-2])
print("Sliced list x is: ", x)
# and similar for a list "y" but this time starting from the final element
y = (cardnumber[-1::-2])
print("Sliced list y is: ", y)

# STEP 3
# using "list comprehensions", for each element in x convert it to
# an int and multiply by 2. It's OK for x to overwrite itself while
# doing it because lists are mutable.
x = [(2*(int(i))) for i in x]
print("List of doubled x ints: ", x)

# STEP 4
# again using list comps, create a copy "x_summed", and for each element:-
# if the value is 10 or more, replace it with the sum of the digits (e.g. 12 ---> 3)
# if the value is <10, leave it as it is
x_summed = [(1+(i%10)) if i>=10 else i for i in x]
print("List of summed x doubles: ", x_summed)

# convert list "y" to ints as well
y = [int(i) for i in y]
print("List of y ints: ", y)

# STEP 5
# add up all the values in the two lists "x_summed" and "y"
total = sum(x_summed) + sum(y)
print("For information, sum of values = ", total)

# STEP 6
# divide by 10. If modulus is 0 then it's a good card number, otherwise bad (so exit).
if total%10 != 0:
    print("INVALID")
    exit(1)

# an 'else' is not needed after the above 'if' because the program
# would have already exited if the card number was invalid, so we know that
# the card number is good

# STEP 7
if (len(cardnumber) == 13 or len(cardnumber) == 16) and cardnumber[0] == "4":
    print("VISA")

elif len(cardnumber) == 16 and cardnumber[0] == "5":
    print("MASTERCARD")

elif len(cardnumber) == 15 and cardnumber[0] == "3":
    print("AMEX")

else:
    print("INVALID")
    exit(1)
