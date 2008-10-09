// ********************************************************************************************
// *** The selector chooses only the PAT muons that satisfy a invariant mass requirement    ***
// *** when combined with a PAT tau.                                                        ***
// ***                                                                                      ***
// ***                                                                  October 8, 2008     ***
// ***                                                                  Alfredo Gurrola     ***
// ********************************************************************************************

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Common/interface/Handle.h"
#include "Math/GenVector/VectorUtil.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

struct PATMuonPATTauMassSelector {

public:

  typedef std::vector< pat::Muon > collection;
//typedef std::vector<edm::View < pat::Muon > > collection;
  typedef edm::Handle< collection > HandleToCollection;
  typedef std::vector < const pat::Muon *> container;
  typedef container::const_iterator const_iterator;

  PATMuonPATTauMassSelector ( const edm::ParameterSet & cfg ) :
  MuonSource_( cfg.getParameter< edm::InputTag >( "src" ) ),
  TauSource_( cfg.getParameter< edm::InputTag >( "TauSource" ) ),
  MassMinCut_( cfg.getParameter< double >( "MassMinCut" ) ),
  MassMaxCut_( cfg.getParameter< double >( "MassMaxCut" ) ) { }

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }

  void select( const HandleToCollection & hc, 
               const edm::Event & e,
               const edm::EventSetup& s) {
    selected_.clear();

    edm::Handle<edm::View<pat::Muon> > muonHandle;
    e.getByLabel(MuonSource_,muonHandle);
    edm::View<pat::Muon> muons = *muonHandle;

    edm::Handle<edm::View<pat::Tau> > tauHandle;
    e.getByLabel(TauSource_,tauHandle);
    edm::View<pat::Tau> taus = *tauHandle;
    
    collection col = *hc;
    //collection col = muons;
    for(edm::View<pat::Muon>::const_iterator trk = muons.begin();trk != muons.end(); ++trk) {
      bool goodMuon = true;
      for(edm::View<pat::Tau>::const_iterator tauit = taus.begin();tauit != taus.end(); ++tauit) {
        double Invariant_Mass = (trk->energy() + tauit->energy()) * (trk->energy() + tauit->energy());
        Invariant_Mass -= ((trk->px() + tauit->px()) * (trk->px() + tauit->px()));
        Invariant_Mass -= ((trk->py() + tauit->py()) * (trk->py() + tauit->py()));
        Invariant_Mass -= ((trk->pz() + tauit->pz()) * (trk->pz() + tauit->pz()));
        Invariant_Mass = sqrt(Invariant_Mass);
        if( (Invariant_Mass<=MassMinCut_) ||
            (Invariant_Mass>=MassMaxCut_) )
            {goodMuon = false;}
      }
      if (goodMuon) {selected_.push_back( & * trk );}
      std::cout << "Good Muon ? :  " << goodMuon << std::endl;
    }
  }

  size_t size() const { return selected_.size(); }

private:

  container selected_;
  edm::InputTag MuonSource_;
  edm::InputTag TauSource_;
  double MassMinCut_;
  double MassMaxCut_;

};
