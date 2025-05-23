#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4OpBoundaryProcess.hh"
#include "DetectorConstruction.hh"
#include "OneHit.hh"
#include "PhotonHit.hh"
#include "Structures.hh"
#include "ComPairHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class SensitiveDetector : public G4VSensitiveDetector
{
public:
  //Constructor
  SensitiveDetector(const G4String &SDname, const G4String &HitCollectionName,const G4String &PhotonHitCollectionName,const G4String &ComPairHitCollectionName);
  //Distructor
  virtual ~SensitiveDetector();

  void Initialize(G4HCofThisEvent *HCE);
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);
  void EndOfEvent(G4HCofThisEvent *HCE);
  void ProcessEventHit(G4Step *step);
  void ProcessOpticalPhoton(G4Step *step);



  // Add methods
  // // Total deposit Energy
  void AddEdep(G4double edep) {fE += edep;};
  void DeleteTotalE()  { fE = 0.;};
  G4double GetTotalE()  const {return fE;};

  // // Total Track Length
  void AddTrackLength(G4double l) {fL += l;};
  void DeleteTotalTrackLength()  { fL = 0.;};
  G4double GetTotalTrackLength()  const {return fL;};

  /// For ComPair Tree
  G4int processID(G4String procName);
  G4bool check_FirstInteraction(G4Step *aStep);

  void setFlagComPair(G4bool flag) { fComPairFlag = flag;};
  G4bool getFlagComPair() const { return fComPairFlag ; };
  G4bool processComPair(G4Step *aStep);

  void addComptonCounter() {fComptonCounter += 1;};
  G4int getComptonCounter()  const { return fComptonCounter;};
  void deleteComptonCounter() {fComptonCounter = 0;};

  void addPairCounter() {fPairCounter += 1;};
  G4int getPairCounter()  const { return fPairCounter;};
  void deletePairCounter() {fPairCounter = 0;};


  // // // // // //
  //Utility Functions
  G4OpBoundaryProcessStatus RetrieveCurrentPhotonStatus();
  G4bool checkBoundary(G4OpBoundaryProcessStatus theStatus, G4String Bound);
  G4String Status(G4OpBoundaryProcessStatus theStatus);
  G4bool IsAGamma(G4Step* aStep);
  G4bool IsAnOpticalPhoton(G4Step* aStep);
  void RetrivePrimaryVertex(G4Track* aTrack);
  void FindVolumeEnumerator(const G4VTouchable* Touchable,VOLUME* vol);
  void CleanSDMemory();
  void PrintSDMemoryStatus();

private:
  HitCollection *fHitCollection;
  PhotonHitCollection *fPhotonHitCollection;
  ComPairHitCollection *fComPairHitCollection;

  G4double      fE;
  G4double      fL;

  VOLUME fVVolume;
  //Vectors for EventHit
  std::vector<G4double> fPrimaryVertex;
  std::vector<VOLUME> fVolumes;
  std::vector<G4double> fEnergyDep;
  std::vector<G4int> fPhotGen;

  std::vector<G4int> fPhotGenWLS;
  std::vector<G4int> fPhotGenScint;

  std::vector<G4int> fPhotAbs;

  std::vector<G4int> fPhotAbsWLS;
  std::vector<G4int> fPhotAbsScint;

  std::vector<G4int> fPhotonTrackID;
  std::vector<G4int> fPhotonParentID;
  std::vector<G4double> fPhotonEnergy;

  std::vector<G4double> fEnergyInStep;
  std::vector<G4ThreeVector> fPositionInStep;

  //opticalphoton
  G4double fOPEne;
  G4ThreeVector fAbsPos;
  VOLUME fAbsVolumes;
  G4int fCreatorProcessNumber;
  G4String fCreatorProcessName;
  G4double fLocalTime;
  G4double fGlobalTime;

  // For ComPairTree
  G4int fParticleID;
  G4int fProcessID;

  G4bool fComPairFlag;
  G4int fComptonCounter;
  G4int fPairCounter;

};


#endif
