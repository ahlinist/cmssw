#ifndef RecoLocalMuon_CSCRecHitHistograms_H
#define RecoLocalMuon_CSCRecHitHistograms_H

/** \class CSCRecHitHistograms
 *  Collection of histograms for 2D CSCRecHit test.
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
    hRecPositionYvsX = new TH2F(N+"_hRecPositionYvsX", "CSCRecHit y vs x", 600, -60., 60., 600, -60., 60.);
    hSimPositionYvsX = new TH2F(N+"_hSimPositionYvsX", "CSCSimHit y vs x", 600, -60., 60., 600, -60., 60.);
    hMisPositionYvsX = new TH2F(N+"_hMisPositionYvsX", "Un-matchedSimHit y vs x", 100, -50., 50., 280, -70., 70.);
// Local Y position
    hRecPositionY    = new TH1F(N+"_hRecPositionY", "CSCRecHit y local", 200, -40., 40.);
    hSimPositionY    = new TH1F(N+"_hSimPositionY", "CSCSimHit y local", 200, -40., 40.);
    hResPositionY    = new TH1F(N+"_hResPositionY", "CSCHit Dy", 101, -2.02, 2.02);  
    hPullY           = new TH1F(N+"_hPullY", "CSCHit Y residual", 101, -5.05, 5.05);
// Phi
    hResphi          = new TH1F(N+"_hResphi", "Delta phi", 101, -0.505, 0.505);
    hPullDphi        = new TH1F(N+"_hPullDphi", "Pull Dphi", 101, -5.05, 5.05);
    hrDphi           = new TH1F(N+"_hrDphi", "r x Dphi", 101, -0.101, 0.101);
    hPullrDphi       = new TH1F(N+"_hPullrDphi", "Pull r x Dphi", 101, -5.05, 5.05);
    hrDphiSW         = new TH1F(N+"_hrDphiSW", "r x Dphi/stripWidth", 101, -0.101, 0.101);
// chi^2 study
    hChi2All         = new TH1F(N+"_hChi2All", "#chi^{2}", 200, 0., 1000.);    
    hChi2Core        = new TH1F(N+"_hChi2Core", "#chi^{2}", 200, 0., 1000.);    
    hChi2Out         = new TH1F(N+"_hChi2Out", "#chi^{2}", 200, 0., 1000.);    
    hPullvsChi2      = new TH2F(N+"_hPullvsChi2", "#Delta r #phi / #sigma vs #chi^{2}", 50, 0., 1000., 51, -10.2, 10.2);    
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
    hRecPositionYvsX  = (TH2F *) file->Get(name+"_RecPositionYvsX");
    hSimPositionYvsX  = (TH2F *) file->Get(name+"_SimPositionYvsX");
    hMisPositionYvsX  = (TH2F *) file->Get(name+"_MisPositionYvsX");
// Y
    hRecPositionY     = (TH1F *) file->Get(name+"_RecPositionY");
    hSimPositionY     = (TH1F *) file->Get(name+"_SimPositionY");
    hResPositionY     = (TH1F *) file->Get(name+"_ResPositionY");
    hPullY            = (TH1F *) file->Get(name+"_PullY");
// Phi
    hResphi           = (TH1F *) file->Get(name+"_Resphi");
    hPullDphi         = (TH1F *) file->Get(name+"_PullDphi");
    hrDphi            = (TH1F *) file->Get(name+"_rDphi");
    hPullrDphi        = (TH1F *) file->Get(name+"_PullrDphi");
    hrDphiSW          = (TH1F *) file->Get(name+"_rDphiSW");

// chi^2 study 
    hChi2All         = (TH1F *) file->Get(name+"_hChi2All");
    hChi2Core        = (TH1F *) file->Get(name+"_hChi2Core");
    hChi2Out         = (TH1F *) file->Get(name+"_hChi2Out");
    hPullvsChi2      = (TH2F *) file->Get(name+"_hPullvsChi2");

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
    delete hRecPositionYvsX;
    delete hSimPositionYvsX;
    delete hMisPositionYvsX;
// Y
    delete hRecPositionY;
    delete hSimPositionY;
    delete hResPositionY;
    delete hPullY;
// Phi
    delete hResphi;
    delete hPullDphi;    
    delete hrDphi;    
    delete hPullrDphi;    
    delete hrDphiSW;
// chi^2 study
    delete hChi2All;
    delete hChi2Core;
    delete hChi2Out;
    delete hPullvsChi2;
  }

  // Operations
  void FillHaveMatch(int match) {hHaveMatch->Fill(match);}

  // Operations
  void FillNoMatch(float simx, float simy ) {hMisPositionYvsX->Fill(simx,simy);}

  /// Fill all the histos
  void Fill(float recx, float recy, float simx, float simy, float recphi, float simphi,
            float r, float sigma_xreco, float sigma_yreco, float stripWidth, double dstrip, double sPhiPitch, double chi2) {
	
    float dphi = recphi - simphi;
    float dx = recx - simx;
    float dy = recy - simy;
    if ( dy < 0. ) dy = -dy;
    if ( dy == 0. ) dy = 0.00001;     
    if ( dx < 0. ) dx = -dx;
    if ( dx == 0. ) dx = 0.00001;
    float sigma_rdphi = sPhiPitch*sigma_xreco/(stripWidth*dphi) * sPhiPitch*dstrip/(stripWidth*dphi);
    sigma_rdphi += sigma_yreco/r * sigma_yreco/r;
    sigma_rdphi = r * dphi * sqrt(sigma_rdphi);
  
    float sigma_dphi =  sPhiPitch*dstrip/stripWidth;
 
    if (sigma_rdphi < 0.) sigma_rdphi =  -sigma_rdphi;
    float rdphipull = r * dphi/sigma_rdphi;

// X
    hRecPositionX->Fill(recx);
    hSimPositionX->Fill(simx);
    hResPositionX->Fill(recx-simx);
    if (sigma_xreco == 0) sigma_xreco = 0.001;
    hPullX->Fill((recx-simx)/sigma_xreco);
    hRecPositionYvsX->Fill(recx,recy);
    hSimPositionYvsX->Fill(simx,simy);
// Y  
    hRecPositionY->Fill(recy);
    hSimPositionY->Fill(simy);
    hResPositionY->Fill(recy-simy);
    if (sigma_yreco == 0) sigma_yreco = 0.001;
    hPullY->Fill((recy-simy)/sigma_yreco);
// Phi
    float resol = recphi-simphi;
    hResphi->Fill(resol*1000.);    // in mrads
    hPullDphi->Fill(resol/sigma_dphi);
    hrDphi->Fill(r * dphi);
    hPullrDphi->Fill(rdphipull);
    hrDphiSW->Fill(r*dphi/stripWidth);
// chi^2 study
    hChi2All->Fill(chi2); 
    if (fabs(rdphipull) < 2.) hChi2Core->Fill(chi2);  
    if (fabs(rdphipull) > 3.) hChi2Out->Fill(chi2);
    hPullvsChi2->Fill(chi2,rdphipull);

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
    hRecPositionYvsX->Write();
    hSimPositionYvsX->Write();
    hMisPositionYvsX->Write();
// Y
    hRecPositionY->Write();
    hSimPositionY->Write();
    hResPositionY->Write();
    hPullY->Write();
// Phi
    hResphi->Write();
    hPullDphi->Write();
    hrDphi->Write();
    hPullrDphi->Write();
    hrDphiSW->Write();
// chi^2 study
    hChi2All->Write();
    hChi2Core->Write();
    hChi2Out->Write();
    hPullvsChi2->Write();

  }


// Have Match
    TH1F *hHaveMatch;
// X
    TH1F *hRecPositionX;
    TH1F *hSimPositionX;
    TH1F *hResPositionX;
    TH1F *hPullX;
    TH2F *hRecPositionYvsX;
    TH2F *hSimPositionYvsX;
    TH2F *hMisPositionYvsX;
// Y
    TH1F *hRecPositionY;
    TH1F *hSimPositionY;
    TH1F *hResPositionY;
    TH1F *hPullY;
// Phi
    TH1F *hResphi;
    TH1F *hPullDphi;
    TH1F *hrDphi;
    TH1F *hPullrDphi;
    TH1F *hrDphiSW;
// chi^2 study
    TH1F *hChi2All;
    TH1F *hChi2Core;
    TH1F *hChi2Out;
    TH2F *hPullvsChi2;

    TString name;
};
#endif


