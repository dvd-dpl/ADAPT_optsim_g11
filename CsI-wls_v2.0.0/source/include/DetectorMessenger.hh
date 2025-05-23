//
// ********************************************************************
// * License and Disclaimer                                           *
// ********************************************************************
//
#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "DetectorConstruction.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;

class DetectorMessenger : public G4UImessenger
{
public:

  DetectorMessenger(DetectorConstruction* );
  virtual ~DetectorMessenger();

  //Other functions

  virtual void SetNewValue(G4UIcommand*, G4String);

private:

  DetectorConstruction*   fDetector;

  G4UIdirectory*             fDetDir;

  G4UIcmdWith3VectorAndUnit* fSetTileSize;
  G4UIcmdWithADoubleAndUnit* fSetSiPMThikness;
  G4UIcmdWithADoubleAndUnit* fSetSiPMSide;
  G4UIcmdWithADoubleAndUnit* fSetSiPMSpacing;
  G4UIcmdWithADoubleAndUnit* fSetFiberSize;
  G4UIcmdWithADoubleAndUnit* fSetFiberExtra;
  G4UIcmdWithADouble* fSetCladdingPercentuage;
  G4UIcmdWithABool* fSetSiPMCaloFlag;
};

#endif
