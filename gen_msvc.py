#!/usr/bin/python

def gen(prefix, count):
    line="#define X_STRUCT_L"+prefix+str(count)+"(ACT"
    for i in range(1, count+1):
        line+=",M"+str(i)
    line+=")    X_STRUCT_L"+prefix+"DEF(ACT, M1)"
    if count > 1:
        line+="    X_STRUCT_L"+prefix+str(count-1)+"(ACT"
        for i in range(2, count+1):
            line+=",M"+str(i)
        line+=")"
    print line

for i in range(1,33):
    gen("1_", i)
for i in range(1,33):
    gen("2_", i)