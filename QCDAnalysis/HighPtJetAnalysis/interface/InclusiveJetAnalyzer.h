#ifndef __INCLUSIVE_JET_ANALYZER__
#define __INCLUSIVE_JET_ANALYZER__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/HistBox.h"
#include <string>
#include <TFile.h>
#include "QCDAnalysis/HighPtJetAnalysis/interface/JetAnaSelector.h"
#include <TLorentzVector.h>

class InclusiveJetAnalyzer : public edm::EDAnalyzer{
 public:
  explicit InclusiveJetAnalyzer(const edm::ParameterSet&);
  ~InclusiveJetAnalyzer(){}
 private:
  class EventInfo{
  public:
    EventInfo(const std::string &mode);
    void setXsection(const double &xsection){_xSection=xsection;}
    void setEvent(const double &ptHat);
    inline const double weight () const {return _weight;}
    inline const double xSection() const {return _xSection;}
    inline const double ptHat() const {return _ptHat;}
    inline const double total_Events() const {return _totalEvents;}
    inline const unsigned int mode() const {return _mode;}
    enum modes {mNoWeight,mXsection,mXsectionCsaOverlay};
    std::vector<double> _ptHatMins,_samplesXsection;
    std::vector<unsigned int> _v_totStat_samples,_v_totStat_ptHats,
      _v_ET20Stat_ptHats,_v_ET30Stat_ptHats,_v_ET50Stat_ptHats,_v_ET80Stat_ptHats,_v_ET110Stat_ptHats,_v_ET150Stat_ptHats;
  private:
    double _weight,_xSection,_ptHat;
    unsigned int _mode,_totalEvents;
    EventInfo(){}
  };

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(const edm::Run &run, const edm::EventSetup &es);
  template <typename T> void initAlgoHistos(const std::vector< edm::Handle<T> > &a);
  template <typename T> void fillHistos(const std::vector< edm::Handle<T> > &a);
  void fillEventHistos();
  std::vector<std::string> theMustHave,theNoGo;
  std::string outFileName;
  edm::JetAnaSelector mySelector;
  bool firstEvent;
  std::vector<double> _rapBins,_ptBins;
  double _ptMin;
  std::map<std::string,AlgoHistBox> algoHistos;
  TFile *theFile;
  EventInfo evInfo;
  EventHistBox eventHistos;
};



template <typename T> 
void InclusiveJetAnalyzer::initAlgoHistos(const std::vector< edm::Handle<T> > &vecHandle){
  for (typename std::vector< edm::Handle<T> >::const_iterator 
	 ojit=vecHandle.begin();
       ojit!=vecHandle.end();++ojit){
    edm::Provenance const* tprov=ojit->provenance();
    std::string bname(tprov->branchName());
    algoHistos[bname].initHistos(bname,_ptBins,_rapBins,_ptMin);
   std::cout<<"[HighPtJetAna] Adding Collection: "<<bname<<std::endl;
  }
}

template <typename T> 
void InclusiveJetAnalyzer::fillHistos(const std::vector< edm::Handle<T> > &vecHandle){
  for (typename std::vector< edm::Handle<T> >::const_iterator 
	 ojit=vecHandle.begin();
       ojit!=vecHandle.end();++ojit){
    edm::Provenance const* tprov=ojit->provenance();
    std::string bname(tprov->branchName());
    for (typename T::const_iterator gjit=(*ojit)->begin();gjit!=(*ojit)->end();++gjit)
      {
	using namespace std;
	TLorentzVector tmpVec(gjit->p4().px(),gjit->p4().py(),gjit->p4().pz(),gjit->p4().energy());
	algoHistos[bname].fill(tmpVec,evInfo.weight());
      }
    
  }
    
}



#endif
