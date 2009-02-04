// ********************************************************************************************
// *** The selector chooses only the PAT muons that satisfy a invariant mass requirement    ***
// *** when combined with a PAT tau.                                                        ***
// ***                                                                                      ***
// ***                                                                  October 8, 2008     ***
// ***                                                                  Alfredo Gurrola     ***
// ********************************************************************************************

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/Handle.h"
//#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

struct PATMuonAntiPionSelector {

public:

  typedef std::vector< pat::Muon > collection;
  typedef edm::Handle< collection > HandleToCollection;
  typedef std::vector < const pat::Muon *> container;
  typedef container::const_iterator const_iterator;

  PATMuonAntiPionSelector ( const edm::ParameterSet & cfg ) :
  MuonSource_( cfg.getParameter< edm::InputTag >( "src" ) ),
  CaloCompCoefficient_( cfg.getParameter< double >( "CaloCompCoefficient" ) ),
  SegmCompCoefficient_( cfg.getParameter< double >( "SegmCompCoefficient" ) ),
  AntiPionCut_( cfg.getParameter< double >( "AntiPionCut" ) ) { }

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }

  void select( const HandleToCollection & hc, 
               const edm::Event & e,
               const edm::EventSetup& s) {
    selected_.clear();

    edm::Handle<edm::View<pat::Muon> > muonHandle;
    e.getByLabel(MuonSource_,muonHandle);
    edm::View<pat::Muon> muons = *muonHandle;

    collection col = *hc;
    for(edm::View<pat::Muon>::const_iterator trk = muons.begin();trk != muons.end(); ++trk) {
      bool goodMuon = true;
      if(((CaloCompCoefficient_ * muon::caloCompatibility(*trk))+
          (SegmCompCoefficient_ * muon::segmentCompatibility(*trk)))<=AntiPionCut_) {goodMuon = false;}
      if (goodMuon) {selected_.push_back( & * trk );}
      //std::cout << "caloCompatibility :  " << muon::caloCompatibility(*trk) << std::endl;
      //std::cout << "segmentCompatibility :  " << muon::segmentCompatibility(*trk) << std::endl;
      //std::cout << "Good Muon ? :  " << goodMuon << std::endl;
    }
  }

  size_t size() const { return selected_.size(); }

private:

  container selected_;
  edm::InputTag MuonSource_;
  double CaloCompCoefficient_;
  double SegmCompCoefficient_;
  double AntiPionCut_;
};
