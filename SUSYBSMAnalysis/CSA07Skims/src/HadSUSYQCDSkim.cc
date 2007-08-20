/** \class HadSUSYQCDSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel jets 100 GeV, dphi, R1, R2 cuts
 * QCD control, test L1 acoplanar path (QCD trigger path)
 *
 * $Date: 2007/07/27 21:04:22 $
 * $Revision: 1.2 $
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

//#include "DataFormats/JetReco/interface/CaloJetfwd.h" 
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYQCDSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

class PtSorter {
public:
  template <class T> bool operator() ( const T& a, const T& b ) {
    return ( a.pt() > b.pt() );
  }
};

double DeltaPhi( double v1, double v2 )
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 double diff = fabs( v2 - v1 );
 double corr = 2.*acos(-1.) - diff;
 if ( diff < acos(-1.) ) { return diff;} else { return corr;}
}

HadSUSYQCDSkim::HadSUSYQCDSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  CaloJetsrc_ = iConfig.getParameter<InputTag>( "CaloJetsrc" );
  CaloJetPtmin_ = 
    iConfig.getUntrackedParameter<double>( "CaloJetPtmin", 100. );
  NminCaloJet_ = iConfig.getUntrackedParameter<int>( "NminCaloJet", 2 );
  if ( NminCaloJet_ < 2 ) {
    edm::LogError( "HadSUSYQCDSkim" ) 
      << "Setting NminCaloJet to 2 !!";
    NminCaloJet_ = 2;
  }
  CaloMETsrc_ = iConfig.getParameter<InputTag>( "CaloMETsrc" );
}

/*------------------------------------------------------------------------*/

HadSUSYQCDSkim::~HadSUSYQCDSkim() 
{}

/*------------------------------------------------------------------------*/

bool HadSUSYQCDSkim::filter( edm::Event& iEvent, 
			     const edm::EventSetup& iSetup )
{
  nEvents_++;

  Handle<CaloJetCollection> CaloJetsHandle;
//  try {
    iEvent.getByLabel( CaloJetsrc_, CaloJetsHandle );
//  } 
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYQCDSkim" ) 
//      << "Unable to get CaloJet collection "
//      << CaloJetsrc_.label();
//    return false;
//  }
  if ( CaloJetsHandle->empty() ) return false;
  CaloJetCollection TheCaloJets = *CaloJetsHandle;
  std::stable_sort( TheCaloJets.begin(), TheCaloJets.end(), PtSorter() );

  Handle<CaloMETCollection> METHandle;
//  try {
    iEvent.getByLabel( CaloMETsrc_, METHandle );
//  }
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYQCDSkim" )
//      << "Unable to get CaloMET collection "
//      << CaloMETsrc_.label();
//    return false;
//  }
  if ( METHandle->empty() ) return false;
  double METphi = METHandle->begin()->phi();

  // Jet and phi cuts
  int nJet = 0;
  double phiJet[2];
  for ( CaloJetCollection::const_iterator it = TheCaloJets.begin(); 
	it != TheCaloJets.end(); it++ ) {
    if ( (fabs(it->eta()) < 3.0) &&
	 (it->pt() > CaloJetPtmin_) ) {
      phiJet[nJet++] = it->phi();
      if ( DeltaPhi( it->phi(), METphi ) < 0.3 ) return false;
    }
  }
  if ( nJet < NminCaloJet_ ) return false;

  double dphi1 = DeltaPhi( phiJet[0], METphi );
  double dphi2 = DeltaPhi( phiJet[1], METphi );
  double R1 = sqrt( dphi2*dphi2 
		    + (2.*acos(-1.)-dphi1)*(2.*acos(-1.)-dphi1) );
  if ( R1 < 0.5 ) return false;
  double R2 = sqrt( dphi1*dphi1 
		    + (2.*acos(-1.)-dphi2)*(2.*acos(-1.)-dphi2) ); 
  if ( R2 < 0.5 ) return false;
  if ( DeltaPhi( METphi, phiJet[1] ) < (20./180.*acos(-1.)) )
    return false;
  
  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void HadSUSYQCDSkim::endJob()
{
  edm::LogVerbatim( "HadSUSYQCDSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
