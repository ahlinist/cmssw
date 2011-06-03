#ifndef TauAnalysis_CandidateTools_SVfitLegTrackExtractor_h
#define TauAnalysis_CandidateTools_SVfitLegTrackExtractor_h

/** \class SVfitLegTrackExtractor
 *
 * Auxiliary class to encapsulate the different methods
 * for accessing the tracks of pat::Electrons and pat::Muons
 * and the signal cone tracks of pat::Taus
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: SVfitLegTrackExtractor.h,v 1.5 2011/03/31 16:31:33 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <vector>

namespace {
  template<typename T>
  bool isValidRef(const edm::Ref<T>& ref)
  {
    return ( (ref.isAvailable() || ref.isTransient()) && ref.isNonnull() );  
  }
}

// define template for generic particle Candidate case
// (dummy implementation returning empty track vector)
template <typename T>
class SVfitLegTrackExtractor
{
 public:
  std::vector<reco::TrackBaseRef> operator()(const T& lepton) const
  {
    assert(0);
  }
};

// add template specialization for pat::(GSF)Electrons
template <>
class SVfitLegTrackExtractor<pat::Electron>
{
 public:
  std::vector<reco::TrackBaseRef> operator()(const pat::Electron& electron) const
  {
    //std::cout << "<SVfitLegTrackExtractor<pat::Electron>::operator()>:" << std::endl;
    std::vector<reco::TrackBaseRef> tracks;
    if ( isValidRef(electron.core()) && isValidRef(electron.gsfTrack()) ) 
      tracks.push_back(reco::TrackBaseRef(electron.gsfTrack()));
    //std::cout << "--> tracks.size = " << tracks.size() << std::endl;
    return tracks;
  }
};

// add template specialization for pat::Muons
template <>
class SVfitLegTrackExtractor<pat::Muon>
{
 public:
  std::vector<reco::TrackBaseRef> operator()(const pat::Muon& muon) const
  {
    //std::cout << "<SVfitLegTrackExtractor<pat::Muon>::operator()>:" << std::endl;
    std::vector<reco::TrackBaseRef> tracks;
    if ( isValidRef(muon.innerTrack()) )
      tracks.push_back(reco::TrackBaseRef(muon.innerTrack()));
    //std::cout << "--> tracks.size = " << tracks.size() << std::endl;
    return tracks;
  }
};

// add template specialization for pat::Taus,
// returning the tracks within the signal cone of the tau-jet
template <>
class SVfitLegTrackExtractor<pat::Tau>
{
 public:
  std::vector<reco::TrackBaseRef> operator()(const pat::Tau& tau) const
  {
    //std::cout << "<SVfitLegTrackExtractor<pat::Tau>::operator()>:" << std::endl;
    std::vector<reco::TrackBaseRef> tracks;
    const reco::PFCandidateRefVector& signalChargedHadrons = tau.signalPFChargedHadrCands();
    unsigned numChargedHadrons = signalChargedHadrons.size();
    for ( unsigned iChargedHadron = 0; iChargedHadron < numChargedHadrons; ++iChargedHadron ) {
      if ( isValidRef(signalChargedHadrons.at(iChargedHadron)->trackRef()) ) 
	tracks.push_back(reco::TrackBaseRef(signalChargedHadrons.at(iChargedHadron)->trackRef()));
      else if ( isValidRef(signalChargedHadrons.at(iChargedHadron)->gsfTrackRef()) ) 
	tracks.push_back(reco::TrackBaseRef(signalChargedHadrons.at(iChargedHadron)->gsfTrackRef()));
    }
    //std::cout << "--> tracks.size = " << tracks.size() << std::endl;
    return tracks;
  }
};

// Class to determine if a leg has any neutral objects associated to it
// Generic dummy case
template <typename T>
class SVfitLegHasNeutralsExtractor
{
  public:
    bool operator()(const T& lepton) const {
      return true;
    }
};

template <>
class SVfitLegHasNeutralsExtractor<pat::Electron>
{
  public:
    bool operator()(const pat::Electron& lepton) const {
      return false;
    }
};

template <>
class SVfitLegHasNeutralsExtractor<pat::Muon>
{
  public:
    bool operator()(const pat::Muon& lepton) const {
      return false;
    }
};

template <>
class SVfitLegHasNeutralsExtractor<pat::Tau>
{
  public:
    bool operator()(const pat::Tau& tau) const {
      return tau.signalPFGammaCands().size();
    }
};

#endif
