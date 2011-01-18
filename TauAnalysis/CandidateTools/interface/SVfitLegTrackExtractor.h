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
 * \version $Revision: 1.1 $
 *
 * $Id: SVfitLegTrackExtractor.h,v 1.1 2010/08/30 13:26:49 veelken Exp $
 *
 */

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <vector>

// define template for generic particle Candidate case
// (dummy implementation returning empty track vector)
template <typename T>
class SVfitLegTrackExtractor
{
 public:
  std::vector<reco::TrackBaseRef> operator()(const T& lepton) const
  {
    //std::cout << "<SVfitLegTrackExtractor::operator()>:" << std::endl;
    std::vector<reco::TrackBaseRef> tracks;
    return tracks;
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
    tracks.push_back(reco::TrackBaseRef(electron.gsfTrack()));
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
    tracks.push_back(reco::TrackBaseRef(muon.innerTrack()));
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
      tracks.push_back(reco::TrackBaseRef(signalChargedHadrons.at(iChargedHadron)->trackRef()));
    }
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
