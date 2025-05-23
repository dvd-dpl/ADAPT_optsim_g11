#include "SensitiveDetector.hh"
#include "OneHit.hh"
#include "ComPairHit.hh"

#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistoryHandle.hh"
#include "G4TouchableHistory.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4Tubs.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "PhysicsList.hh"
#include "G4GeometryTolerance.hh"
#include <math.h>
#include "Structures.hh"

using namespace std;

static const G4double GeometricalTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();

// My definitions -> ComPairTree
G4int UNKNOWN = -2;
G4int PRIMARY = -1;
G4int PHOTOELECTRIC = 0;
G4int PAIR = 1;
G4int COMPTON = 2;

G4int ELECTRON = +11;
G4int POSITRON = -11;
G4int GAMMA    = +22;

// My definitions -> Photon Hit (process generator)
G4int SCINTILLATION = 0;
G4int WLS = 1;

SensitiveDetector::SensitiveDetector(const G4String &SDname,const G4String &HitCollectionName,const G4String &PhotonHitCollectionName,  const G4String &ComPairHitCollectionName): G4VSensitiveDetector(SDname),fHitCollection(NULL),fPhotonHitCollection(NULL),fComPairHitCollection(NULL),fComPairFlag(false),fComptonCounter(0),fPairCounter(0){
  //opticalphoton
  fOPEne=0;
  //Initialize vectors
  InitializeVolume(&fVVolume);
  for (int i=0;i<6;i++){
    fPrimaryVertex.push_back(0.);
  }

  G4cout<<"Creating SD with name: "<<SDname<<G4endl;
  collectionName.insert(HitCollectionName);
  collectionName.insert(PhotonHitCollectionName);
  collectionName.insert(ComPairHitCollectionName);
}

SensitiveDetector::~SensitiveDetector(){}

void SensitiveDetector::Initialize(G4HCofThisEvent *HCE){
  fHitCollection = new HitCollection(GetName(),collectionName[0]);
  static G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); //<<-- this is to get an ID for the colletionName[0]
  G4cout<<"*** "<<fHitCollection->GetName()<<" initialized [ID = "<<HCID<<"]"<<G4endl;
  HCE->AddHitsCollection(HCID, fHitCollection);

  fPhotonHitCollection = new PhotonHitCollection(GetName(),collectionName[1]);
  static G4int PHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]); //<<-- this is to get an ID for the colletionName[0]
  G4cout<<"*** "<<fPhotonHitCollection->GetName()<<" initialized [ID = "<<PHCID<<"]"<<G4endl;
  HCE->AddHitsCollection(PHCID, fPhotonHitCollection);

  fComPairHitCollection = new ComPairHitCollection(GetName(),collectionName[2]);
  static G4int CPHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[2]); //<<-- this is to get an ID for the colletionName[0]
  G4cout<<"*** "<<fComPairHitCollection->GetName()<<" initialized [ID = "<<CPHCID<<"]"<<G4endl;
  HCE->AddHitsCollection(CPHCID, fComPairHitCollection);
}

G4int SensitiveDetector::processID(G4String procName){
  G4int myProc;
  if(procName.contains("comp"))
    myProc = COMPTON;
  else if(procName.contains("conv"))
    myProc = PAIR;
  else if(procName.contains("phot"))
    myProc = PHOTOELECTRIC;
  else if(procName.contains("primary"))
    myProc = PRIMARY;
  else
    myProc = UNKNOWN;
  return myProc;
}


G4bool SensitiveDetector::processComPair(G4Step *aStep){
  G4bool ComPairFlag = SensitiveDetector::getFlagComPair();
  ComPairHit *aComPairHit = new ComPairHit();

  // Gamma infos
  G4String thisParticleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4int thisParticleID = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  G4String thisProcess =  aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  G4double thisEnergy = aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy();
  G4ThreeVector thisMomentum = aStep->GetTrack()->GetDynamicParticle()->GetMomentumDirection();

  G4StepPoint *preStepPoint =  aStep->GetPreStepPoint();
  //const G4VTouchable* Touchable=PreStep->GetTouchable();
  const G4VTouchable* theTouchable = preStepPoint->GetTouchable();

  FindVolumeEnumerator(theTouchable,&fVVolume);

  //G4int FiberNo  = theTouchable->GetReplicaNumber(1);
  //G4int VolumeNo   = theTouchable->GetReplicaNumber(2);
  //G4int ModuleNo  = theTouchable->GetReplicaNumber(3);

  G4int FiberNo  = fVVolume.Fiber;
  G4int VolumeNo = fVVolume.Volume;
  G4int ModuleNo = fVVolume.Module;

  G4ThreeVector worldPos = preStepPoint->GetPosition();

  if(thisParticleID==GAMMA){
    if(thisProcess.contains("compt"))
      SensitiveDetector::addComptonCounter();
    else if(thisProcess.contains("conv"))
      SensitiveDetector::addPairCounter();

    G4int ProcessType;
    G4double  IntX;
    G4double  IntY;
    G4double  IntZ;
    G4int     IntModule;
    G4int     IntVolume;
    G4int     IntFiber;
    G4int     FirstParticleID;
    G4double  FirstEnergy;
    G4double  FirstDX;
    G4double  FirstDY;
    G4double  FirstDZ;
    G4int     SecondParticleID;
    G4double  SecondEnergy;
    G4double  SecondDX;
    G4double  SecondDY;
    G4double  SecondDZ;

    if(ComPairFlag==false){
      if(thisProcess.contains("compt")){
	const G4TrackVector *sec_vec = aStep->GetfSecondary();
	if(sec_vec->size()>0){
	  G4String mySecParticle_Name  = sec_vec->at(0)->GetDefinition()->GetParticleName();
	  G4int mySecParticle          = sec_vec->at(0)->GetDynamicParticle()->GetPDGcode();
	  G4double mySecEnergy         = sec_vec->at(0)->GetDynamicParticle()->GetKineticEnergy();
	  G4ThreeVector mySecDirection = sec_vec->at(0)->GetDynamicParticle()->GetMomentumDirection();

	  G4cout<<" [INFO ComPair]: !! Firts COMPTON Interaction !! "<<G4endl;
	  G4cout<<" Particle = "<<mySecParticle_Name<<" ("<<mySecParticle<<") Energy = "<<G4BestUnit(mySecEnergy,"Energy")
		<<" Direction = ("<<mySecDirection[0]<<","<<mySecDirection[1]<<","<<mySecDirection[2]<<")"<<G4endl;

	  G4cout<<" New "<< thisParticleName<<" ("<<thisParticleID<<") KineticEnergy = "<<G4BestUnit(thisEnergy,"Energy")
		<<" New "<< thisParticleName<<" ("<<thisParticleID<<") Direction = ("<<thisMomentum[0]<<","<<thisMomentum[1]<<","<<thisMomentum[2]<<")"<<G4endl;

	  G4cout<<" Interaction Point = ("<<G4BestUnit(worldPos[0],"Length")<<","<<G4BestUnit(worldPos[1],"Length")<<","<<G4BestUnit(worldPos[2],"Length")<<")"
		<<" [Module = "<<ModuleNo<<" Volume = "<<VolumeNo<<" Fiber = "<<FiberNo<<"]"<<G4endl;

	  G4cout<<" EnergySum = "<<G4BestUnit(thisEnergy+mySecEnergy,"Energy")<<G4endl;
	  G4cout<<"\t\t ===================================== \t\t"<<G4endl;

	  SensitiveDetector::addComptonCounter();
	  SensitiveDetector::setFlagComPair(true);

	  ProcessType = COMPTON;
	  IntX = worldPos[0];
	  IntY = worldPos[1];
	  IntZ = worldPos[2];
	  IntModule = ModuleNo;
	  IntVolume  = VolumeNo;
	  IntFiber = FiberNo;

	  // electron
	  FirstParticleID = mySecParticle;
	  FirstEnergy = mySecEnergy;
	  FirstDX = mySecDirection[0];
	  FirstDY = mySecDirection[1];
	  FirstDZ = mySecDirection[2];

	  // gamma
	  SecondParticleID = thisParticleID;
	  SecondEnergy = thisEnergy;
	  SecondDX = thisMomentum[0];
	  SecondDY = thisMomentum[1];
	  SecondDZ = thisMomentum[2];
	}
      }
      else if(thisProcess.contains("conv")){
	const G4TrackVector *sec_vec  = aStep->GetfSecondary();
	if(sec_vec->size()>0){
	  G4String mySecParticle_Name0  = sec_vec->at(0)->GetDefinition()->GetParticleName();
	  G4int mySecParticle0          = sec_vec->at(0)->GetDynamicParticle()->GetPDGcode();
	  G4double mySecEnergy0         = sec_vec->at(0)->GetDynamicParticle()->GetKineticEnergy();
	  G4ThreeVector mySecDirection0 = sec_vec->at(0)->GetDynamicParticle()->GetMomentumDirection();

	  G4String mySecParticle_Name1  = sec_vec->at(1)->GetDefinition()->GetParticleName();
	  G4int mySecParticle1          = sec_vec->at(1)->GetDynamicParticle()->GetPDGcode();
	  G4double mySecEnergy1         = sec_vec->at(1)->GetDynamicParticle()->GetKineticEnergy();
	  G4ThreeVector mySecDirection1 = sec_vec->at(1)->GetDynamicParticle()->GetMomentumDirection();

	  G4cout<<" [INFO ComPair]: !! First PAIR Interaction !! "<<G4endl;
	  G4cout<<" Particle_0 = "<<mySecParticle_Name0<<" ("<<mySecParticle0<<") Energy_0 = "<<G4BestUnit(mySecEnergy0,"Energy")
		<<" Direction_0 = ("<<mySecDirection0[0]<<","<<mySecDirection0[1]<<","<<mySecDirection0[2]<<")"<<G4endl;
	  G4cout<<" Particle_1 = "<<mySecParticle_Name1<<" ("<<mySecParticle1<<") Energy_1 = "<<G4BestUnit(mySecEnergy1,"Energy")
		<<" Direction_1 = ("<<mySecDirection1[0]<<","<<mySecDirection1[1]<<","<<mySecDirection1[2]<<")"<<G4endl;

	  G4cout<<" Interaction Point = ("<<G4BestUnit(worldPos[0],"Length")<<","<<G4BestUnit(worldPos[1],"Length")<<","<<G4BestUnit(worldPos[2],"Length")<<")"
		<<" [Module = "<<ModuleNo<<" Volume = "<<VolumeNo<<" Fiber = "<<FiberNo<<"]"<<G4endl;

	  G4cout<<" EnergySum = "<<G4BestUnit(mySecEnergy0+mySecEnergy1,"Energy")<<G4endl;
	  G4cout<<"\t\t ===================================== \t\t"<<G4endl;

	  SensitiveDetector::setFlagComPair(true);
	  SensitiveDetector::addPairCounter();

	  ProcessType = PAIR;
	  IntX = worldPos[0];
	  IntY = worldPos[1];
	  IntZ = worldPos[2];
	  IntModule = ModuleNo;
	  IntVolume  = VolumeNo;
	  IntFiber = FiberNo;

	  if(mySecParticle0 == ELECTRON){
	    // electron
	    FirstParticleID = mySecParticle0;
	    FirstEnergy = mySecEnergy0;
	    FirstDX = mySecDirection0[0];
	    FirstDY = mySecDirection0[1];
	    FirstDZ = mySecDirection0[2];

	    // positron
	    SecondParticleID = mySecParticle1;
	    SecondEnergy = mySecEnergy1;
	    SecondDX = mySecDirection1[0];
	    SecondDY = mySecDirection1[1];
	    SecondDZ = mySecDirection1[2];
	  }

	  else if(mySecParticle1 == POSITRON){
	    // electron
	    FirstParticleID = mySecParticle1;
	    FirstEnergy = mySecEnergy1;
	    FirstDX = mySecDirection1[0];
	    FirstDY = mySecDirection1[1];
	    FirstDZ = mySecDirection1[2];

	    // positron
	    SecondParticleID = mySecParticle0;
	    SecondEnergy = mySecEnergy0;
	    SecondDX = mySecDirection0[0];
	    SecondDY = mySecDirection0[1];
	    SecondDZ = mySecDirection0[2];
	  }
	}
      }

      if(thisProcess.contains("conv") or thisProcess.contains("comp")){
	aComPairHit->SetProcessType(ProcessType);

	aComPairHit->SetInteractionPositionX(IntX);
	aComPairHit->SetInteractionPositionY(IntY);
	aComPairHit->SetInteractionPositionZ(IntZ);

	aComPairHit->SetInteractionModule(IntModule);
	aComPairHit->SetInteractionVolume(IntVolume);
	aComPairHit->SetInteractionFiber(IntFiber);

	aComPairHit->SetFirstParticleID(FirstParticleID);
	aComPairHit->SetFirstEnergy(FirstEnergy);
	aComPairHit->SetFirstDirectionX(FirstDX);
	aComPairHit->SetFirstDirectionY(FirstDY);
	aComPairHit->SetFirstDirectionZ(FirstDZ);

	aComPairHit->SetSecondParticleID(SecondParticleID);
	aComPairHit->SetSecondEnergy(SecondEnergy);
	aComPairHit->SetSecondDirectionX(SecondDX);
	aComPairHit->SetSecondDirectionY(SecondDY);
	aComPairHit->SetSecondDirectionZ(SecondDZ);

	fComPairHitCollection->insert(aComPairHit);
      }
    }
  }
  return true;
}



G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *){

  //EventHit
  ProcessEventHit(aStep);
  processComPair(aStep);

  // Optical Photon
  if(IsAnOpticalPhoton(aStep)){
    ProcessOpticalPhoton(aStep);
  }

  return true;
}

void SensitiveDetector::ProcessEventHit(G4Step *aStep){
  G4StepPoint* PreStep=aStep->GetPreStepPoint();
  G4Track* aTrack=aStep->GetTrack();
  G4double EDep=-aStep->GetDeltaEnergy();
  const G4DynamicParticle* DynamicParticle = aTrack->GetDynamicParticle();
  G4PrimaryParticle* PrimaryParticle=DynamicParticle->GetPrimaryParticle();
  const G4VTouchable* Touchable=PreStep->GetTouchable();
  G4VPhysicalVolume* Volume=Touchable->GetVolume();
  G4int CopyN = Volume->GetCopyNo();
  G4int pho_STATUS = aStep->GetTrack()->GetTrackStatus();
  G4String VName=Volume->GetName();
  const vector<const G4Track*>* sec_vec = aStep->GetSecondaryInCurrentStep();
  G4String creator_process = "NONE";
  G4int stepNum = aStep->GetTrack()->GetCurrentStepNumber();

  //PrimaryParticle
  if (PrimaryParticle){
    RetrivePrimaryVertex(aTrack); //INFO
  }

  //Every Step in Calo
  if (EDep>0. && Volume->GetName().contains("Calorimeter")){
    G4cout<<"CIAO Energy: "<<G4BestUnit(EDep,"Energy")<<"Position "<<PreStep->GetPosition()<<G4endl;
    fEnergyInStep.push_back(EDep);
    fPositionInStep.push_back(PreStep->GetPosition());
  }
  //EveryStep
  FindVolumeEnumerator(Touchable,&fVVolume);
  bool flag=true;

  for (int j=0;j<fVolumes.size();j++){
    if (IsSame(&fVVolume,&fVolumes.at(j))){
      flag=false;
      //Edep
      fEnergyDep.at(j)+=EDep;
      //Abs Phot
      if(pho_STATUS==2 && IsAnOpticalPhoton(aStep)){
	fPhotAbs.at(j)+=1;
	creator_process = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
	if(creator_process.contains("WLS"))
	  fPhotAbsWLS.at(j)+=1;
	if(creator_process.contains("Scint"))
	  fPhotAbsScint.at(j)+=1;
      }
      //Phot Generated
      //for (int k=0;k<sec_vec->size();k++){
      if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="opticalphoton"){
	G4String process  = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
	G4String particle = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
	if(stepNum==1){
	  //G4String sec_name = sec_vec->at(k)->GetParticleDefinition()->GetParticleName();
          fPhotGen.at(j)+=1;
	  //creator_process = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
	  if(process.contains("WLS"))
	    fPhotGenWLS.at(j)+=1;
	  if(process.contains("Scint"))
	    fPhotGenScint.at(j)+=1;

	  //G4String creator_process = sec_vec->at(k)->GetCreatorProcess()->GetProcessName();
	  //G4cout << "CP: " << creator_process << G4endl;
	  //if(!creator_process.contains("WLS"))
	  //G4cout << "CP: " << creator_process << G4endl;
	}
      }
    }
  }
  if(flag){
    fVolumes.push_back(fVVolume);
    //Edep
    fEnergyDep.push_back(EDep);
    //Abs Phot
    if(pho_STATUS==2 && IsAnOpticalPhoton(aStep)){
      fPhotAbs.push_back(0);
      fPhotAbsWLS.push_back(0);
      fPhotAbsScint.push_back(0);
    }
    else{
      fPhotAbs.push_back(0);
      fPhotAbsWLS.push_back(0);
      fPhotAbsScint.push_back(0);
    }

    //Generated Photons
    fPhotGen.push_back(0);
    fPhotGenWLS.push_back(0);
    fPhotGenScint.push_back(0);
    if(stepNum==1){
      if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="opticalphoton") {
  	fPhotGen.back()+=1;
        fPhotGenWLS.back()+=1;
      	fPhotGenScint.back()+=1;
      }
    }
  }
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*){
  //Fill the hit
  OneHit *aHit = new OneHit();
  int cc=0;

  std::vector<VOLUME> Volumes;
  std::vector<G4double> EnergyDep;
  std::vector<G4int> PhotGen;
  std::vector<G4int> PhotGenWLS;
  std::vector<G4int> PhotGenScint;
  std::vector<G4int> PhotAbs;
  std::vector<G4int> PhotAbsWLS;
  std::vector<G4int> PhotAbsScint;

  for(int jj=0;jj<fEnergyDep.size();jj++){
    if(fEnergyDep.at(jj)>0 or fPhotAbs.at(jj)>0 or fPhotGen.at(jj)>0){
      cc++;
      // cout<<" entry_"<<jj<<" ioConto = "<<cc<<" (VectorSize = "<<fEnergyDep.size()<<")"
      // 	  <<" fVolumes : "<<fVolumes.at(jj).Name
      // 	  <<" fEnergyDep: "<<fEnergyDep.at(jj)
      // 	  <<" fPhotAbs: "<<fPhotAbs.at(jj)
      // 	  <<" fPhotGen: "<<fPhotGen.at(jj)
      // 	  <<endl;
      Volumes.push_back(fVolumes.at(jj));
      EnergyDep.push_back(fEnergyDep.at(jj));
      PhotAbs.push_back(fPhotAbs.at(jj));
      PhotAbsWLS.push_back(fPhotAbsWLS.at(jj));
      PhotAbsScint.push_back(fPhotAbsScint.at(jj));
      PhotGen.push_back(fPhotGen.at(jj));
      PhotGenWLS.push_back(fPhotGenWLS.at(jj));
      PhotGenScint.push_back(fPhotGenScint.at(jj));
    }
  }
  aHit->SetPrimaryVertex(fPrimaryVertex);
  aHit->SetVolumes(Volumes);
  aHit->SetEnergy(EnergyDep);

  aHit->SetAbsorbedPhotons(PhotAbs);
  aHit->SetAbsorbedPhotonsWLS(PhotAbsWLS);
  aHit->SetAbsorbedPhotonsScint(PhotAbsScint);

  aHit->SetGeneratedPhotons(PhotGen);
  aHit->SetGeneratedPhotonsWLS(PhotGenWLS);
  aHit->SetGeneratedPhotonsScint(PhotGenScint);

  aHit->fEnergyInStep=fEnergyInStep;
  aHit->fPositionInStep=fPositionInStep;

  fHitCollection->insert(aHit);

  // cout<<" *Resume "<<fEnergyDep.size()<<" "<<EnergyDep.size()<<endl;
  //Clean Memory
  Volumes.clear();
  EnergyDep.clear();
  PhotGen.clear();
  PhotAbs.clear();
  PhotGenWLS.clear();
  PhotAbsWLS.clear();
  PhotGenScint.clear();
  PhotAbsScint.clear();
  fEnergyInStep.clear();
  fPositionInStep.clear();

  G4int ioConto= 0;
  for (int i=0;i<fPhotonTrackID.size();i++){
    for (int j=0;j<fPhotonParentID.size();j++){
      if(fPhotonTrackID.at(i)==fPhotonParentID.at(j)){
	//cout<<" ** Scintillation photon padre = "<<G4BestUnit(fPhotonEnergy.at(i),"Energy")<<" WLS photon generated "<<G4BestUnit(fPhotonEnergy.at(j),"Energy")<<endl;
	//if((fPhotonEnergy.at(i)-fPhotonEnergy.at(j))<0.)
	//{cout<<"!!!!ALLERT...E_WLS>E_Scint"<<endl;
	//ioConto++;}
      }
    }
  }
  G4cout<<" Bad events = "<<ioConto<<endl;
  /*
  for (int i=0;i<fVolumes.size();i++){
    G4cout<<"\n";
    G4cout<<" --- Sono nel volume: "<<fVolumes.at(i).Name<<G4endl;
    G4cout<<"G = "<<fPhotGen.at(i)
	  <<" GWLS = "<<fPhotGenWLS.at(i)
	  <<" GScint = "<<fPhotGenScint.at(i)
	  <<" *** GWLS + GScint = "<<fPhotGenWLS.at(i)+fPhotGenScint.at(i)
	  <<G4endl;
    G4cout<<"A = "<<fPhotAbs.at(i)
	  <<" AWLS = "<<fPhotAbsWLS.at(i)
	  <<" AScint = "<<fPhotAbsScint.at(i)
	  <<" *** AWLS + AScint = "<<fPhotAbsWLS.at(i)+fPhotAbsScint.at(i)
	  <<G4endl;

  }
  */
  G4cout<<"!!!EndOfEvent!!!"<<G4endl;
  //DumpInfo
  /*
  G4cout<<"Vertex Position"<<G4endl;
  for (int i=0;i<fPrimaryVertex.size();i++) G4cout<<"Vertex Attributes are "<<fPrimaryVertex.at(i)<<G4endl;
  for (int i=0;i<fVolumes.size();i++){
    G4cout<<"\n";
    DumpVolume(&fVolumes.at(i));
    G4cout<<"Energy Dep "<<G4BestUnit(fEnergyDep.at(i),"Energy")<<G4endl;
    G4cout<<"Photons Generated "<<fPhotGen.at(i)<<G4endl;
    G4cout<<"Photons Absorbed "<<fPhotAbs.at(i)<<G4endl;
  }
  */
  //Clean Memory
  fVolumes.clear();
  fEnergyDep.clear();
  fPhotGen.clear();
  fPhotAbs.clear();
  fPhotGenWLS.clear();
  fPhotAbsWLS.clear();
  fPhotGenScint.clear();
  fPhotAbsScint.clear();
}

void SensitiveDetector::ProcessOpticalPhoton(G4Step *aStep){
  G4StepPoint* PreStep=aStep->GetPreStepPoint();
  G4Track* aTrack=aStep->GetTrack();
  const G4DynamicParticle* DynamicParticle = aTrack->GetDynamicParticle();
  const G4VTouchable* Touchable=PreStep->GetTouchable();
  G4VPhysicalVolume* Volume=Touchable->GetVolume();
  G4int CopyN = Volume->GetCopyNo();
  G4String VName=Volume->GetName();
  G4double KinEne =  DynamicParticle->GetKineticEnergy();
  G4int pho_STATUS = aStep->GetTrack()->GetTrackStatus();

  G4String creator_process = "NONE";
  G4int creatorProcessNumber = -999;
  G4double localTime = 0.;
  G4double globalTime = 0.;

  G4OpBoundaryProcessStatus TheStatus=RetrieveCurrentPhotonStatus();
  if(aStep->GetPostStepPoint()->GetTouchable()->GetVolume()){
    if (checkBoundary(TheStatus,"Reflection")) {
      G4String VolumeName = aStep->GetPostStepPoint()->GetTouchable()->GetVolume()->GetName();
      //G4cout<<"Riflessione qui "<<VolumeName<<G4endl;
    }
    if (checkBoundary(TheStatus,"Refraction")){
      G4String VolumeName = aStep->GetPostStepPoint()->GetTouchable()->GetVolume()->GetName();
      //G4cout<<"Rifrazione qui"<<VolumeName<<G4endl;
    }
  }

  if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="opticalphoton" and aStep->GetTrack()->GetCurrentStepNumber()==1){
    fPhotonTrackID.push_back(aStep->GetTrack()->GetTrackID());
    fPhotonParentID.push_back(aStep->GetTrack()->GetParentID());
    fPhotonEnergy.push_back(aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy());
  }
  //End Of Photon Hit
  if ((fWorldBoundary==aStep->GetPostStepPoint()->GetStepStatus()) || (pho_STATUS==2)){
    // Tag for photons
    creator_process = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
    if(creator_process.contains("Scint"))
       creatorProcessNumber=SCINTILLATION;
    if(creator_process.contains("WLS"))
      creatorProcessNumber=WLS;
    localTime = aStep->GetTrack()->GetLocalTime();
    globalTime = aStep->GetTrack()->GetGlobalTime();

    fLocalTime = localTime;
    fGlobalTime = globalTime;

    fCreatorProcessNumber = creatorProcessNumber;
    fCreatorProcessName = creator_process;
    //cout<<"Io sono un fotone ottico e sono stato creato dal seguente processo: "<<creator_process<<endl;
    //cout<<"Il mio tempo di arrivo è (local) = "<<G4BestUnit(localTime,"Time")<<" (global) = "<<G4BestUnit(globalTime,"Time")<<endl;

    //Retrive
    //Photon Energy
    fOPEne=KinEne;
    //fAbsPos=Touchable->GetHistory()->GetTopTransform().TransformPoint(aStep->GetPostStepPoint()->GetPosition());
    fAbsPos=aStep->GetPostStepPoint()->GetPosition();

    FindVolumeEnumerator(Touchable,&fAbsVolumes);
    //Fill the hit
    PhotonHit *pHit = new PhotonHit();

    pHit->SetPhotonEnergy(fOPEne);
    pHit->SetPhotonAbsPosition(fAbsPos);
    pHit->SetPhotonAbsVolume(fAbsVolumes);

    pHit->SetPhotonCreatorProcessName(fCreatorProcessName);
    pHit->SetPhotonCreatorProcessNumber(fCreatorProcessNumber);
    pHit->SetPhotonGlobalTime(fGlobalTime);
    pHit->SetPhotonLocalTime(fLocalTime);

    fPhotonHitCollection->insert(pHit);
    //Dum
    /*
    G4cout<<"HERE Photon Energy "<<G4BestUnit(fOPEne,"Energy")<<G4endl;
    G4cout<<"Absorption Position "<<G4BestUnit(fAbsPos,"Length")<<G4endl;
    DumpVolume(&fAbsVolumes);
    */
    //Clean
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SensitiveDetector::RetrivePrimaryVertex(G4Track* aTrack){
  G4ThreeVector PositionAtVertex=aTrack->GetVertexPosition();
  G4ThreeVector DirectionAtVertex=aTrack->GetVertexMomentumDirection();
  for (int i=0;i<3;i++){
    fPrimaryVertex.at(i)=PositionAtVertex[i];
    fPrimaryVertex.at(i+3)=DirectionAtVertex[i];
  }
  //for (int i=0;i<fPrimaryVertex.size();i++) G4cout<<"HERE Vertex Attributes are "<<fPrimaryVertex.at(i)<<G4endl;
}

void SensitiveDetector::FindVolumeEnumerator(const G4VTouchable* Touchable,VOLUME* vol){
  G4String VName=Touchable->GetVolume()->GetName();
  G4int Depth=Touchable->GetHistoryDepth();

  if (VName.contains("World")){
    vol->Name="World";
    vol->Module=0;
    vol->Volume=0;
    vol->Fiber=0;
    vol->Detector=0;
  }
  else if(VName.contains("Tile")){
    vol->Name="Tile";
    vol->Module=Touchable->GetCopyNumber(2);
    vol->Volume=1;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("Glue")){
    vol->Name="Glue";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=2;
    vol->Fiber=0;
    vol->Detector=0;
  }
  else if(VName.contains("CoreX")){
    vol->Name="CoreX";
    vol->Module=Touchable->GetCopyNumber(3);
    vol->Volume=3;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CladdingX")){
    vol->Name="CladdingX";
    vol->Module=Touchable->GetCopyNumber(3);
    vol->Volume=4;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CoreY")){
    vol->Name="CoreY";
    vol->Module=Touchable->GetCopyNumber(3);
    vol->Volume=5;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CladdingY")){
    vol->Name="CladdingY";
    vol->Module=Touchable->GetCopyNumber(3);
    vol->Volume=6;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CaloSiPMXA")){
    vol->Name="CaloSiPMXA";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=7;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }

  /*
  else if(VName.contains("CaloSiPMXB")){
    vol->Name="CaloSiPMXB";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=8;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  */

  else if(VName.contains("CaloSiPMYA")){
    vol->Name="CaloSiPMYA";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=9;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }

  /*
  else if(VName.contains("CaloSiPMYB")){
    vol->Name="CaloSiPMYB";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=10;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  */

  else if(VName.contains("SiPMXA")){
    vol->Name="SiPMXA";
    vol->Module=Touchable->GetCopyNumber(2);
    vol->Volume=11;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("MirrorFiberX")){
    vol->Name="MirrorFiberX";
    vol->Module=Touchable->GetCopyNumber(2);
    vol->Volume=12;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("SiPMYA")){
    vol->Name="SiPMYA";
    vol->Module=Touchable->GetCopyNumber(2);
    vol->Volume=13;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("MirrorFiberY")){
    vol->Name="MirrorFiberY";
    vol->Module=Touchable->GetCopyNumber(2);
    vol->Volume=14;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  
  else if(VName.contains("CaloEdgeXA")){
    vol->Name="CaloEdgeXA";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=15;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CaloEdgeXB")){
    vol->Name="CaloEdgeXB";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=16;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CaloEdgeYA")){
    vol->Name="CaloEdgeYA";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=17;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }
  else if(VName.contains("CaloEdgeYB")){
    vol->Name="CaloEdgeYB";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=18;
    vol->Fiber=Touchable->GetCopyNumber(0);
    vol->Detector=0;
  }

  else if(VName.contains("Wrapping")){
    vol->Name="Wrapping";
    vol->Module=Touchable->GetCopyNumber(1);
    vol->Volume=19;
    vol->Fiber=0;
    vol->Detector=0;
  }
  
  else{
    InitializeVolume(vol);
  }

}

G4bool SensitiveDetector::IsAGamma(G4Step* aStep){
  G4bool flag = false;
  G4Track *aTrack = aStep->GetTrack();
  const G4ParticleDefinition* aDef = aTrack->GetParticleDefinition();
  G4String aName = aDef->GetParticleName();
  if(aName=="gamma"){
    flag=true;
  }
  return flag;
}

G4bool SensitiveDetector::IsAnOpticalPhoton(G4Step* aStep){
  G4bool flag = false;
  G4Track *aTrack = aStep->GetTrack();
  const G4ParticleDefinition* aDef = aTrack->GetParticleDefinition();
  G4String aName = aDef->GetParticleName();
  if(aName=="opticalphoton"){
    flag=true;
  }
  return flag;
}

void SensitiveDetector::CleanSDMemory(){
  SensitiveDetector::DeleteTotalE();
  SensitiveDetector::DeleteTotalTrackLength();
}

void SensitiveDetector::PrintSDMemoryStatus(){
  G4double TotE = SensitiveDetector::GetTotalE();
  G4double TotL = SensitiveDetector::GetTotalTrackLength();

  G4cout<<" O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O"<<G4endl;
  G4cout<<" Total Energy deposited = "<<G4BestUnit(TotE,"Energy")<<G4endl;
  G4cout<<" Total TrackLength deposited = "<<G4BestUnit(TotL,"Length")<<G4endl;
  G4cout<<" O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O.o.O"<<G4endl;
}

//Check Functions
G4OpBoundaryProcessStatus SensitiveDetector::RetrieveCurrentPhotonStatus(){
  G4OpBoundaryProcessStatus theStatus = Undefined;
  G4OpBoundaryProcess* fOpProcess = NULL;
  G4ProcessManager* OpManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  if (OpManager) {
    G4int MAXofPostStepLoops = OpManager->GetPostStepProcessVector()->entries();
    G4ProcessVector* fPostStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);
    for(G4int i=0; i<MAXofPostStepLoops; i++) {
      G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
      fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
      if (fOpProcess) {
  	theStatus = fOpProcess->GetStatus();
      }
    }
  }
  return theStatus;
}

G4bool SensitiveDetector::checkBoundary(G4OpBoundaryProcessStatus theStatus, G4String Bound){
  G4bool flag = false;
  G4String Status = SensitiveDetector::Status(theStatus);
  if (Status.contains(Bound)){
    flag = true;
  }
  return flag;
}

G4String SensitiveDetector::Status(G4OpBoundaryProcessStatus theStatus){
  G4String status;
  if(theStatus==Undefined)
    status = "Undefined";
  else if(theStatus==FresnelRefraction)
    status = "FresnelRefraction";
  else if(theStatus==FresnelReflection)
    status = "FresnelReflection";
  else if(theStatus==TotalInternalReflection)
    status = "TotalInternalReflection";
  else if(theStatus==LambertianReflection)
    status = "LambertianReflection";
  else if(theStatus==LobeReflection)
    status = "LobeReflection";
  else if(theStatus==SpikeReflection)
    status = "SpikeReflection";
  else if(theStatus==BackScattering)
    status = "BackScattering";
  else if(theStatus==Absorption)
    status = "Absorption";
  else if(theStatus==Detection)
    status = "Detection";
  else if(theStatus ==NotAtBoundary)
    status = "NotAtBoundary";
  else if(theStatus==SameMaterial)
    status = "SameMaterial";
  else if(theStatus==StepTooSmall)
    status = "StepTooSmall";
  else if(theStatus==NoRINDEX)
    status = "NoRINDEX";
  else if(theStatus==PolishedLumirrorAirReflection)
    status = "PolishedLumirrorAirReflection";
  else if(theStatus==PolishedLumirrorGlueReflection)
    status = "PolishedLumirrorGlueReflection";
  else if(theStatus==PolishedAirReflection)
    status = "PolishedAirReflection";
  else if(theStatus==PolishedTeflonAirReflection)
    status = "PolishedTeflonAirReflection";
  else if(theStatus==PolishedTiOAirReflection)
    status = "PolishedTiOAirReflection";
  else if(theStatus==PolishedTyvekAirReflection)
    status = "PolishedTyvekAirReflection";
  else if(theStatus==PolishedVM2000AirReflection)
    status = "PolishedVM2000AirReflection";
  else if(theStatus==PolishedVM2000GlueReflection)
    status = "PolishedVM2000GlueReflection";
  else if(theStatus==EtchedLumirrorAirReflection)
    status = "EtchedLumirrorAirReflection";
  else if(theStatus==EtchedLumirrorGlueReflection)
    status = "EtchedLumirrorGlueReflection";
  else if(theStatus==EtchedAirReflection)
    status = "EtchedAirReflection";
  else if(theStatus== EtchedTeflonAirReflection)
    status = "EtchedTeflonAirReflection";
  else if(theStatus==EtchedTiOAirReflection)
    status = "EtchedTiOAirReflection";
  else if(theStatus==EtchedTyvekAirReflection)
    status = "EtchedTyvekAirReflection";
  else if(theStatus==EtchedVM2000AirReflection)
    status = "EtchedVM2000AirReflection";
  else if(theStatus==EtchedVM2000GlueReflection)
    status = "EtchedVM2000GlueReflection";
  else if(theStatus==GroundLumirrorAirReflection)
    status = "GroundLumirrorAirReflection";
  else if(theStatus==GroundLumirrorGlueReflection)
    status = "GroundLumirrorGlueReflection";
  else if(theStatus==GroundAirReflection)
    status = "GroundAirReflection";
  else if(theStatus==GroundTeflonAirReflection)
    status = "GroundTeflonAirReflection";
  else if(theStatus==GroundTiOAirReflection)
    status = "GroundTiOAirReflection";
  else if(theStatus==GroundTyvekAirReflection)
    status = "GroundTyvekAirReflection";
  else if(theStatus==GroundVM2000AirReflection)
    status = "GroundVM2000AirReflection";
  else if(theStatus==GroundVM2000GlueReflection)
    status = "GroundVM2000GlueReflection";
  else if(theStatus==Dichroic )
    status = "Dichroic";
  else
    status = "NOT RECOGNIZED!";
    //G4cout<< theStatus<<G4endl;
  return status;
}
