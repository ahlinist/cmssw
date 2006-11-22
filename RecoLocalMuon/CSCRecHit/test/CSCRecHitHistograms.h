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
#include <iostream>


class H2DRecHit {
public:
  /// Constructor from collection name
  H2DRecHit(std::string name_) {
    TString N = name_.c_str();
    name=N;
// Matching Sim/Rec hits
    hHaveMatch       = new TH1F(N+"_hHaveMatch", "matching sim/reco pairs ?", 2, -0.5, 1.5);
// Local X position    
    hRecPositionX    = new TH1F(N+"_hRecPositionX", "CSCRecHit x local", 200, -24., 24.);
    hSimPositionX    = new TH1F(N+"_hSimPositionX", "CSCSimHit x local", 200, -24., 24.);
    hResPositionX    = new TH1F(N+"_hResPositionX", "CSCHit local Dx", 101, -0.202, 0.202);
    hPullX           = new TH1F(N+"_hPullX", "CSCHit x residual", 101, -5.05, 5.05);
    hResPositionXvsX = new TH2F(N+"_hResPositionXvsX", "CSCHit Dx vs x_{sim}", 200, -24., 24., 101, -2.02, 2.02); 
    hRecPositionYvsX = new TH2F(N+"_hRecPositionYvsX", "CSCRecHit y vs x", 600, -60., 60., 600, -60., 60.);
    hSimPositionYvsX = new TH2F(N+"_hSimPositionYvsX", "CSCSimHit y vs x", 600, -60., 60., 600, -60., 60.);
    hRecXvsSimX      = new TH2F(N+"_hRecXvsSimX", "CSCHit x_{rec} vs x_{sim}", 200, -40., 40., 200, -40., 40.);
// Local Y position
    hRecPositionY    = new TH1F(N+"_hRecPositionY", "CSCRecHit y local", 200, -40., 40.);
    hSimPositionY    = new TH1F(N+"_hSimPositionY", "CSCSimHit y local", 200, -40., 40.);
    hResPositionY    = new TH1F(N+"_hResPositionY", "CSCHit Dy", 101, -2.02, 2.02);  
    hPullY           = new TH1F(N+"_hPullY", "CSCHit Y residual", 101, -5.05, 5.05);
    hResPositionYvsY = new TH2F(N+"_hResPositionYvsY", "CSCHit Dy vs y", 120, -60., 60., 101, -2.02, 2.02);
    hRecYvsSimY      = new TH2F(N+"_hRecYvsSimY", "CSCRecHit y_{rec} vs y_{sim}", 200, -60., 60., 200, -60., 60.);
// Phi
    hResphi          = new TH1F(N+"_hResphi", "Delta phi", 101, -0.505, 0.505);
    hPullphi         = new TH1F(N+"_hPullphi", "Pull phi", 101, -5.05, 5.05);
    hResphib         = new TH1F(N+"_hResphib", "Delta phi", 101, -0.505, 0.505);
    hResphit         = new TH1F(N+"_hResphit", "Delta phi", 101, -0.505, 0.505);
    hrDphi           = new TH1F(N+"_hrDphi", "r x Dphi", 101, -0.101, 0.101);
    hrDphib          = new TH1F(N+"_hrDphib", "r x Dphi", 101, -0.101, 0.101);
    hrDphit          = new TH1F(N+"_hrDphit", "r x Dphi", 101, -0.101, 0.101);
    hPullrDphi       = new TH1F(N+"_hPullrDphi", "Pull r x Dphi", 101, -5.05, 5.05);
// Eta
    hDeta            = new TH1F(N+"_hDeta", "CSC Deta ", 101, -0.0101, 0.0101);
  }

  /// Constructor from collection name and TFile.
  H2DRecHit(TString name_, TFile* file) {
    name=name_;

// Matching sim/reco pair ?
    hHaveMatch        = (TH1F *) file->Get(name+"_HaveMatch");
// X
    hRecPositionX     = (TH1F *) file->Get(name+"_RecPositionX");
    hSimPositionX     = (TH1F *) file->Get(name+"_SimPositionX");
    hResPositionX     = (TH1F *) file->Get(name+"_ResPositionX");
    hPullX            = (TH1F *) file->Get(name+"_PullX");
    hResPositionXvsX  = (TH2F *) file->Get(name+"_ResPositionXvsX");
    hRecPositionYvsX  = (TH2F *) file->Get(name+"_RecPositionYvsX");
    hSimPositionYvsX  = (TH2F *) file->Get(name+"_SimPositionYvsX");
    hRecXvsSimX       = (TH2F *) file->Get(name+"_RecXvsSimX");
// Y
    hRecPositionY     = (TH1F *) file->Get(name+"_RecPositionY");
    hSimPositionY     = (TH1F *) file->Get(name+"_SimPositionY");
    hResPositionY     = (TH1F *) file->Get(name+"_ResPositionY");
    hPullY            = (TH1F *) file->Get(name+"_PullY");
    hResPositionYvsY  = (TH2F *) file->Get(name+"_ResPositionYvsY");
    hRecYvsSimY       = (TH2F *) file->Get(name+"_RecYvsSimY");  
// Phi
    hResphi           = (TH1F *) file->Get(name+"_Resphi");
    hPullphi          = (TH1F *) file->Get(name+"_Pullphi");
    hResphib          = (TH1F *) file->Get(name+"_Resphib");
    hResphit          = (TH1F *) file->Get(name+"_Resphit");
    hrDphi            = (TH1F *) file->Get(name+"_rDphi");
    hrDphib            = (TH1F *) file->Get(name+"_rDphib");
    hrDphit            = (TH1F *) file->Get(name+"_rDphit");
    hPullrDphi        = (TH1F *) file->Get(name+"_PullrDphi");
// Eta
    hDeta             = (TH1F *) file->Get(name+"_Deta");

  }


  /// Destructor
  virtual ~H2DRecHit() {

// Have match
    delete hHaveMatch;
// X
    delete hRecPositionX;
    delete hSimPositionX;
    delete hResPositionX;
    delete hPullX;
    delete hResPositionXvsX;
    delete hRecPositionYvsX;
    delete hSimPositionYvsX;
    delete hRecXvsSimX;
// Y
    delete hRecPositionY;
    delete hSimPositionY;
    delete hResPositionY;
    delete hPullY;
    delete hResPositionYvsY;
    delete hRecYvsSimY;
// Phi
    delete hResphi;
    delete hPullphi;
    delete hResphib;
    delete hResphit;
    delete hrDphi;    
    delete hPullrDphi;    
    delete hrDphib;    
    delete hrDphit;    
// Eta
    delete hDeta;

  }

  // Operations
  void FillHaveMatch(int match) {hHaveMatch->Fill(match);}

  /// Fill all the histos
  void Fill(float recx, float recy, float simx, float simy, float recphi, float simphi, float rdphi, float receta, 
            float simeta, float deta, float r, float apothem, float sigma_xreco, float sigma_yreco, float sigma_phireco) {
	
// X
    hRecPositionX->Fill(recx);
    hSimPositionX->Fill(simx);
    hResPositionX->Fill(recx-simx);
    if (sigma_xreco == 0) sigma_xreco = 0.0001;
    hPullX->Fill((recx-simx)/sigma_xreco);
    hResPositionXvsX->Fill(simx,recx-simx);
    hRecPositionYvsX->Fill(recx,recy);
    hSimPositionYvsX->Fill(simx,simy);
    hRecXvsSimX->Fill(simx,recx);
// Y  
    hRecPositionY->Fill(recy);
    hSimPositionY->Fill(simy);
    hResPositionY->Fill(recy-simy);
    if (sigma_yreco == 0) sigma_yreco = 0.0001;
    hPullY->Fill((recy-simy)/sigma_yreco);
    hResPositionYvsY->Fill(simy,recy-simy);
    hRecYvsSimY->Fill(simy,recy);
// Phi
    float resol = recphi-simphi;
    hResphi->Fill(resol*1000.);    // in mrads
    if (sigma_phireco == 0) sigma_phireco = 0.0001;
    hPullphi->Fill(resol/sigma_phireco);
    if (simy < 0. && (-1.*simy/apothem) > 0.6) {
      hResphib->Fill(resol*1000.);
    } else if  (simy > 0. && (simy/apothem) > 0.6) {
      hResphit->Fill(resol*1000.);
    }
    hrDphi->Fill(rdphi);
    hPullphi->Fill(rdphi/sigma_phireco/r);
    if (simy < 0. && (-1.*simy/apothem) > 0.6) {
      hrDphib->Fill(rdphi);
    } else if  (simy > 0. && (simy/apothem) > 0.6) {
      hrDphit->Fill(rdphi);
    }
// Eta
    hDeta->Fill(receta-simeta);
  }

  /// Write all the histos to currently opened file
  void Write() {

// Have Match
    hHaveMatch->Write();
// X
    hRecPositionX->Write();
    hSimPositionX->Write();
    hResPositionX->Write();
    hPullX->Write();
    hResPositionXvsX->Write();
    hRecPositionYvsX->Write();
    hSimPositionYvsX->Write();
    hRecXvsSimX->Write();
// Y
    hRecPositionY->Write();
    hSimPositionY->Write();
    hResPositionY->Write();
    hPullY->Write();
    hResPositionYvsY->Write();
    hRecYvsSimY->Write();
// Phi
    hResphi->Write();
    hPullphi->Write();
    hResphib->Write();
    hResphit->Write();
    hrDphi->Write();
    hPullrDphi->Write();
    hrDphib->Write();
    hrDphit->Write();
// Eta
    hDeta->Write();
  }


// Have Match
    TH1F *hHaveMatch;
// X
    TH1F *hRecPositionX;
    TH1F *hSimPositionX;
    TH1F *hResPositionX;
    TH1F *hPullX;
    TH2F *hResPositionXvsX;
    TH2F *hRecPositionYvsX;
    TH2F *hSimPositionYvsX;
    TH2F *hRecXvsSimX;
// Y
    TH1F *hRecPositionY;
    TH1F *hSimPositionY;
    TH1F *hResPositionY;
    TH1F *hPullY;
    TH2F *hResPositionYvsY;
    TH2F *hRecYvsSimY;
// Phi
    TH1F *hResphi;
    TH1F *hPullphi;
    TH1F *hResphib;
    TH1F *hResphit;
    TH1F *hrDphi;
    TH1F *hPullrDphi;
    TH1F *hrDphib;
    TH1F *hrDphit;
// Eta
    TH1F *hDeta;

  TString name;
};
#endif


