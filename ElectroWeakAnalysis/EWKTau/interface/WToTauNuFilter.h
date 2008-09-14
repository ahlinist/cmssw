// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h" // Framework services
#include "PhysicsTools/UtilAlgos/interface/TFileService.h" // Framework service for histograms
#include "TH1.h" // RooT histogram class

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"

class WToTauNuFilter : public edm::EDFilter 
{

  public:
    explicit WToTauNuFilter(const edm::ParameterSet&);
    ~WToTauNuFilter();

  private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    
    bool display_;
    edm::InputTag metSource_;
    edm::InputTag caloJetSource_;
    edm::InputTag tauSource_;
    
    // Cut parameters
    float metCut_;
    float jetCut1_;
    float jetCut2_;
    
    // efficiency counters
    int nEvents_;
    int nPassedMETCut_;
    int nPassedJetCut1_;
    int nPassedJetCut2_;
    
    // Histograms
    TH1F *h_MET_Et;
    TH1F *h_CaloJet_Et;

};

