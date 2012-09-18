#include "TFile.h"
#include "TTree.h"
#include "GBRTrainer.h"
#include "GBRForest.h"
#include "Cintex/Cintex.h"


void regress(TString iName,bool iWeight = false) {
  TString  lName       = iName; TFile *lInput = TFile::Open(lName); 
  //"Train/rw_s12-zjets_53_data_phi.root"; 
  TTree   *lRegress    = (TTree*)lInput    ->Get("Flat");
  std::string lWeight = "(1==1)"; if(iWeight) lWeight += "*PtWeight";
  GBRTrainer *train = new GBRTrainer;
  train->AddTree(lRegress);
  train->SetTrainingCut(lWeight);// && (pt_z < 50 || (pt_z > 50 && u1_mva < 50-pt_z)))");
  train->SetMinEvents(250);
  train->SetShrinkage(0.05);
  train->SetMinCutSignificance(3.0);  
  
  //train->SetTargetVar( "rphi_z-uphi+ 2.*TMath::Pi()*(rphi_z-uphi < -TMath::Pi()) - 2.*TMath::Pi()*(rphi_z-uphi > TMath::Pi())  " ); 
  train->SetTargetVar( "-pt_z/u1_mva" );
  //train->SetTargetXVar("rpt_z*(cos(rphi_z)*cos(uphi)     + sin(rphi_z)*sin(uphi)    )/u");
  //train->SetTargetYVar("rpt_z*(cos(rphi_z)*sin(uphi)     - sin(rphi_z)*cos(uphi)    )/u"); 
  
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
  lVec->push_back( "uphi_mva"   );

  for (int i=0; i<int(lVec->size()); ++i) {
     train->AddInputVar(lVec->at(i));
  }
  
  ROOT::Cintex::Cintex::Enable();   
  const GBRForest *forest = train->TrainForest(1000);
  
  TFile *fout = new TFile("gbrmet_52.root","RECREATE");    
  fout->WriteObject(forest,"U1Correction");
  fout->WriteObject(lVec, "varlist");
  fout->Close();
}
