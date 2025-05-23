#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicalVolume;
class G4LogicalVolume;
class OptMaterials;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    //Set functions
    void SetTileSize(G4ThreeVector var){fTileSize=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetFiberExtra(G4double var){fFiberExtra=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMThikness(G4double var){fSiPMThikness=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMSide(G4double var){fSiPMSide=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMSpacing(G4double var){fSiPMSpacing=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMCaloThikness(G4double var){fSiPMCaloThikness=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMCaloSide(G4double var){fSiPMCaloSide=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMCaloSpacing(G4double var){fSiPMCaloSpacing=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetFiberSize(G4double var){fFiberSize=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetCladdingPercentuage(double var){fCladdingPercentuage=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    void SetSiPMCaloFlag(bool var){fSiPMCaloFlag=var;G4RunManager::GetRunManager()->ReinitializeGeometry();};
    //Get Functions
    G4ThreeVector GetTileSize() const { return fTileSize;};
    G4double GetSiPMThikness() const { return fSiPMThikness;};
    G4double GetFiberExtra() const { return fFiberExtra;};
    G4double GetSiPMSide() const { return fSiPMSide;};
    G4double GetSiPMSpacing() const { return fSiPMSpacing;};
    G4double GetSiPMCaloThikness() const { return fSiPMThikness;};
    G4double GetSiPMCaloSide() const { return fSiPMSide;};
    G4double GetSiPMCaloSpacing() const { return fSiPMSpacing;};
    G4double GetFiberSize() const { return fFiberSize;};
    double GetCladdingPercentuage() const { return fCladdingPercentuage;};
    bool GetSiPMCaloFlag() const { return fSiPMCaloFlag;};
  protected:
    OptMaterials *fMaterials;
    //Geometry messenger variables
    DetectorMessenger* fDetectorMessenger;


    G4ThreeVector fTileSize;
    int fNTile;
    G4double fFiberExtra;
    G4double fGlueThick;
    G4double fSiPMThikness;
    G4double fSiPMSide;
    G4double fSiPMSpacing;
    G4double fSiPMCaloThikness;
    G4double fSiPMCaloSide;
    G4double fSiPMCaloSpacing;
    G4double fFiberSize;
    G4double fGlueEdges;
    double fCladdingPercentuage;
    bool fSiPMCaloFlag;
    //Volume vectors
    std::vector<G4LogicalVolume*> LogicFiberX;
    std::vector<G4LogicalVolume*> LogicFiberY;
    std::vector<G4LogicalVolume*> LogicCoreX;
    std::vector<G4LogicalVolume*> LogicCoreY;
    std::vector<G4LogicalVolume*> LogicCladdingX;
    std::vector<G4LogicalVolume*> LogicCladdingY;
    std::vector<G4LogicalVolume*> LogicSiPMXA;
    std::vector<G4LogicalVolume*> LogicMirrorFiberX;
    std::vector<G4LogicalVolume*> LogicSiPMYA;
    std::vector<G4LogicalVolume*> LogicMirrorFiberY;

    std::vector<G4VPhysicalVolume*> PhysCladdingX;
    std::vector<G4VPhysicalVolume*> PhysCladdingY;
    std::vector<G4VPhysicalVolume*> PhysicalTile;
    std::vector<G4VPhysicalVolume*> PhysicalMirrorFiberX;
    std::vector<G4VPhysicalVolume*> PhysicalMirrorFiberY;
    std::vector<G4VPhysicalVolume*> PhysCaloSiPMXB;
    std::vector<G4VPhysicalVolume*> PhysCaloSiPMXA;
    std::vector<G4VPhysicalVolume*> PhysCaloSiPMYA;
    std::vector<G4VPhysicalVolume*> PhysCaloSiPMYB;
    std::vector<G4VPhysicalVolume*> PhysCoreX;
    std::vector<G4VPhysicalVolume*> PhysCoreY;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
