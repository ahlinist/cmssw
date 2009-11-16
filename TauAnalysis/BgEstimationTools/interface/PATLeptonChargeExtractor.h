#ifndef TauAnalysis_RecoTools_PATLeptonChargeExtractor_h
#define TauAnalysis_RecoTools_PATLeptonChargeExtractor_h

/** \class PATLeptonTrackExtractor
 *
 * Auxiliary class to encapsulate the different methods 
 * for accessing the charge of pat::Electrons, pat::Muons and pat::Taus
 * 
 * \author Chriatian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATLeptonTrackExtractor.h,v 1.2 2009/05/26 12:36:29 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

template <typename T>
class PATLeptonChargeExtractor
{
 public:
  double operator()(const T& lepton) const
  {
    return lepton.charge();
  }
};

// add template specialization for pat::Taus,
// returning the charge of the "leading" (i.e. highest Pt) track 
// within the signal cone of the tau-jet if there is a "leading" track
// and otherwise return the sum of charges of all tracks within signal plus isolation cones
//
// NOTE: this implementation is specialized for background estimation (and tau id. efficiency measurements);
//       it aims to avoid the case of zero pat::Tau charge, such that the two cases like/opposite sign
//       of muon and tau-jet charge are defined for as many as possible tau-jet candidates
//
template <>
class PATLeptonChargeExtractor<pat::Tau>
{
 public:
  double operator()(const pat::Tau& tau) const
  {
    double charge = 0.;
    
    double highestPtTrack = -1.;
    
    for ( reco::TrackRefVector::const_iterator track = tau.signalTracks().begin();
	  track != tau.signalTracks().end(); ++track ) {
      if ( (*track)->pt() > highestPtTrack ) {	  
	charge = (*track)->charge();
	highestPtTrack = (*track)->pt();
      }
    }
      
    for ( reco::TrackRefVector::const_iterator track = tau.isolationTracks().begin();
	  track != tau.isolationTracks().end(); ++track ) {
      if ( (*track)->pt() > highestPtTrack ) {	  
	charge = (*track)->charge();
	highestPtTrack = (*track)->pt();
      }
    }
      
    return charge;
  }
};

#endif
