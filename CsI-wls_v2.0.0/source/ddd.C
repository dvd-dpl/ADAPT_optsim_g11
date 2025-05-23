//TString INFileName = "CircularFiber";
const int TotalFibers = 10000;
const int nSiPM = 4;
Int_t threshold = 0;
Int_t FibX = 130;
Int_t FibY = 130;

void ddd(TString INFileName = "CircularFiber"){
  INFileName +=".root";
  TFile *fin = new TFile(INFileName);

  cout<<" [INFO]: Open root file: "<<fin->GetName()<<endl;
  //TGraph *gX = new TGraph();
  //TGraph *gY = new TGraph();
  //Histograms

  TH1D *h_edep = new TH1D("h_edep","Energy_Deposited_MeV",1000,0,20);
  TH1D *h_totGenScint = new TH1D("h_totGenScint","Total_Photons_Generated_in_Calo",5000,0,50000);
  TH1D *h_Vxd = new TH1D("h_Vxd","WLS_photons_in_SiPM_XR_%",1000,0,100);
  TH1D *h_Vxs = new TH1D("h_Vxs","WLS_photons_in_SiPM_XL_%",1000,0,100);
  TH1D *h_Vyd = new TH1D("h_Vyd","WLS_photons_in_SiPM_YR_%",1000,0,100);
  TH1D *h_Vys = new TH1D("h_Vys","WLS_photons_in_SiPM_YL_%",1000,0,100);
  TH1D *h_Esc = new TH1D("h_Esc","Total_Photons_Escaped_%",1000,0,100);
  //new
  TH1D *h_CaloSipm = new TH1D("h_CaloSipm","Total_Photons_Absorbed_in_Calo_SiPMs_%",1000,0,100);
  TH1D *h_ViewX= new TH1D("h_ViewX","WLS_Generated_in_X_View_%",1000,0,100);
  TH1D *h_ViewY= new TH1D("h_ViewY","WLS_Generated_in_Y_View_%",1000,0,100);
  TH1D *h_PVxd = new TH1D("h_PVxd","Total_Photons_Absorbed_in_SiPM_XR_%",1000,0,100);
  TH1D *h_PVxs = new TH1D("h_PVxs","Total_Photons_Absorbed_in_SiPM_XL_%",1000,0,100);
  TH1D *h_PVyd = new TH1D("h_PVyd","Total_Photons_Absorbed_in_SiPM_YR_%",1000,0,100);
  TH1D *h_PVys = new TH1D("h_PVys","Total_Photons_Absorbed_in_SiPM_YL_%",1000,0,100);
  TH1D *h_CaloAbs = new TH1D("h_CaloAbs","Total_Photons_Absorbed_in_Calo_%",1000,0,100);
  TH1D *h_CladAbs = new TH1D("h_CladAbs","Total_Photons_Absorbed_in_Clad_%",1000,0,100);

  //Tree and Branches Declaration
  TTree *tree = (TTree*)fin->Get("Event_Tree");
  Int_t       size;
  Int_t       EventID;
  Int_t       Module_ID[TotalFibers], Volume_ID[TotalFibers], Fiber_ID[TotalFibers];
  Double_t    photonAbsorbed[TotalFibers];
  Double_t    photonAbsorbedScint[TotalFibers];
  Double_t    photonAbsorbedWLS[TotalFibers];
  Double_t    photonGenerated[TotalFibers];
  Double_t    photonGeneratedScint[TotalFibers];
  Double_t    photonGeneratedWLS[TotalFibers];
  Double_t    EDep[TotalFibers];

  // Set branch addresses.
  tree->SetBranchAddress("EventID"   ,&EventID);
  tree->SetBranchAddress("VectorSize",&size);
  tree->SetBranchAddress("Module_ID" ,&Module_ID);
  tree->SetBranchAddress("Volume_ID" ,&Volume_ID);
  tree->SetBranchAddress("Fiber_ID"  ,&Fiber_ID);
  tree->SetBranchAddress("NumberOfPhotonAbsorbed",&photonAbsorbed);
  tree->SetBranchAddress("NumberOfPhotonGenerated",&photonGenerated);
  tree->SetBranchAddress("NumberOfPhotonAbsorbed_Scint",&photonAbsorbedScint);
  tree->SetBranchAddress("NumberOfPhotonGenerated_Scint",&photonGeneratedScint);
  tree->SetBranchAddress("NumberOfPhotonAbsorbed_WLS",&photonAbsorbedWLS);
  tree->SetBranchAddress("NumberOfPhotonGenerated_WLS",&photonGeneratedWLS);
  tree->SetBranchAddress("TotalEnergyDeposit_MeV",&EDep);

  Int_t iFibX = 0, iFibY = 0;
  Int_t nentries = tree->GetEntries();
  Int_t totalGen = 0;
  Int_t totalGenScint = 0;
  Int_t totalGenWLS = 0;
  Int_t totalGenWLS_X = 0;
  Int_t totalGenWLS_Y = 0;

  Int_t totalAbs = 0;
  Int_t totalAbsScint = 0;
  Int_t totalAbsWLS = 0;
  Int_t totalAbsSipmX_dx = 0;
  Int_t totalAbsSipmY_dx = 0;
  Int_t totalAbsSipmX_sx = 0;
  Int_t totalAbsSipmY_sx = 0;

  Int_t totalAbsScint_X=0;
  Int_t totalAbsScint_Y=0;
  Int_t totalAbsWLS_W=0;
  Int_t totalAbsScint_W=0;

  Int_t totalAbsSipmCalo=0;
  Int_t totalAbsCalo=0;
  Int_t totalAbsInClad=0;
  Int_t totalAbsinWorld=0;



  Double_t totalEnergyDeposit = 0;
  Int_t photonSiPM[nSiPM];

  for(int iev=0;iev<nentries;iev++){
    tree->GetEntry(iev);
    cout<<" *** Now event "<<iev<<endl;
    for(int i=0;i<size;i++){
      if(Volume_ID[i]==1){
	totalGen += photonGenerated[i];
	totalEnergyDeposit += EDep[i];
  totalAbsCalo += (photonAbsorbedScint[i]+photonAbsorbedWLS[i]);
      }
      if(Volume_ID[i]==2 or Volume_ID[i]==4){
	totalGenWLS += photonGeneratedWLS[i];
	totalAbsScint += photonAbsorbedScint[i];
      }
      if(Volume_ID[i]==3 or Volume_ID[i]==5){
	totalAbsInClad += (photonAbsorbedScint[i] + photonAbsorbedWLS[i]);
      }
      if(Volume_ID[i]==7){
	totalAbsSipmX_dx += photonAbsorbedWLS[i];
      }
      if(Volume_ID[i]==6){
	totalAbsSipmCalo += (photonAbsorbedScint[i]+photonAbsorbedWLS[i]) ;
      }
      if(Volume_ID[i]==8){
	totalAbsSipmX_sx += photonAbsorbedWLS[i];
      }
      if(Volume_ID[i]==9){
	totalAbsSipmY_dx += photonAbsorbedWLS[i];
      }
      if(Volume_ID[i]==10){
	totalAbsSipmY_sx += photonAbsorbedWLS[i];
      }
      if(Volume_ID[i]==2){
	totalGenWLS_X += photonGeneratedWLS[i];
  totalAbsScint_X += photonAbsorbedScint[i];
      }
       if(Volume_ID[i]==4){
	totalGenWLS_Y += photonGeneratedWLS[i];
  totalAbsScint_Y += photonAbsorbedScint[i];
      }
      if(Volume_ID[i]==0){
 totalAbsWLS_W += photonAbsorbedWLS[i];
 totalAbsScint_W += photonAbsorbedScint[i];
 totalAbsinWorld += photonAbsorbedWLS[i] + photonAbsorbedScint[i];
     }
    }

  cout<<" Photons generation yield is = "<<((float)totalGen/(float)totalEnergyDeposit)<<" Ph./MeV"<<endl;
  cout<<" Total Energy Deposit = "<<totalEnergyDeposit<<" MeV"<<endl;
  cout<<" Total Photons Generated in Calo = "<<totalGen<<endl;
  cout<<" Total Photons Absorbed in Calo = "<<((float)totalAbsCalo/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons Absorbed in SiPMs Calo = "<<((float)totalAbsSipmCalo/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons detected in X view dx = "<<((float)totalAbsSipmX_dx/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons detected in X view sx = "<<((float)totalAbsSipmX_sx/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons detected in Y view dx = "<<((float)totalAbsSipmY_dx/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons detected in Y view sx = "<<((float)totalAbsSipmY_sx/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons lost in fibers cladding = "<<((float)totalAbsInClad/(float)totalGen)*100<<"%"<<endl;
  cout<<" Total Photons escaped = "<<((float)totalAbsinWorld/(float)totalGen)*100<<"%"<<endl;
  cout<<" WLS Photons"<<endl;
  cout<<" WLS photons generated in X view = "<<((float)totalAbsScint_X/(float)totalGen)*100<<"%"<<endl;
  cout<<" WLS photons generated in Y view = "<<((float)totalAbsScint_Y/(float)totalGen)*100<<"%"<<endl;
  cout<<" WLS Photons detected in X view dx = "<<((float)totalAbsSipmX_dx/(float)totalGenWLS_X)*100<<"%"<<endl;
  cout<<" WLS Photons detected in X view sx = "<<((float)totalAbsSipmX_sx/(float)totalGenWLS_X)*100<<"%"<<endl;
  cout<<" WLS Photons detected in Y view dx = "<<((float)totalAbsSipmY_dx/(float)totalGenWLS_Y)*100<<"%"<<endl;
  cout<<" WLS Photons detected in Y view sx = "<<((float)totalAbsSipmY_sx/(float)totalGenWLS_Y)*100<<"%"<<endl;
  cout<<" ****************************************" <<endl;

  //Fill the histograms
  h_edep->Fill(totalEnergyDeposit);
  h_totGenScint->Fill(totalGen);
  h_Vxd->Fill(((float)totalAbsSipmX_dx/(float)totalGenWLS_X)*100);
  h_Vxs->Fill(((float)totalAbsSipmX_sx/(float)totalGenWLS_X)*100);
  h_Vyd->Fill(((float)totalAbsSipmY_dx/(float)totalGenWLS_Y)*100);
  h_Vys->Fill(((float)totalAbsSipmY_sx/(float)totalGenWLS_Y)*100);
  h_PVxd->Fill(((float)totalAbsSipmX_dx/(float)totalGen)*100);
  h_PVxs->Fill(((float)totalAbsSipmX_sx/(float)totalGen)*100);
  h_PVyd->Fill(((float)totalAbsSipmY_dx/(float)totalGen)*100);
  h_PVys->Fill(((float)totalAbsSipmY_sx/(float)totalGen)*100);
  h_Esc->Fill(((float)totalAbsinWorld/(float)totalGen)*100);
  h_CaloSipm->Fill(((float)totalAbsSipmCalo/(float)totalGen)*100);
  h_ViewX->Fill(((float)totalAbsScint_X/(float)totalGen)*100);
  h_ViewY->Fill(((float)totalAbsScint_X/(float)totalGen)*100);
  h_CaloAbs->Fill(((float)totalAbsCalo/(float)totalGen)*100);
  h_CladAbs->Fill(((float)totalAbsInClad/(float)totalGen)*100);



  totalGen = 0;
  totalGenScint = 0;
  totalGenWLS = 0;
  totalAbs = 0;
  totalAbsScint = 0;
  totalAbsWLS = 0;
  totalEnergyDeposit = 0;
  totalAbsSipmX_dx = 0;
  totalAbsSipmY_dx = 0;
  totalAbsSipmX_sx = 0;
  totalAbsSipmY_sx = 0;
  totalGenWLS_X = 0;
  totalGenWLS_Y = 0;
  totalAbsScint_X=0;
  totalAbsScint_Y=0;
  totalAbsWLS_W=0;
  totalAbsSipmCalo=0;
  totalAbsCalo=0;
  totalAbsScint_W=0;
  totalAbsInClad=0;
  totalAbsinWorld=0;
}

//Optical Photons Tree
TTree *treeOpt = (TTree*)fin->Get("Optical_Photons_Tree");

//Histogram Declaration

TH1D *h_oScintCalo = new TH1D("h_oScintCalo","Photons_Generated_in_Calo_nm",1000,0,1000);
TH1D *h_oWLSAbs = new TH1D("h_oWLSAbs","Photons_Absorbed_in_WLS",1000,0,1000);
TH1D *h_oSiPMABs = new TH1D("h_oSiPMABs","Photons_ABsorbed_in_SiPMs",1000,0,1000);
TH1D *h_oEsc = new TH1D("h_oEsc","Total_Photons_Escaped",1000,0,1000);
TH1D *h_oEscScint = new TH1D("h_oEscScint","Total_Photons_Escaped_Scint",1000,0,1000);
TH1D *h_oEscWLS = new TH1D("h_oEscWLS","Total_Photons_Escaped_WLS",1000,0,1000);
TH1D *h_oDetTime = new TH1D("h_oDetTime","SiPM_Detection_Time_ns",1000,0,10000);

Int_t       OEventID;
Double_t    OPhoton_Wavelenght;
Int_t       OVolume_ID, OFiber_ID;
Double_t    OGlobalTime;
Int_t       OPhoton_CreatorProcessNumber;

// Set branch addresses.
treeOpt->SetBranchAddress("EventID"   ,&OEventID);
treeOpt->SetBranchAddress("Photon_Wavelenght_eV"   ,&OPhoton_Wavelenght);
treeOpt->SetBranchAddress("Volume_ID"   ,&OVolume_ID);
treeOpt->SetBranchAddress("Fiber_ID"   ,&OFiber_ID);
treeOpt->SetBranchAddress("Photon_GlobalTime"   ,&OGlobalTime);
treeOpt->SetBranchAddress("Photon_CreatorProcessNumber"   ,&OPhoton_CreatorProcessNumber);

//Variable Declaration
Int_t Onentries = treeOpt->GetEntries();
std::vector<Double_t> wlsgen;
std::vector<Double_t> sipmabs;
for (int i=0;i<200;i++){
  wlsgen.push_back(0.);
  sipmabs.push_back(0.);
}
Double_t CollEff=0;



for (int oi;oi<Onentries;oi++){
  treeOpt->GetEntry(oi);
  if(OVolume_ID==2 && OPhoton_CreatorProcessNumber==0){
    wlsgen.at(OFiber_ID) += 1.;
  }
  if(OVolume_ID==7 && OPhoton_CreatorProcessNumber==1){
    sipmabs.at(OFiber_ID) += 1.;
  }
  if(OPhoton_CreatorProcessNumber==0){
    h_oScintCalo->Fill(1239.8/(float)OPhoton_Wavelenght);
  }
  if(OPhoton_CreatorProcessNumber==0 && (OVolume_ID==2 || OVolume_ID==4)){
    h_oWLSAbs->Fill(1239.8/(float)OPhoton_Wavelenght);
  }
  if(OVolume_ID>6){
    h_oSiPMABs->Fill(1239.8/(float)OPhoton_Wavelenght);
    h_oDetTime->Fill(OGlobalTime);
  }
  if(OPhoton_CreatorProcessNumber==0 && (OVolume_ID==0)){
    h_oEscScint->Fill(1239.8/(float)OPhoton_Wavelenght);
    h_oEsc->Fill(1239.8/(float)OPhoton_Wavelenght);
  }
  if(OPhoton_CreatorProcessNumber==1 && (OVolume_ID==0)){
    h_oEscWLS->Fill(1239.8/(float)OPhoton_Wavelenght);
    h_oEsc->Fill(1239.8/(float)OPhoton_Wavelenght);
  }

}
TGraph* g = new TGraph();
Int_t j=0;
for (int ii=0;ii<200;ii++){
  if (wlsgen.at(ii)>0){
  //wlsgen.at(i)=sipmabs.at(i)/wlsgen.at(i);
  g->SetPoint(ii,ii,(sipmabs.at(ii)/wlsgen.at(ii))*100);
  cout<<ii<<" Absorbed in SiPM "<< sipmabs.at(ii)<< "Generated at WLS "<<wlsgen.at(ii)<<endl;
  //j++;
}
else{
  g->SetPoint(ii,ii,0);
}
  //g->SetPoint(i,i,wlsgen.at(i));
}
g->SetMarkerStyle(20);
g->SetMarkerSize(1);
g->SetMarkerColor(2);
g->Draw();



/////////////OUTPUT
  cout<<"\n Statistic: "<<nentries<<" MIP events"<<endl;
  cout<<" Total Energy Deposit MPV = "<<h_edep->GetMean()<<" +/- "<<h_edep->GetRMS()<<endl;
  cout<<" Total Photons Generated in the Calo (MPV) = "<<h_totGenScint->GetMean()<<" +/- "<<h_totGenScint->GetRMS()<<endl;
  cout<<" Total Photons Absorbed in Calo (MPV) = "<<h_CaloAbs->GetMean()<<"%"<<" +/- "<<h_CaloAbs->GetRMS()<<"%"<<endl;
  cout<<" Total Photons Absorbed in SiPMs Calo (MPV) = "<<h_CaloSipm->GetMean()<<"%"<<" +/- "<<h_CaloSipm->GetRMS()<<"%"<<endl;
  cout<<" Total Photons detected in X view dx (MPV) = "<<h_PVxd->GetMean()<<"%"<<" +/- "<<h_PVxd->GetRMS()<<"%"<<endl;
  cout<<" Total Photons detected in X view sx (MPV) = "<<h_PVxs->GetMean()<<"%"<<" +/- "<<h_PVxs->GetRMS()<<"%"<<endl;
  cout<<" Total Photons detected in Y view dx (MPV) = "<<h_PVyd->GetMean()<<"%"<<" +/- "<<h_PVyd->GetRMS()<<"%"<<endl;
  cout<<" Total Photons detected in Y view sx (MPV) = "<<h_PVys->GetMean()<<"%"<<" +/- "<<h_PVys->GetRMS()<<"%"<<endl;
  cout<<" Total Photons lost in fibers cladding (MPV) = "<<h_CladAbs->GetMean()<<"%"<<" +/- "<<h_CladAbs->GetRMS()<<"%"<<endl;
  cout<<" Total Photons escaped (MPV) = "<<h_Esc->GetMean()<<"%"<<" +/- "<<h_Esc->GetRMS()<<"%"<<endl;
  cout<<" WLS Photons"<<endl;
  cout<<" WLS photons generated in X view (MPV) = "<<h_ViewX->GetMean()<<"%"<<" +/- "<<h_ViewX->GetRMS()<<"%"<<endl;
  cout<<" WLS photons generated in Y view (MPV) = "<<h_ViewY->GetMean()<<"%"<<" +/- "<<h_ViewY->GetRMS()<<"%"<<endl;
  cout<<" WLS Photons detected in X view dx (MPV) = "<<h_Vxd->GetMean()<<"%"<<" +/- "<<h_Vxd->GetRMS()<<"%"<<endl;
  cout<<" WLS Photons detected in X view sx (MPV) = "<<h_Vxs->GetMean()<<"%"<<" +/- "<<h_Vxs->GetRMS()<<"%"<<endl;
  cout<<" WLS Photons detected in Y view dx (MPV) = "<<h_Vyd->GetMean()<<"%"<<" +/- "<<h_Vyd->GetRMS()<<"%"<<endl;
  cout<<" WLS Photons detected in Y view sx (MPV) = "<<h_Vys->GetMean()<<"%"<<" +/- "<<h_Vys->GetRMS()<<"%"<<endl;
  cout<<" ****************************************" <<endl;

  TFile* tf = new TFile("H_Results_"+INFileName+".root","RECREATE");
  tf->cd();
  h_edep->Write();
  h_totGenScint->Write();
  h_CaloAbs->Write();
  h_CaloSipm->Write();
  h_CladAbs->Write();
  h_PVxd->Write();
  h_PVxs->Write();
  h_PVyd->Write();
  h_PVys->Write();
  h_Esc->Write();
  h_ViewX->Write();
  h_ViewY->Write();
  h_Vxd->Write();
  h_Vxs->Write();
  h_Vyd->Write();
  h_Vys->Write();
  //Opitcal
  h_oScintCalo->Write();
  h_oWLSAbs->Write();
  h_oSiPMABs->Write();
  h_oEsc->Write();
  h_oEscScint->Write();
  h_oEscWLS->Write();
  h_oDetTime->Write();
  tf->Close();


}
