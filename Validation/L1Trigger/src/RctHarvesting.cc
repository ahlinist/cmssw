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

      MonitorElement *etNum2D = store->get(directories_[i]+"/rctEgammaEtEffNum2D");
      MonitorElement *etDenom2D = store->get(directories_[i]+"/rctEgammaEtEffDenom2D");
      if(etNum2D&&etDenom2D)
	{
	  int binsx = etNum2D->getTH2F()->GetNbinsX();
	  double minx  = etNum2D->getTH2F()->GetXaxis()->GetXmin();
	  double maxx  = etNum2D->getTH2F()->GetXaxis()->GetXmax();

	  int binsy = etNum2D->getTH2F()->GetNbinsY();
	  double miny  = etNum2D->getTH2F()->GetYaxis()->GetXmin();
	  double maxy  = etNum2D->getTH2F()->GetYaxis()->GetXmax();

	  MonitorElement *rctEgammaEtEff2D = store->book2D("rctEgammaEtEff2D","e/#gamma Efficiency vs ET",binsx,minx,maxx,binsy,miny,maxy);

	  for(int i=1;i<binsx;++i)
	    for( int j=1;j<binsy;++j)
	      rctEgammaEtEff2D->getTH2F()->SetBinContent(i,j,((float)etNum2D->getTH2F()->GetBinContent(i,j))/((float)etDenom2D->getTH2F()->GetBinContent(i,j)));
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

      MonitorElement *phiNum = store->get(directories_[i]+"/rctEgammaPhiEffNum");
      MonitorElement *phiDenom = store->get(directories_[i]+"/rctEgammaPhiEffDenom");
      if(phiNum&&phiDenom)
	{
	  int bins = phiNum->getTH1F()->GetNbinsX();
	  double min  = phiNum->getTH1F()->GetXaxis()->GetXmin();
	  double max  = phiNum->getTH1F()->GetXaxis()->GetXmax();
	  MonitorElement *rctEgammaPhiEff = store->book1D("rctEgammaPhiEff","e/#gamma Efficiency vs #phi",bins,min,max);
	  rctEgammaPhiEff->getTH1F()->Divide(phiNum->getTH1F(),phiDenom->getTH1F(),1.,1.,"B");
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

      MonitorElement *isoPhiNum = store->get(directories_[i]+"/rctIsoEgammaPhiEffNum");
      if(isoPhiNum&&phiDenom)
	{
	  int bins = isoPhiNum->getTH1F()->GetNbinsX();
	  double min  = isoPhiNum->getTH1F()->GetXaxis()->GetXmin();
	  double max  = isoPhiNum->getTH1F()->GetXaxis()->GetXmax();
	  MonitorElement *rctIsoEgammaPhiEff = store->book1D("rctIsoEgammaPhiEff","Iso e/#gamma Efficiency vs #eta",bins,min,max);
	  rctIsoEgammaPhiEff->getTH1F()->Divide(isoPhiNum->getTH1F(),phiDenom->getTH1F(),1.,1.,"B");
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


