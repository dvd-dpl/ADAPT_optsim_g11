import os
import sys
import math
import string
import os.path
import array 
import glob
import sys
import random
import time
import numpy as np

# Load Settings from Settings
from Settings import *

## 0.o.0.o.O.o.O ##
##  Job Settings ##
InputDirPath = "/lustrehome/marcocecca/ADAPT/simulation/CsI-wls_v1.2.0"
#InputDirPath = "./"
MainNameSim = "Scan_glue"
OutputFileDir = str(InputDirPath)+"/JobsOutput/"+str(MainNameSim)
ProjectName = "apt"
exe = "apt"
## 0.o.0.o.O.o.O ##

#dirMAC  = "./macro/"
#rootDir = "."



def BuildCondorSumbitInstructions(SubmitInstructionFileName,tcshfile,logname,outputname,errorname,request_cpus=1,request_memory=2000):
    ## fill here
    f=open(SubmitInstructionFileName,'w')
    f.write("#submit file config\n")
    f.write("# file name :"+SubmitInstructionFileName+"\n")
    f.write("universe = vanilla\n")
    f.write("executable = "+tcshfile+" \n")
    f.write("output = " + outputname +" \n")
    f.write("error = " + errorname+" \n")
    f.write("log = " + logname +" \n")
    f.write("request_cpus = "+str(request_cpus)+"\n")
    f.write("request_memory = "+str(request_memory)+"\n")
    f.write("rank = Memory\n")
    f.write("requirements   = MaxHosts == 1\n")
    f.write("queue\n")
    f.close()
        
def writeshfile(shfile, name):
    f = open(shfile, "w")
    exe_dirname = str(InputDirPath)+"/"+str(ProjectName)+"_build"
    work_dirname = str(OutputFileDir)
    name = name
    macroFile = name+".mac"
    rootFile  = name+""
    f.write("#!/bin/sh")
    f.write("\n \n")
    f.write("hostname")
    f.write("\n \n")
    f.write('source /lustrehome/marcocecca/ADAPT/simulation/geantsetup.sh')
    f.write("\n \n")
    f.write("cd "+str(InputDirPath))
    f.write("\n \n")
    f.write("export TEMP_DIR=$_CONDOR_SCRATCH_DIR")
    f.write("\n \n")
    f.write('mkdir $TEMP_DIR')
    f.write("\n\n")
    f.write("export TMPWORK=$TEMP_DIR")
    f.write("\n\n")
    f.write("export EXE_DIR="+str(exe_dirname))
    f.write("\n\n")
    command = "cp $EXE_DIR/"+str(exe)+" $TMPWORK/. -v"
    f.write(command)
    f.write("\n\n")
    f.write("export OUTWORKDIR="+str(work_dirname))
    f.write("\n\n")
    command = "cp $OUTWORKDIR/macro/"+str(macroFile)+" $TMPWORK/. -v"
    f.write(command)
    f.write("\n\n")
    f.write('cd $TMPWORK')
    f.write("\n\n")
    command ="./"+str(exe)+" "+str(macroFile)
    f.write(command)
    f.write("\n \n")
    f.write("ls -orth")
    f.write("\n\n")
    f.write("cp "+str(rootFile)+"*.root $OUTWORKDIR/rootOutput")
    f.write("\n \n")
    f.write("cd ")
    f.write("\n\n")
    f.write("rm -rf $TMPWORK")
    f.write("\n\n")
    f.write("cd "+str(InputDirPath))
    
    f.close()

def init_dir():
    command = 'mkdir -p '+str(OutputFileDir)
    os.system(command)
    command = 'cd '+str(OutputFileDir)
    os.system(command)

    outdir = OutputFileDir
    job_o = outdir+'/qsub_o/'
    job_e = outdir+'/qsub_e/'
    job_sh = outdir+'/SH/'
    job_macro = outdir+'/macro/'
    job_output = outdir+'/rootOutput/'

    command ="mkdir -p "+str(job_o)
    os.system(command)

    command ="mkdir -p "+str(job_e)
    os.system(command)

    command ="mkdir -p "+str(job_sh)
    os.system(command)

    command ="mkdir -p "+str(job_macro)
    os.system(command)

    command ="mkdir -p "+str(job_output)
    os.system(command)
    
    command = "cd "+str(InputDirPath)
    os.system(command)
    
#
# MAIN PROGRAM
#
if __name__ == '__main__':

    TILE_SIDE = 150
    GLUE_THICKNESS = 1
    NTILES = 3
    STEP      = TILE_SIDE + GLUE_THICKNESS                                        # 151 mm
    CALO_SIDE   = NTILES*TILE_SIDE + (NTILES-1)*GLUE_THICKNESS   # 452 mm
    ORIGIN      = -CALO_SIDE/2.0                                 # –226 mm
    HALF        =  CALO_SIDE/2.0                                 #  226 mm

    GLUE_IDX = 0        # 0 = linea a sinistra (x = –75.5 mm); 1 = a destra (+75.5 mm)
    x_glue = ORIGIN + (GLUE_IDX+1)*TILE_SIDE + (2*GLUE_IDX+1)*GLUE_THICKNESS/2

    glue_centers_axis = [ ORIGIN + TILE_SIDE + GLUE_THICKNESS/2, 
                        ORIGIN + 2*TILE_SIDE + 3*GLUE_THICKNESS/2 ]

    DX = 0.5           # mm (passo trasversale, direzione x)
    DY = 25.0          # mm (passo lungo la colonna, direzione y)
    X_RANGE = 10.0     # mm (±1 cm attorno alla colla)

    xs = np.arange(x_glue - X_RANGE, x_glue + X_RANGE + 1e-6, DX)
    ys = np.arange(-HALF, HALF + 1e-6, DY)

    pts = [(x, y) for x in xs for y in ys]
    
    
    # Job Settings
    nJobs= 0
    dirname = str(OutputFileDir)
    Submission = True
    
    init_dir()
    print("[INFO] Create directories for the Submission pipeline in : "+str(OutputFileDir))
    # -------------------------------------------------- #

    DIR_MAC = "./mac_electron"
    DIR_ROOT = "rootOutput_electron"
    N_EVENTS = 20
    subdir = f"scanGlue_x{GLUE_IDX}"
    for i, (x, y) in enumerate(pts):
        mac   = f"{dirname}/Scan_{i:04d}_x{x:+.1f}_y{y:+.1f}.mac"
        root  = mac.replace(DIR_MAC, DIR_ROOT).replace(".mac", "")
        os.makedirs(os.path.dirname(mac),  exist_ok=True)
        os.makedirs(os.path.dirname(root), exist_ok=True)
        with open(mac, "w") as f:
            f.write("/run/initialize\n"
                    "/tracking/verbose 0\n"
                    "/gps/particle e-\n"
                    f"/gps/position {x:.3f} {y:.3f} 20 mm\n"
                    "/gps/direction 0 0 -1\n"
                    "/gps/pos/type Beam\n"
                    "/gps/ene/type Mono\n"
                    "/gps/ene/mono 1000 MeV\n"
                    f"/RunManager/NameOfOutputFile {root}\n"
                    f"/run/beamOn {N_EVENTS}\n")

            name = mac
            filename = name
    
            print(name)

            name0 = str(name)
            macroFile = dirname+'/macro/'+ name0
            print("[INFO]: File .mac -> ",macroFile)

            shname = dirname+'/SH/' + name0 + '.sh'
            print("[INFO]: source file ->",shname)
            writeshfile(shname,name0)
    
            command = 'chmod +x ' + shname
            os.system(command)

            csiname = dirname+'/SH/' + name0 + '.csi'
            logname = dirname+'/qsub_o/' + name0 + '.log'
            outname = dirname+'/qsub_o/' + name0 + '.o'
            errname = dirname+'/qsub_e/' + name0 + '.e'
    
            BuildCondorSumbitInstructions(csiname,shname,logname,outname,errname,request_cpus=1,request_memory=4000)
            command = 'condor_submit '+ csiname + ' -name ettore'
            
            if(Submission == True):
                print(command) 
                #os.system(command)
                nJobs += 1
                
                
    # break
    print('\n\t----------')
    print('Submitted number of jobs = ', nJobs)

    
    
