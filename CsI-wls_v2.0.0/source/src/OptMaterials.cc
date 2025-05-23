#include "OptMaterials.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "DetectorConstruction.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

OptMaterials* OptMaterials::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
OptMaterials::OptMaterials(){
  //materials
  fAir=NULL;
  fVoid=NULL;
  fCsI=NULL;
  fPMMA=NULL;
  fGLUE=NULL;
  fGLUE_EDGES=NULL;
  fPS=NULL;
  //Nist
  fNistMan = G4NistManager::Instance();
  fNistMan->SetVerbose(0);
  //CreateMaterials();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OptMaterials::~OptMaterials(){
  delete    fAir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OptMaterials* OptMaterials::GetInstance(){
  if (fInstance == 0){
    fInstance = new OptMaterials();
  }
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* OptMaterials::GetMaterial(const G4String material){
  CreateMaterials();
  G4Material* mat=NULL;
  if (material=="Vacuum"){
    mat=fVoid;
  }
  else if (material=="Air"){
    mat=fAir;
  }
  else if (material=="CsI"){
    mat=fCsI;
  }
  else if (material=="PMMA"){
    mat=fPMMA;
  }
  else if (material=="GLUE"){
    mat=fGLUE;
  }
  else if (material=="GLUE_EDGES"){
    mat=fGLUE_EDGES;
  }
 
  else if (material=="PS"){
    mat=fPS;
  }
  else if (material=="Si"){
    mat=fSi;
  }
  else{
    mat =  fNistMan->FindOrBuildMaterial(material);
    if (!mat) mat = G4Material::GetMaterial(material);
    if (!mat) {
      std::ostringstream o;
      o << "Material " << material << " not found!";
      G4Exception("OptMaterials::GetMaterial","",FatalException,o.str().c_str());
    }
  }
  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OptMaterials::CreateMaterials(){
  //Reading Optical Material File
  //PlasticBulckOpticsReader(fProerScintillatorMaterial,&fScintillationMaterial,&fPlasticAbsorptionLength,&fRefractiveIndexPlastic,&fScintillationYield,&fBirksConstant,&fResolutionScale,&fFastTimeConstant,&fSlowTimeConstant,&fYieldRatio,&fScintillatorEmissionSpectrum);


  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4String> elements;
  G4int ncomponents;
  G4double fractionmass;

  // Materials Definitions
  // =====================

  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  fVoid = fNistMan->FindOrBuildMaterial("G4_Galactic");

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  fAir = fNistMan->FindOrBuildMaterial("G4_AIR");

  //--------------------------------------------------
  // CsI
  //--------------------------------------------------

  fCsI = fNistMan->FindOrBuildMaterial("G4_CESIUM_IODIDE");

  //--------------------------------------------------
  // Si
  //--------------------------------------------------

  fSi = fNistMan->FindOrBuildMaterial("G4_Si");

  //--------------------------------------------------
  // Polystyrene
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(8);
  elements.push_back("H");     natoms.push_back(8);

  density = 1.050*g/cm3;

  fPS = fNistMan->ConstructNewMaterial("Polystyrene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // PMMA
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.190*g/cm3;

  fPMMA = fNistMan->ConstructNewMaterial("PMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // GLUE
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(18);
  elements.push_back("H");     natoms.push_back(19);
  elements.push_back("O");     natoms.push_back(3);

  density = 1.231*g/cm3;

  fGLUE = fNistMan->ConstructNewMaterial("GLUE", elements, natoms, density);

  elements.clear();
  natoms.clear();

   //--------------------------------------------------
  // GLUE_EDGES
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(18);
  elements.push_back("H");     natoms.push_back(19);
  elements.push_back("O");     natoms.push_back(3);

  density = 1.231*g/cm3;

  fGLUE_EDGES = fNistMan->ConstructNewMaterial("GLUE_EDGES", elements, natoms, density);

  elements.clear();
  natoms.clear();

  /*
  //--------------------------------------------------
  // TiO2
  //--------------------------------------------------

  elements.push_back("Ti");     natoms.push_back(1);
  elements.push_back("O");      natoms.push_back(2);

  density     = 4.26*g/cm3;

  G4Material* TiO2 = fNistMan->ConstructNewMaterial("TiO2", elements, natoms, density);

  elements.clear();
  natoms.clear();

  */

  //--------------------------------------------------
  // Mylar
  //--------------------------------------------------
  elements.push_back("O");     natoms.push_back(2);
  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(4);

  density = 1.39*g/cm3;

  G4Material* Mylar = fNistMan->ConstructNewMaterial("Mylar", elements, natoms, density);

  elements.clear();
  natoms.clear();

  /*  
  //--------------------------------------------------
  // Teflon
  //--------------------------------------------------
  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("F");     natoms.push_back(4);

  density = 2.2*g/cm3;

  G4Material* Teflon = fNistMan->ConstructNewMaterial("Teflon", elements, natoms, density);

  elements.clear();
  natoms.clear();


  //--------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  //--------------------------------------------------

  density = 1.52*g/cm3;

  G4Material* fCoating = new G4Material("Coating", density, ncomponents=2);

  fCoating->AddMaterial(TiO2,         fractionmass = 15*perCent);
  fCoating->AddMaterial(fPolystyrene, fractionmass = 85*perCent);
  */


  //
  // ------------ Generate & Add Material Properties Table ------------
  //
  //Optical material responses
  //Loading Data From OptData

  std::string WlsEmissData="OptData/WLS_EmissionSpectrumY11_converted_ordered.txt";
  std::string SpectrumData="OptData/CsI_Na_EmissionSpectrum_ordered.txt";
  std::string WlsAbsData="OptData/WLSABS_Original_ordered.txt";
  //std::string WlsAbsData="OptData/WLSABSZhang.txt";
  //std::string WlsAbsData="OptData/FakeAbs.txt";
  std::string PMMAAbsData="OptData/PMMAAbsorptionLength.txt";

  std::vector<G4double> VScintillatorEnergy;
  std::vector<G4double> VScintillatorIntensity;
  std::vector<G4double> VWLSAbsEnergy;
  std::vector<G4double> VWLSAbsLength;
  std::vector<G4double> VWLSEmissEnergy;
  std::vector<G4double> VWLSEmissSpectrum;
  std::vector<G4double> VPMMAAbsEnergy;
  std::vector<G4double> VPMMAAbsLength;

  /* For DEBUG */
  /////////////////////////////////////////
  G4cout<<"Reading Calo scintilation file "<<G4endl;
  VectorsFromData (SpectrumData,&VScintillatorEnergy,&VScintillatorIntensity);
  G4cout<<"Reading WLS absorption data"<<G4endl;
  VectorsFromData (WlsAbsData,&VWLSAbsEnergy,&VWLSAbsLength);
  G4cout<<"Reading WLS emission data"<<G4endl;
  VectorsFromData (WlsEmissData,&VWLSEmissEnergy,&VWLSEmissSpectrum);
  G4cout<<"Reading PMMA absorption data"<<G4endl;
  VectorsFromData (PMMAAbsData,&VPMMAAbsEnergy,&VPMMAAbsLength);
  
  int VectorSize = VScintillatorEnergy.size();
  int VectorSizeWLS = VWLSAbsEnergy.size();
  int VectorSizePMMA = VPMMAAbsEnergy.size();
  int WLSEmiss_VectorSize = VWLSEmissEnergy.size();

  // Absorption length have to be read in cm
  for (int i=0;i<VWLSAbsEnergy.size();i++){
    VWLSAbsLength.at(i)=VWLSAbsLength.at(i)*cm;
  }

  for (int i=0;i<VPMMAAbsEnergy.size();i++){
    VPMMAAbsLength.at(i)=VPMMAAbsLength.at(i)*cm;
  }


  //---------------------------------------------------------------------------------

  //c++ Standard vectors have to be converted in c vectors
  G4double* ScintEmission=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ScintEmission+i)= VScintillatorEnergy.at(i);}
  G4double* ScintIntensity=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ScintIntensity+i)= VScintillatorIntensity.at(i);}
  G4double* WLSAbsorptionE=new G4double[VectorSizeWLS];
  for (int i=0;i<VectorSizeWLS;i++){*(WLSAbsorptionE+i)= VWLSAbsEnergy.at(i);}
  G4double* WLSAbsorptionL=new G4double[VectorSizeWLS];
  //for (int i=0;i<VectorSizeWLS;i++){*(WLSAbsorptionL+i)= VWLSAbsLength.at(i);}
  for (int i=0;i<VectorSizeWLS;i++){*(WLSAbsorptionL+i)= VWLSAbsLength.at(i);}
  G4double* WLSEmissionE=new G4double[WLSEmiss_VectorSize];
  for (int i=0;i<WLSEmiss_VectorSize;i++){*(WLSEmissionE+i)= VWLSEmissEnergy.at(i);}
  G4double* WLSEmissionS=new G4double[WLSEmiss_VectorSize];
  for (int i=0;i<WLSEmiss_VectorSize;i++){*(WLSEmissionS+i)= VWLSEmissSpectrum.at(i);}
  G4double* PMMAAbsorptionE=new G4double[VectorSizePMMA];
  for (int i=0;i<VectorSizePMMA;i++){*(PMMAAbsorptionE+i)= VPMMAAbsEnergy.at(i);}
  G4double* PMMAAbsorptionL=new G4double[VectorSizePMMA];
  for (int i=0;i<VectorSizePMMA;i++){*(PMMAAbsorptionL+i)= VPMMAAbsLength.at(i);}

  //For DEBUG
  //CALO
  G4cout<<"Calo emission data from "<<SpectrumData<<G4endl;
  for (int i=0;i<VScintillatorEnergy.size();i++){
    G4cout<<"Energy: "<<G4BestUnit(VScintillatorEnergy.at(i),"Energy")<<" Intensity: "<<VScintillatorIntensity.at(i)<<G4endl;
  }
  //WLS
  G4cout<<"WLS absorption length data from "<<WlsAbsData<<G4endl;
  for (int i=0;i<VWLSAbsEnergy.size();i++){
    G4cout<<"Energy: "<<G4BestUnit(VWLSAbsEnergy.at(i),"Energy")<<" Absorption Length: "<<G4BestUnit(VWLSAbsLength.at(i),"Length")<<G4endl;
  }

  G4cout<<"WLS Emission Spectrum data from "<<WlsEmissData<<G4endl;
  for (int i=0;i<VWLSEmissEnergy.size();i++){
    G4cout<<"Energy: "<<G4BestUnit(VWLSEmissEnergy.at(i),"Energy")<<" Probability: "<<VWLSEmissSpectrum.at(i)<<G4endl;
  }
  //PMMA
  G4cout<<"PMMA absorption length data from "<<PMMAAbsData<<G4endl;
  for (int i=0;i<VPMMAAbsEnergy.size();i++){
    G4cout<<"***Energy: "<<G4BestUnit(VPMMAAbsEnergy.at(i),"Energy")<<" Length: "<<G4BestUnit(PMMAAbsorptionL[i],"Length")<<G4endl;
  }
  
  
  //--------------------------------------------------
  // Void Bulk Optics
  //--------------------------------------------------
  G4double* RIVoid=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RIVoid+i)=1.0;}
  G4double* ALVoid=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALVoid+i)=1000*m;}

  G4MaterialPropertiesTable* mpt_Void = new G4MaterialPropertiesTable();
  mpt_Void->AddProperty("RINDEX",ScintEmission,RIVoid,VectorSize);
  mpt_Void->AddProperty("ABSLENGTH",ScintEmission,ALVoid,VectorSize);

  fVoid->SetMaterialPropertiesTable(mpt_Void);

  //--------------------------------------------------
  // Air Bulk Optics
  //--------------------------------------------------
  G4double* RIAir=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RIAir+i)=1.000284275;}
  G4double* ALAir=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALAir+i)=10*cm;}

  G4MaterialPropertiesTable* mpt_Air = new G4MaterialPropertiesTable();
  mpt_Air->AddProperty("RINDEX",ScintEmission,RIAir,VectorSize);
  mpt_Air->AddProperty("ABSLENGTH",ScintEmission,ALAir,VectorSize);

  fAir->SetMaterialPropertiesTable(mpt_Air);

  //--------------------------------------------------
  // SiPM Bulk Optics
  //--------------------------------------------------
  G4double* RISi=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RISi+i)=1.59;} //RI of SiPM Optical grease
  G4double* ALSi=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALSi+i)=10*nm;}

  G4MaterialPropertiesTable* mpt_Si = new G4MaterialPropertiesTable();

  mpt_Si->AddProperty("RINDEX",ScintEmission,RISi,VectorSize);
  mpt_Si->AddProperty("ABSLENGTH",ScintEmission,ALSi,VectorSize);

  fSi->SetMaterialPropertiesTable(mpt_Si);

  //--------------------------------------------------
  // PS Bulk Optics (Fiber Core)
  //--------------------------------------------------
  G4double* RIPS = new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RIPS+i)=1.59;}
  G4double* ALPS = new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALPS+i)=3.5*m;} //was 100*nm

  G4MaterialPropertiesTable* mpt_PS = new G4MaterialPropertiesTable();

  mpt_PS->AddProperty("RINDEX",ScintEmission,RIPS,VectorSize);
  //mpt_PS->AddProperty("ABSLENGTH",WLSAbsorptionE,WLSAbsorptionL,VectorSizeWLS);
  mpt_PS->AddProperty("WLSABSLENGTH",WLSAbsorptionE,WLSAbsorptionL,VectorSizeWLS);
  mpt_PS->AddProperty("WLSCOMPONENT",WLSEmissionE,WLSEmissionS,WLSEmiss_VectorSize);
  mpt_PS->AddConstProperty("WLSTIMECONSTANT",8.5*ns);
  //G4double MeanNumberOfPhotons = 0.67;
  G4double MeanNumberOfPhotons = 1.;
  mpt_PS->AddConstProperty("WLSMEANNUMBERPHOTONS",MeanNumberOfPhotons);
  fPS->SetMaterialPropertiesTable(mpt_PS);

  //--------------------------------------------------
  // PMMA Bulk Optics (Fiber Cladding)
  //--------------------------------------------------

  G4double* RIPMMA=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RIPMMA+i)=1.49;}
  G4double* ALPMMA=new G4double[VectorSizePMMA];
  for (int i=0;i<VectorSizePMMA;i++){*(ALPMMA+i)=1*mm;};//PMMAAbsorptionL[i];}//14.04*cm;}  // was 3.5*m

  G4MaterialPropertiesTable* mpt_PMMA = new G4MaterialPropertiesTable();
  mpt_PMMA->AddProperty("RINDEX",ScintEmission,RIPMMA,VectorSize);
  mpt_PMMA->AddProperty("ABSLENGTH",PMMAAbsorptionE,ALPMMA,VectorSizePMMA);

  fPMMA->SetMaterialPropertiesTable(mpt_PMMA);

  //--------------------------------------------------
  // GLUE Bulk Optics
  //--------------------------------------------------

  G4double* RIGLUE=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RIGLUE+i)=1.55;} //1.55
  G4double* ALGLUE=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALGLUE+i)=0.5*m;};//1*mm   PMMAAbsorptionL[i];}//14.04*cm;}  // was 3.5*m

  G4MaterialPropertiesTable* mpt_GLUE = new G4MaterialPropertiesTable();
  mpt_GLUE->AddProperty("RINDEX",ScintEmission,RIGLUE,VectorSize);
  mpt_GLUE->AddProperty("ABSLENGTH",ScintEmission,ALGLUE,VectorSize);

  fGLUE->SetMaterialPropertiesTable(mpt_GLUE);

  //--------------------------------------------------
  // GLUE_EDGES Optics
  //--------------------------------------------------

  G4double* RIGLUE_EDGES=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RIGLUE_EDGES+i)=1.58;} //1.55
  G4double* ALGLUE_EDGES=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALGLUE_EDGES+i)=0.5*m;};//1*mm   PMMAAbsorptionL[i];}//14.04*cm;}  // was 3.5*m

  G4MaterialPropertiesTable* mpt_GLUE_EDGES = new G4MaterialPropertiesTable();
  mpt_GLUE_EDGES->AddProperty("RINDEX",ScintEmission,RIGLUE_EDGES,VectorSize);
  mpt_GLUE_EDGES->AddProperty("ABSLENGTH",ScintEmission,ALGLUE_EDGES,VectorSize);

  fGLUE_EDGES->SetMaterialPropertiesTable(mpt_GLUE_EDGES);

  
  //--------------------------------------------------
  //  CsI(Na) Scintillator Bulk Optics
  //--------------------------------------------------
  //G4cout<<"HERE "<<VectorSize<<G4endl;
  G4double* RICsI=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(RICsI+i)=1.84;} //1.84 //aal picco di emissione
  G4double* ALCsI=new G4double[VectorSize];
  for (int i=0;i<VectorSize;i++){*(ALCsI+i)=0.5*m;} //0.5*m

  G4MaterialPropertiesTable* mpt_CsI = new G4MaterialPropertiesTable();
  mpt_CsI->AddProperty("RINDEX",ScintEmission,RICsI,VectorSize);
  mpt_CsI->AddProperty("ABSLENGTH",ScintEmission,ALCsI,VectorSize);
  mpt_CsI->AddProperty("SCINTILLATIONCOMPONENT1",ScintEmission,ScintIntensity,VectorSize);
  //mpt_CsI->AddProperty("SLOWCOMPONENT",ScintEmission,ScintIntensity,VectorSize);

  mpt_CsI->AddConstProperty("SCINTILLATIONYIELD",4100/MeV); //41000/MeV); 
  mpt_CsI->AddConstProperty("RESOLUTIONSCALE",1);
  mpt_CsI->AddConstProperty("SCINTILLATIONTIMECONSTANT1",630*ns);
  //mpt_CsI->AddConstProperty("SLOWTIMECONSTANT",630*ns);
  //mpt_CsI->AddConstProperty("YIELDRATIO",1);

  fCsI->SetMaterialPropertiesTable(mpt_CsI);

  //Set Birks' constant
  //fCsI->GetIonisation()->SetBirksConstant(0.55); //Semi-empirical calculation of quenching factors for ions in scintillators V.I. Tretyak

}
//Others FUnctions
void OptMaterials::VectorsFromData (std::string path,std::vector<G4double>* v1,std::vector<G4double>* v2){
  std::ifstream datafile;
  std::string line,line2;
  datafile.open(path);
  int c=0;
  while(getline (datafile,line,'\n')){
    G4String test = (G4String)line;
    if (!test.contains("#")){
    std::stringstream input_line(line);
    while(getline(input_line,line2,':')){
      if(line2!=""){
	  if(c%2==0){
	    G4double conv=std::stod(line2)*eV;
	    v1->push_back(conv);
	  }
	  else {
	    G4double conv=std::stod(line2);
	    v2->push_back(conv);
	  }
      }
	c++;
    }
    }
  }
  datafile.close();
}

void OptMaterials::PrintOptMaterialSettings(){
}
