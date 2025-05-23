#include "OneHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include <iomanip>

OneHit::OneHit(){
  fPrimaryVertex.clear();
  fVolumes.clear();
  fEnergyDep.clear();
  fEnergyInStep.clear();
  fPositionInStep.clear();
}

OneHit::~OneHit() {}
