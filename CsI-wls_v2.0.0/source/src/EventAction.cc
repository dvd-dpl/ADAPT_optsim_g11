#include "EventAction.hh"
#include "RunAction.hh"
#include "OneHit.hh"
#include "ComPairHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runA): G4UserEventAction(),fRunAction(runA),fHCID(-1),fPHCID(-1),fCPHCID(-1){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HitCollection* EventAction::GetHitsCollection(G4int HCID, const G4Event* event) const{
  auto hitsCollection = static_cast<HitCollection*>(event->GetHCofThisEvent()->GetHC(HCID));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << HCID;
    G4Exception("B4cEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

PhotonHitCollection* EventAction::GetPhotonHitsCollection(G4int PHCID, const G4Event* event) const{
  auto phitsCollection = static_cast<PhotonHitCollection*>(event->GetHCofThisEvent()->GetHC(PHCID));

  if ( ! phitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access photon hitsCollection ID " << PHCID;
    G4Exception("B4cEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return phitsCollection;
}



ComPairHitCollection*
EventAction::GetComPairHitsCollection(G4int CPHCID, const G4Event* event) const
{
  auto ComPairHitsCollection = static_cast<ComPairHitCollection*>(event->GetHCofThisEvent()->GetHC(CPHCID));
  if ( ! ComPairHitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << CPHCID;
    G4Exception("B4cEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }
  return ComPairHitsCollection;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::BeginOfEventAction(const G4Event*){
  cout<<" Event Start :) "<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event){
  auto analysisManager = G4AnalysisManager::Instance();
  FileManager* NFileManager= fRunAction->GetFileManager();
  if ( fHCID == -1 ) {
    fHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("HitCollection");
    }
  if ( fPHCID == -1 ) {
      fPHCID
        = G4SDManager::GetSDMpointer()->GetCollectionID("PhotonHitCollection");
      }

  if ( fCPHCID == -1 )
    fCPHCID = G4SDManager::GetSDMpointer()->GetCollectionID("ComPairHitCollection");


  // ***Primary Hit Collection*** //
  auto HC = GetHitsCollection(fHCID, event);
  G4int nEvt = event->GetEventID();
  G4int eventID = nEvt;

  // Get hit with total values
  if(HC->entries()>0){
    G4int numberOfHits = HC->entries();
    TRACKEREVENTN* TreeStructure = NFileManager->get_TrackerEvent();
    int vectorsize = (*HC)[0]->fVolumes.size();
    for(G4int i = 0;i<numberOfHits;i++){
    //Scalar
    int vectorsizeStep=(*HC)[i]->fEnergyInStep.size();
    //G4cout<<"Ciao  "<<vectorsizeStep<<G4endl;
    TreeStructure->RunID=nEvt;
    TreeStructure->VectorSize=vectorsize;
    TreeStructure->VectorSizeInStep=vectorsizeStep;
    TreeStructure->PosX=(*HC)[i]->fPrimaryVertex.at(0)/mm;
    TreeStructure->PosY=(*HC)[i]->fPrimaryVertex.at(1)/mm;
    TreeStructure->PosZ=(*HC)[i]->fPrimaryVertex.at(2)/mm;
    TreeStructure->DirX=(*HC)[i]->fPrimaryVertex.at(3);
    TreeStructure->DirY=(*HC)[i]->fPrimaryVertex.at(4);
    TreeStructure->DirZ=(*HC)[i]->fPrimaryVertex.at(5);
    //Vector
    for (int k=0;k<vectorsizeStep;k++){
      TreeStructure->InStepEnergy[k]=(*HC)[i]->fEnergyInStep.at(k)/MeV;
      TreeStructure->InStepPosX[k]=(*HC)[i]->fPositionInStep.at(k)[0]/mm;
      TreeStructure->InStepPosY[k]=(*HC)[i]->fPositionInStep.at(k)[1]/mm;
      TreeStructure->InStepPosZ[k]=(*HC)[i]->fPositionInStep.at(k)[2]/mm;
    }
    for (int j=0; j<vectorsize;j++){
      TreeStructure->VolumeName.push_back((*HC)[i]->fVolumes.at(j).Name);
      TreeStructure->ModuleEnum[j]=(*HC)[i]->fVolumes.at(j).Module;
      TreeStructure->VolumeEnum[j]=(*HC)[i]->fVolumes.at(j).Volume;
      TreeStructure->FiberEnum[j]=(*HC)[i]->fVolumes.at(j).Fiber;
      TreeStructure->TotalEnergyDep[j]=(*HC)[i]->fEnergyDep.at(j);
      TreeStructure->TotalPhotonGenerated[j]=(*HC)[i]->fPhotGen.at(j);
      TreeStructure->NumbOfPhotonsAbsorbed[j]=(*HC)[i]->fPhotAbs.at(j);
      TreeStructure->TotalPhotonGeneratedWLS[j]=(*HC)[i]->fPhotGenWLS.at(j);
      TreeStructure->NumbOfPhotonsAbsorbedWLS[j]=(*HC)[i]->fPhotAbsWLS.at(j);
      TreeStructure->TotalPhotonGeneratedScint[j]=(*HC)[i]->fPhotGenScint.at(j);
      TreeStructure->NumbOfPhotonsAbsorbedScint[j]=(*HC)[i]->fPhotAbsScint.at(j);

    }
    NFileManager->fill_EventTree();
    TreeStructure->VolumeName.clear();
    }

  }


  // ***ComPair Hit Collection*** //
  COMPAIREVENTN* compairEvent = NFileManager->get_ComPairEvent();
  auto CPHC = GetComPairHitsCollection(fCPHCID, event);

  // Get hit with total values
  G4int         ComPair_ProcessType;
  G4double      ComPair_IntX ,ComPair_IntY ,ComPair_IntZ;
  G4int         ComPair_IntModule, ComPair_IntVolume, ComPair_IntFiber;
  G4int         ComPair_FirstParticleID;
  G4double      ComPair_FirstEnergy;
  G4double      ComPair_FirstDX, ComPair_FirstDY, ComPair_FirstDZ;
  G4int         ComPair_SecondParticleID;
  G4double      ComPair_SecondEnergy;
  G4double      ComPair_SecondDX,ComPair_SecondDY,ComPair_SecondDZ;

  if(CPHC->entries()>0){
    auto ComPairHit = (*CPHC)[CPHC->entries()-1];
    G4int numberOfComPairHits = CPHC->entries();

    for(G4int j = 0;j<numberOfComPairHits;j++){
      ComPair_ProcessType = (*CPHC)[j]->GetProcessType();

      ComPair_IntX = (*CPHC)[j]->GetInteractionPositionX();
      ComPair_IntY =(*CPHC)[j]->GetInteractionPositionY();
      ComPair_IntZ =(*CPHC)[j]->GetInteractionPositionZ();

      ComPair_IntModule = (*CPHC)[j]->GetInteractionModule();
      ComPair_IntVolume  = (*CPHC)[j]->GetInteractionVolume();
      ComPair_IntFiber = (*CPHC)[j]->GetInteractionFiber();

      ComPair_FirstParticleID  = (*CPHC)[j]->GetFirstParticleID();
      ComPair_FirstEnergy      = (*CPHC)[j]->GetFirstEnergy();
      ComPair_FirstDX          = (*CPHC)[j]->GetFirstDirectionX();
      ComPair_FirstDY          = (*CPHC)[j]->GetFirstDirectionY();
      ComPair_FirstDZ          = (*CPHC)[j]->GetFirstDirectionZ();

      ComPair_SecondParticleID = (*CPHC)[j]->GetSecondParticleID();
      ComPair_SecondEnergy     = (*CPHC)[j]->GetSecondEnergy();
      ComPair_SecondDX = (*CPHC)[j]->GetSecondDirectionX();
      ComPair_SecondDY = (*CPHC)[j]->GetSecondDirectionY();
      ComPair_SecondDZ = (*CPHC)[j]->GetSecondDirectionZ();

      compairEvent->eventID = eventID;
      compairEvent->ProcessType = ComPair_ProcessType;

      compairEvent->IntX = ComPair_IntX;
      compairEvent->IntY = ComPair_IntY;
      compairEvent->IntZ = ComPair_IntZ;

      compairEvent->IntModule = ComPair_IntModule;
      compairEvent->IntVolume  = ComPair_IntVolume;
      compairEvent->IntFiber = ComPair_IntFiber;

      compairEvent->FirstParticleID = ComPair_FirstParticleID;
      compairEvent->FirstEnergy = ComPair_FirstEnergy;
      compairEvent->FirstDX = ComPair_FirstDX;
      compairEvent->FirstDY = ComPair_FirstDY;
      compairEvent->FirstDZ = ComPair_FirstDZ;

      compairEvent->SecondParticleID = ComPair_SecondParticleID;
      compairEvent->SecondEnergy = ComPair_SecondEnergy;
      compairEvent->SecondDX = ComPair_SecondDX;
      compairEvent->SecondDY = ComPair_SecondDY;
      compairEvent->SecondDZ = ComPair_SecondDZ;
    }
    // NFileManager->fill_ComPairTree();
  }

  // Write the Tree also if a first interaction is not recognized (processType = -999)
  else{
    compairEvent->eventID = eventID;
    compairEvent->ProcessType = -999;

    compairEvent->IntX = -999;
    compairEvent->IntY = -999;
    compairEvent->IntZ = -999;

    compairEvent->IntModule = -999;
    compairEvent->IntVolume  = -999;
    compairEvent->IntFiber = -999;

    compairEvent->FirstParticleID = -999;
    compairEvent->FirstEnergy = -999;
    compairEvent->FirstDX = -999;
    compairEvent->FirstDY = -999;
    compairEvent->FirstDZ = -999;

    compairEvent->SecondParticleID = -999;
    compairEvent->SecondEnergy = -999;
    compairEvent->SecondDX = -999;
    compairEvent->SecondDY = -999;
    compairEvent->SecondDZ = -999;
  }

  NFileManager->fill_ComPairTree();


  // ***Photon Hit Collection*** //
  auto PHC = GetPhotonHitsCollection(fPHCID, event);
  // Get hit with total values
  if(PHC->entries()>0){
    G4int numberOfPHits = PHC->entries();
    TRACKEREVENTNP* PTreeStructure = NFileManager->get_PhotonEvent();
    for(G4int i = 0;i<numberOfPHits;i++){
      //Scalar
      PTreeStructure->RunID=nEvt;
      std::strcpy(PTreeStructure->VolumeName,(*PHC)[i]->fAbsVolumes.Name.c_str());
      PTreeStructure->ModuleEnum=(*PHC)[i]->fAbsVolumes.Module;
      PTreeStructure->VolumeEnum=(*PHC)[i]->fAbsVolumes.Volume;
      PTreeStructure->FiberEnum=(*PHC)[i]->fAbsVolumes.Fiber;
      PTreeStructure->PosX=(*PHC)[i]->fAbsPos[0]/mm;
      PTreeStructure->PosY=(*PHC)[i]->fAbsPos[1]/mm;
      PTreeStructure->PosZ=(*PHC)[i]->fAbsPos[2]/mm;
      PTreeStructure->KEne=(*PHC)[i]->fOPEne/eV;

      PTreeStructure->GlobalTime=(*PHC)[i]->fGlobalTime/ns;
      PTreeStructure->LocalTime=(*PHC)[i]->fLocalTime/ns;
      std::strcpy(PTreeStructure->ProcessName,(*PHC)[i]->fCreatorProcessName.c_str());
      PTreeStructure->ProcessNumber=(*PHC)[i]->fCreatorProcessNumber;

      NFileManager->fill_PhotonTree();
    }

  }
  cout<<" ADIOs AMEGOs!! :) "<<endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
