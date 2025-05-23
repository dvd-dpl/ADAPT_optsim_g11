import time, sys, os
import pandas as pd


def fromdt(filepath):
    data=open(filepath,'r')
    conv=open(filepath.replace('.txt','_converted.txt'),'w+')
    cc=0
    for line in data:
        if cc==0:
            conv.write(line.replace('\n','')+' Converted in eV:Intensity\n')
        else:
            splitted=line.split(sep=',')
            Ene=float(splitted[0])
            Int=float(splitted[1])
            string=str(1239.84193/Ene)+':'+str(Int)+'\n'
            conv.write(string)
        cc+=1
    conv.close()
    data.close()

if __name__=="__main__":
    inpfile = sys.argv[1]
    data=fromdt(inpfile)
