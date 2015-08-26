import string

f = open("result.txt")
lines = f.readlines()

of = open("output.csv","w")

a = ""

for line in lines:
   
    if "maxIter" in line:
        a += "\n"
    if "rate" in line:
        rate = line.split(" ")[3].split(":")[1]
        rate = rate.rstrip()
        print rate
        a += rate
        a += ","

of.write(a)