#ifndef TauAnalysis_RecoTools_PATLeptonTrackVectorExtractor_h
#define TauAnalysis_RecoTools_PATLeptonTrackVectorExtractor_h

/** \class PATLeptonTrackVectorExtractor
 *
 * Auxiliary class to encapsulate the different methods 
 * for accessing the tracks of pat::Electrons and pat::Muons 
 * and "signal" tracks of pat::Taus
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATLeptonTrackVectorExtractor.h,v 1.1 2011/04/05 13:27:58 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

template <typename T>
class PATLeptonTrackVectorExtractor
{
 public:
  std::vector<const reco::Track*> operator()(const T& lepton) const
  {
    assert (0);
  }
};

// add template specialization for pat::(GSF)Electrons
template <>
class PATLeptonTrackVectorExtractor<pat::Electron>
{
 public:
  std::vector<const reco::Track*> operator()(const pat::Electron& electron) const
  {
    //std::cout << "<PATLeptonTrackVectorExtractor<pat::Electron>::operator()>" << std::endl;
    std::vector<const reco::Track*> retVal;
    if ( electron.gsfTrack().isNonnull() ) retVal.push_back(electron.gsfTrack().get());
    return retVal;
  }
};

// add template specialization for pat::Muons
template <>
class PATLeptonTrackVectorExtractor<pat::Muon>
{
 public:
  std::vector<const reco::Track*> operator()(const pat::Muon& muon) const
  {
    //std::cout << "<PATLeptonTrackVectorExtractor<pat::Muon>::operator()>" << std::endl;
    std::vector<const reco::Track*> retVal;
    if ( muon.track().isNonnull() ) retVal.push_back(muon.track().get());
    return retVal;
  }
};

// add template specialization for pat::Taus
template <>
class PATLeptonTrackVectorExtractor<pat::Tau>
{
 public:
  std::vector<const reco::Track*> operator()(const pat::Tau& tau) const
  {
    //std::cout << "<PATLeptonTrackVectorExtractor<pat::Tau>::operator()>" << std::endl;
    std::vector<const reco::Track*> retVal;
    if ( tau.isPFTau() ) {
      const reco::PFCandidateRefVector& pfChargedHadrons = tau.signalPFChargedHadrCands();
      for ( reco::PFCandidateRefVector::const_iterator pfChargedHadron = pfChargedHadrons.begin();
	    pfChargedHadron != pfChargedHadrons.end(); ++pfChargedHadron ) {
	if ( (*pfChargedHadron)->trackRef().isNonnull() ) retVal.push_back((*pfChargedHadron)->trackRef().get());
      }
    } else { 
      const reco::TrackRefVector& signalTracks = tau.signalTracks();
      for ( reco::TrackRefVector::const_iterator signalTrack = signalTracks.begin();
	    signalTrack != signalTracks.end(); ++signalTrack ) {
	if ( signalTrack->isNonnull() ) retVal.push_back(signalTrack->get());
      }
    }
    return retVal;
  }
};

#endif
