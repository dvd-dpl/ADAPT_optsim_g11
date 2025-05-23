#ifndef Structures_h
#define Structures_h 1
#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4OpBoundaryProcess.hh"

struct VOLUME {
  G4String Name;
  G4int Module;
  G4int Volume;
  G4int Fiber;
  G4int Detector;
};

void InitializeVolume(VOLUME* vol);
void DumpVolume(VOLUME* vol);
bool IsSame(VOLUME* volA,VOLUME* volB);

#endif
