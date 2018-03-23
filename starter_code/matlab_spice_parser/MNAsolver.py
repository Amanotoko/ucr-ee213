import numpy as np
import math

global mna
global mna_1
global rhs
global size

def parse_MNA_matrix():

    global mna
    global mna_1
    global rhs
    global size

    file = open("MNAmatrix.txt","r")

    count = 0
    for line in file:
        line = line.strip()
        elements = np.array(parse_float(line.split("\t")))
        if count == 0:
            size = int(math.sqrt(len(elements)))
            mna = np.reshape(elements,(size,size))
            print(mna)
        elif count == 1:
            mna_1 = np.reshape(elements, (size, size))
            print(mna_1)
        else:
            rhs = np.reshape(elements, (size, 1))
            print(rhs)
        count = count + 1

def parse_float(arr):
    res = []
    for element in arr:
        res.append(float(element))
    return res

def mna_calculator():

    global mna
    global mna_1
    global rhs
    global size

    res = np.linalg.solve(mna, rhs)
    print(res)

    try:
        res_1 = np.linalg.solve(mna_1,rhs)
    except:
        print("no solution")

    


parse_MNA_matrix()
mna_calculator()

