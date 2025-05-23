#include "PhotonHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include <iomanip>

PhotonHit::PhotonHit(){
  fOPEne=0.;
  fAbsPos=G4ThreeVector(0.,0.,0.);
}

PhotonHit::~PhotonHit() {}
