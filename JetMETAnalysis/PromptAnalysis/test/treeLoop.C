#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TBranch.h"
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include "TChain.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TChain.h"

using namespace std;

void analyze(const char *inname){
  
  TFile *infile = new TFile(inname);
  TTree *intree = (TTree*)infile->Get("promptanaTree/tree");
  
  cout<<"Total number of events: "<<(Int_t)intree->GetEntries()<<endl;
  
  std::auto_ptr<std::vector<double> >  _calometpt                 ( new std::vector<double>()  ) ; 
  std::auto_ptr<std::vector<double> >  _calometphi                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometsumet              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometpx                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometpy                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometMETInmHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometMETInpHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometMETPhiInmHF        ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometMETPhiInpHF        ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometSETInmHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometSETInpHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometetFractionEm       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometetFractionHad      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometmaxEtInEmTowers    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometmaxEtInHadTowers   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometemEtInEB           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometemEtInEE           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calometemEtInHF           ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calomethadEtInHB          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calomethadEtInHE          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calomethadEtInHF          ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  _calomethadEtInHO          ( new std::vector<double>()  ) ;

  intree->SetBranchAddress("calometPt",&_calometpt);
  intree->SetBranchAddress("calometPhi",&_calometphi);
  intree->SetBranchAddress("calometSumEt",&_calometsumet);
  intree->SetBranchAddress("calometPx",&_calometpx);
  intree->SetBranchAddress("calometPy",&_calometpy);
  intree->SetBranchAddress("calometMETInmHF",&_calometMETInmHF);
  intree->SetBranchAddress("calometMETInpHF",&_calometMETInpHF);
  intree->SetBranchAddress("calometMETPhiInmHF",&_calometMETPhiInmHF);
  intree->SetBranchAddress("calometMETPhiInpHF",&_calometMETPhiInpHF);
  intree->SetBranchAddress("calometSETInmHF",&_calometSETInmHF);
  intree->SetBranchAddress("calometSETInpHF",&_calometSETInpHF);
  intree->SetBranchAddress("calometEmEtFraction",&_calometetFractionEm);
  intree->SetBranchAddress("calometEtFractionHadronic",&_calometetFractionHad);
  intree->SetBranchAddress("calometMaxEtInEmTowers",&_calometmaxEtInEmTowers);
  intree->SetBranchAddress("calometMaxEtInHadTowers",&_calometmaxEtInHadTowers);
  intree->SetBranchAddress("calometEmEtInEB",&_calometemEtInEB);
  intree->SetBranchAddress("calometEmEtInEE",&_calometemEtInEE);
  intree->SetBranchAddress("calometEmEtInHF",&_calometemEtInHF);
  intree->SetBranchAddress("calometHadEtInHB",&_calomethadEtInHB);
  intree->SetBranchAddress("calometHadEtInHE",&_calomethadEtInHE);
  intree->SetBranchAddress("calometHadEtInHF",&_calomethadEtInHF);
  intree->SetBranchAddress("calometHadEtInHO",&_calomethadEtInHO);

   for (Int_t i=0;i<(Int_t)intree->GetEntries(); i++)
    {
      if (i%100000 == 0) printf ("Entry: %d\n", i);

      intree->GetEntry(i);

      for (int i = 0; i<int(_calometpt->size()); i++) 
	{
	  cout<<_calometpt->at(i)<<endl;
	  cout<<_calometemEtInEB->at(i)<<endl;
	}
    }
}
