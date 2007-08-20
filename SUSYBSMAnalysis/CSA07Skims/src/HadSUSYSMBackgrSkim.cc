/* \class HadSUSYSMBackgrSkim
 *
 * all hadronic SUSY Skim
 * >= 2 jets, 1st jet 110 GeV + MET 100 GeV
 * Total SM background (JetMet trigger path)
 *
 * $Date: 2007/07/28 16:07:06 $
 * $Revision: 1.1 $
 *
 * \author Michael Tytgat, Maria Spiropulu - CERN
 *
 */

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <cstdio>
#include <vector>
#include <memory>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"    

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYSMBackgrSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

class PtSorter {
public:
  template <class T> bool operator() ( const T& a, const T& b ) {
    return ( a.pt() > b.pt() );
  }
};

HadSUSYSMBackgrSkim::HadSUSYSMBackgrSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  CaloJetsrc_ = iConfig.getParameter<InputTag>( "CaloJetsrc" );
  NminCaloJet_ = iConfig.getUntrackedParameter<int>( "NminCaloJet", 2 );
  CaloJetPtmin_ = 
    iConfig.getUntrackedParameter<double>( "CaloJetPtmin", 30. );
  CaloJet1Ptmin_ = 
    iConfig.getUntrackedParameter<double>( "CaloJet1Ptmin", 110. );
  CaloMETsrc_ = iConfig.getParameter<InputTag>( "CaloMETsrc" );
  CaloMETmin_ = iConfig.getUntrackedParameter<double>( "CaloMETmin", 100. );
}

/*------------------------------------------------------------------------*/

HadSUSYSMBackgrSkim::~HadSUSYSMBackgrSkim() 
{}

/*------------------------------------------------------------------------*/

bool HadSUSYSMBackgrSkim::filter( edm::Event& iEvent, 
				       const edm::EventSetup& iSetup )
{
  nEvents_++;

  Handle<CaloJetCollection> CaloJetsHandle;

//  try {
    iEvent.getByLabel( CaloJetsrc_, CaloJetsHandle );
//  } 
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYSMBackgrSkim" ) 
//      << "Unable to get CaloJet collection "
//      << CaloJetsrc_.label();
//    return false;
//  }

  if ( CaloJetsHandle->empty() ) return false;

  Handle<CaloMETCollection> CaloMETHandle;

//  try {
    iEvent.getByLabel( CaloMETsrc_, CaloMETHandle );
//  }
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYSMBackgrSkim" )
//      << "Unable to get CaloMET collection "
//      << CaloMETsrc_.label();
//    return false;
//  }

  // MET cut
  if ( (CaloMETHandle->begin())->et() < CaloMETmin_ ) return false;

  // jet cuts
  reco::CaloJetCollection TheJets = *CaloJetsHandle;
  std::stable_sort( TheJets.begin(), TheJets.end(), PtSorter() );

  if ( (TheJets.begin())->pt() < CaloJet1Ptmin_ ) return false; // 1st jet

  int nJet = 0;
  for ( CaloJetCollection::const_iterator it = TheJets.begin(); 
	it != TheJets.end(); it++ ) {
    if ( it->pt() > CaloJetPtmin_ ) nJet++;
  }  
  if ( nJet < NminCaloJet_ ) return false;

  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void HadSUSYSMBackgrSkim::endJob()
{
  edm::LogVerbatim( "HadSUSYSMBackgrSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
