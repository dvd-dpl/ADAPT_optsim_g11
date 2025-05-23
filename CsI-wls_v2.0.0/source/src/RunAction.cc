#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "RunActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "Analysis.hh"
#include "TreeStructures.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(): G4UserRunAction(), fOutputFileName("./Data"){
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  fRunMessenger = new RunActionMessenger(this);
  fFileManager=new FileManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction(){
  delete fFileManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*){
  G4String fileName = fOutputFileName;
  G4String fileOutputC = fileName;
  fileOutputC += ".root";

  fFileManager->CreateOutputFile(fileOutputC);
  fFileManager->InitializeEventTree();
  fFileManager->InitializePhotonTree();
  fFileManager->InitializeComPairTree();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*){

  //Tree
  TTree* Albero=fFileManager->get_EventTree();
  if(Albero){
    fFileManager->writeInOutputFile(Albero);
    }
  TTree* PAlbero=fFileManager->get_PhotonTree();
    if(PAlbero){
      fFileManager->writeInOutputFile(PAlbero);
      }
    
    TTree *ComPairAlbero=fFileManager->get_ComPairTree();
    if(ComPairAlbero){
      fFileManager->writeInOutputFile(ComPairAlbero);
    }
    fFileManager->closeFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
