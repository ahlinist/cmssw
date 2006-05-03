#ifndef RecoLocalMuon_CSCRecHitHistograms_H
#define RecoLocalMuon_CSCRecHitHistograms_H

/** \class CSCRecHitHistograms
 *  Collection of histograms for 1D DT RecHit test.
 *
 * Author: D. Fortin  - UC Riverside
 */


#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include <string>



class H2DRecHit {
public:
  /// Constructor from collection name
  H2DRecHit(std::string name_) {
    TString N = name_.c_str();
    name=N;
    
    hRecPositionX = new TH1F(N+"_hRecPositionX", "CSCRecHit x position (cm)", 100, -100., 100.);
    hRecPositionY = new TH1F(N+"_hRecPositionY", "CSCRecHit y position (cm)", 100, -200., 200.);
    hSimPositionX = new TH1F(N+"_hSimPositionX", "CSCSimHit x position (cm)", 100, -100., 100.);
    hSimPositionY = new TH1F(N+"_hSimPositionY", "CSCSimHit y position (cm)", 100, -200., 200.);
    hResPositionX = new TH1F(N+"_hResPositionX", "CSC hit resolution x (cm)", 101, -0.505, 0.505);
    hResPositionY = new TH1F(N+"_hResPositionY", "CSC hit resolution y (cm)", 101, -2.02, 2.02);
    hResPositionYvsY = new TH2F(N+"_hResPositionYvsY", 
				"CSC hit resolution y vs y (cm)", 100, -200., 200., 41, -2.05, 2.05);

    hphi = new TH1F(N+"_hphi", "CSCRecHit phi (rads)", 130, -3.25, 3.25);
    hrDphi = new TH1F(N+"_hrDphi", "CSC r x Delta phi (cm)", 101, -0.101, 0.101);
    heta = new TH1F(N+"_heta", "CSCRecHit eta ", 100, 0.5, 2.5);
    hdeta = new TH1F(N+"_hdeta", "CSC Delta eta ", 101, -0.0101, 0.0101);
    hdetavseta = new TH2F(N+"_hdetavseta", "CSC Delta eta vs eta", 100, 0.5, 2.5, 51, -0.0102, 0.0102);



  }

  /// Constructor from collection name and TFile.
  /// It retrieves all the histos of the set from the file.
  H2DRecHit(TString name_, TFile* file) {
    name=name_;
    hRecPositionX     = (TH1F *) file->Get(name+"_RecPositionX");
    hRecPositionY     = (TH1F *) file->Get(name+"_RecPositionY");
    hSimPositionX     = (TH1F *) file->Get(name+"_SimPositionX");
    hSimPositionY     = (TH1F *) file->Get(name+"_SimPositionY");
    hResPositionX     = (TH1F *) file->Get(name+"_ResPositionX");
    hResPositionY     = (TH1F *) file->Get(name+"_ResPositionY");
    hResPositionYvsY  = (TH2F *) file->Get(name+"_ResPositionYvsY");
 
    hphi              = (TH1F *) file->Get(name+"_phi");
    hrDphi            = (TH1F *) file->Get(name+"_rDphi");
    heta              = (TH1F *) file->Get(name+"_eta");
    hdeta             = (TH1F *) file->Get(name+"_deta");
    hdetavseta        = (TH2F *) file->Get(name+"_detavseta");

  }

  /// Destructor
  virtual ~H2DRecHit() {
    delete hRecPositionX;
    delete hRecPositionY;
    delete hSimPositionX;
    delete hSimPositionY;
    delete hResPositionX;
    delete hResPositionY;
    delete hResPositionYvsY;

    delete hphi;
    delete hrDphi;
    delete heta;
    delete hdeta;
    delete hdetavseta;

  }

  // Operations
  /// Fill all the histos
  void Fill(float recx, float recy, float simx, float simy, float recphi, float rdphi, float receta, float simeta, float deta) {
    hRecPositionX->Fill(recx);
    hRecPositionY->Fill(recy);
    hSimPositionX->Fill(simx);
    hSimPositionY->Fill(simy);
    hResPositionX->Fill(recx-simx);
    hResPositionY->Fill(recy-simy);
    hResPositionYvsY->Fill(simy,simy-recy);

    hphi->Fill(recphi);
    hrDphi->Fill(rdphi);
    heta->Fill(receta);
    hdeta->Fill(deta);
    hdetavseta->Fill(simeta,deta);

  
  }

  /// Write all the histos to currently opened file
  void Write() {
    hRecPositionX->Write();
    hRecPositionY->Write();
    hSimPositionX->Write();
    hSimPositionY->Write();
    hResPositionX->Write();
    hResPositionY->Write();
    hResPositionYvsY->Write();

    hphi->Write();
    heta->Write();
    hrDphi->Write();
    hdeta->Write();
    hdetavseta->Write();

  }
  
  TH1F *hRecPositionX;
  TH1F *hRecPositionY;
  TH1F *hSimPositionX;
  TH1F *hSimPositionY;
  TH1F *hResPositionX;
  TH1F *hResPositionY;
  TH2F *hResPositionYvsY;

  TH1F *hrDphi;
  TH1F *heta;
  TH1F *hphi;
  TH1F *hdeta;
  TH2F *hdetavseta;


  TString name;
};
#endif


