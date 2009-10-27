#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

HistManagerBase::HistManagerBase(const edm::ParameterSet& cfg)
  : dqmStore_(0), 
    dqmError_(0),
    normMethod_(kNormUndefined) 
{
  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

//--- per default, compute bin-errors for one-dimensional, but not for two-dimensional histograms 
//    (bin-errors of two-dimensional histograms take two much space in .root file/memory and are not shown in plots)
//
//    NOTE: in case of events with negative weights 
//          (e.g. when using fake-rate technique for data-driven background estimation)
//          TH1::Sumw2 **must** be called before histogram filling,
//          because TH1::Sumw2 will fail in case of bins with negative contents,
//          causing the histogram not to be drawable
//
  sumWeightsTH1_ = ( cfg.exists("sumWeightsTH1") ) ? cfg.getParameter<bool>("sumWeightsTH1") : true;
  sumWeightsTH2_ = ( cfg.exists("sumWeightsTH2") ) ? cfg.getParameter<bool>("sumWeightsTH2") : false;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void HistManagerBase::bookHistograms()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  dqmStore_ = &(*edm::Service<DQMStore>());
  dqmStore_->setCurrentFolder(dqmDirectory_store_);

  bookHistogramsImp();
}

void HistManagerBase::fillHistograms(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{
  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  fillHistogramsImp(evt, es, evtWeight);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

MonitorElement* HistManagerBase::book1D(const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax)
{
  if ( dqmError_ ) return 0;

  MonitorElement* me = dqmStore_->book1D(name, title, numBinsX, xMin, xMax);
  if ( sumWeightsTH1_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book1D(const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning)
{
  if ( dqmError_ ) return 0;

  MonitorElement* me = dqmStore_->book1D(name, title, numBinsX, xBinning);
  if ( sumWeightsTH1_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book2D(const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax, int numBinsY, double yMin, double yMax)
{
  if ( dqmError_ ) return 0;

  MonitorElement* me = dqmStore_->book2D(name, title, numBinsX, xMin, xMax, numBinsY, yMin, yMax);
  if ( sumWeightsTH2_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book2D(const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning, int numBinsY, float* yBinning)
{
  if ( dqmError_ ) return 0;

  MonitorElement* me = dqmStore_->book2D(name, title, numBinsX, xBinning, numBinsY, yBinning);
  if ( sumWeightsTH2_ ) me->getTH1()->Sumw2();

  return me;
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(HistManagerPluginFactory, "HistManagerPluginFactory");
