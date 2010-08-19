pair = []
like = []

for i in range(0, 10000):
    pair += ["",]
    like += [0,]
    
    
for names in ("mmg","eeg"):
    file = open(names+".out","r")
    lines = file.readlines()
    file.close()

    import re
    counter = 0
    for line in lines:
        elements = re.split(" ", line)
        coupling1 = elements[0]
        coupling2  = elements[1]
        llh = float(elements[2][:-1])

        pair[counter] = coupling1 + " " + coupling2
        like[counter] += llh
        counter += 1
        
file = open("all.out", "w")
for i in range(0, counter):
    file.write(pair[i] + " " + str(like[i]) + "\n")
file.close()
