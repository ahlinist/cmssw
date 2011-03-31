#ifndef TauAnalysis_CandidateTools_NSVfitSingleParticleTrackExtractor_h
#define TauAnalysis_CandidateTools_NSVfitSingleParticleTrackExtractor_h

/** \class NSVfitSingleParticleTrackExtractor
 *
 * Auxiliary class to encapsulate the different methods
 * for accessing the tracks of pat::Electrons and pat::Muons
 * and the signal cone tracks of pat::Taus
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: NSVfitSingleParticleTrackExtractor.h,v 1.4 2011/03/31 15:00:38 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <vector>

namespace{
  template<typename T>
  bool isValidRef(const edm::Ref<T>& ref)
  {
    return ( (ref.isAvailable() || ref.isTransient()) && ref.isNonnull() );  
  }
}

// define template for generic particle Candidate case
// (dummy implementation returning empty track vector)
template <typename T>
class NSVfitSingleParticleTrackExtractor
{
 public:
  std::vector<reco::Track*> operator()(const T& lepton) const
  {
    assert(0);
  }
};

// add template specialization for pat::(GSF)Electrons
template <>
class NSVfitSingleParticleTrackExtractor<pat::Electron>
{
 public:
  std::vector<const reco::Track*> operator()(const pat::Electron& electron) const
  {
    //std::cout << "<NSVfitSingleParticleTrackExtractor<pat::Electron>::operator()>:" << std::endl;
    std::vector<const reco::Track*> tracks;
    if ( isValidRef(electron.core()) && isValidRef(electron.gsfTrack()) ) 
      tracks.push_back(electron.gsfTrack().get());
    //std::cout << "--> tracks.size = " << tracks.size() << std::endl;
    return tracks;
  }
};

// add template specialization for pat::Muons
template <>
class NSVfitSingleParticleTrackExtractor<pat::Muon>
{
 public:
  std::vector<const reco::Track*> operator()(const pat::Muon& muon) const
  {
    //std::cout << "<NSVfitSingleParticleTrackExtractor<pat::Muon>::operator()>:" << std::endl;
    std::vector<const reco::Track*> tracks;
    if ( isValidRef(muon.innerTrack()) )
      tracks.push_back(muon.innerTrack().get());
    //std::cout << "--> tracks.size = " << tracks.size() << std::endl;
    return tracks;
  }
};

// add template specialization for pat::Taus,
// returning the tracks within the signal cone of the tau-jet
template <>
class NSVfitSingleParticleTrackExtractor<pat::Tau>
{
 public:
  std::vector<const reco::Track*> operator()(const pat::Tau& tau) const
  {
    //std::cout << "<NSVfitSingleParticleTrackExtractor<pat::Tau>::operator()>:" << std::endl;
    std::vector<const reco::Track*> tracks;
    const reco::PFCandidateRefVector& signalChargedHadrons = tau.signalPFChargedHadrCands();
    unsigned numChargedHadrons = signalChargedHadrons.size();
    for ( unsigned iChargedHadron = 0; iChargedHadron < numChargedHadrons; ++iChargedHadron ) {
      if ( isValidRef(signalChargedHadrons.at(iChargedHadron)->trackRef()) ) 
	tracks.push_back(signalChargedHadrons.at(iChargedHadron)->trackRef().get());
    }
    //std::cout << "--> tracks.size = " << tracks.size() << std::endl;
    return tracks;
  }
};

#endif
