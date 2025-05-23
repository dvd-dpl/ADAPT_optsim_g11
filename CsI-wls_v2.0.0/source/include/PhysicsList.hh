#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;
class G4VPhysicsConstructor;
class G4OpWLS;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VUserPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList();

    // Construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    virtual void SetCuts();

  // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructDecay();
    void ConstructEM();
    void ConstructOp();
    void ConstructRDecay();

  //for the Messenger
    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);


  private:
  G4VPhysicsConstructor* emPhysicsList;
  G4VPhysicsConstructor* emExtraPhysics;
  G4VPhysicsConstructor* DecayPhysics;
  G4VPhysicsConstructor* HadronElastic;
  G4VPhysicsConstructor* HadronPhysics;
  G4VPhysicsConstructor* StoppingPhysics;
  G4VPhysicsConstructor* IonPysics;
  G4VPhysicsConstructor* NeutronPysics;

  static G4int fVerboseLevel;
  static G4int fMaxNumPhotonStep;
  static G4Cerenkov* fCerenkovProcess;
  static G4Scintillation* fScintillationProcess;
  static G4OpAbsorption* fAbsorptionProcess;
  static G4OpRayleigh* fRayleighScatteringProcess;
  static G4OpMieHG* fMieHGScatteringProcess;
  static G4OpBoundaryProcess* fBoundaryProcess;
  static G4OpWLS* fWLSProcess;
  
  G4bool fScintillationSwitch;
  G4bool fCerenkovSwitch;
  G4bool fRadioActiveDecay;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
