#include "TauAnalysis/BgEstimationTools/plugins/ObjValCorrelationAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/BgEstimationTools/interface/BgEstCovMatrix.h"

#include <TMath.h>

#include <iostream>
#include <iomanip>

ObjValCorrelationAnalyzer::ObjValCorrelationAnalyzer(const edm::ParameterSet& cfg)
{
  //std::cout << "<ObjValCorrelationAnalyzer::ObjValCorrelationAnalyzer>:" << std::endl;

  processName_ = cfg.getParameter<std::string>("processName");

  fileNames_ = cfg.getParameter<vstring>("fileNames");

  treeName_ = cfg.getParameter<std::string>("treeName");

  branchNames_ = cfg.getParameter<vstring>("branches");

  for ( vstring::const_iterator branchName = branchNames_.begin(); 
	branchName != branchNames_.end(); ++branchName ) {
    branchEntryType* branchEntry = new branchEntryType(*branchName);
    branchEntries_.push_back(branchEntry);
  }
}

ObjValCorrelationAnalyzer::~ObjValCorrelationAnalyzer()
{
  for ( std::vector<branchEntryType*>::iterator it = branchEntries_.begin();
	it != branchEntries_.end(); ++it ) {
    delete (*it);
  }
}

void ObjValCorrelationAnalyzer::beginJob(const edm::EventSetup& es)
{
//--- create chain of all ROOT files 
//    specified in fileNames configuration parameter
  TChain chain(treeName_.data());
  for ( vstring::const_iterator fileName = fileNames_.begin();
	fileName != fileNames_.end(); ++fileName ) {
    chain.Add(fileName->data());
  }

//--- set branch adresses
  for ( std::vector<branchEntryType*>::iterator branchEntry = branchEntries_.begin();
	branchEntry != branchEntries_.end(); ++branchEntry ) {
    chain.SetBranchAddress((*branchEntry)->branchName_.data(), &(*branchEntry)->objValue_);
  }

//--- read all entries from chain;
//    compute correlation of observables stored in different branches
  unsigned numVar = branchEntries_.size();
  TVectorD value(numVar);
  BgEstCovMatrix cov(numVar);

  int numEntries = chain.GetEntries();
  for ( int iEntry = 0 ; iEntry < numEntries; ++iEntry ) {
    chain.GetEvent(iEntry);  

    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      value(iVar) = branchEntries_[iVar]->objValue_;
    }

    cov.update(value);
  }

//--- covariance matrix representing (linear) correlation
//    between observables
  std::cout << "<ObjValCorrelationAnalyzer::printCovMatrix>:" << std::endl;
  std::cout << " process = " << processName_ << std::endl;
  std::cout << " number of events processed = " << numEntries << std::endl;
  cov.print(std::cout, &branchNames_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ObjValCorrelationAnalyzer);
