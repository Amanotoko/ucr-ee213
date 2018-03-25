import numpy as np
import math

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

def mna_calculator_freq():

    global mna
    global rhs
    global res
    res = None
    try:
        res = np.linalg.solve(mna, rhs)
    except e:
        print("No Solution")
        print(e)
    return res

def run():
    parse_MNA_matrix()
    print("Freq:")
    print("MNA: "+str(mna))
    print("RHS: "+str(rhs))
    print("RES: "+str(mna_calculator_freq()))

run()