#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "OneHit.hh"
#include "PhotonHit.hh"
#include "ComPairHit.hh"
#include "RunAction.hh"

class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:
  HitCollection *GetHitsCollection(G4int HCID,const G4Event* event) const;
  PhotonHitCollection *GetPhotonHitsCollection(G4int PHCID,const G4Event* event) const;
  ComPairHitCollection* GetComPairHitsCollection(G4int CPHCID, const G4Event* event) const;
 
//data members
  RunAction* fRunAction;
  G4int        fHCID;
  G4int        fPHCID;
  G4int        fCPHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
