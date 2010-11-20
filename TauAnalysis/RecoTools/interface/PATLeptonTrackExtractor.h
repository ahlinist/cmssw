#ifndef TauAnalysis_RecoTools_PATLeptonTrackExtractor_h
#define TauAnalysis_RecoTools_PATLeptonTrackExtractor_h

/** \class PATLeptonTrackExtractor
 *
 * Auxiliary class to encapsulate the different methods 
 * for accessing the tracks of pat::Electrons and pat::Muons 
 * and the "leading" track of pat::Taus
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: PATLeptonTrackExtractor.h,v 1.3 2010/09/02 11:30:07 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

template <typename T>
class PATLeptonTrackExtractor
{
 public:
  const reco::Track* operator()(const T& lepton) const
  {
    //std::cout << "<PATLeptonTrackExtractor::operator()>:" << std::endl;
    return lepton.track().get();
  }
};

// add template specialization for pat::(GSF)Electrons,
// since their track needs to be accessed by calling a different method
template <>
class PATLeptonTrackExtractor<pat::Electron>
{
 public:
  const reco::Track* operator()(const pat::Electron& electron) const
  {
    //std::cout << "<PATLeptonTrackExtractor<pat::Electron>::operator()>:" << std::endl;
    return electron.gsfTrack().get();
  }
};

// add template specialization for pat::Taus,
// returning the "leading" (i.e. highest Pt) track 
// within the signal cone of the tau-jet
template <>
class PATLeptonTrackExtractor<pat::Tau>
{
 public:
  const reco::Track* operator()(const pat::Tau& tau) const
  {
    //std::cout << "<PATLeptonTrackExtractor<pat::Tau>::operator()>:" << std::endl;
    if ( tau.isPFTau() ) {
      return tau.leadPFChargedHadrCand()->trackRef().get();
    } else {
      return tau.leadTrack().get();
    }
  }
};

// add dummy template specialization for the generic
// particle Candidate case
template <>
class PATLeptonTrackExtractor<reco::Candidate>
{
 public:
  const reco::Track* operator()(const reco::Candidate& candidate) const
  {
    //std::cout << "<PATLeptonTrackExtractor<reco::Candidate>::operator()>:" << std::endl;
    return 0;
  }
};

#endif
