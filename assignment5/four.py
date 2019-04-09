m = 0
p = 0.0

while m < 1000:
    p = (1 - (m/(2**20)))**m
    if p <= 0.54:
        print("m = " + str(m))
        print("p = " + str(p) + "\n")
    p = 0.0
    m += 1