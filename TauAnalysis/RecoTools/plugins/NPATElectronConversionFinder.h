#ifndef TauAnalysis_RecoTools_NPATElectronConversionFinder_h
#define TauAnalysis_RecoTools_NPATElectronConversionFinder_h

//-------------------------------------------------------------------------------------------
//
//  photon conversion veto for electron identification
//  author: Jeff Kolb, 15 June 2011
//
//  This module selects electrons with the following logic:
//    (minMissingInnerHits <= missingInnerHits <= maxMissingInnerHits ) &&
//    (invertConversionVeto*NOT( minRxy <= Rxy && minFitProb <= fitProb && maxHitsBeforeVertex >= hitsBeforeVertex ) )
//
//  python configuration parameters:
//   1) maxMissingInnerHits 
//      - maximum number of missing (expected) pixel hits
//      - default = 0
//   2) minMissingInnerHits 
//      - minimum number of missing (expected) pixel hits
//      - default = 0
//   3) minRxy
//      - minimum distance between vertex and beam in the transverse plane
//      - default = 2.0cm
//   4) minFitProb
//      - minimum Chi^2 probability for conversion vertex fit
//      - default = 1e-6
//   5) maxHitsBeforeVertex
//      - maximum number of hits associated with the electron between the vertex and beam 
//      - default = 0
//   5) invertConversionVeto
//      - inverts selection based on (minRxy,minFitProb,maxHitsBeforeVertex)
//      - does NOT affect missing inner hits selection
//      - default is false
//
//  Using the default values for all parameters will reject electrons from conversion 
//  according to the EGamma POG recommendations as of Spring 2011
//
//-------------------------------------------------------------------------------------------


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

class NPATElectronConversionFinderImp
{
 public:
  typedef std::vector<pat::Electron> PATElectronCollection;
  typedef std::vector<pat::Electron> collection;

  NPATElectronConversionFinderImp(const edm::ParameterSet& iConfig);
  ~NPATElectronConversionFinderImp();

  std::vector<const pat::Electron*>::const_iterator begin() const { return selected_.begin(); }
  std::vector<const pat::Electron*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<PATElectronCollection>&, edm::Event&, const edm::EventSetup&);

 private:
  std::vector<const pat::Electron*> selected_;
  
  int maxMissingInnerHits_;
  int minMissingInnerHits_;
  double minRxy_;
  double minFitProb_;
  int maxHitsBeforeVertex_;
  bool invertConversionVeto_;
};

#endif
