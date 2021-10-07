from cs50 import get_string
import re

txt = get_string("Text: ").strip()
l = 0

for i in txt:
    x = i.isalpha()
    if(x == True):
        l += 1
        
words = len(txt.split(' '))
sent = len(re.split('[.!?]',txt)) -1
    
L = (l*100)/words
S = (sent*100)/words    
index = ((0.0588 * L) - (0.296 * S) - 15.8)

if (index < 1):
    print("Before Grade 1")
elif (index > 16):
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")

    