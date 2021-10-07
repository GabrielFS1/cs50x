from cs50 import get_string
import sys

sum = 0
n = 0

number = get_string("Number: ")
       
#bigger or smaller than a credit card number        
if (len(number) > 16 or len(number) < 13):
    print('INVALID')
    sys.exit(1)
else:
    for i in range(-2, -len(number) -1, -2):
        n = 2 * (int(number[i]))
        
        #if two digits
        if(n < 9):
            sum = sum + n
        else:
            sum += ((n % 10) + (n // 10))
    for j in range(-1, -len(number) -1, -2):
        sum = sum + int(number[j])
    
    #check the flag of card        
    if(sum % 10 == 0):
        if(number[0] == '4'):
            print('VISA')
        elif(number[0] == '3' and number[1] in '47'):
            print('AMEX')
        elif(number[0] == '5' and number[1] in '12345'):
            print('MASTERCARD')
        else:
            print('INVALID')