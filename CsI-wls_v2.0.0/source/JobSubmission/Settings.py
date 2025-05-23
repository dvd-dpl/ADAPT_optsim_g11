## 0.o.0.o.O.o.O.o.O.o.O ##
##  GeometricalSettings  ##
#Tracker#
fTrakerViews    = 2
fNumberOfLayers = 4 
fTrakerGap      = 1      #in mm
fFiberRadius    = 0.230  #in mm
fFiberLength    = 10     #in cm

#Calorimeter"
fCaloLengthZ    = 10 #in cm
fCaloPixelSize  = 1  #in cm
fCaloGap        = 3  #in cm
  

## 0.o.0.o.O.o.O ##
##  gps Settings ##
fparName = "e-" 
fAion = 16
fZion = 8

fE = 100 #in MeV

fPosX = 0
fPosY = 0
fPosZ = 10

fDirX = 0
fDirY = 0
fDirZ = -1
## 0.o.0.o.O.o.O ##

## 0.o.0.o.O.o.O ##
##  Job Settings ##

## 0.o.0.o.O.o.O ##
##  Job Settings ##
StartRunID = 0
InputDirPath = "/lustre/home/dserini/FiberTraker/"
#InputDirPath = "/home/davide/Simulations/tilesim/OverallTkrSimulation"
MainName = "eSim"
OutputFileDir = str(InputDirPath)+"/JobsOutput/"+str(MainName)
OutputFileName = str(MainName)
ProjectName = "FibTracker"
exe = "FibTracker"
## 0.o.0.o.O.o.O ##
    
