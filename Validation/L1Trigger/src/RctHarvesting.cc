#include "Validation/L1Trigger/interface/RctHarvesting.h"
#include "Math/GenVector/VectorUtil.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;
using namespace reco;
using namespace edm;

//
// constructors and destructor
//
RctHarvesting::RctHarvesting( const edm::ParameterSet& iConfig ) :
  directories_(iConfig.getParameter<std::vector<std::string> >("directories")),
  runOnEndJob_(iConfig.getUntrackedParameter<bool>("runOnEndJob",false)),
  runOnEndRun_(iConfig.getUntrackedParameter<bool>("runOnEndRun",true))
{

}


void RctHarvesting::harvest()
{
  DQMStore *store = &*edm::Service<DQMStore>();
  for(unsigned int i=0;i<directories_.size();++i)
    {

      store->setCurrentFolder(directories_[i]);
      //make and fill plots;


      MonitorElement *etNum = store->get(directories_[i]+"/rctEgammaEtEffNum");
      MonitorElement *etDenom = store->get(directories_[i]+"/rctEgammaEtEffDenom");
      if(etNum&&etDenom)
	{
	  int bins = etNum->getTH1F()->GetNbinsX();
	  double min  = etNum->getTH1F()->GetXaxis()->GetXmin();
	  double max  = etNum->getTH1F()->GetXaxis()->GetXmax();
	  MonitorElement *rctEgammaEtEff = store->book1D("rctEgammaEtEff","e/#gamma Efficiency vs ET",bins,min,max);
	  rctEgammaEtEff->getTH1F()->Divide(etNum->getTH1F(),etDenom->getTH1F(),1.,1.,"B");
	}

      MonitorElement *etaNum = store->get(directories_[i]+"/rctEgammaEtaEffNum");
      MonitorElement *etaDenom = store->get(directories_[i]+"/rctEgammaEtaEffDenom");
      if(etaNum&&etaDenom)
	{
	  int bins = etaNum->getTH1F()->GetNbinsX();
	  double min  = etaNum->getTH1F()->GetXaxis()->GetXmin();
	  double max  = etaNum->getTH1F()->GetXaxis()->GetXmax();
	  MonitorElement *rctEgammaEtaEff = store->book1D("rctEgammaEtaEff","e/#gamma Efficiency vs #eta",bins,min,max);
	  rctEgammaEtaEff->getTH1F()->Divide(etaNum->getTH1F(),etaDenom->getTH1F(),1.,1.,"B");
	}

      MonitorElement *isoEtNum = store->get(directories_[i]+"/rctIsoEgammaEtEffNum");
      if(isoEtNum&&etDenom)
	{
	  int bins = isoEtNum->getTH1F()->GetNbinsX();
	  double min  = isoEtNum->getTH1F()->GetXaxis()->GetXmin();
	  double max  = isoEtNum->getTH1F()->GetXaxis()->GetXmax();
	  MonitorElement *rctIsoEgammaEtEff = store->book1D("rctIsoEgammaEtEff","Iso e/#gamma Efficiency vs ET",bins,min,max);
	  rctIsoEgammaEtEff->getTH1F()->Divide(isoEtNum->getTH1F(),etDenom->getTH1F(),1.,1.,"B");
	}

      MonitorElement *isoEtaNum = store->get(directories_[i]+"/rctIsoEgammaEtaEffNum");
      if(isoEtaNum&&etaDenom)
	{
	  int bins = isoEtaNum->getTH1F()->GetNbinsX();
	  double min  = isoEtaNum->getTH1F()->GetXaxis()->GetXmin();
	  double max  = isoEtaNum->getTH1F()->GetXaxis()->GetXmax();
	  MonitorElement *rctIsoEgammaEtaEff = store->book1D("rctIsoEgammaEtaEff","Iso e/#gamma Efficiency vs #eta",bins,min,max);
	  rctIsoEgammaEtaEff->getTH1F()->Divide(isoEtaNum->getTH1F(),etaDenom->getTH1F(),1.,1.,"B");
	}

    }
}

RctHarvesting::~RctHarvesting()
{
   
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}




//--------------------------------------------------------
void RctHarvesting::beginRun(const edm::Run& r, const EventSetup& context) {

}

//--------------------------------------------------------
void RctHarvesting::beginLuminosityBlock(const LuminosityBlock& lumiSeg, 
				      const EventSetup& context) {
  
}

// ----------------------------------------------------------
void 
RctHarvesting::analyze(const Event& iEvent, const EventSetup& iSetup )
{  

}




//--------------------------------------------------------
void RctHarvesting::endLuminosityBlock(const LuminosityBlock& lumiSeg, 
				    const EventSetup& context) {
}
//--------------------------------------------------------
void RctHarvesting::endRun(const Run& r, const EventSetup& context){
  if(runOnEndRun_)
    harvest();

}
//--------------------------------------------------------
void RctHarvesting::endJob(){
  if(runOnEndJob_)
    harvest();
  
  return;
}

DEFINE_FWK_MODULE(RctHarvesting);


