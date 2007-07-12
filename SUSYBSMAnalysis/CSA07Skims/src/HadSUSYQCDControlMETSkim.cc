/* \class HadSUSYQCDControlMETSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel jets 100 GeV, (no MET) QCD control MET trigger turn on, 
 * MET tails (QCD trigger path)
 *
 * $Date:$
 * $Revision:$
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

#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYQCDControlMETSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

HadSUSYQCDControlMETSkim::HadSUSYQCDControlMETSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  CaloJetsrc_ = iConfig.getParameter<InputTag>( "CaloJetsrc" );
  CaloJetPtmin_ = 
    iConfig.getUntrackedParameter<double>( "CaloJetPtmin", 100. );
  NminCaloJet_ = iConfig.getUntrackedParameter<int>( "NminCaloJet", 2 );
}

/*------------------------------------------------------------------------*/

HadSUSYQCDControlMETSkim::~HadSUSYQCDControlMETSkim() 
{}

/*------------------------------------------------------------------------*/

bool HadSUSYQCDControlMETSkim::filter( edm::Event& iEvent, 
				       const edm::EventSetup& iSetup )
{
  nEvents_++;

  Handle<CaloJetCollection> CaloJetsHandle;

  try {
    iEvent.getByLabel( CaloJetsrc_, CaloJetsHandle );
  } 
  catch ( cms::Exception& ex ) {
    edm::LogError( "HadSUSYQCDControlMETSkim" ) 
      << "Unable to get CaloJet collection "
      << CaloJetsrc_.label();
    return false;
  }

  if ( CaloJetsHandle->empty() ) return false;

  int nJet = 0;
  for ( CaloJetCollection::const_iterator it = CaloJetsHandle->begin(); 
	it != CaloJetsHandle->end(); it++ ) {
    if ( (fabs(it->eta()) < 3.0) &&
	 (it->pt() > CaloJetPtmin_) ) nJet++;
  }
  
  if ( nJet < NminCaloJet_ ) return false;

  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void HadSUSYQCDControlMETSkim::endJob()
{
  edm::LogVerbatim( "HadSUSYQCDControlMETSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
