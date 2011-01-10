#include "TauAnalysis/Core/plugins/ObjectDebugEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

template<typename T>
ObjectDebugEventDump<T>::ObjectDebugEventDump(const edm::ParameterSet& cfg)
  : EventDumpBase(cfg),
    src_(cfg.getParameter<edm::InputTag>("src"))
{
  typedef std::vector<std::string> vstring;
  vstring values = cfg.getParameter<vstring>("values");

  for ( vstring::const_iterator value = values.begin();
	value != values.end(); ++value ) {
    stringObjFunctions_.push_back(StringObjectFunction<T>(*value));
  }

  separator_ = cfg.exists("separator") ? 
    cfg.getParameter<std::string>("separator") : " ";
}
template<typename T>
ObjectDebugEventDump<T>::~ObjectDebugEventDump()
{
//--- nothing to be done yet...
}

template<typename T>
void ObjectDebugEventDump<T>::print(const edm::Event& evt, const edm::EventSetup& es, 
				    const GenericAnalyzer_namespace::filterResults_type& filterResults_cumulative, 
				    const GenericAnalyzer_namespace::filterResults_type& filterResults_individual, 
				    double eventWeight) const
{
//--- retrieve run and event numbers from the event
  edm::RunNumber_t runNumber = evt.id().run();
  //edm::LuminosityBlockNumber_t lumiSectionNumber = evt.luminosityBlock();
  edm::EventNumber_t eventNumber = evt.id().event();

  typedef edm::View<T> patCollectionType;
  edm::Handle<patCollectionType> patObjects;
  evt.getByLabel(src_, patObjects);

  for ( typename patCollectionType::const_iterator patObject = patObjects->begin();
	patObject != patObjects->end(); ++patObject ) {
    *outputStream_ << runNumber << separator_ << eventNumber;
    for ( typename vStringObjectFunction::const_iterator stringObjFunction = stringObjFunctions_.begin();
	  stringObjFunction != stringObjFunctions_.end(); ++stringObjFunction ) {
      *outputStream_ << separator_ << (*stringObjFunction)(*patObject);
    }
    *outputStream_ << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

typedef ObjectDebugEventDump<pat::Electron> PATElectronDebugEventDump;
typedef ObjectDebugEventDump<pat::Muon> PATMuonDebugEventDump;
typedef ObjectDebugEventDump<pat::Tau> PATTauDebugEventDump;
typedef ObjectDebugEventDump<pat::Jet> PATJetDebugEventDump;
typedef ObjectDebugEventDump<pat::MET> PATMEtDebugEventDump;

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

typedef ObjectDebugEventDump<PATElecMuPair> PATElecMuPairDebugEventDump;
typedef ObjectDebugEventDump<PATElecTauPair> PATElecTauPairDebugEventDump;
typedef ObjectDebugEventDump<PATMuTauPair> PATMuTauPairValDebugEventDump;
typedef ObjectDebugEventDump<PATDiTauPair> PATDiTauPairValDebugEventDump;

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATElectronDebugEventDump, "PATElectronDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATMuonDebugEventDump, "PATMuonDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATTauDebugEventDump, "PATTauDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATJetDebugEventDump, "PATJetDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATMEtDebugEventDump, "PATMEtDebugEventDump");

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATElecMuPairDebugEventDump, "PATElecMuPairDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATElecTauPairDebugEventDump, "PATElecTauPairDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATMuTauPairValDebugEventDump, "PATMuTauPairValDebugEventDump");
DEFINE_EDM_PLUGIN(EventDumpPluginFactory, PATDiTauPairValDebugEventDump, "PATDiTauPairValDebugEventDump");
