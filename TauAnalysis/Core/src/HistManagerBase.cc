#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

const char* defaultTProfileOption = " ";
int defaultTProfileNumYbins = 1000;   // dummy value (required for technical reasons when calling DQMStore::bookProfile)
double defaultTProfileYmin = -1.e+38; // minimum for y-values entering TProfile histograms (value actually permissible even for float)
double defaultTProfileYmax = +1.e+38; // maximum for y-values entering TProfile histograms (value actually permissible even for float)

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

bool checkExistence(const DQMStore& dqmStore, const std::string& dqmDirectory, const std::string& meName)
{
  //std::cout << "<checkExistence>:" << std::endl;

  std::string meName_full = dqmDirectoryName(dqmDirectory).append(meName);
  //std::cout << " meName_full = " << meName_full << std::endl;

  if ( dqmStore.get(meName_full) != 0 ) {
    edm::LogError ("checkExistence") << " Monitor element = " << meName << " does already exist --> skipping !!";
    return true;
  } else {
    return false;
  }
}

MonitorElement* HistManagerBase::book1D(const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax)
{
  if ( dqmError_ || checkExistence(*dqmStore_, dqmDirectory_store_, name) ) return 0;

  MonitorElement* me = dqmStore_->book1D(name, title, numBinsX, xMin, xMax);
  if ( sumWeightsTH1_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book1D(const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning)
{
  if ( dqmError_ || checkExistence(*dqmStore_, dqmDirectory_store_, name) ) return 0;

  MonitorElement* me = dqmStore_->book1D(name, title, numBinsX, xBinning);
  if ( sumWeightsTH1_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book2D(const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax, int numBinsY, double yMin, double yMax)
{
  if ( dqmError_ || checkExistence(*dqmStore_, dqmDirectory_store_, name) ) return 0;

  MonitorElement* me = dqmStore_->book2D(name, title, numBinsX, xMin, xMax, numBinsY, yMin, yMax);
  if ( sumWeightsTH2_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book2D(const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning, int numBinsY, float* yBinning)
{
  if ( dqmError_ || checkExistence(*dqmStore_, dqmDirectory_store_, name) ) return 0;

  MonitorElement* me = dqmStore_->book2D(name, title, numBinsX, xBinning, numBinsY, yBinning);
  if ( sumWeightsTH2_ ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::bookProfile1D(const std::string& name, const std::string& title, 
					       int numBinsX, double xMin, double xMax)
{
  if ( dqmError_ || checkExistence(*dqmStore_, dqmDirectory_store_, name) ) return 0;

  MonitorElement* me = dqmStore_->bookProfile(name, title, numBinsX, xMin, xMax,
					      defaultTProfileNumYbins, defaultTProfileYmin, defaultTProfileYmax, defaultTProfileOption);

  return me;
}

MonitorElement* HistManagerBase::bookProfile1D(const std::string& name, const std::string& title, 
					       int numBinsX, float* xBinning)
{
  if ( dqmError_ || checkExistence(*dqmStore_, dqmDirectory_store_, name) ) return 0;

  double* xBinning_double = new double[numBinsX + 1];
  for ( int iBin = 0; iBin <= numBinsX; ++iBin ) {
    xBinning_double[iBin] = xBinning[iBin];
  }

  MonitorElement* me = dqmStore_->bookProfile(name, title, numBinsX, xBinning_double, 
					      defaultTProfileNumYbins, defaultTProfileYmin, defaultTProfileYmax, defaultTProfileOption);

  delete[] xBinning_double;

  return me;
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(HistManagerPluginFactory, "HistManagerPluginFactory");
