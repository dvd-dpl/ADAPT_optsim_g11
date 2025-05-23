//TString INFileName = "CircularFiber";
const int TotalFibers = 10000;
const int nSiPM = 4;
Int_t threshold = 0;
Int_t FibX = 130;
Int_t FibY = 130;

void ccc(TString INFileName = "CircularFiber"){
  INFileName +=".root";
  TFile *fin = new TFile(INFileName);

  cout<<" [INFO]: Open root file: "<<fin->GetName()<<endl;
  //TGraph *gX = new TGraph();
  //TGraph *gY = new TGraph();

  //Tree and Branches Declaration
  TTree *tree = (TTree*)fin->Get("Event_Tree");
  Int_t       size;
  Int_t       EventID;
  Int_t       Module_ID[TotalFibers], Volume_ID[TotalFibers], Fiber_ID[TotalFibers];
  Double_t    photonAbsorbed[TotalFibers];
  Double_t    photonGenerated[TotalFibers];
  Double_t    EDep[TotalFibers];

  // Set branch addresses.
  tree->SetBranchAddress("EventID"   ,&EventID);
  tree->SetBranchAddress("VectorSize",&size);
  tree->SetBranchAddress("Module_ID" ,&Module_ID);
  tree->SetBranchAddress("Volume_ID" ,&Volume_ID);
  tree->SetBranchAddress("Fiber_ID"  ,&Fiber_ID);
  tree->SetBranchAddress("NumberOfPhotonAbsorbed_WLS",&photonAbsorbed);
  tree->SetBranchAddress("NumberOfPhotonGenerated_Scint",&photonGenerated);
  tree->SetBranchAddress("TotalEnergyDeposit_MeV",&EDep);

  TH1D *h1FX = new TH1D("h1FX","h1FX",266,-0.5,265.5);
  TH1D *h1FY = new TH1D("h1FY","h1FY",266,-0.5,265.5);

  TH1D *h1FX_total = new TH1D("h1FX_total","h1FX_total",266,-0.5,265.5);
  TH1D *h1FY_total = new TH1D("h1FY_total","h1FY_total",266,-0.5,265.5);

  char buff[100];
  TH1D *hSiPM[nSiPM];
  for(int j = 0;j<nSiPM;j++){
    sprintf(buff,"hSiPM[%d]",j);
    hSiPM[j] = new TH1D(buff,buff,250,10.,20.);
  }
  TH1D *hVX = new TH1D("hVX","hVX",1000,0.,100.);
  TH1D *hVY = new TH1D("hVY","hVY",1000,0.,100.);

  Int_t iFibX = 0, iFibY = 0;
  Int_t nentries = tree->GetEntries();
  Int_t totalPhotons = 0;
  Double_t totalEnergyDeposit = 0;
  Int_t photonSiPM[nSiPM];

  for(int iev=0;iev<nentries;iev++){
    tree->GetEntry(iev);
    cout<<" *** Now event "<<iev<<endl;
    for(int i=0;i<size;i++){
      if(Volume_ID[i]==1){
	totalPhotons = photonGenerated[i];
	totalEnergyDeposit = EDep[i];
      }
      if(Volume_ID[i]==6){
	for(int j = 0;j<nSiPM;j++){
	  if(Fiber_ID[i]==j){
	    photonSiPM[j] = photonAbsorbed[i];
	    hSiPM[j]->Fill((float)photonSiPM[j]/totalPhotons*100);
	  }
	}
      }
      if(photonAbsorbed[i]>threshold){
	if(Volume_ID[i]==2){
	  h1FX->Fill((float)Fiber_ID[i],(float)photonAbsorbed[i]);
	  h1FX_total->Fill((float)Fiber_ID[i],(float)photonAbsorbed[i]);
	  //cout<<"XView FiberID = "<<Fiber_ID[i]<<" photonAbsorbed = "<<photonAbsorbed[i]<<endl;
	  //gX->SetPoint(iFibX,Fiber_ID[i],photonAbsorbed[i]);
	  iFibX++;
	  //h1FX->Fill((float)Fiber_ID[i],(float)photonAbsorbed[i]);
	}
	if(Volume_ID[i]==4){
	  h1FY->Fill((float)Fiber_ID[i],(float)photonAbsorbed[i]);
	  h1FY_total->Fill((float)Fiber_ID[i],(float)photonAbsorbed[i]);
	  //cout<<"VolumeID = "<<Volume_ID[i]<<" FiberID = "<<Fiber_ID[i]<<" photonAbsorbed = "<<photonAbsorbed[i]<<endl;
	  //gY->SetPoint(iFibY,Fiber_ID[i],photonAbsorbed[i]);
	  iFibY++;
	}
      }
    }
    hVX->Fill((double)(h1FX->Integral()/totalPhotons)*100);
    hVY->Fill((double)(h1FY->Integral()/totalPhotons)*100);
    cout<<" X : "<<(double)(h1FX->Integral()/totalPhotons)*100<<endl;
    cout<<" Y : "<<(double)(h1FY->Integral()/totalPhotons)*100<<endl;
    h1FX->Reset();
    h1FY->Reset();
  }

  // cout<<"\t\t*****************"<<endl;
  // cout<<" [INFO]: Total Photon Generated = "<<totalPhotons<<endl;
  // cout<<" [INFO]: Total Energy Deposit = "<<totalEnergyDeposit<<" MeV"<<endl;
  // cout<<" [INFO]: Yield = "<<totalPhotons/totalEnergyDeposit<<" ph/MeV"<<endl;
  // cout<<" --- "<<endl;
  // for(int j = 0;j<nSiPM;j++)
  //   cout<<" [INFO]: Total Photon Absorbed in SiPM "<<j<<" = "<<photonSiPM[j]<< " ( "<<(float)photonSiPM[j]/totalPhotons*100<<"% )"<<endl;
  // cout<<" --- "<<endl;
  // cout<<" [INFO]: Total Photon Absorbed in X View = "<<h1FX->Integral()<< " ( "<<(double)(h1FX->Integral()/totalPhotons)*100<<"% )"<<endl;
  // cout<<" [INFO]: Total Photon Absorbed in Y View = "<<h1FY->Integral()<< " ( "<<(double)(h1FY->Integral()/totalPhotons)*100<<"% )"<<endl;
  // cout<<"\t\t*****************"<<endl;

  //cout<<iFibX<<" "<<iFibY<<endl;
  //gStyle->SetOptFit(0);
  //gStyle->SetOptStat(1);

  // TF1 *fX = new TF1("fX","gaus",0,TotalFibers);
  // fX->SetParameter(0,6000);
  // fX->SetParLimits(0,1000,10000);

  // fX->SetParameter(1,FibX);
  // fX->SetParLimits(1,FibX-20,FibX+20);

  // fX->SetParameter(2,3);
  // fX->SetParLimits(2,0.1,10);

  TCanvas *cHX = new TCanvas("cHX","cHX");
  h1FX_total->Draw("hist");
  TCanvas *cHY = new TCanvas("cHY","cHY");
  h1FY_total->Draw("hist");

  TCanvas *cSiPM[nSiPM];
  for(int i =0;i<nSiPM;i++){
    sprintf(buff,"cSiPM[%d]",i);
    cSiPM[i] = new TCanvas(buff,buff);
    cSiPM[i]->cd();
    hSiPM[i]->Draw();
  }
  TCanvas *cVX = new TCanvas("cVX","cVX");
  cVX->cd();
  hVX->Draw();

  TCanvas *cVY = new TCanvas("cVY","cVY");
  cVY->cd();
  hVY->Draw();


  // TCanvas *cX = new TCanvas("cX","cX");
  // gX->SetMarkerStyle(20);
  // gX->SetMarkerColor(4);
  // gX->SetMarkerSize(1);

  // gX->SetTitle("ViewX");
  // gX->GetXaxis()->SetTitle("Fiber_ID");
  // gX->GetYaxis()->SetTitle("# Photon Absorbed");
  // gX->GetYaxis()->SetTitleOffset(1.35);

  // gX->GetYaxis()->SetRangeUser(0,10000);

  // gX->Fit(fX);
  // gX->Draw("APsame");

  // TF1 *fY = new TF1("fY","gaus",0,TotalFibers);
  // fY->SetParameter(0,6000);
  // fY->SetParLimits(0,1000,10000);

  // fY->SetParameter(1,FibY);
  // fY->SetParLimits(1,FibY-20,FibY+20);

  // fY->SetParameter(2,3);
  // fY->SetParLimits(2,0.1,10);

  // TCanvas *cY = new TCanvas("cY","cY");
  // gY->SetMarkerStyle(20);
  // gY->SetMarkerColor(8);
  // gY->SetMarkerSize(1);

  // gY->SetTitle("ViewY");
  // gY->GetXaxis()->SetTitle("Fiber_ID");
  // gY->GetYaxis()->SetTitle("# Photon Absorbed");
  // gY->GetYaxis()->SetTitleOffset(1.35);

  // gY->GetYaxis()->SetRangeUser(0,10000);

  // gY->Fit(fY);
  // gY->Draw("APsame");
}
