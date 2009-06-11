#include "TauAnalysis/BgEstimationTools/plugins/ObjValCorrelationAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TMatrixD.h>
#include <TVectorD.h>
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

void initCovMatrix(TVectorD& mean, TMatrixD& cov, const std::vector<float>& var, unsigned numVar)
{
  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    mean(iVar) = var[iVar];
  }
  
  for ( unsigned iRow = 0; iRow < numVar; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numVar; ++iColumn ) {
      cov(iRow, iColumn) = 0.;
    }
  }
}

void updateCovMatrix(TVectorD& mean, TMatrixD& cov, int i, const std::vector<float>& var, unsigned numVar)
{
  double weight = i/(i + 1.);

  TVectorD delta(numVar);
  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    delta(iVar) = var[iVar] - mean(iVar);
  }

  for ( unsigned iRow = 0; iRow < numVar; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numVar; ++iColumn ) {
      cov(iRow, iColumn) += weight*delta(iRow)*delta(iColumn);
    }
  }

  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    mean(iVar) += delta(iVar)/(i + 1.);
  }
}

void normalizeCovMatrix(TVectorD& mean, TVectorD& sigma, TMatrixD& cov, unsigned numVar, int numEntries)
{
//--- normalize covariance matrix
//
//    NOTE: mean is already normalized
//          to number of entries
//
  for ( unsigned iRow = 0; iRow < numVar; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numVar; ++iColumn ) {
      cov(iRow, iColumn) /= numEntries;
    }
  }
  
  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    sigma(iVar) = TMath::Sqrt(cov(iVar, iVar));
  }

  for ( unsigned iRow = 0; iRow < numVar; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numVar; ++iColumn ) {
      cov(iRow, iColumn) /= (sigma(iRow)*sigma(iColumn));
    }
  }
}

void printCovMatrix(std::ostream& outputStream, const std::vector<std::string>& branchNames,
		    TVectorD& mean, TVectorD& sigma, TMatrixD& cov, unsigned numVar)
{
  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    outputStream << branchNames[iVar] << ": mean = " << mean(iVar) << ", sigma = " << sigma(iVar) << std::endl;
  }

  const unsigned widthColumn = 20;

  outputStream << "Covariance Matrix:" << std::endl;
  outputStream << std::setw(widthColumn) << std::left << " ";
  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    outputStream << std::setw(widthColumn) << std::left << branchNames[iVar];
  }
  outputStream << std::endl;
  for ( unsigned iCharacter = 0; iCharacter < (widthColumn*(numVar + 1)); ++iCharacter ) {
    outputStream << "-";
  }
  outputStream << std::endl;
  for ( unsigned iRow = 0; iRow < numVar; ++iRow ) {
    outputStream << std::setw(widthColumn) << std::left << branchNames[iRow];
    for ( unsigned iColumn = 0; iColumn < numVar; ++iColumn ) {
      outputStream << std::setw(4) << std::left << " ";
      outputStream << std::setw(widthColumn - 4) << std::setprecision(3) << std::fixed << std::left << cov(iRow, iColumn);
    }
    outputStream << std::endl;
  }
  for ( unsigned iCharacter = 0; iCharacter < (widthColumn*(numVar + 1)); ++iCharacter ) {
    outputStream << "-";
  }
  outputStream << std::endl;  
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
// 
//    NOTE: algorithm for numerically stabe computation of covariance matrix taken from:
//            Ronald A. Thisted 
//            "Elements of Statistical Computing: Numerical Computation", pp. 84-91
//            Chapman & Hall, 1988 
//          (cited from wikipedia)
//
  unsigned numVar = branchEntries_.size();
  std::vector<float> var(numVar);
  TVectorD mean(numVar);
  TVectorD sigma(numVar);
  TMatrixD cov(numVar, numVar);

  int numEntries = chain.GetEntries();
  for ( int iEntry = 0 ; iEntry < numEntries; ++iEntry ) {
    chain.GetEvent(iEntry);  

    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      var[iVar] = branchEntries_[iVar]->objValue_;
    }

    if ( iEntry == 0 ) {
      initCovMatrix(mean, cov, var, numVar);
    } else {
      updateCovMatrix(mean, cov, iEntry, var, numVar);
    }
  }

//--- normalize mean, sigma and covariance matrix
//    to number of entries processed
  normalizeCovMatrix(mean, sigma, cov, numVar, numEntries);
  
//--- covariance matrix representing (linear) correlation
//    between observables
  std::cout << "<ObjValCorrelationAnalyzer::printCovMatrix>:" << std::endl;
  std::cout << " process = " << processName_ << std::endl;
  std::cout << " number of events processed = " << numEntries << std::endl;
  printCovMatrix(std::cout, branchNames_, mean, sigma, cov, numVar);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ObjValCorrelationAnalyzer);
