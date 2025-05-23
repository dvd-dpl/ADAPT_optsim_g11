#ifndef FileManager_h
#define FileManager_h 1

#include "TreeStructures.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "globals.hh"
#include "RunAction.hh"

using namespace std;

class FileManager {
  public:
    //FileManager() : _fFileOUT(NULL),_ftMessangers(NULL),_ftPrimary(NULL),_ftPhoton(NULL),_ftSecond(NULL) {};
  FileManager() : _fFileOUT(NULL),_ftEvent(NULL),_ftPhoton(NULL), _ftComPair(NULL) {};
    virtual ~FileManager(){};

    void set_fileOUTPUT(TFile *fout) {_fFileOUT = fout;};
    TFile *get_fileOUTPUT() const {return _fFileOUT;};

    void CreateOutputFile(TString Name){
      _fFileOUT = new TFile(Name, "RECREATE");
      cout<<"[FileManager INFO]: Create Output file: "<<_fFileOUT->GetName()<<endl;
      _fFileOUT->cd();
    };

    //Initialize Trees
    void InitializeEventTree(){
      _ftEvent = new TTree("Event_Tree","Event_Tree");
      //Scalar
      _ftEvent->Branch("VectorSize", &_ftrackereventn.VectorSize, "VectorSize/I");
      _ftEvent->Branch("EventID", &_ftrackereventn.RunID, "EventID/I");
      _ftEvent->Branch( "PositionX_At_Vertex_mm", &_ftrackereventn.PosX,"PositionX_At_Vertex_mm/D");
      _ftEvent->Branch( "PositionY_At_Vertex_mm", &_ftrackereventn.PosY,"PositionY_At_Vertex_mm/D");
      _ftEvent->Branch( "PositionZ_At_Vertex_mm", &_ftrackereventn.PosZ,"PositionZ_At_Vertex_mm/D");
      _ftEvent->Branch( "DirectionX_At_Vertex", &_ftrackereventn.DirX,"DirectionX_At_Vertex/D");
      _ftEvent->Branch( "DirectionY_At_Vertex", &_ftrackereventn.DirY,"DirectionY_At_Vertex/D");
      _ftEvent->Branch( "DirectionZ_At_Vertex", &_ftrackereventn.DirZ,"DirectionZ_At_Vertex/D");
      //Vectors
      //Enumerator
      _ftEvent->Branch("Volume_Name",&_ftrackereventn.VolumeName);
      _ftEvent->Branch("Module_ID",&_ftrackereventn.ModuleEnum,"Module_ID[VectorSize]/I");
      _ftEvent->Branch("Volume_ID",&_ftrackereventn.VolumeEnum,"Volume_ID[VectorSize]/I");
      _ftEvent->Branch("Fiber_ID",&_ftrackereventn.FiberEnum,"Fiber_ID[VectorSize]/I");
      //Energy
      _ftEvent->Branch("TotalEnergyDeposit_MeV", &_ftrackereventn.TotalEnergyDep, "TotalEnergyDeposit_MeV[VectorSize]/D");
      //Secondary
      _ftEvent->Branch("NumberOfPhotonGenerated", &_ftrackereventn.TotalPhotonGenerated, "NumberOfPhotonGenerated[VectorSize]/D");
      _ftEvent->Branch("NumberOfPhotonGenerated_WLS", &_ftrackereventn.TotalPhotonGeneratedWLS, "NumberOfPhotonGeneratedWLS[VectorSize]/D");
      _ftEvent->Branch("NumberOfPhotonGenerated_Scint", &_ftrackereventn.TotalPhotonGeneratedScint, "NumberOfPhotonGeneratedScint[VectorSize]/D");
      _ftEvent->Branch("NumberOfPhotonAbsorbed", &_ftrackereventn.NumbOfPhotonsAbsorbed, "NumberOfPhotonAbsorbed[VectorSize]/D");
      _ftEvent->Branch("NumberOfPhotonAbsorbed_WLS", &_ftrackereventn.NumbOfPhotonsAbsorbedWLS, "NumberOfPhotonAbsorbedWLS[VectorSize]/D");
      _ftEvent->Branch("NumberOfPhotonAbsorbed_Scint", &_ftrackereventn.NumbOfPhotonsAbsorbedScint, "NumberOfPhotonAbsorbedScint[VectorSize]/D");
      //InStep
      _ftEvent->Branch("VectorSizeInStep", &_ftrackereventn.VectorSizeInStep, "VectorSizeInStep/I");
      _ftEvent->Branch("InStepEnergy_MeV", &_ftrackereventn.InStepEnergy, "InStepEnergy_MeV[VectorSizeInStep]/D");
      _ftEvent->Branch("InStepPosX_mm", &_ftrackereventn.InStepPosX, "InStepPosX_mm[VectorSizeInStep]/D");
      _ftEvent->Branch("InStepPosY_mm", &_ftrackereventn.InStepPosY, "InStepPosY_mm[VectorSizeInStep]/D");
      _ftEvent->Branch("InStepPosZ_mm", &_ftrackereventn.InStepPosZ, "InStepPosZ_mm[VectorSizeInStep]/D");
    };

    void InitializePhotonTree(){
      _ftPhoton = new TTree("Optical_Photons_Tree","Optical_Photons_Tree");
      //Scalar
      _ftPhoton->Branch("EventID", &_ftrackereventnp.RunID, "EventID/I");
      //Enumerator
      _ftPhoton->Branch("Volume_Name",&_ftrackereventnp.VolumeName,"Volume_Name/C");
      _ftPhoton->Branch("Module_ID",&_ftrackereventnp.ModuleEnum,"Module_ID/I");
      _ftPhoton->Branch("Volume_ID",&_ftrackereventnp.VolumeEnum,"Volume_ID/I");
      _ftPhoton->Branch("Fiber_ID",&_ftrackereventnp.FiberEnum,"Fiber_ID/I");
      //Energy
      _ftPhoton->Branch("Photon_Wavelenght_eV", &_ftrackereventnp.KEne, "Photon_Wavelenght_eV/D");
      //Position
      _ftPhoton->Branch("Photon_Absorption_Position_X_mm", &_ftrackereventnp.PosX, "Photon_Absorption_Position_X_mm/D");
      _ftPhoton->Branch("Photon_Absorption_Position_Y_mm", &_ftrackereventnp.PosY, "Photon_Absorption_Position_Y_mm/D");
      _ftPhoton->Branch("Photon_Absorption_Position_Z_mm", &_ftrackereventnp.PosZ, "Photon_Absorption_Position_Z_mm/D");
      //Time
      _ftPhoton->Branch("Photon_GlobalTime", &_ftrackereventnp.GlobalTime, "GlobalTime/D");
      _ftPhoton->Branch("Photon_LocalTime", &_ftrackereventnp.LocalTime, "LocalTime/D");
      //ProcessGenerator
      _ftPhoton->Branch("Photon_CreatorProcessName", &_ftrackereventnp.ProcessName, "Photon_CreatorProcessName/C");
      _ftPhoton->Branch("Photon_CreatorProcessNumber", &_ftrackereventnp.ProcessNumber, "Photon_CreatorProcessNumber/I");

    }


  void InitializeComPairTree(){
    _ftComPair = new TTree("ComPairHit","ComPairHit");
    _ftComPair->Branch("eventID", &_fcompaireventn.eventID, "eventID/I");
    _ftComPair->Branch("ProcessType",&_fcompaireventn.ProcessType, "ProcessType/I");

    _ftComPair->Branch("InteractionPointX",&_fcompaireventn.IntX,"IntX/D");
    _ftComPair->Branch("InteractionPointY",&_fcompaireventn.IntY,"IntY/D");
    _ftComPair->Branch("InteractionPointZ",&_fcompaireventn.IntZ,"IntZ/D");

    _ftComPair->Branch("InteractionModule",&_fcompaireventn.IntModule,"IntModule/I");
    _ftComPair->Branch("InteractionVolume",&_fcompaireventn.IntVolume,"IntVolume/I");
    _ftComPair->Branch("InteractionFiber",&_fcompaireventn.IntFiber,"IntFiber/I");

    _ftComPair->Branch("FirstParticle_ID",&_fcompaireventn.FirstParticleID,"FirstParticleID/I");
    _ftComPair->Branch("FirstParticle_Energy",&_fcompaireventn.FirstEnergy,"FirstEnergy/D");
    _ftComPair->Branch("FirstParticle_DirectionX",&_fcompaireventn.FirstDX,"FirstDX/D");
    _ftComPair->Branch("FirstParticle_DirectionY",&_fcompaireventn.FirstDY,"FirstDY/D");
    _ftComPair->Branch("FirstParticle_DirectionZ",&_fcompaireventn.FirstDZ,"FirstDZ/D");

    _ftComPair->Branch("SecondParticle_ID",&_fcompaireventn.SecondParticleID,"SecondParticleID/I");
    _ftComPair->Branch("SecondParticle_Energy",&_fcompaireventn.SecondEnergy,"SecondEnergy/D");
    _ftComPair->Branch("SecondParticle_DirectionX",&_fcompaireventn.SecondDX,"SecondDX/D");
    _ftComPair->Branch("SecondParticle_DirectionY",&_fcompaireventn.SecondDY,"SecondDY/D");
    _ftComPair->Branch("SecondParticle_DirectionZ",&_fcompaireventn.SecondDZ,"SecondDZ/D");
  };

  void set_ComPairTree(TTree *t) {_ftComPair = t;};
  TTree *get_ComPairTree() const {return _ftComPair;};
  void fill_ComPairTree() {_ftComPair->Fill();};

    //Set and Get
    void set_EventTree(TTree *t) {_ftEvent = t;};
    TTree *get_EventTree() const {cout<<"[FileManager DEBUG]: Get Primary Tree"<<endl; return _ftEvent;};

    void set_PhotonTree(TTree *t) {_ftPhoton = t;};
    TTree *get_PhotonTree() const {cout<<"[FileManager DEBUG]: Get Photon Tree"<<endl; return _ftPhoton;};


    //Fill
    void fill_EventTree(){
      if(_fFileOUT->IsOpen()){
        _fFileOUT->cd();
        _ftEvent->Fill();
      }
      else
        cout<<"The File is not Open!"<<endl;
    };

    void fill_PhotonTree(){
      if(_fFileOUT->IsOpen()){
        _fFileOUT->cd();
        _ftPhoton->Fill();
      }
      else
        cout<<"The File is not Open!"<<endl;
    };

    void writeInOutputFile(TObject *object){
      if(_fFileOUT->IsOpen()){
        _fFileOUT->cd();
        object->Write();
        cout<<"[FileManager INFO]: Write "<<object->GetName()<<endl;
      }
    }

    void closeFile(){
      if(_fFileOUT->IsOpen()){
        _fFileOUT->Close();
        cout<<"[FileManager INFO] CLOSE Output File "<<_fFileOUT->GetName()<<endl;
      }
    };

  TRACKEREVENTN* get_TrackerEvent() {return &_ftrackereventn;};
  TRACKEREVENTNP* get_PhotonEvent() {return &_ftrackereventnp;};
  COMPAIREVENTN* get_ComPairEvent() {return &_fcompaireventn;};
  private:
    TFile *_fFileOUT;
    TTree *_ftEvent;
    TTree *_ftPhoton;
    TTree *_ftComPair;

    TRACKEREVENTN _ftrackereventn;
    TRACKEREVENTNP _ftrackereventnp;
    COMPAIREVENTN _fcompaireventn;


};

#endif
