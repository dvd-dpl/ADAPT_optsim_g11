#include "DetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
  : fDetector(det)
{
  fDetDir = new G4UIdirectory("/APT/");
  fDetDir->SetGuidance(" Geometry Setup ");

  fSetTileSize = new G4UIcmdWith3VectorAndUnit("/APT/Geometry/setTileSize",this);
  fSetTileSize ->SetGuidance("Set size of one Calorimeter Tile");
  fSetTileSize ->SetParameterName("csx","csy","csz",false);
  fSetTileSize ->SetUnitCategory("Length");
  fSetTileSize ->SetDefaultUnit("cm");
  fSetTileSize ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetTileSize ->SetToBeBroadcasted(false);

  fSetSiPMThikness = new G4UIcmdWithADoubleAndUnit("/APT/Geometry/setSiPMThickness",this);
  fSetSiPMThikness ->SetGuidance("Set SiPM Thickness");
  fSetSiPMThikness ->SetParameterName("sth",false);
  fSetSiPMThikness ->SetUnitCategory("Length");
  fSetSiPMThikness ->SetDefaultUnit("cm");
  fSetSiPMThikness ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSiPMThikness ->SetToBeBroadcasted(false);

  fSetSiPMSide = new G4UIcmdWithADoubleAndUnit("/APT/Geometry/setSiPMSide",this);
  fSetSiPMSide ->SetGuidance("Set SiPM side size");
  fSetSiPMSide ->SetParameterName("sss",false);
  fSetSiPMSide ->SetUnitCategory("Length");
  fSetSiPMSide ->SetDefaultUnit("cm");
  fSetSiPMSide ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSiPMSide ->SetToBeBroadcasted(false);

  fSetSiPMSpacing = new G4UIcmdWithADoubleAndUnit("/APT/Geometry/setSiPMSpacing",this);
  fSetSiPMSpacing ->SetGuidance("set spacing between SiPMs");
  fSetSiPMSpacing ->SetParameterName("ssps",false);
  fSetSiPMSpacing ->SetUnitCategory("Length");
  fSetSiPMSpacing ->SetDefaultUnit("cm");
  fSetSiPMSpacing ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSiPMSpacing ->SetToBeBroadcasted(false);

  fSetFiberSize = new G4UIcmdWithADoubleAndUnit("/APT/Geometry/setFiberDiameter",this);
  fSetFiberSize ->SetGuidance("Set the fiber diameter");
  fSetFiberSize ->SetParameterName("fdm",false);
  fSetFiberSize ->SetUnitCategory("Length");
  fSetFiberSize ->SetDefaultUnit("mm");
  fSetFiberSize ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetFiberSize ->SetToBeBroadcasted(false);

  fSetFiberExtra = new G4UIcmdWithADoubleAndUnit("/APT/Geometry/setFiberExtra",this);
  fSetFiberSize ->SetGuidance("Set the fiber extra");
  fSetFiberSize ->SetParameterName("fext",false);
  fSetFiberSize ->SetUnitCategory("Length");
  fSetFiberSize ->SetDefaultUnit("mm");
  fSetFiberSize ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetFiberSize ->SetToBeBroadcasted(false);

  fSetCladdingPercentuage = new G4UIcmdWithADouble("/APT/Geometry/setCladdingPercentuage",this);
  fSetCladdingPercentuage ->SetGuidance("Set the percentuage of cladding for the fiber diameter");
  fSetCladdingPercentuage ->SetParameterName("cpt",false);
  fSetCladdingPercentuage ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetCladdingPercentuage ->SetToBeBroadcasted(false);

  fSetSiPMCaloFlag = new G4UIcmdWithABool("/APT/Geometry/SiPMCaloFlag",this);
  fSetSiPMCaloFlag ->SetGuidance("If true SiPM over all the side");
  fSetSiPMCaloFlag ->SetParameterName("scf",false);
  fSetSiPMCaloFlag ->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSiPMCaloFlag ->SetToBeBroadcasted(false);

}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fSetTileSize;
  delete fSetFiberExtra;
  delete fSetSiPMThikness;
  delete fSetSiPMSide;
  delete fSetSiPMSpacing;
  delete fSetFiberSize;
  delete fSetCladdingPercentuage;
  delete fSetSiPMCaloFlag;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String val){

    if( command == fSetTileSize )
        fDetector->SetTileSize(G4UIcmdWith3VectorAndUnit::GetNew3VectorValue(val));
    else if( command == fSetSiPMThikness)
        fDetector->SetSiPMThikness(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
    else if( command == fSetSiPMSide)
        fDetector->SetSiPMSide(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
    else if( command == fSetSiPMSide)
        fDetector->SetSiPMSpacing(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
    else if( command == fSetSiPMSpacing)
        fDetector->SetSiPMSpacing(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
    else if( command == fSetFiberSize)
        fDetector->SetFiberSize(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
    else if( command == fSetFiberExtra)
        fDetector->SetFiberExtra(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
    else if( command == fSetCladdingPercentuage)
        fDetector->SetCladdingPercentuage(G4UIcmdWithADouble::GetNewDoubleValue(val));
    else if( command == fSetSiPMCaloFlag)
        fDetector->SetSiPMCaloFlag(G4UIcmdWithABool::GetNewBoolValue(val));

}
