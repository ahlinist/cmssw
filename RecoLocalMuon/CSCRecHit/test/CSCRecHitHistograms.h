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



class H1DRecHit {
public:
  /// Constructor from collection name
  H1DRecHit(std::string name_) {
    TString N = name_.c_str();
    name=N;
    
    hRecPositionX = new TH1F(N+"_hRecPositionX", "CSCRecHit x position (cm)", 100, -100., 100.);
    hRecPositionY = new TH1F(N+"_hRecPositionY", "CSCRecHit y position (cm)", 100, -200., 200.);
    hSimPositionX = new TH1F(N+"_hSimPositionX", "CSCSimHit x position (cm)", 100, -100., 100.);
    hSimPositionY = new TH1F(N+"_hSimPositionY", "CSCSimHit y position (cm)", 100, -200., 200.);
    hResPositionX = new TH1F(N+"_hResPositionX", "CSC hit resolution x (cm)", 100, -10., 10.);
    hResPositionY = new TH1F(N+"_hResPositionY", "CSC hit resolution y (cm)", 100, -20., 20.);

    hrDphi = new TH1F(N+"_hrDphi", "CSC r x delta phi (cm)", 100, -2., 2.);
  }

  /// Constructor from collection name and TFile.
  /// It retrieves all the histos of the set from the file.
  H1DRecHit(TString name_, TFile* file) {
    name=name_;
    hRecPositionX     = (TH1F *) file->Get(name+"_RecPositionX");
    hRecPositionY     = (TH1F *) file->Get(name+"_RecPositionY");
    hSimPositionX     = (TH1F *) file->Get(name+"_SimPositionX");
    hSimPositionY     = (TH1F *) file->Get(name+"_SimPositionY");
    hResPositionX     = (TH1F *) file->Get(name+"_ResPositionX");
    hResPositionY     = (TH1F *) file->Get(name+"_ResPositionY");
    hrDphi            = (TH1F *) file->Get(name+"_rDphi");
  }

  /// Destructor
  virtual ~H1DRecHit() {
    delete hRecPositionX;
    delete hRecPositionY;
    delete hSimPositionX;
    delete hSimPositionY;
    delete hResPositionX;
    delete hResPositionY;
    delete hrDphi;
  }

  // Operations
  /// Fill all the histos
  void Fill(float recx, float recy, float simx, float simy, float rdphi) {
    hRecPositionX->Fill(recx);
    hRecPositionY->Fill(recy);
    hSimPositionX->Fill(simx);
    hSimPositionY->Fill(simy);
    hResPositionX->Fill(simx-recx);
    hResPositionY->Fill(simy-recy);
    hrDphi->Fill(rdphi);  
  }

  /// Write all the histos to currently opened file
  void Write() {
    hRecPositionX->Write();
    hRecPositionY->Write();
    hSimPositionX->Write();
    hSimPositionY->Write();
    hResPositionX->Write();
    hResPositionY->Write();
    hrDphi->Write();
  }
  
  TH1F *hRecPositionX;
  TH1F *hRecPositionY;
  TH1F *hSimPositionX;
  TH1F *hSimPositionY;
  TH1F *hResPositionX;
  TH1F *hResPositionY;
  TH1F *hrDphi;

  TString name;
};
#endif


