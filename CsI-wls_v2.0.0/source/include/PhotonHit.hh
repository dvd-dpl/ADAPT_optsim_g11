#ifndef PhotonHit_h
#define PhotonHit_h 1

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include <vector>
#include "Structures.hh"
#include "G4String.hh"
class PhotonHit : public G4VHit{
public:

  PhotonHit();
  virtual ~PhotonHit();

  // methods from base class

  // set methods
  void SetPhotonEnergy(G4double ene){
    fOPEne=ene;
  };
  void SetPhotonAbsPosition(G4ThreeVector pos){
    fAbsPos=pos;
  };
  void SetPhotonAbsVolume(VOLUME vol){
    fAbsVolumes=vol;
  };

  void SetPhotonCreatorProcessNumber(G4int proc){
    fCreatorProcessNumber = proc;
  };

  void SetPhotonCreatorProcessName(G4String proc){
    fCreatorProcessName = proc;
  };

  void SetPhotonGlobalTime(G4double time){
    fGlobalTime= time;
  };

  void SetPhotonLocalTime(G4double time){
    fLocalTime= time;
  };
  
public:
  G4double fOPEne;
  G4ThreeVector fAbsPos;
  VOLUME fAbsVolumes;
  G4int fCreatorProcessNumber;
  G4String fCreatorProcessName;
  G4double fGlobalTime;
  G4double fLocalTime;
};

typedef G4THitsCollection<PhotonHit> PhotonHitCollection;


#endif
