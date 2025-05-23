#include "PhysicsList.hh"
#include "G4RunManager.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhysics.hh"

#include "G4SystemOfUnits.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include "G4RadioactiveDecay.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"


#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hIonisation.hh"
#include "G4Decay.hh"

#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLivermorePhysics.hh"
//#include "OptPhysics.hh"

// For the WLS
#include "G4OpWLS.hh"

//Include Particles

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Eta.hh"
#include "G4EtaPrime.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZero.hh"
#include "G4AntiKaonZero.hh"
#include "G4KaonZeroLong.hh"
#include "G4KaonZeroShort.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

G4int PhysicsList::fVerboseLevel = 0;
G4Cerenkov* PhysicsList::fCerenkovProcess = 0;
G4int PhysicsList::fMaxNumPhotonStep = 500;
G4Scintillation* PhysicsList::fScintillationProcess = 0;
G4OpAbsorption* PhysicsList::fAbsorptionProcess = 0;
G4OpRayleigh* PhysicsList::fRayleighScatteringProcess = 0;
G4OpMieHG* PhysicsList::fMieHGScatteringProcess = 0;
G4OpBoundaryProcess* PhysicsList::fBoundaryProcess = 0;
G4OpWLS* PhysicsList::fWLSProcess = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList(): G4VUserPhysicsList(){

  //Same Physics of FTFP_BERT
  //EM Physics
  emPhysicsList = new G4EmStandardPhysics();
  //Synchroton Radiation & GN Physics
  emExtraPhysics = new G4EmExtraPhysics();
  //Decays
  DecayPhysics = new G4DecayPhysics();
  // Hadron Elastic scattering
  HadronElastic = new G4HadronElasticPhysics();
  // Hadron Physics
  HadronPhysics = new G4HadronPhysicsFTFP_BERT();
  // Stoping Physics
  StoppingPhysics = new G4StoppingPhysics();
  // Ion Physics
  IonPysics = new G4IonPhysics();
  //Neutron Pysics
  NeutronPysics = new G4NeutronTrackingCut();

  fScintillationSwitch=true;
  fCerenkovSwitch=false;
  fRadioActiveDecay=false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList(){
  if(emPhysicsList) delete emPhysicsList;
  if(emExtraPhysics) delete emExtraPhysics;
  if(DecayPhysics) delete DecayPhysics;
  if(HadronElastic) delete HadronElastic;
  //if(HadronPhysics) delete HadronPhysics;
  if(StoppingPhysics) delete StoppingPhysics;
  if(IonPysics) delete IonPysics;
  if(NeutronPysics) delete NeutronPysics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle(){
  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();


  emPhysicsList->ConstructParticle();
  emExtraPhysics->ConstructParticle();
  DecayPhysics->ConstructParticle();
  HadronElastic->ConstructParticle();
  HadronPhysics->ConstructParticle();
  StoppingPhysics->ConstructParticle();
  IonPysics->ConstructParticle();
  NeutronPysics->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess(){
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  ph->AddTransportation();
  ConstructOp();
  if (fRadioActiveDecay) ConstructRDecay();

  //FTFP_BERT
  emPhysicsList->ConstructProcess();
  emExtraPhysics->ConstructProcess();
  DecayPhysics->ConstructProcess();
  HadronElastic->ConstructProcess();
  HadronPhysics->ConstructProcess();
  StoppingPhysics->ConstructProcess();
  IonPysics->ConstructProcess();
  NeutronPysics->ConstructProcess();
}


void PhysicsList::ConstructOp(){

    fCerenkovProcess = new G4Cerenkov("Cerenkov");
    fCerenkovProcess->SetMaxNumPhotonsPerStep(50);
    //fCerenkovProcess->SetMaxBetaChangePerStep(10.0);
    fCerenkovProcess->SetTrackSecondariesFirst(false);
    fScintillationProcess = new G4Scintillation("Scintillation");
    fScintillationProcess->SetTrackSecondariesFirst(false);
    fAbsorptionProcess = new G4OpAbsorption("Absorption");
    fRayleighScatteringProcess = new G4OpRayleigh("RL_Scatter");
    fMieHGScatteringProcess = new G4OpMieHG("MieHG_Scatter");
    fBoundaryProcess = new G4OpBoundaryProcess("Boundary_Process");

    // For the WLS
    fWLSProcess = new G4OpWLS("WLS_Process");
    fWLSProcess->UseTimeProfile("delta");
    fWLSProcess->SetVerboseLevel(fVerboseLevel);
    fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
    fScintillationProcess->SetVerboseLevel(fVerboseLevel);
    fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
    fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
    fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
    fBoundaryProcess->SetVerboseLevel(fVerboseLevel);


     // Use Birks Correction in the Scintillation process
    if(G4Threading::IsMasterThread()){
      G4EmSaturation* emSaturation =
        G4LossTableManager::Instance()->EmSaturation();
      fScintillationProcess->AddSaturation(emSaturation);
    }


    //Particle iterator (Optical Processes)

    auto particleIterator=GetParticleIterator();
    particleIterator->reset();
    while( (*particleIterator)() ){
      G4ParticleDefinition* particle = particleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
      if (fCerenkovSwitch){
        if (fCerenkovProcess->IsApplicable(*particle)) {
  	pmanager->AddProcess(fCerenkovProcess);
  	pmanager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
        }
      }
      if (fScintillationSwitch){
        if (fScintillationProcess->IsApplicable(*particle)) {
  	pmanager->AddProcess(fScintillationProcess);
  	//pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
  	pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
        }
      }
      if (particleName == "opticalphoton") {
        G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
        pmanager->AddDiscreteProcess(fAbsorptionProcess);
        //pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
        //pmanager->AddDiscreteProcess(fMieHGScatteringProcess);
        pmanager->AddDiscreteProcess(fBoundaryProcess);
	pmanager->AddDiscreteProcess(fWLSProcess);
      }
    }
}


void PhysicsList::ConstructRDecay(){
  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  G4bool ARMflag = false;
  radioactiveDecay->SetARM(ARMflag);
  G4LossTableManager* man = G4LossTableManager::Instance();
  G4VAtomDeexcitation* deex = man->AtomDeexcitation();
  if (!deex) {
    G4EmParameters::Instance()->SetAugerCascade(ARMflag);
    deex = new G4UAtomicDeexcitation();
    deex->InitialiseAtomicDeexcitation();
    man->SetAtomDeexcitation(deex);
  }

  // register radioactiveDecay
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetVerbose(G4int verbose){
  fVerboseLevel = verbose;
  fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
  fScintillationProcess->SetVerboseLevel(fVerboseLevel);
  fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
  fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fBoundaryProcess->SetVerboseLevel(fVerboseLevel);
}

void PhysicsList::SetCuts(){
  SetDefaultCutValue(1*m);///0.7*mm
  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
