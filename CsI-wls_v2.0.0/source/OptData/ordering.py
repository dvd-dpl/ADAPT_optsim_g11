import os,sys
import pandas as pd
import numpy as np

fin  = sys.argv[1]
fout = fin.replace(".txt","_ordered.txt")
df = pd.read_csv(fin,sep=":")
df = df.sort_values(df.keys()[0])
df.to_csv(fout,sep=":",index=False)
print("[INFO]: Create outfile:", fout)
