# while loop continues if the if statement contained within is not met
# i.e. if a negative number is entered
# but if a positive number is entered, break and continue with program
while True:
    # convert the string input to a float
    dollars = float(input("O hai! How much change is owed?  "))
    if dollars >=0:
        break

# cents variable created as int    
cents = int(round(dollars * 100))

coins = 0

#How many 25c coins can be given?
coins = coins + (cents//25)
cents = cents % 25

#How many 10c coins can be given?
coins = coins + (cents//10)
cents = cents % 10

#How many 5c coins can be given?
coins = coins + (cents//5)
cents = cents % 5

#How many 1c coins can be given?
coins = coins + (cents//1)
cents = cents % 1

print(coins)