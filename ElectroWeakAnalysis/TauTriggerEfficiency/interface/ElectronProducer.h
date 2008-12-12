#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include <vector>



class ElectronProducer : public edm::EDProducer {
  
public:
  explicit ElectronProducer(const edm::ParameterSet&);
  ~ElectronProducer();

  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  typedef math::XYZTLorentzVector LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag e_;
  edm::InputTag e_idAssocProd_;
  edm::InputTag e_ctfTrackCollection_;
  double ptMinElectron_;
  bool e_doID_;
  bool e_doTrackIso_;
  double e_trackMinPt_;
  double e_lipCut_;
  double e_minIsoDR_;
  double e_maxIsoDR_;
  double e_isoMaxSumPt_;
  bool e_fromZ_;
  double e_zMmin_,e_zMmax_;
  double e_fromZet_;
  double e_etaMax_;
  
  double ElectronTrkIsolation(const reco::TrackCollection*, const reco::GsfElectron&);

};

