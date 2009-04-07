#ifndef TauAnalysis_RecoTools_PATLeptonTrackExtractor_h
#define TauAnalysis_RecoTools_PATLeptonTrackExtractor_h

/** \class PATLeptonTrackExtractor
 *
 * Auxiliary class to encapsulate the different methods 
 * for accessing the tracks of pat::Electrons and pat::Muons 
 * and the "leading" track of pat::Taus
 * 
 * \author Chriatian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATLeptonTrackExtractor.h,v 1.2 2009/02/06 14:18:59 veelken Exp $
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
  const reco::Track* operator()(const T& lepton) 
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
  const reco::Track* operator()(const pat::Electron& electron) 
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
  const reco::Track* operator()(const pat::Tau& tau) 
  {
    //std::cout << "<PATLeptonTrackExtractor<pat::Tau>::operator()>:" << std::endl;
    return tau.leadTrack().get();
  }
};

// add dummy template specialization for the generic
// particle Candidate case
template <>
class PATLeptonTrackExtractor<reco::Candidate>
{
 public:
  const reco::Track* operator()(const reco::Candidate& candidate) 
  {
    //std::cout << "<PATLeptonTrackExtractor<reco::Candidate>::operator()>:" << std::endl;
    return 0;
  }
};

#endif
