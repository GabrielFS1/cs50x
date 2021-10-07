import csv
import re
from sys import argv, exit

count = 0
large_str = {}
strs = []

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

#open the database and put the data of the row in a dictionary
with open(argv[1], newline='') as file:
    reader = csv.DictReader(file)
    
    #get the strs 
    strs = reader.fieldnames
    
    #open the dna file
    f = open(argv[2], "r")
    dna = (f.read())
    
    #get all the the longests strs
    for i in strs[1:]:
        str_count = re.findall(r'(?:{})+'.format(i), dna)
        if(len(str_count) > 0):
            largest = max(str_count, key=len)
            large_str[i] = (len(largest) // len(i))
        else:
            large_str[i] = 0
    
    #read all lines of the database
    for row in reader:
        a = dict(row)
        count = 0
        
        for j in strs[1:]:
            if (int(a[j]) == large_str[j]):
                count += 1
        if(count == len(strs[1:])):
            print(a['name'])
            exit(0)
    print('No match')
    exit(1)