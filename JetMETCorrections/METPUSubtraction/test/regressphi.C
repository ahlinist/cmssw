#include "TFile.h"
#include "TTree.h"
#include "GBRTrainer.h"
#include "GBRForest.h"
#include "Cintex/Cintex.h"


void regressphi() {
  TString  lName       = "../Ntupler/Data_v2/r12-dimu_53X_type1_5XPU.root"; TFile *lInput = TFile::Open(lName); 
  TTree   *lRegress    = (TTree*)lInput    ->Get("Flat");

  GBRTrainer *train = new GBRTrainer;
  train->AddTree(lRegress);
  train->SetTrainingCut("(1 == 1)*(nbtag==0)");//TMath::Min(rpt_z,50)*(nbtag == 0)");
  train->SetMinEvents(250);
  train->SetShrinkage(0.05);
  train->SetMinCutSignificance(2.0);  
  
  train->SetTargetVar( "phi_z-pfuphi+ 2.*TMath::Pi()*(phi_z-pfuphi < -TMath::Pi()) - 2.*TMath::Pi()*(phi_z-pfuphi > TMath::Pi())  " ); 
  //train->SetTargetVar( "phi_z-nopuuphi+ 2.*TMath::Pi()*(phi_z-nopuuphi < -TMath::Pi()) - 2.*TMath::Pi()*(phi_z-nopuuphi > TMath::Pi())  " ); 

  std::vector<std::string> *lVec = new std::vector<std::string>;
  //lVec->push_back( "sumet"      );
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

  for (int i=0; i<int(lVec->size()); ++i) {
     train->AddInputVar(lVec->at(i));
  }
  
  ROOT::Cintex::Cintex::Enable();   
  const GBRForest *forest = train->TrainForest(100);
  
  TFile *fout = new TFile("gbrmetphi_52_data.root","RECREATE");    
  fout->WriteObject(forest,"PhiCorrection");
  fout->WriteObject(lVec, "varlist");
  fout->Close();
}
