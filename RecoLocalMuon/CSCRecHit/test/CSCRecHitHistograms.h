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
    
    hRecPositionX = new TH1F(N+"_hRecPositionX", "CSCRecHit x position (cm)", 48, -15., 15.);
    hRecPositionY = new TH1F(N+"_hRecPositionY", "CSCRecHit y position (cm)", 180, -180., 180.);
    hRecPositionYvsX = new TH2F(N+"_hRecPositionYvsX", "CSCRecHit y vs x (cm)", 600, -60., 60., 
				                                                600, -60., 60.);

    hRecYvsSimY = new TH2F(N+"_hRecYvsSimY", "CSCRecHit y_{rec} vs y_{sim}", 150, -150., 150.,
			                                                     150, -150., 150.);


    hSimPositionX = new TH1F(N+"_hSimPositionX", "CSCSimHit x position (cm)", 48, -24., 24.);
    hSimPositionY = new TH1F(N+"_hSimPositionY", "CSCSimHit y position (cm)", 180, -180., 180.);
    hSimPositionYvsX = new TH2F(N+"_hSimPositionYvsX", "CSCSimHit y vs x (cm)", 600, -60., 60., 
				                                                600, -60., 60.);


    hResPositionX = new TH1F(N+"_hResPositionX", "CSC hit resolution x (cm)", 101, -0.202, 0.202);
    hResPositionY = new TH1F(N+"_hResPositionY", "CSC hit resolution y (cm)", 101, -0.7575, 0.7575);
    hResPositionYvsY = new TH2F(N+"_hResPositionYvsY", 
				"CSC hit resolution y vs y (cm)", 180, -180., 180., 
                                                                   81, -1.0125, 1.0125);

    hphi = new TH1F(N+"_hphi", "CSCRecHit phi (rads)", 130, -3.25, 3.25);
    hrDphi = new TH1F(N+"_hrDphi", "CSC r x Delta phi (cm)", 101, -0.101, 0.101);

    heta = new TH1F(N+"_heta", "CSCRecHit eta ", 150, 0.9, 2.4);
    hdeta = new TH1F(N+"_hdeta", "CSC Delta eta ", 101, -0.0101, 0.0101);
    hdetavseta = new TH2F(N+"_hdetavseta", "CSC Delta eta vs eta", 150, 0.9, 2.4, 
			                                           101, -0.0101, 0.0101);


    hRecphi = new TH1F(N+"_hRecphi", "Rec phi ", 180, -180., 180.);
    hSimphi = new TH1F(N+"_hSimphi", "Sim phi", 180, -180., 180.);
    hRecphivsSimphi = new TH2F(N+"_hRecphivsSimphi", "Rec phi vs Sim phi", 180, -180, 180, 
			                                                   101, -0.2525, 0.2525);

  }

  /// Constructor from collection name and TFile.
  /// It retrieves all the histos of the set from the file.
  H2DRecHit(TString name_, TFile* file) {
    name=name_;
    hRecPositionX     = (TH1F *) file->Get(name+"_RecPositionX");
    hRecPositionY     = (TH1F *) file->Get(name+"_RecPositionY");
    hRecPositionYvsX  = (TH2F *) file->Get(name+"_RecPositionYvsX");

    hSimPositionX     = (TH1F *) file->Get(name+"_SimPositionX");
    hSimPositionY     = (TH1F *) file->Get(name+"_SimPositionY");
    hSimPositionYvsX  = (TH2F *) file->Get(name+"_SimPositionYvsX");

    hResPositionX     = (TH1F *) file->Get(name+"_ResPositionX");
    hResPositionY     = (TH1F *) file->Get(name+"_ResPositionY");
    hResPositionYvsY  = (TH2F *) file->Get(name+"_ResPositionYvsY");
 
    hphi              = (TH1F *) file->Get(name+"_phi");
    hrDphi            = (TH1F *) file->Get(name+"_rDphi");
    heta              = (TH1F *) file->Get(name+"_eta");
    hdeta             = (TH1F *) file->Get(name+"_deta");
    hdetavseta        = (TH2F *) file->Get(name+"_detavseta");
    hRecYvsSimY       = (TH2F *) file->Get(name+"_RecYvsSimY");

    hRecphi           = (TH1F *) file->Get(name+"_Recphi");
    hSimphi           = (TH1F *) file->Get(name+"_Simphi");
    hRecphivsSimphi   = (TH2F *) file->Get(name+"_RecphivsSimphi");

  }

  /// Destructor
  virtual ~H2DRecHit() {
    delete hRecPositionX;
    delete hRecPositionY;
    delete hRecPositionYvsX;

    delete hSimPositionX;
    delete hSimPositionY;
    delete hSimPositionYvsX;

    delete hResPositionX;
    delete hResPositionY;
    delete hResPositionYvsY;

    delete hphi;
    delete hrDphi;
    delete heta;
    delete hdeta;
    delete hdetavseta;
    delete hRecYvsSimY;

    delete hRecphi;  
    delete hSimphi; 
    delete hRecphivsSimphi; 

  }

  // Operations
  /// Fill all the histos
  void Fill(float recx, float recy, float simx, float simy, float recphi, float simphi, float rdphi, float receta, float simeta, float deta) {

	
    float PIfactor = 180./3.1415927;

    hRecPositionX->Fill(recx);
    hRecPositionY->Fill(recy);
    hRecPositionYvsX->Fill(recx,recy);

    hSimPositionX->Fill(simx);
    hSimPositionY->Fill(simy);
    hSimPositionYvsX->Fill(simx,simy);

    hResPositionX->Fill(recx-simx);
    hResPositionY->Fill(recy-simy);
    hResPositionYvsY->Fill(simy,simy-recy);

    hphi->Fill(recphi);
    hrDphi->Fill(rdphi);
    heta->Fill(receta);
    hdeta->Fill(deta);
    hdetavseta->Fill(simeta,deta);
    hRecYvsSimY->Fill(simy,recy);  

    hRecphi->Fill(recphi*PIfactor);  
    hSimphi->Fill(simphi*PIfactor); 
    hRecphivsSimphi->Fill(simphi*PIfactor,(recphi-simphi)*PIfactor);
  }

  /// Write all the histos to currently opened file
  void Write() {
    hRecPositionX->Write();
    hRecPositionY->Write();
    hRecPositionYvsX->Write();

    hSimPositionX->Write();
    hSimPositionY->Write();
    hSimPositionYvsX->Write();

    hResPositionX->Write();
    hResPositionY->Write();
    hResPositionYvsY->Write();

    hphi->Write();
    heta->Write();
    hrDphi->Write();
    hdeta->Write();
    hdetavseta->Write();
    hRecYvsSimY->Write();

    hRecphi->Write();
    hSimphi->Write();
    hRecphivsSimphi->Write();
  }
  
  TH1F *hRecPositionX;
  TH1F *hRecPositionY;
  TH2F *hRecPositionYvsX;

  TH1F *hSimPositionX;
  TH1F *hSimPositionY;
  TH2F *hSimPositionYvsX;

  TH1F *hResPositionX;
  TH1F *hResPositionY;
  TH2F *hResPositionYvsY;

  TH1F *hrDphi;
  TH1F *heta;
  TH1F *hphi;
  TH1F *hdeta;
  TH2F *hdetavseta;
  TH2F *hRecYvsSimY;

  TH1F *hRecphi;
  TH1F *hSimphi;
  TH2F *hRecphivsSimphi;

  TString name;
};
#endif


