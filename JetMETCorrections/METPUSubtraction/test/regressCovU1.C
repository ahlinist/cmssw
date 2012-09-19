#include "TFile.h"
#include "TTree.h"
#include "GBRTrainer.h"
#include "GBRForest.h"
#include "Cintex/Cintex.h"


void regressCovU1(std::string iName) {
  TString  lName       = iName; TFile *lInput = TFile::Open(lName); 
  //TString  lName       = "../Data/r11-dimu_jetv3_v29b.root"; TFile *lInput = TFile::Open(lName); 
  //TString  lName       = "../Data/s12-zjets_v29b.root"; TFile *lInput = TFile::Open(lName); 
  //TString  lName       = "../Data/f11-zjets_jetv4_v11.root"; TFile *lInput = TFile::Open(lName); 
  TTree   *lRegress    = (TTree*)lInput    ->Get("Flat");
  
  GBRTrainer *train = new GBRTrainer;
  train->AddTree(lRegress);
  train->SetTrainingCut("(nbtag == 0)*(pt_z > 0)");//*TMath::Max(pt_z,100)*(pt_z > 2)");
  train->SetMinEvents(250);
  train->SetShrinkage(0.3);
  train->SetMinCutSignificance(3.0);  
  
  train->SetTargetVar( "abs(pt_z*cos(phi_z-uphix_mva)-ux_mva)/ux_mva*sqrt(TMath::Pi()/2.)" );
  
  std::vector<std::string> *lVec = new std::vector<std::string>;
  lVec->push_back( "sumet"      );
  lVec->push_back( "npv"        );
  lVec->push_back( "pfu"        );
  lVec->push_back( "pfuphi"     );
  lVec->push_back( "tksumet"    );
  lVec->push_back( "tku"        );
  lVec->push_back( "tkuphi"     );
  lVec->push_back( "nopusumet"  );
  lVec->push_back( "nopuu"      );
  lVec->push_back( "nopuuphi"   );
  lVec->push_back( "pusumet"    );
  lVec->push_back( "pumet"      );
  lVec->push_back( "pumetphi"   );
  lVec->push_back( "pucsumet"   );
  lVec->push_back( "pucu"       );
  lVec->push_back( "pucuphi"    );
  lVec->push_back( "jetpt1"     );
  lVec->push_back( "jeteta1"    );
  lVec->push_back( "jetphi1"    );
  lVec->push_back( "jetpt2"     );
  lVec->push_back( "jeteta2"    );
  lVec->push_back( "jetphi2"    );
  lVec->push_back( "nalljet"    );
  lVec->push_back( "njet"       );
  lVec->push_back( "uphix_mva"  );
  lVec->push_back( "ux_mva"    );

  for (int i=0; i<int(lVec->size()); ++i) {
     train->AddInputVar(lVec->at(i));
  }
  
  ROOT::Cintex::Cintex::Enable();   
  const GBRForest *forest = train->TrainForest(300);
  
  TFile *fout = new TFile("gbru1cov.root","RECREATE");    
  fout->WriteObject(forest,"CovU1");
  fout->WriteObject(lVec, "varlist");
  fout->Close();
}
