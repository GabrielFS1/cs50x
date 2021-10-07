from cs50 import get_int

while True:
    h = get_int("Height: ")
    if(h > 0 and h <= 8):
        break
n = h 
y = 0
for i in range(h):
   for j in range(n-1):
       print(' ', end = '')
   n -= 1
   for x in range(y+1):
       print('#', end = '')
   y += 1
   print()
