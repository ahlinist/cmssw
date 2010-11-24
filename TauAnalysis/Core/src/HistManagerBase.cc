#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

const char* defaultTProfileOption = " ";
int defaultTProfileNumYbins = 1000;   // dummy value (required for technical reasons when calling DQMStore::bookProfile)
double defaultTProfileYmin = -1.e+38; // minimum for y-values entering TProfile histograms (value actually permissible even for float)
double defaultTProfileYmax = +1.e+38; // maximum for y-values entering TProfile histograms (value actually permissible even for float)

HistManagerBase::HistManagerBase(const edm::ParameterSet& cfg)
  : AnalyzerPluginBase(cfg),
    dqmStore_(0), 
    dqmError_(0),
    normMethod_(kNormUndefined) 
{
  //std::cout << "<HistManagerBase::HistManagerBase>:" << std::endl;

  dqmDirectory_store_  = dqmRootDirectory;
  dqmDirectory_store_ += cfg.getParameter<std::string>("dqmDirectory_store");
  dqmDirectory_store_  = dqmDirectoryName(dqmDirectory_store_);
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
  //std::cout << "<HistManagerBase::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  dqmStore_ = &(*edm::Service<DQMStore>());
  dqmStore_->setCurrentFolder(dqmDirectory_store_);
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
  
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

MonitorElement* HistManagerBase::book1D(DQMStore& dqmStore, const std::string& dqmDirectory,
					const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax, 
					bool sumWeights)
{
  if ( checkExistence(dqmStore, dqmDirectory, name) ) return 0;

  MonitorElement* me = dqmStore.book1D(name, title, numBinsX, xMin, xMax);
  if ( sumWeights ) me->getTH1()->Sumw2();

  return me;
}


MonitorElement* HistManagerBase::book1D(const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax)
{
  if ( !dqmError_ ) {
    return book1D(*dqmStore_, dqmDirectory_store_, name, title, numBinsX, xMin, xMax, sumWeightsTH1_);
  } else { 
    return 0;
  }
}

MonitorElement* HistManagerBase::book1D(DQMStore& dqmStore, const std::string& dqmDirectory,
					const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning,
					bool sumWeights)
{
  if ( checkExistence(dqmStore, dqmDirectory, name) ) return 0;

  MonitorElement* me = dqmStore.book1D(name, title, numBinsX, xBinning);
  if ( sumWeights ) me->getTH1()->Sumw2();

  return me;  
}

MonitorElement* HistManagerBase::book1D(const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning)
{
  if ( !dqmError_ ) {
    return book1D(*dqmStore_, dqmDirectory_store_, name, title, numBinsX, xBinning, sumWeightsTH1_);
  } else { 
    return 0;
  }
}

MonitorElement* HistManagerBase::book2D(DQMStore& dqmStore, const std::string& dqmDirectory,
					const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax, int numBinsY, double yMin, double yMax,
					bool sumWeights)
{
  if ( checkExistence(dqmStore, dqmDirectory, name) ) return 0;

  MonitorElement* me = dqmStore.book2D(name, title, numBinsX, xMin, xMax, numBinsY, yMin, yMax);
  if ( sumWeights ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book2D(const std::string& name, const std::string& title, 
					int numBinsX, double xMin, double xMax, int numBinsY, double yMin, double yMax)
{
  if ( !dqmError_ ) {
    return book2D(*dqmStore_, dqmDirectory_store_, name, title, numBinsX, xMin, xMax, numBinsY, yMin, yMax, sumWeightsTH2_);
  } else { 
    return 0;
  }
}

MonitorElement* HistManagerBase::book2D(DQMStore& dqmStore, const std::string& dqmDirectory,
					const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning, int numBinsY, float* yBinning,
					bool sumWeights)
{
  if ( checkExistence(dqmStore, dqmDirectory, name) ) return 0;

  MonitorElement* me = dqmStore.book2D(name, title, numBinsX, xBinning, numBinsY, yBinning);
  if ( sumWeights ) me->getTH1()->Sumw2();

  return me;
}

MonitorElement* HistManagerBase::book2D(const std::string& name, const std::string& title, 
					int numBinsX, float* xBinning, int numBinsY, float* yBinning)
{
  if ( !dqmError_ ) {
    return book2D(*dqmStore_, dqmDirectory_store_, name, title, numBinsX, xBinning, numBinsY, yBinning, sumWeightsTH2_);
  } else {
    return 0;
  }
}

MonitorElement* HistManagerBase::bookProfile1D(DQMStore& dqmStore, const std::string& dqmDirectory,
					       const std::string& name, const std::string& title, 
					       int numBinsX, double xMin, double xMax)
{
  if ( checkExistence(dqmStore, dqmDirectory, name) ) return 0;

  MonitorElement* me = dqmStore.bookProfile(name, title, numBinsX, xMin, xMax,
					    defaultTProfileNumYbins, defaultTProfileYmin, defaultTProfileYmax, defaultTProfileOption);
  
  return me;
}

MonitorElement* HistManagerBase::bookProfile1D(const std::string& name, const std::string& title, 
					       int numBinsX, double xMin, double xMax)
{
  if ( !dqmError_ ) {
    return bookProfile1D(*dqmStore_, dqmDirectory_store_, name, title, numBinsX, xMin, xMax);
  } else {
    return 0;
  }
}

MonitorElement* HistManagerBase::bookProfile1D(DQMStore& dqmStore, const std::string& dqmDirectory,
					       const std::string& name, const std::string& title, 
					       int numBinsX, float* xBinning)
{
  if ( checkExistence(dqmStore, dqmDirectory, name) ) return 0;

  double* xBinning_double = new double[numBinsX + 1];
  for ( int iBin = 0; iBin <= numBinsX; ++iBin ) {
    xBinning_double[iBin] = xBinning[iBin];
  }

  MonitorElement* me = dqmStore.bookProfile(name, title, numBinsX, xBinning_double, 
					    defaultTProfileNumYbins, defaultTProfileYmin, defaultTProfileYmax, defaultTProfileOption);

  delete[] xBinning_double;

  return me;
}

MonitorElement* HistManagerBase::bookProfile1D(const std::string& name, const std::string& title, 
					       int numBinsX, float* xBinning)
{
  if ( !dqmError_ ) {
    return bookProfile1D(*dqmStore_, dqmDirectory_store_, name, title, numBinsX, xBinning);
  } else {
    return 0;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(HistManagerPluginFactory, "HistManagerPluginFactory");
