#include <iostream>

#include "RecoTBCalo/H2TBAnalysis/interface/JetAnalysis.h"
#include "RecoTBCalo/H2TBAnalysis/interface/TemplateAnalysis.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "DataFormats/Common/interface/EventID.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"


class TB06Analyzer : public edm::EDAnalyzer {
public:
  explicit TB06Analyzer(edm::ParameterSet const& conf);
  virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
  virtual void endJob();
private:
  // variables persistent across events should be declared here.
  //
  JetAnalysis jet_analysis_;
  TemplateAnalysis temp_analysis_;
  std::string recjets_,genjets_,recmet_,genmet_,calotowers_;
  std::string tbobj_;

  int errCnt_;
  int runNum_;
  const int errMax(){return 100;}
};

// Boiler-plate constructor definition of an analyzer module:
//
TB06Analyzer::TB06Analyzer(edm::ParameterSet const& conf) {

  // If your module takes parameters, here is where you would define
  // their names and types, and access them to initialize internal
  // variables. Example as follows:
  //
  std::cout << " Beginning TB06Analyzer Analysis " << std::endl;

  recjets_    = conf.getParameter< std::string > ("recjets");
  genjets_    = conf.getParameter< std::string > ("genjets");
  recmet_     = conf.getParameter< std::string > ("recmet");
  genmet_     = conf.getParameter< std::string > ("genmet");
  calotowers_ = conf.getParameter< std::string > ("calotowers");
  tbobj_      = conf.getUntrackedParameter< std::string > ("tbobj","");

  errCnt_=0;
  runNum_=0;
  jet_analysis_.setup(conf);
  temp_analysis_.setup(conf);

}

// Boiler-plate "analyze" method declaration for an analyzer module.
//
void TB06Analyzer::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {


  // To get information from the event setup, you must request the "Record"
  // which contains it and then extract the object you need
  edm::ESHandle<CaloGeometry> geometry;
  iSetup.get<IdealGeometryRecord>().get(geometry);

  // These declarations create handles to the types of records that you want
  // to retrieve from event "iEvent".
  //  
  edm::Handle<CaloJetCollection> recjets;
  edm::Handle<GenJetCollection> genjets;
  edm::Handle<CaloTowerCollection> caloTowers;
  edm::Handle<METCollection> recmet;
  edm::Handle<METCollection> genmet;
  edm::Handle<edm::HepMCProduct> mctruthHandle;

  edm::Handle<HBHERecHitCollection> hbhe_hits;
  edm::Handle<HBHEDigiCollection> hbhe_digis;
  edm::Handle<HORecHitCollection> ho_hits;
  edm::Handle<HODigiCollection> ho_digis;
  edm::Handle<HFRecHitCollection> hf_hits;
  edm::Handle<HFDigiCollection> hf_digis;
  edm::Handle<EBRecHitCollection> eb_hits;
  edm::Handle<EBDigiCollection> eb_digis;

  edm::Handle<HcalTBTriggerData> trigger;
  edm::Handle<HcalTBBeamCounters> tb_qadc;
  edm::Handle<HcalTBEventPosition> tb_epos;
  edm::Handle<HcalTBTiming> tb_time;

  edm::EventID id_ = iEvent.id();
  runNum_ = (int)(id_.run());

  // Extract Data objects (event fragments)
  // make sure to catch exceptions if they don't exist...
  string errMsg("");
  try {iEvent.getByLabel(recjets_,recjets);} catch (...) { errMsg=errMsg + "  -- No RecJets";}
  try {iEvent.getByLabel(recmet_,recmet);} catch (...) {errMsg=errMsg + "  -- No RecMET";}
  try {iEvent.getByLabel(calotowers_,caloTowers);} catch (...) {errMsg=errMsg + "  -- No CaloTowers";}

  try {
    iEvent.getByType(hbhe_hits);
  } catch (...) {
    errMsg=errMsg + "  -- No HBHE hits";
  }

  try {
    iEvent.getByType(hbhe_digis);
  } catch (...) {
    errMsg=errMsg + "  -- No HBHE digis";
  }

  try {
    iEvent.getByType(ho_hits);
  } catch (...) {
    errMsg=errMsg + "  -- No HO hits";
  }

  try {
    iEvent.getByType(ho_digis);
  } catch (...) {
    errMsg=errMsg + "  -- No HO digis";
  }

  try {
    iEvent.getByType(hf_hits);
  } catch (...) {
    errMsg=errMsg + "  -- No HF hits";
  }

  try {
    iEvent.getByType(hf_digis);
  } catch (...) {
    errMsg=errMsg + "  -- No HF digis";
  }

  try {
    iEvent.getByType(eb_hits);
  } catch (...) {
    errMsg=errMsg + "  -- No EB hits";
  }

  try {
    iEvent.getByType(eb_digis);
  } catch (...) {
    errMsg=errMsg + "  -- No EB digis";
  }

  // Trigger/TB Information
   if(tbobj_.size()>2){
    try{ iEvent.getByLabel(tbobj_,trigger);} catch (...) {errMsg=errMsg + "  -- No TB Trigger info";}
    try{ iEvent.getByLabel(tbobj_,tb_qadc); } catch(...) {  errMsg=errMsg + "  -- No TB QADC info";}
    try{ iEvent.getByLabel(tbobj_,tb_epos); } catch(...) {  errMsg=errMsg + "  -- No TB Event Position info";}
    try{ iEvent.getByLabel(tbobj_,tb_time); } catch(...) {  errMsg=errMsg + "  -- No TB Timing info";}
  }
  else{
    try{ iEvent.getByType(trigger);} catch (...) {errMsg=errMsg + "  -- No TB Trigger info";}
    try{ iEvent.getByType(tb_qadc); } catch(...) {  errMsg=errMsg + "  -- No TB QADC info";}
    try{ iEvent.getByType(tb_epos); } catch(...) {  errMsg=errMsg + "  -- No TB Event Position info";}
    try{ iEvent.getByType(tb_time); } catch(...) {  errMsg=errMsg + "  -- No TB Timing info";}
  }

  // MC objects
  HepMC::GenEvent mctruth;
  try {
    iEvent.getByLabel("VtxSmeared", "", mctruthHandle);
    mctruth = mctruthHandle->getHepMCData(); 
  } catch (...) {
    errMsg=errMsg + "  -- No MC truth";
  }

  try {
    iEvent.getByLabel (genjets_,genjets);
  } catch (...) {
    errMsg=errMsg + "  -- No GenJets";
  }

  try {
    iEvent.getByLabel (genmet_,genmet);
  } catch (...) {
    errMsg=errMsg + "  -- No GenMet";
  }
  
  if ((errMsg != "") && (errCnt_ < errMax())){
    errCnt_=errCnt_+1;
    errMsg=errMsg + ".";
    std::cout << "%TB06Analyzer-Warning" << errMsg << std::endl;
    if (errCnt_ == errMax()){
      errMsg="%TB06Analyzer-Warning -- Maximum error count reached -- No more messages will be printed.";
      std::cout << errMsg << std::endl;    
    }
  }

  // run the jet analysis, passing required event fragments
  //
  jet_analysis_.analyze(*recjets,*genjets,
			*recmet,*genmet,
			*caloTowers,mctruth,
			*hbhe_hits,*hbhe_digis,
			*ho_hits,*ho_digis,
			*hf_hits,*hf_digis,
			*trigger,*geometry);

  temp_analysis_.analyze(*recjets,
                         *hbhe_hits,*ho_hits,*hf_hits,
                         *trigger,*tb_epos,*tb_qadc,*tb_time
                         );
}

// "endJob" is an inherited method that you may implement to do post-EOF processing
// and produce final output.
//
void TB06Analyzer::endJob() {
  jet_analysis_.done();
  temp_analysis_.done();
}

// Here are the necessary incantations to declare your module to the
// framework, so it can be referenced in a cmsRun file.
//
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TB06Analyzer)

