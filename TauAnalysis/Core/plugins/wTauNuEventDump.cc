#include "TauAnalysis/Core/plugins/wTauNuEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

wTauNuEventDump::wTauNuEventDump(const edm::ParameterSet& cfg)
  : GenericEventDump(cfg)
{
  //std::cout << "<wTauNuEventDump::wTauNuEventDump>:" << std::endl;
}

wTauNuEventDump::~wTauNuEventDump()
{
//--- nothing to be done yet...
}

void wTauNuEventDump::print(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
			   const std::vector<std::pair<std::string, bool> >& filterResults_cumulative, 
			   const std::vector<std::pair<std::string, bool> >& filterResults_individual, 
			   double eventWeight) const
{  
  printEventHeaderInfo(iEvent, eventWeight);

  printEventSelectionInfo(filterResults_cumulative, filterResults_individual, outputStream_);

  *outputStream_ << ">>GENERATOR LEVEL INFORMATION<<" << std::endl;

  edm::Handle<edm::View<reco::GenParticle> > genParticleCollection;
  iEvent.getByLabel(genParticleSource_, genParticleCollection);
  edm::Handle<edm::View<reco::GenJet> > genTauJetCollection;
  iEvent.getByLabel(genTauJetSource_, genTauJetCollection);
  edm::Handle<edm::View<reco::GenJet> > genJetCollection;
  iEvent.getByLabel("iterativeCone5GenJets", genJetCollection);

 # edm::Handle<pat::JetCollection> patJets;
 # iEvent.getByLabel(patJetSource_, patJets);

  printGenParticleInfo(genParticleCollection, genTauJetCollection, outputStream_);
#  printGenJetInfo(genJetCollection,outputStream_);

  *outputStream_ << ">>RECONSTRUCTION LEVEL INFORMATION<<" << std::endl;

  printMissingEtInfo(iEvent);
  printTauInfo(iEvent);
  printJetInfo(iEvent);  
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, wTauNuEventDump, "wTauNuEventDump");
