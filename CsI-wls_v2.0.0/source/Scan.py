import numpy as np
import sys,os
import matplotlib.pyplot as plt

dirMAC  = "./mac/"
rootDir = "./rootOutput/"
   
def WriteMacFile(filename,nevt):
    file=open(filename,'w+')
    rootFile = filename.replace(dirMAC,rootDir)
    rootFile = rootFile.replace(".mac",".root")
    file.write(f'/run/initialize\n')
    file.write(f'/tracking/verbose 0\n')
    file.write(f'/gps/particle e-\n')
    file.write(f'/gps/position {pos[0]} {pos[1]} {pos[2]} cm\n')
    file.write(f'/gps/direction 0 0 -1\n')
    file.write(f'/gps/pos/type Beam\n')
    file.write(f'/gps/ang/type iso\n')
    file.write(f'/gps/ene/type Mono\n')
    file.write(f'/gps/ene/mono 500 keV\n')
    file.write(f'/RunManager/NameOfOutputFile {rootFile}\n')
    file.write(f'/run/beamOn {nevt}')
    file.close()

def TileScan(tileNum):
    xmin = -22.5
    ymin = -22.5
    l = 15.
    xbias = xmin+int(tileNum%3)*l
    ybias = ymin+int(tileNum/3)*l
    return(xbias,ybias)

if __name__=="__main__":
    #Settings
    min=0.75
    max=15.0
    step=1.5
    Nevt=20
    tileNum = 8
    Nrep=int((max-min)/step)
    for tileNum in range(9):
        xbias,ybias = TileScan(tileNum)
        ##############
        print("[INFO] Now Scanning tile->",tileNum," with ",Nrep*Nrep," steps (delta = ",step,"cm)","Position BIAS->(",xbias,",",ybias,")")
        for j in range(Nrep+1):
            for i in range(Nrep+1):
                xpos=min+step*i+xbias
                ypos=min+step*j+ybias
                pos=[xpos,ypos,0]
                filename=f"ScanPos_Tile{tileNum}_x{pos[0]}_y{pos[1]}_z{pos[2]}.mac"
                filename = dirMAC + filename
                print(filename)
                WriteMacFile(filename,Nevt)
                #os.system(f"./apt {filename}")
        
