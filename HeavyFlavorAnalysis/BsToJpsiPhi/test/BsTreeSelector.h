#ifndef BSTREESELECTOR_H
#define BSTREESELECTOR_H


#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "VariableList.h"
#include "../interface/BsToJpsiPhiRootTree.h"

//**************************************************************************
// implementation of the selector for writing out selected trees for the fit

// definition of  tree to write out
class BstoJpsiPhiSelectedTree{
public:
  BstoJpsiPhiSelectedTree( BsToJpsiPhiRootTree * origTree, TString name, const TString filename) : myName(name),myFilename(filename) {
    tree = new TTree("BsTree", "BsTree");
    tree->Branch(  "BsCt",          &origTree->BsCt_,         "BsCt/D");   
    tree->Branch(  "BsDist3d"     , &origTree->BsDist3d_,     "BsDist3d/D"    );
    tree->Branch(  "BsDist3dErr"  , &origTree->BsDist3dErr_,  "BsDist3dErr/D" );
    tree->Branch(  "BsDist2d"     , &origTree->BsDist2d_,     "BsDist2d/D"    );
    tree->Branch(  "BsDist2dErr"  , &origTree->BsDist2dErr_,  "BsDist2dErr/D" );
    tree->Branch(  "BsTime3d"     , &origTree->BsTime3d_,     "BsTime3d/D"    );
    tree->Branch(  "BsTime3dErr"  , &origTree->BsTime3dErr_,  "BsTime3dErr/D" );
    tree->Branch(  "BsTime2d"     , &origTree->BsTime2d_,     "BsTime2d/D"    );
    tree->Branch(  "BsTime2dErr"  , &origTree->BsTime2dErr_,  "BsTime2dErr/D" );
    tree->Branch(  "costheta"     , &origTree->costheta_,     "costheta/D"    );
    tree->Branch(  "phi"          , &origTree->phi_,          "phi/D"         );
    tree->Branch(  "cospsi"       , &origTree->cospsi_,       "cospsi/D"      );
    tree->Branch(  "BsFitM"       , &origTree->BsFitM_,       "BsFitM/D"      );
  }
  ~BstoJpsiPhiSelectedTree(){write();}
  
  void write(){
    myFilename += "_";  myFilename += myName;  myFilename += ".root";
    
    TFile *file = new TFile(myFilename, "RECREATE");
    tree->Write();
    file->Write();
    file->Close();
  }
  
  TString myName;
  TString myFilename;
  TTree *tree;
};

/////////////////////////////////////////
/// definition of the selecting functions
class SelectorForFit {

public:
  SelectorForFit(BsToJpsiPhiRootTree* inputTree, const TString filename) : 
    BsJpsiPhiMuMuKKTree   (inputTree, "BsJpsiPhiMuMuKK", filename),
    BsJPsiKKTree          (inputTree, "BsJPsiKK", filename),
    BdJpsiKstarMuMuKpiTree(inputTree, "BdJpsiKstarMuMuKpi",filename),
    BsOtherTree           (inputTree, "BsOther",filename),
    BdOtherTree           (inputTree, "BdOther",filename),
    OtherTree             (inputTree, "Other",filename),
    BsJpsiEtaTree         (inputTree, "BsJpsiEta",filename),
    BdJpsiK10Tree         (inputTree, "BdJpsiK10",filename),
    BdJpsiK0Tree          (inputTree, "BdJpsiK0",filename),
    BpJpsiKpTree          (inputTree, "BpJpsiKp",filename)    {}
  
  ~SelectorForFit(){}

  void fill(){
    if(isGenBsJpsiPhiMuMuKKEvent_ == 1)           BsJpsiPhiMuMuKKTree.tree->Fill();
    else if(isGenBsJpsiKKEvent_ == 1)             BsJPsiKKTree.tree->Fill();
    else if (isGenBdJpsiKstarMuMuKpiEvent_ == 1) BdJpsiKstarMuMuKpiTree.tree->Fill();
    else if (isGenBsJpsiEtaEvent_ == 1)          BsJpsiEtaTree.tree->Fill();
    else if (isGenBdJpsiK10Event_ == 1)          BdJpsiK10Tree.tree->Fill();
    else if (isGenBdJpsiK0Event_ == 1)           BdJpsiK0Tree.tree->Fill();
    else if (isGenBpJpsiKpEvent_ == 1)           BpJpsiKpTree.tree->Fill();
    else if (isGenBsEvent_ == 1)                 BsOtherTree.tree->Fill();
    else if (isGenBdEvent_ == 1)                 BdOtherTree.tree->Fill();
    else                                         OtherTree.tree->Fill();
  }
  
  BstoJpsiPhiSelectedTree BsJpsiPhiMuMuKKTree   ;
  BstoJpsiPhiSelectedTree BsJPsiKKTree          ;
  BstoJpsiPhiSelectedTree BdJpsiKstarMuMuKpiTree;
  BstoJpsiPhiSelectedTree BsOtherTree           ;
  BstoJpsiPhiSelectedTree BdOtherTree           ;
  BstoJpsiPhiSelectedTree OtherTree             ;
  BstoJpsiPhiSelectedTree BsJpsiEtaTree         ;
  BstoJpsiPhiSelectedTree BdJpsiK10Tree         ;
  BstoJpsiPhiSelectedTree BdJpsiK0Tree          ;
  BstoJpsiPhiSelectedTree BpJpsiKpTree          ;
  
};


#endif
