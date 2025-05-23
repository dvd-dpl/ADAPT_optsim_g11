#include <vector>
#include "G4UnitsTable.hh"
#include "Structures.hh"

void InitializeVolume(VOLUME* vol){
  vol->Name="Void";
  vol->Module=-1;
  vol->Volume=-1;
  vol->Fiber=-1;
  vol->Detector=-1;
}

void DumpVolume(VOLUME* vol){
  G4cout<<"Volume Name: "<<vol->Name<<" Enumerator "<<vol->Module<<" "<<vol->Volume<<" "<<vol->Fiber<<G4endl;
}

bool IsSame(VOLUME* volA,VOLUME* volB){
  bool flag=true;
  if (volA->Name!=volB->Name) flag=false;
  if (volA->Module!=volB->Module) flag=false;
  if (volA->Volume!=volB->Volume) flag=false;
  if (volA->Fiber!=volB->Fiber) flag=false;
  if (volA->Detector!=volB->Detector) flag=false;
  return flag;
}
