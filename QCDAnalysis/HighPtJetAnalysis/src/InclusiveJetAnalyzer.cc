#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Provenance/interface/Provenance.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/JetAnaSelector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

using namespace std;
using namespace edm;

InclusiveJetAnalyzer::InclusiveJetAnalyzer(const edm::ParameterSet& iConfig):
  theMustHave(iConfig.getUntrackedParameter<std::vector<string> >("mustHave")),
  theNoGo(iConfig.getUntrackedParameter<std::vector<string> >("noGo")),
  outFileName(iConfig.getUntrackedParameter<string>("outFile","IchHabeKeinenNamen.root")),
  mySelector(theMustHave,theNoGo),
  _rapBins(iConfig.getUntrackedParameter<std::vector<double> >("rapBins")),
  _ptBins(iConfig.getUntrackedParameter<std::vector<double> >("ptBins")),
  _ptMin(iConfig.getUntrackedParameter<double>("ptMin")),
  evInfo(iConfig.getUntrackedParameter<string>("mode")){
  
  firstEvent=true;
  // read parameters for the modes:
  evInfo._ptHatMins=iConfig.getUntrackedParameter<std::vector<double> >("ptHatMins");

  switch(evInfo.mode()){
  case EventInfo::mNoWeight:
    cout<<"[InclusiveJetAnalyzer] Applying no weights to jets!"<<endl;
    break;
  case EventInfo::mXsection:
    cout<<"[InclusiveJetAnalyzer] Applying single Sample weights!"<<endl;
    evInfo._samplesXsection=iConfig.getUntrackedParameter<std::vector<double> >("samplesXsection");
    evInfo._v_totStat_samples=iConfig.getUntrackedParameter<std::vector<unsigned int> >("totStat_samples");
    break;
  case EventInfo::mXsectionCsaOverlay:
    cout<<"[InclusiveJetAnalyzer] Applying CSA08-MC-Overlay!"<<endl;
    evInfo._samplesXsection=iConfig.getUntrackedParameter<std::vector<double> >("samplesXsection");
    evInfo._v_totStat_samples=iConfig.getUntrackedParameter<std::vector<unsigned int> >("totStat_samples");
    evInfo._v_totStat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("totStat_ptHats");
    evInfo._v_ET20Stat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("ET20Stat_ptHats");
    evInfo._v_ET30Stat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("ET30Stat_ptHats");
    evInfo._v_ET50Stat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("ET50Stat_ptHats");
    evInfo._v_ET80Stat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("ET80Stat_ptHats");
    evInfo._v_ET110Stat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("ET110Stat_ptHats");
    evInfo._v_ET150Stat_ptHats=iConfig.getUntrackedParameter<std::vector<unsigned int> >("ET150Stat_ptHats");
    break;
  default:
    cerr<<"[InclusiveJetAnalyzer] No Mode Given! -exit!"<<endl;
    exit(1);
    break;
  }
    
}

void InclusiveJetAnalyzer::beginJob(const edm::EventSetup &iConfig){
 
}

void InclusiveJetAnalyzer::endJob(){
  ROOTContextSentinel b;
  theFile=new TFile(outFileName.c_str(),"RECREATE","Output");
  eventHistos.write(theFile);
  for (map<std::string,AlgoHistBox>::iterator iter=algoHistos.begin();iter!=algoHistos.end();++iter){
    (*iter).second.write(theFile);
  }
  theFile->Close();
}

void InclusiveJetAnalyzer::beginRun(const edm::Run &run, const edm::EventSetup &iConfig){
  edm::Handle<GenRunInfoProduct> genRunInfoProd;
  double gCrossSect=0;
  run.getByLabel("generator", genRunInfoProd);
  if (genRunInfoProd.isValid()){
    gCrossSect=genRunInfoProd->externalXSecLO().value();
  }
  else {
    cout<<"[InclusiveJetAnalyzer] cross section is inVALID!!!"<<endl;
    gCrossSect=-3.5;
  }
  evInfo.setXsection(gCrossSect);
}

void InclusiveJetAnalyzer::analyze(const edm::Event &iEvent, const edm::EventSetup &iConfig){
   
  std::vector< edm::Handle<reco::CaloJetCollection> > vecOfCaloJets;
  
  std::vector< edm::Handle<reco::GenJetCollection> > vecOfGenJets;
  iEvent.getMany(mySelector,vecOfCaloJets);

  //Maybe View is cool? Does unfortunately not work the following way... 
  //so i stay with templates for the moment.
  //std::vector<edm::Handle<edm::View<reco::Jet> > > vecOfGenJets;

  iEvent.getMany(mySelector,vecOfGenJets);

  
  if (firstEvent) {
    initAlgoHistos<reco::CaloJetCollection> (vecOfCaloJets);
    initAlgoHistos<reco::GenJetCollection> (vecOfGenJets);
    eventHistos.initHistos(evInfo._ptHatMins);
    firstEvent=false;

  }
  edm::Handle<GenEventInfoProduct> hEventInfo;
  iEvent.getByLabel("generator", hEventInfo);
  double ptHat;
  try {  iEvent.getByLabel("generator", hEventInfo); }
  catch (...) {cerr<<"NoEventScale"<<endl;}
  if (hEventInfo.isValid()){
    ptHat = hEventInfo->binningValues()[0];
  }
  else ptHat=-1;

  evInfo.setEvent(ptHat);
  fillEventHistos();
  fillHistos<reco::CaloJetCollection>(vecOfCaloJets);
  fillHistos<reco::GenJetCollection>(vecOfGenJets);
  
}


void InclusiveJetAnalyzer::fillEventHistos(){
  eventHistos.h_ptHat->Fill(evInfo.ptHat());
  eventHistos.h_ptHatStat->Fill(evInfo.ptHat());
  eventHistos.h_weight->Fill(evInfo.weight());
  eventHistos.h_xSection->Fill(evInfo.xSection());
}

InclusiveJetAnalyzer::EventInfo::EventInfo(const string &mode){
  if (mode=="noWeight") _mode=mNoWeight;
  else if (mode=="xSection") _mode=mXsection;
  else if (mode=="xSectionCsaOverlay") _mode=mXsectionCsaOverlay;
  else {
    cerr<<"[InclusiveJetAnalyzer] No mode given! - EXIT!"<<endl;
    exit(1);
  }
  _weight=-1;
  _xSection=-1;
  _totalEvents=0;
  _ptHat=-1;
}


void InclusiveJetAnalyzer::EventInfo::setEvent(const double &ptHat){
  _ptHat=ptHat;

  if (_mode==mNoWeight) {
    _weight=1;
    return;
  }

  //find sample/ptHat-Bin-indizes:
  unsigned int iPtHat=10;
  unsigned int iXsec=10;
  unsigned int nSamples=_v_totStat_samples.size();
  
  for (unsigned int i=0;i<nSamples;++i){
    if (_xSection == _samplesXsection.at(i)) { 
      iXsec=i;
    }
    if (_mode==mXsection) continue;
    
    if (ptHat >= _ptHatMins.at(i) && ptHat < _ptHatMins.at(i+1)){
      iPtHat=i;
    }
  }
  if (iXsec== 10) {
    cerr<<"[CSA_Samples] No useable sample found!"<<endl;
    exit(1);
  }
  if (iPtHat == 10 && _mode==mXsectionCsaOverlay){
    cerr<<"[CSA_Samples] No useable ptHat bin found!"<<endl;
    exit(1);
  }

  if (_mode==mXsection) {
    _weight=1E3*_xSection/(double)(_v_totStat_samples.at(iXsec));
  }
  else if (_mode==mXsectionCsaOverlay){
    double stat=0;
    switch (iXsec){
    case 0: 
      stat=_v_ET20Stat_ptHats.at(iPtHat);
      break;
    case 1: 
      stat=_v_ET30Stat_ptHats.at(iPtHat);
      break;
    case 2: 
      stat=_v_ET50Stat_ptHats.at(iPtHat);
      break;
    case 3: 
      stat=_v_ET80Stat_ptHats.at(iPtHat);
      break;
    case 4: 
      stat=_v_ET110Stat_ptHats.at(iPtHat);
      break;
    case 5: 
      stat=_v_ET150Stat_ptHats.at(iPtHat);
      break;
    default:
      cerr<<"[CSA_Samples] No useable ptHat bin found!"<<endl;
      exit(1);
      break;
    }
    _weight=1E3*_xSection*(double)stat/((double)_v_totStat_ptHats.at(iPtHat)*(double)(_v_totStat_samples.at(iXsec)));
  }
  else {
    cerr<<"[InclusiveJetAnalyzer] ERROR! - no mode given."<<endl;
    exit(1);
  }
}
