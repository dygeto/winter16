#!/usr/bin/python

from random import choice
from string import ascii_lowercase
from random import randint

myString1 = (''.join(choice(ascii_lowercase) for i in range(10)))
myString2 = (''.join(choice(ascii_lowercase) for i in range(10)))
myString3 = (''.join(choice(ascii_lowercase) for i in range(10)))
myString4 = (''.join(choice(ascii_lowercase) for i in range(10)))

myFile1 = open("file1.txt", "w")
myFile1.write(myString1)
myFile1.close()
myFile2 = open("file2.txt", "w")
myFile2.write(myString2)
myFile3 = open("file3.txt", "w")
myFile3.write(myString3)
myFile4 = open("file4.txt", "w")
myFile4.write(myString4)

print(myString1)
print(myString2)
print(myString3)
print(myString4)

myInt1 = randint(1,42)
myInt2 = randint(1,42)
myInt3 = myInt1 * myInt2

print(myInt1)
print(myInt2)
print(myInt3)
