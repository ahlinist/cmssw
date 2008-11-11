//
// $Id: PATLeptonSelProducer.h,v 1.14 2008/10/10 14:02:33 lowette Exp $
//

#ifndef ElectroWeakAnalysis_EWKTau_PATLeptonSelProducer_h
#define ElectroWeakAnalysis_EWKTau_PATLeptonSelProducer_h

/**
  \class    ewk::PATLeptonSelProducer PATLeptonSelProducer.h "ElectroWeakAnalysis/EWKTau/interface/PATLeptonSelProducer.h"
  \brief    Fills output of isolation and lepton id. selectors into pat::Lepton object

   The PATLeptonSelProducer fills the output of isolation and electron/muon/tau id. selectors 
   into pat::Electron/pat::Muon/pat::Tau objects

  \author   Christian Veelken
  \version  $Id: PATLeptonSelProducer.h,v 1.14 2008/10/10 14:02:33 lowette Exp $
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <vector>
#include <string>

template <class T>
class PATLeptonSelProducer : public edm::EDProducer {

  public:

    explicit PATLeptonSelProducer(const edm::ParameterSet & iConfig);
    ~PATLeptonSelProducer();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:
    edm::InputTag leptonSrc_;

    typedef std::pair<edm::InputTag, std::string> selFlagEntry;
    std::vector<selFlagEntry> selFlags_;
};

typedef PATLeptonSelProducer<pat::Electron> PATElectronSelProducer;
typedef PATLeptonSelProducer<pat::Muon> PATMuonSelProducer;
typedef PATLeptonSelProducer<pat::Tau> PATTauSelProducer;

#endif
