import string

f = open("result.txt")
lines = f.readlines()

of = open("output.csv","w")

maxIter = "10"
id = "5"
rate = "0.039"

for line in lines:
   
    if "maxIter =" in line:
        maxIter = line.split()[2]
        print line
    if "constraint" in line and not "make" in line:
        id = line.split(":")[6].rstrip()
        print line
    if ("rate" in line) and not "All" in line and not "32000" in line:
        rate = line.split()[2].split(":")[1]
        of.write(maxIter + "," + id + "," + rate + "\n")
        print line