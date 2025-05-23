//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef OptMaterials_h
#define OptMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4OpticalSurface.hh"
#include <vector>
#include <string>

class OptMaterials
{
  public:

  OptMaterials();
  virtual ~OptMaterials();

  void CreateMaterials();
  static OptMaterials* GetInstance();
  G4Material* GetMaterial(const G4String);
  void PrintOptMaterialSettings();
  void VectorsFromData (std::string path,std::vector<G4double>* v1,std::vector<G4double>* v2);


  private:

  static OptMaterials* fInstance;

  G4NistManager*     fNistMan;
  //Materials
  G4Material*        fAir;
  G4Material*        fVoid;
  G4Material*        fCsI;
  G4Material*        fPMMA;
  G4Material*        fGLUE;
  G4Material*        fGLUE_EDGES;
  G4Material*        fPS;
  G4Material*        fSi;
  G4Material*        Mylar;

};

#endif
