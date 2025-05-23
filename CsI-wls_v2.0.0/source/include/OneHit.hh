#ifndef OneHit_h
#define OneHit_h 1

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include <vector>
#include "Structures.hh"

class OneHit : public G4VHit{
public:

  OneHit();
  virtual ~OneHit();

  // methods from base class

  // set methods
  void SetPrimaryVertex(std::vector<G4double> pver){
    for (int i=0;i<pver.size();i++){
      fPrimaryVertex.push_back(pver.at(i));
    }
  };
  void SetVolumes(std::vector<VOLUME> vol){
    for (int i=0;i<vol.size();i++){
      fVolumes.push_back(vol.at(i));
    }
  };
  void SetEnergy(std::vector<G4double> ene){
    for (int i=0;i<ene.size();i++){
      fEnergyDep.push_back(ene.at(i));
    }
  };
  void SetAbsorbedPhotons(std::vector<G4int> ene){
    for (int i=0;i<ene.size();i++){
      fPhotAbs.push_back(ene.at(i));
    }
  };

  void SetAbsorbedPhotonsWLS(std::vector<G4int> ene){
    for (int i=0;i<ene.size();i++){
      fPhotAbsWLS.push_back(ene.at(i));
    }
  };

  void SetAbsorbedPhotonsScint(std::vector<G4int> ene){
    for (int i=0;i<ene.size();i++){
      fPhotAbsScint.push_back(ene.at(i));
    }
  };

  void SetGeneratedPhotons(std::vector<G4int> ene){
    for (int i=0;i<ene.size();i++){
      fPhotGen.push_back(ene.at(i));
    }
  };

  void SetGeneratedPhotonsWLS(std::vector<G4int> ene){
    for (int i=0;i<ene.size();i++){
      fPhotGenWLS.push_back(ene.at(i));
    }
  };

  void SetGeneratedPhotonsScint(std::vector<G4int> ene){
    for (int i=0;i<ene.size();i++){
      fPhotGenScint.push_back(ene.at(i));
    }
  };

public:
  //G4double      fEDep;
  //G4double      fTrackLength;
  std::vector<G4double> fPrimaryVertex;
  std::vector<VOLUME> fVolumes;
  std::vector<G4double> fEnergyDep;
  std::vector<G4int> fPhotGen;
  std::vector<G4int> fPhotGenWLS;
  std::vector<G4int> fPhotGenScint;
  std::vector<G4int> fPhotAbs;
  std::vector<G4int> fPhotAbsWLS;
  std::vector<G4int> fPhotAbsScint;
  std::vector<G4double> fEnergyInStep;
  std::vector<G4ThreeVector> fPositionInStep;

};

typedef G4THitsCollection<OneHit> HitCollection;


#endif
