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
 * \version $Revision: 1.5 $
 *
 * $Id: PATLeptonTrackExtractor.h,v 1.5 2011/04/15 16:56:40 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

// prevent generic template instance from being called
template <typename T>
class PATLeptonTrackExtractor
{
 public:
  const reco::Track* operator()(const T& lepton) const
  {
    assert(0);
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
    return electron.gsfTrack().get();
  }
};

// add template specialization for pat::Muons
template <>
class PATLeptonTrackExtractor<pat::Muon>
{
 public:
  const reco::Track* operator()(const pat::Muon& muon) const
  {
    return muon.track().get();
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
    const reco::Track* retVal = 0;
    if ( tau.isPFTau() && tau.leadPFChargedHadrCand().get() ) {
      retVal = tau.leadPFChargedHadrCand()->trackRef().get();
    } else if ( tau.isCaloTau() ) {
      retVal = tau.leadTrack().get();
    }
    return retVal;
  }
};

// add dummy template specialization for the generic
// particle Candidate case
// CV: template instance for generic particle Candidates is deprecated
//     and has only been added for backwards compatibility (2011/04/15)
template <>
class PATLeptonTrackExtractor<reco::Candidate>
{
 public:
  const reco::Track* operator()(const reco::Candidate& candidate) const
  {
    return 0;
  }
};

#endif
