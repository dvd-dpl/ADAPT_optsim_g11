#ifndef ComPairHit_h
#define ComPairHit_h 1

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"

class ComPairHit : public G4VHit{
public:
  ComPairHit();
  virtual ~ComPairHit();
  
  // methods from base class
  void Print();

  // set methods
  void SetProcessType(G4int process) {fProcessType = process;};
  
  void SetInteractionPositionX(G4double x) {fIntX = x;};
  void SetInteractionPositionY(G4double y) {fIntY = y;};
  void SetInteractionPositionZ(G4double z) {fIntZ = z;};

  void SetInteractionModule(G4double l) {fIntModule = l;};
  void SetInteractionVolume(G4double v)  {fIntVolume  = v;};
  void SetInteractionFiber(G4double f) {fIntFiber = f;};
  
  void SetFirstParticleID(G4int ParticleID) {fFirstParticleID = ParticleID;};
  void SetFirstEnergy(G4double energy) {fFirstEnergy = fFirstEnergy;};
  void SetFirstDirectionX(G4double x) {fFirstDX = x;};
  void SetFirstDirectionY(G4double y) {fFirstDY = y;};
  void SetFirstDirectionZ(G4double z) {fFirstDZ = z;};

  void SetSecondParticleID(G4int ParticleID) {fSecondParticleID = ParticleID;};
  void SetSecondEnergy(G4double energy) {fSecondEnergy = fSecondEnergy;};
  void SetSecondDirectionX(G4double x) {fSecondDX = x;};
  void SetSecondDirectionY(G4double y) {fSecondDY = y;};
  void SetSecondDirectionZ(G4double z) {fSecondDZ = z;};

  // get methods
  G4int GetProcessType() const { return fProcessType; };
  
  G4double GetInteractionPositionX() const { return fIntX; };
  G4double GetInteractionPositionY() const { return fIntY; };
  G4double GetInteractionPositionZ() const { return fIntZ; };

  G4int GetInteractionModule() const { return fIntModule; };
  G4int GetInteractionVolume()  const { return fIntVolume ; };
  G4int GetInteractionFiber() const { return fIntFiber; };
  
  G4int    GetFirstParticleID()  const { return fFirstParticleID; };
  G4double GetFirstEnergy()      const { return fFirstEnergy;     };
  G4double GetFirstDirectionX()  const { return fFirstDX;         };
  G4double GetFirstDirectionY()  const { return fFirstDY;         };
  G4double GetFirstDirectionZ()  const { return fFirstDZ;         };

  G4int GetSecondParticleID()    const { return fSecondParticleID; };
  G4double GetSecondEnergy()     const { return fSecondEnergy;     };
  G4double GetSecondDirectionX() const { return fSecondDX;         };
  G4double GetSecondDirectionY() const { return fSecondDY;         };
  G4double GetSecondDirectionZ() const { return fSecondDZ;         };
  
private:
  G4int         fProcessType;
  G4double      fIntX    , fIntY   , fIntZ    ;
  G4int         fIntModule, fIntVolume, fIntFiber;
  G4int         fFirstParticleID;
  G4double      fFirstEnergy;
  G4double      fFirstDX,fFirstDY,fFirstDZ;
  G4int         fSecondParticleID;
  G4double      fSecondEnergy;
  G4double      fSecondDX,fSecondDY,fSecondDZ;
  
};

typedef G4THitsCollection<ComPairHit> ComPairHitCollection;


#endif
