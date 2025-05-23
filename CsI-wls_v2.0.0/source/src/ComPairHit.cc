#include "ComPairHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include <iomanip>

ComPairHit::ComPairHit()
  :fProcessType(0),fIntX(0.), fIntY(0.), fIntZ(0.), fIntModule(0), fIntVolume(0), fIntFiber(0), fFirstParticleID(0), fFirstEnergy(0.), fFirstDX(0.), fFirstDY(0.),fFirstDZ(0.),fSecondParticleID(0), fSecondEnergy(0), fSecondDX(0) ,fSecondDY(0) ,fSecondDZ(0){}
   
ComPairHit::~ComPairHit() {}

void ComPairHit::Print(){}
