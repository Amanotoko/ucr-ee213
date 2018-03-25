import numpy as np
import math
import numdifftools as tools

global mna
global rhs
global size

def parse_MNA_matrix():

    global mna
    global rhs
    global res
    global size

    file = open("MNAmatrix.txt","r")

    count = 0
    for line in file:
        line = line.strip()
        elements = np.array(parse_float(line.split("\t")))
        if count == 0:
            size = int(math.sqrt(len(elements)))
            mna = np.reshape(elements,(size,size))
        else:
            rhs = np.reshape(elements, (size, 1))
        count = count + 1

def parse_float(arr):
    res = []
    for element in arr:
        res.append(float(element))
    return res


def calculation_iteration(N, guess):
    global mna
    global rhs

    x = guess

    if x is None:
        x = np.zeros(len(A[0]))

    D = np.diagflat(np.diag(mna))
    R = mna - D

    for i in range(N):
        rhs = rhs-np.dot(R,x)
        print("the "+str(i)+"th iteration is:")
        print(x)
        next = np.linalg.solve(D,rhs)
        x = next
    return x

def run():
    parse_MNA_matrix()
    guess = np.reshape(np.array([-70,-5,1]),(3,1))
    sol = calculation_iteration(10,guess)
    print("res is: ")
    print(sol)

run()