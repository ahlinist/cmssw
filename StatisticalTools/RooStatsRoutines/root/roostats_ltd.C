static const char* desc =
"=====================================================================\n"
"|                                                                    \n"
"|\033[1m        roostats_ltd.C  alpha                         \033[0m\n"
"|                                                                    \n"
"| Standard: RooStats-based c++ routine for 95% C.L. limit calculation\n"
"|                                                                    \n"
"| Model-independent: accepts any properly configured RooFit model    \n"
"|                                                                    \n"
"| Standalone: depends only on ROOT                                   \n"
"|                                                                    \n"
"|\033[1m Gena Kukartsev                                       \033[0m\n"
"|\033[1m Lorenzo Moneta (CLs core)                            \033[0m\n"
"|                                                                    \n"
"| September 2011: first version                                      \n"
"|                                                                    \n"
"=====================================================================\n"
"                                                                     \n"
"Prerequisites:                                                       \n"
"                ROOT version 5.30.00 or higher                       \n"
"                                                                     \n"
"                                                                     \n"
"                                                                     \n"
"The code should be compiled in ROOT:                                 \n"
"                                                                     \n"
"root -l                                                              \n"
"                                                                     \n"
".L roostats_ltd.C+                                                   \n"
"                                                                     \n"
"Usage:                                                               \n"
"                                                                     \n"
"Inputs:                                                              \n"
"                                                                     \n"
"For more details see                                                 \n"
"        https://twiki.cern.ch/twiki/bin/view/CMS/RooStatsLtd         \n"
"                                                                     \n"
"\033[1m       Note!                                           \033[0m\n"
"If you are running nonstandard ROOT environment, e.g. in CMSSW,      \n"
"you need to make sure that the RooFit and RooStats header files      \n"
"can be found since they might be in a nonstandard location.          \n"
"                                                                     \n"
"For CMSSW_4_2_0_pre8 and later, add the following line to your       \n"
"rootlogon.C:                                                         \n"
"      gSystem -> SetIncludePath( \"-I$ROOFITSYS/include\" );         \n";


#include <algorithm>

#include "TCanvas.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TUnixSystem.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"

#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooProdPdf.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooRandom.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/PointSetInterval.h"
#include "RooStats/ConfidenceBelt.h"
#include "RooStats/ProposalHelper.h"
#include "RooStats/HybridCalculator.h"
#include "RooStats/FrequentistCalculator.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/HypoTestPlot.h"
#include "RooStats/NumEventsTestStat.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/SimpleLikelihoodRatioTestStat.h"
#include "RooStats/RatioOfProfiledLikelihoodsTestStat.h"
#include "RooStats/MaxLikelihoodEstimateTestStat.h"
#include "RooStats/HypoTestInverter.h"
#include "RooStats/HypoTestInverterResult.h"
#include "RooStats/HypoTestInverterPlot.h"


class LimitResult; // container class for limit results

LimitResult limit( const char * method = "plr",
		   const char * inFileName = 0,
		   const char * workspaceName = 0,
		   const char * datasetName = 0 );

// ---> implementation below --------------------------------------------


class LimitResult{

  friend class LimitCalc;
  
public:
  LimitResult():
    _observed_limit(0),
    _observed_limit_error(0),
    _expected_limit(0),
    _low68(0),
    _high68(0),
    _low95(0),
    _high95(0),
    _cover68(0),
    _cover95(0){};

  // copy constructor
  LimitResult(const LimitResult & other):
    _observed_limit(other._observed_limit),
    _observed_limit_error(other._observed_limit_error),
    _expected_limit(other._expected_limit),
    _low68(other._low68),
    _high68(other._high68),
    _low95(other._low95),
    _high95(other._high95),
    _cover68(other._cover68),
    _cover95(other._cover95){}

  ~LimitResult(){};

  Double_t GetObservedLimit(){return _observed_limit;};
  Double_t GetObservedLimitError(){return _observed_limit_error;};
  Double_t GetExpectedLimit(){return _expected_limit;};

  Double_t GetOneSigmaLowRange(){return _low68;};
  Double_t GetOneSigmaHighRange(){return _high68;};
  Double_t GetOneSigmaCoverage(){return _cover68;};

  Double_t GetTwoSigmaLowRange(){return _low95;};
  Double_t GetTwoSigmaHighRange(){return _high95;};
  Double_t GetTwoSigmaCoverage(){return _cover95;};

  void Clear( void ){
    _observed_limit = 0;
    _observed_limit_error = 0;
    _expected_limit = 0;
    _low68 = 0;
    _high68 = 0;
    _low95 = 0;
    _high95 = 0;
    _cover68 = 0;
    _cover95 = 0;
  }
  
private:
  Double_t _observed_limit;
  Double_t _observed_limit_error;
  Double_t _expected_limit;
  Double_t _low68;
  Double_t _high68;
  Double_t _low95;
  Double_t _high95;
  Double_t _cover68;
  Double_t _cover95;

  void SetObservedLimit(Double_t limit){_observed_limit = limit;};
  void SetObservedLimitError(Double_t error){_observed_limit_error = error;};
  void SetExpectedLimit(Double_t limit){_expected_limit = limit;};
  void SetOneSigmaLowRange(Double_t band){_low68 = band;};
  void SetOneSigmaHighRange(Double_t band){_high68 = band;};
  void SetTwoSigmaLowRange(Double_t band){_low95 = band;};
  void SetTwoSigmaHighRange(Double_t band){_high95 = band;};

};



class LimitCalc{

public:

  // no public constructors - this is a singleton class
  // use static method LimitCalc::GetInstance() to get the instance
  ~LimitCalc();

  static LimitCalc * GetInstance(void){
    if (!mspInstance) mspInstance = new LimitCalc();
    return mspInstance;
  }

  LimitResult       GetClsLimit( int nPoiScanPoints,
				 int nToys,
				 bool printResult = true );

  RooWorkspace *    GetWorkspace(){ return mpWs;}
  RooAbsData *      GetData( void ){return mpData;};
  const RooArgSet * GetPoiSet( void );
  RooRealVar *      GetFirstPoi( void );
  Double_t          GetFirstPoiMin( void ){return GetFirstPoi()->getMin();};
  Double_t          GetFirstPoiMax( void ){return GetFirstPoi()->getMax();};

  RooAbsData *            LoadData( std::string datasetName );
  RooStats::ModelConfig * LoadModelConfig( std::string mcName );
  RooWorkspace *          LoadWorkspace( std::string wsFileName,
					 std::string wsName );

  void SetSeed(UInt_t seed);
  void SetInverterCalcType(int type){mInverterCalcType=type;};
  void SetTestStatType(int type){mTestStatType=type;};
  void SetSbModelConfig( RooStats::ModelConfig * pSbModelConfig );
  void SetBModelConfig( RooStats::ModelConfig * pBModelConfig );
  void SetData( RooAbsData * data );
  void SetWorkspace( RooWorkspace * data );
  void SetPlot(bool doPlot){mDoPlotHypoTestResult=doPlot;};
  void SetUseProof(bool useProof){mUseProof=useProof;};
  void SetOptimize(bool optimize){mOptimize=optimize;};
  void SetWriteResult(bool writeResult){mWriteResult=writeResult;};
  void SetNProofWorkers(int nProofWorkers){mNProofWorkers=nProofWorkers;};
  bool SetFirstPoiValue( double value );
  bool SetFirstPoiMax( double value );
  bool SetFirstPoiMin( double value );
  void SetTestMode( bool mode ){mTestMode = mode;};
  void SetRebuildSamplingDist( bool rebuild ){mRebuildSamplingDist = rebuild;};
  void SetNToysToRebuild( bool nToys ){mNToysToRebuild = nToys;};

private:

  LimitCalc();
  LimitCalc(const LimitCalc &); // stop default

  void init( UInt_t seed ); //  to be called by constructor

  Double_t GetRandom( std::string pdfName, std::string varName );
  Long64_t FindLowBoundary(std::vector<Double_t> * pCdf, Double_t value);
  Long64_t FindHighBoundary(std::vector<Double_t> * pCdf, Double_t value);

  LimitResult ComputeInverterLimit( bool useCls,
				    int nPoints,
				    double poiMin,  // use default is poimin >= poimax
				    double poiMax,
				    int nToys );

  // internal routine to run the inverter
  // uses: workspace, mpData, sbmodel, bmodel, 
  //       calc type, test stat type
  RooStats::HypoTestInverterResult * RunInverter( int nPoints,
						  double poiMin,
						  double poiMax, 
						  int nSbToys,
						  int nBToys,
						  bool useCls );
 
  // get the expected p-values for given quantiles
  // from hypotestinvresult
  Double_t GetExpectedPValue( RooStats::HypoTestInverterResult * pResult,
			      Int_t index,
			      Double_t nSigma );

  RooStats::MCMCInterval * GetMcmcInterval(double confLevel,
					   int nIter,
					   int nBurnIn,
					   double leftSideTailFraction,
					   int nBins);

  void PlotMcmcPosterior( std::string plotFileName );
  double PrintMcmcUpperLimit( std::string limitFileName = "" );

  Double_t RoundUpperBound(Double_t bound);


  // attributes
  bool mTestMode;
  int mInverterCalcType;
  int mTestStatType;

  // toy mc parameters
  int mNEventsPerToy; // ignored if S+B model PDF is extended

  // pointers whose objects we do not own
  RooStats::ModelConfig * mpSbModel;
  RooStats::ModelConfig * mpBModel;

  // data members (own those objects)
  RooWorkspace * mpWs;
  RooAbsData * mpData;
  RooStats::BayesianCalculator * mpBayesCalc;
  RooStats::SimpleInterval * mpSimpleInterval;

  // for Bayesian MCMC calculation
  RooStats::MCMCInterval * mpMcmcInterval;
  
  // for Feldman-Cousins Calculator
  RooStats::FeldmanCousins * mpFcCalc;

  // random numbers
  TRandom3 mRandom;

  // HypoTestInverter attributes (CLs, FC...)
  bool mDoPlotHypoTestResult;
  bool mUseProof;
  bool mOptimize;
  bool mWriteResult;
  int  mNProofWorkers;
  std::string mNuisPriorName;
  bool mRebuildSamplingDist;
  int  mNToysToRebuild;

  // pointer to class instance
  static LimitCalc * mspInstance;
};



LimitCalc * LimitCalc::mspInstance = 0;



// default constructor
LimitCalc::LimitCalc(){
  init(12345);
}



void LimitCalc::init(UInt_t randomSeed){

  // set test mode
  mTestMode = false;

  // toy MC settings
  mNEventsPerToy = 0;

  // 0 - freq, 1 - hybrid
  mInverterCalcType = 0;

  // 0 - SimpleLikelihood (LEP), 
  // 1 - Profile likelihood ratio (Tevatron),
  // 2 - Two-sided profile likelihood,
  // 3 - One-sided profile likelihood
  mTestStatType = 3;

  // workspace-related
  mpWs = 0;
  mpSbModel = 0;
  mpBModel = 0;
  mpData = 0;

  // Bayesian atttributes
  mpBayesCalc = 0;
  mpSimpleInterval = 0;
  mpMcmcInterval = 0;

  // Feldman-Cousins attributes
  mpFcCalc = 0;

  // set random seed
  SetSeed(randomSeed);

  // HypoTestInverter attributes (CLs, FC...)
  mDoPlotHypoTestResult =  false; 
  mUseProof =  false;
  mOptimize =  false;
  mWriteResult =  false;
  mNProofWorkers =  1;
  mNuisPriorName.clear();
  mRebuildSamplingDist = false;
  mNToysToRebuild = -1;

}


LimitCalc::~LimitCalc(){
  delete mpWs;
  delete mpData;
  delete mpBayesCalc;
  delete mpSimpleInterval;
  delete mpMcmcInterval;
  delete mpFcCalc;
}



void LimitCalc::SetSeed( UInt_t seed ){
  //
  // Set random seed. If 0, set unique random.
  //
  std::string _legend = "[LimitCalc::SetSeed]: ";

  if (seed == 0){
    mRandom.SetSeed();
    UInt_t _seed = mRandom.GetSeed();
    UInt_t _pid = gSystem->GetPid();
    std::cout << _legend << "random seed: " << _seed << std::endl;
    std::cout << _legend << "process ID: " << _pid << std::endl;
    _seed = 31*_seed+_pid;
    std::cout << _legend << "new random seed (31*seed+pid): " << _seed << std::endl;
    mRandom.SetSeed(_seed);
    
    // set RooFit random seed (it has a private copy)
    RooRandom::randomGenerator()->SetSeed(_seed);
  }
  else{
    std::cout << _legend 
	      << "random seed: " << seed 
	      << std::endl;
    mRandom.SetSeed(seed);
    
    // set RooFit random seed (it has a private copy)
    RooRandom::randomGenerator()->SetSeed(seed);
  }

  return;
}



const RooArgSet * LimitCalc::GetPoiSet( void ){
  //
  // Return a pointer to the set of parameters of interest
  // No ownership is transferred
  // Return null pinter if failed
  //
  if (!mpSbModel){
    return 0;
  }
  
  return mpSbModel->GetParametersOfInterest();
}



RooRealVar * LimitCalc::GetFirstPoi( void ){
  //
  // Return a pointer to the first parameters of interest
  // (often we only have one POI)
  // No ownership is transferred
  // Return null pinter if failed
  //

  const RooArgSet * p_poi_set = GetPoiSet();

  if (!p_poi_set){
    return 0;
  }

  RooAbsArg * first_poi = p_poi_set->first();

  return (RooRealVar *)first_poi;
}



bool LimitCalc::SetFirstPoiValue( double value ){
  //
  // Set value of the first POI
  // Return false if fail, true if success
  //
  RooRealVar * poi = GetFirstPoi();

  if (!poi) return false;

  poi->setVal(value);

  return true;
}



bool LimitCalc::SetFirstPoiMax( double value ){
  //
  // Set upper boundary of the range for the first POI
  // Return false if fail, true if success
  //
  RooRealVar * poi = GetFirstPoi();

  if (!poi) return false;

  poi->setMax(value);

  return true;
}



bool LimitCalc::SetFirstPoiMin( double value ){
  //
  // Set lower boundary of the range for the first POI
  // Return false if fail, true if success
  //
  RooRealVar * poi = GetFirstPoi();

  if (!poi) return false;

  poi->setMin(value);

  return true;
}



LimitResult LimitCalc::GetClsLimit( int nPoiScanPoints,
				    int nToys,
				    bool printResult ){
  //
  // Compute CLs limit
  //

  std::string _legend = "[LimitCalc::GetClsLimit]: ";

  LimitResult result;

  // check necessary components

  // check workspace
  if (!mpWs){
    std::cout << _legend 
	      << "workspace not found, no model config loaded, can't continue"
	      << std::endl;
    return result;
  }
  else{
    std::cout << _legend 
	      << "workspace found..."
	      << std::endl;
  }

  // check data
  if (!mpData){
    std::cout << _legend 
	      << "dataset not loaded, can't continue"
	      << std::endl;
    return result;
  }
  else{
    std::cout << _legend 
	      << "data found..."
	      << std::endl;
  }

  // check S+B model config
  if (!mpSbModel){
    std::cout << _legend 
	      << "S+B ModelConfig not loaded, can't continue"
	      << std::endl;
    return result;
  }
  else{
    std::cout << _legend 
	      << "S+B ModelConfig found..."
	      << std::endl;
  }

  // check S+B snapshot
  const RooArgSet * _ss = mpSbModel->GetSnapshot();
  if (!_ss){
    std::cout << _legend 
	      << "found no snapshot for " 
	      << "S+B ModelConfig, can't continue" << std::endl;
    return result;
  }
  else{
    std::cout << _legend 
	      << "S+B snapshot found..."
	      << std::endl;
  }
  delete _ss;

  // check B-only model config
  if (!mpBModel){
    std::cout << _legend 
	      << "B-only ModelConfig not loaded, can't continue"
	      << std::endl;
    return result;
  }
  else{
    std::cout << _legend 
	      << "B-only ModelConfig found..."
	      << std::endl;
  }

  // check B-only snapshot
  _ss = mpBModel->GetSnapshot();
  if (!_ss){
    std::cout << _legend 
	      << "found no snapshot for " 
	      << "B-only ModelConfig, can't continue" << std::endl;
  }
  else{
    std::cout << _legend 
	      << "B-only snapshot found..."
	      << std::endl;
  }
  delete _ss;


  /*
    bool useCls,
    int nPoints,
    double poiMin,  // use default is poimin >= poimax
    double poiMax,
    int nToys );
  */

  // default values define automatic adaptive scan (max<min)
  double poi_min = 0.0;
  double poi_max = -1.0;

  RooRealVar * poi = GetFirstPoi();

  if (!poi){
    std::cout << _legend 
	      << "no parameter of interest found, can't continue " 
	      << std::endl;
  }

  if (nPoiScanPoints > 0){
    poi_min = poi->getMin();
    poi_max = poi->getMax();
  }
  else{
    std::cout << _legend 
	      << "Non-positive number of scan points requested - will attempt an adaptive scan instead" 
	      << std::endl;
  }

  // run CLs calculation
  result=ComputeInverterLimit( true,
			       nPoiScanPoints,
			       poi_min,
			       poi_max,
			       nToys );

  if (printResult){
    std::cout << " observed limit: " << result.GetObservedLimit() << std::endl;
    std::cout << " observed limit uncertainty: " << result.GetObservedLimitError() << std::endl;
    std::cout << " expected limit (median): " << result.GetExpectedLimit() << std::endl;
    std::cout << " expected limit (-1 sig): " << result.GetOneSigmaLowRange() << std::endl;
    std::cout << " expected limit (+1 sig): " << result.GetOneSigmaHighRange() << std::endl;
    std::cout << " expected limit (-2 sig): " << result.GetTwoSigmaLowRange() << std::endl;
    std::cout << " expected limit (+2 sig): " << result.GetTwoSigmaHighRange() << std::endl;
  }

  return result;
}



RooStats::ModelConfig * 
LimitCalc::LoadModelConfig( std::string mcName ){
  //
  // Return a pointer to the ModelConfig in the loaded
  // workspace, or 0 if no workspace is loaded.
  // User does NOT take ownership.
  //
  // Also load the snapshot if available
  //

  std::string _legend = "[LimitCalc::LoadModelConfig]: ";

  if (!mpWs){
    std::cout << _legend 
	      << "workspace not found, no model config loaded"
	      << std::endl;
    return 0;
  }

  RooStats::ModelConfig * _mc = (RooStats::ModelConfig *)mpWs->obj(mcName.c_str());

  if (!_mc){
    std::cout << _legend 
	      << "ModelConfig "
	      << mcName
	      <<" not found"
	      << std::endl;
    return 0;
  }
  
  std::cout << _legend 
	    << "successfully loaded ModelConfig " 
	    << mcName << std::endl;
  
  // need to set workspace so ModelCOnfig knows where to find objects
  _mc -> SetWorkspace(*mpWs);
  
  // try to load parameter snapshot
  const RooArgSet * _ss = _mc->GetSnapshot();
  if (_ss){
    std::cout << _legend 
	      << "successfully loaded snapshot for " 
	      << "ModelConfig " << mcName << std::endl;
  }
  else{
    std::cout << _legend 
	      << "found no snapshot for " 
	      << "ModelConfig " << mcName << std::endl;
  }
  delete _ss;
  
  return _mc;
}



void
LimitCalc::SetSbModelConfig( RooStats::ModelConfig * pSbModel ){
  // 
  // Set class member mpSbModelConfig - the signal plus background model
  //

  std::string _legend = "[LimitCalc::SetSbModelConfig]: ";

  mpSbModel = pSbModel;

  if (!mpSbModel){
    std::cout << _legend 
	      << "signal+background model config not set - null pointer specified"
	      << std::endl;
  }

  return;
}



void
LimitCalc::SetBModelConfig( RooStats::ModelConfig * pBModel ){
  // 
  // Set class member mpBModelConfig - the background-only model
  //

  std::string _legend = "[LimitCalc::SetBModelConfig]: ";

  mpBModel = pBModel;

  if (!mpBModel){
    std::cout << _legend 
	      << "background-only model config not set - null pointer specified"
	      << std::endl;
  }

  return;
}



RooWorkspace * 
LimitCalc::LoadWorkspace( std::string wsFileName,
			  std::string wsName ){
  //
  // load a workspace from a file
  // a copy of the workspace is kept as a class member,
  // the input file is immediately closed
  //
  
  std::string _legend = "[LimitCalc::LoadWorkspace]: ";

  TFile * p_infile = new TFile(wsFileName.c_str(), "read");
  if (p_infile->IsZombie()){
    std::cout << _legend 
	      << "no file " << wsFileName << " found!"
	      << std::endl;
    return 0;
  }
  else{
    std::cout << _legend 
	      << "file " << wsFileName << " open"
	      << std::endl;
  }

  RooWorkspace * _ws = (RooWorkspace *)p_infile->Get(wsName.c_str());
  if (!_ws){
    std::cout << _legend 
	      << "no workspace " << wsName << " found!"
	      << std::endl;
    return 0;
  }
  else{
    std::cout << _legend 
	      << "workspace " << wsName << " loaded"
	      << std::endl;
  }

  // delete current workspace or null
  delete mpWs;

  mpWs = (RooWorkspace *)_ws->Clone();

  delete p_infile;

  // try to load S+B and B-only model configs and their snapshots
  SetSbModelConfig( LoadModelConfig("SbModel") );
  SetBModelConfig( LoadModelConfig("BModel") );
  
  return mpWs;
}




void
LimitCalc::SetData( RooAbsData * data ){
  //
  // Copy data object and point mpData to it.
  // The class takes ownership of the new object.
  //

  std::string _legend = "[LimitCalc::SetData]: ";

  if (!data){
    std::cout << _legend 
	      << "dataset not found!"
	      << std::endl;
    return;
  }

  RooAbsData * _data = (RooAbsData *)data->Clone();

  if (!_data){
    std::cout << _legend 
	      << "failed to copy dataset!"
	      << std::endl;
    return;
  }

  // delete current dataset (or null)
  delete mpData;

  mpData = _data;

  return;
}



void
LimitCalc::SetWorkspace( RooWorkspace * ws ){
  //
  // Copy workspace object and point mpWs to it.
  // The class takes ownership of the new object.
  //

  std::string _legend = "[LimitCalc::SetWorkspace]: ";

  if (!ws){
    std::cout << _legend 
	      << "workspace not found!"
	      << std::endl;
    return;
  }

  RooWorkspace * _ws = (RooWorkspace *)ws->Clone();

  if (!_ws){
    std::cout << _legend 
	      << "failed to copy workspace!"
	      << std::endl;
    return;
  }

  // delete current workspace, if any
  delete mpWs;

  mpWs = _ws;

  return;
}



RooAbsData *
LimitCalc::LoadData( std::string datasetName ){
  //
  // Load dataset from workspace by name, copy and point mpData to it.
  // Leave mpData unchanged if new data object not found.
  // The class takes ownership of the new object.
  // Return the pointer to the new object.
  //

  std::string _legend = "[LimitCalc::LoadData]: ";

  if (!mpWs){
    std::cout << _legend 
	      << "workspace not found, no data loaded"
	      << std::endl;
    return 0;
  }

  RooAbsData * _data = (RooAbsData *)mpWs->data(datasetName.c_str());
  if (!_data){
    std::cout << _legend 
	      << "dataset " << datasetName
	      << " not found, no data loaded"
	      << std::endl;
    return 0;
  }

  // clean up old data and copy new one
  delete mpData;
  mpData = (RooAbsData *)_data->Clone();

  std::cout << _legend 
	    << "dataset " << datasetName
	    << " loaded"
	    << std::endl;

  return mpData;
}



RooStats::MCMCInterval * 
LimitCalc::GetMcmcInterval(double conf_level,
			   int n_iter,
			   int n_burn,
			   double left_side_tail_fraction,
			   int n_bins){
  // use MCMCCalculator  (takes about 1 min)
  // Want an efficient proposal function, so derive it from covariance
  // matrix of fit
  
  RooFitResult * fit = mpWs->pdf("model")->fitTo(*mpData,RooFit::Save(),
					       RooFit::Verbose(kFALSE),
					       RooFit::PrintLevel(-1),
					       RooFit::Warnings(0),
					       RooFit::PrintEvalErrors(-1));
  RooStats::ProposalHelper ph;
  ph.SetVariables((RooArgSet&)fit->floatParsFinal());
  ph.SetCovMatrix(fit->covarianceMatrix());
  ph.SetUpdateProposalParameters(kTRUE); // auto-create mean vars and add mappings
  ph.SetCacheSize(100);
  RooStats::ProposalFunction* pf = ph.GetProposalFunction();
  
  RooStats::MCMCCalculator mcmc( *mpData, *mpSbModel );
  mcmc.SetConfidenceLevel(conf_level);
  mcmc.SetNumIters(n_iter);          // Metropolis-Hastings algorithm iterations
  mcmc.SetProposalFunction(*pf);
  mcmc.SetNumBurnInSteps(n_burn); // first N steps to be ignored as burn-in
  mcmc.SetLeftSideTailFraction(left_side_tail_fraction);
  mcmc.SetNumBins(n_bins);
  
  delete mpMcmcInterval;
  mpMcmcInterval = mcmc.GetInterval();

  return mpMcmcInterval;
}


void LimitCalc::PlotMcmcPosterior( std::string filename ){
  
  TCanvas c1("c1");
  RooStats::MCMCIntervalPlot plot(*mpMcmcInterval);
  plot.Draw();
  c1.SaveAs(filename.c_str());
  
  return;
}


double LimitCalc::PrintMcmcUpperLimit( std::string filename ){
  //
  // print out the upper limit on the first Parameter of Interest
  //

  RooRealVar * firstPOI = (RooRealVar*) mpSbModel->GetParametersOfInterest()->first();
  double _limit = mpMcmcInterval->UpperLimit(*firstPOI);
  cout << "\n95% upper limit on " <<firstPOI->GetName()<<" is : "<<
    _limit <<endl;

  if (filename.size()!=0){
    
    std::ofstream aFile;

    // append to file if exists
    aFile.open(filename.c_str(), std::ios_base::app);

    char buf[1024];
    sprintf(buf, "%7.6f", _limit);

    aFile << buf << std::endl;

    // close outfile here so it is safe even if subsequent iterations crash
    aFile.close();

  }

  return _limit;
}



Double_t 
LimitCalc::GetRandom( std::string pdfName,
		      std::string varName ){
  //
  // generates a random number using a pdf in the workspace
  //
  
  // generate a dataset with one entry
  RooDataSet * _ds = mpWs->pdf(pdfName.c_str())->generate(*mpWs->var(varName.c_str()), 1);

  Double_t _result = ((RooRealVar *)(_ds->get(0)->first()))->getVal();
  delete _ds;

  return _result;
}



Long64_t
LimitCalc::FindLowBoundary(std::vector<Double_t> * pCdf,
			   Double_t value){
  //
  // return number of elements which are < value with precision 1e-10
  //

  Long64_t result = 0;
  std::vector<Double_t>::const_iterator i = pCdf->begin();
  while( (*i<value) && fabs(*i-value)>1.0e-10 && (i!=pCdf->end()) ){
    ++i;
    ++result;
  }
  return result;
}



Long64_t
LimitCalc::FindHighBoundary(std::vector<Double_t> * pCdf,
			    Double_t value){
  //
  // return number of elements which are > value with precision 1e-10
  //

  Long64_t result = 0;
  std::vector<Double_t>::const_iterator i = pCdf->end();
  while(1){ // (*i<value) && (i!=pCdf->begin()) ){
    --i;
    if (*i>value && fabs(*i-value)>1.0e-10 ){
      ++result;
    }
    else break;
    if (i==pCdf->begin()) break;
  }
  return result;
}



Double_t LimitCalc::RoundUpperBound(Double_t bound){
  //
  // find a round upper bound for a floating point
  //
  Double_t power = log10(bound);
  Int_t int_power = power>0.0 ? (Int_t)power : (Int_t)(power-1.0);
  Int_t int_bound = (Int_t)(bound/pow(10,(Double_t)int_power) * 10.0 + 1.0);
  bound = (Double_t)(int_bound/10.0*pow(10,(Double_t)int_power));
  return bound;
}



Int_t banner(){
  //#define __NOBANNER // banner temporary off
#ifndef __NOBANNER
  std::cout << desc << std::endl;
#endif
  return 0 ;
}
static Int_t dummy_ = banner() ;





/////////////////////////////////////////////////////////////////////////
//
// CLs helper methods from Lorenzo Moneta
// This is the core of the CLs calculation
//


LimitResult
LimitCalc::ComputeInverterLimit( bool useCls,
				 int nPoints,
				 double poiMin,  // use default is poimin >= poimax
				 double poiMax,
				 int nToys )
{

  std::string _legend = "[LimitCalc::ComputeInverterLimit]: ";

/*

   Other Parameter to pass in tutorial
   apart from standard for filename, ws, modelconfig and data

    type = 0 Freq calculator 
    type = 1 Hybrid 

    testStatType = 0 LEP
                 = 1 Tevatron 
                 = 2 Profile Likelihood
                 = 3 Profile Likelihood one sided (i.e. = 0 if mu < mu_hat)

    useCLs          scan for CLs (otherwise for CLs+b)    

    npoints:        number of points to scan , for autoscan set npoints = -1 

    poimin,poimax:  min/max value to scan in case of fixed scans 
                    (if min >= max, try to find automatically)                           

    ntoys:         number of toys to use 

    extra options are available as global paramters of the macro. They are: 

    mPlotHypoTestResult   plot result of tests at each point (TS distributions) 
    mUseProof = true;
    mWriteResult = true;
    nworkers = 4;


   */

// FIXME:
  std::string suffix = "_test";

  // result
  //std::vector<Double_t> result;
  LimitResult result;

  // check that workspace is present
  if (!mpWs){
    std::cout << "No workspace found, null pointer" << std::endl;
    return result;
  }
  
  RooStats::HypoTestInverterResult * r = 0;
  RooStats::HypoTestInverterResult * r2 = 0;
  
  // FIXME: terrible hack to check appending results
  if (suffix.find("merge")!=std::string::npos){
    std::string resFile = "Freq_CLs_grid_ts2_test_1.root";
    std::string resFile2 = "Freq_CLs_grid_ts2_test_2.root";
    std::string resName = "result_xsec";
    //std::cout << "Reading an HypoTestInverterResult with name " << resName << " from file " << resFile << std::endl;
    TFile * file = new TFile(resFile.c_str(), "read");
    TFile * file2 = new TFile(resFile2.c_str(), "read");
    r = dynamic_cast<RooStats::HypoTestInverterResult*>( file->Get(resName.c_str()) ); 
    r2 = dynamic_cast<RooStats::HypoTestInverterResult*>( file2->Get(resName.c_str()) ); 
    r->Add(*r2);
  }
  else{
    r = RunInverter( nPoints, poiMin, poiMax,  nToys, nToys, useCls );    
    if (!r) { 
      std::cerr << "Error running the HypoTestInverter - Exit " << std::endl;
      return result;
    }
  }
      		

   double upperLimit = r->UpperLimit();
   double ulError = r->UpperLimitEstimatedError();
   result.SetObservedLimit(upperLimit);
   result.SetObservedLimitError(ulError);

   const char *  limitType = (useCls) ? "CLs" : "Cls+b";
   const char * scanType = (nPoints < 0) ? "auto" : "grid";
   const char *  typeName = (mInverterCalcType == 0) ? "Frequentist" : "Hybrid";
   const char * resultName = (mpWs) ? mpWs->GetName() : r->GetName();
   TString plotTitle = TString::Format("%s CL Scan for workspace %s",typeName,resultName);

   /*
     RooStats::HypoTestInverterPlot *plot = new HypoTestInverterPlot("HTI_Result_Plot",plotTitle,r);
   TCanvas c1;
   //plot->Draw("CLb 2CL");  // plot all and Clb
   plot->Draw("2CL");  // plot all and Clb
   TString resultFileName = TString::Format("%s_%s_ts%d_scan_",limitType,scanType,mTestStatType);
   resultFileName += suffix;
   resultFileName += ".pdf";
   c1.SaveAs(resultFileName);

   if (mPlotHypoTestResult) { 
      TCanvas * c2 = new TCanvas();
      c2->Divide( 2, TMath::Ceil(nEntries/2));
      for (int i=0; i<nEntries; i++) {
         c2->cd(i+1);
         SamplingDistPlot * pl = plot->MakeTestStatPlot(i);
         pl->SetLogYaxis(true);
         pl->Draw();
      }
   }
   */

   Double_t q[5];
   q[0] = r->GetExpectedUpperLimit(0);
   q[1] = r->GetExpectedUpperLimit(-1);
   q[2] = r->GetExpectedUpperLimit(1);
   q[3] = r->GetExpectedUpperLimit(-2);
   q[4] = r->GetExpectedUpperLimit(2);
   result.SetExpectedLimit(q[0]);
   result.SetOneSigmaLowRange(q[1]);
   result.SetOneSigmaHighRange(q[2]);
   result.SetTwoSigmaLowRange(q[3]);
   result.SetTwoSigmaHighRange(q[4]);


   if (mpWs != NULL && mWriteResult) {

      // write to a file the results
      const char *  calcType = (mInverterCalcType == 0) ? "Freq" : "Hybr";
      //const char *  limitType = (useCls) ? "CLs" : "Cls+b";
      //const char * scanType = (nPoints < 0) ? "auto" : "grid";
      TString resultFileName = TString::Format("%s_%s_%s_ts%d_",calcType,limitType,scanType,mTestStatType);      
      //resultFileName += fileName;
      resultFileName += suffix;
      resultFileName += ".root";

      TFile * fileOut = new TFile(resultFileName,"RECREATE");
      r->Write();
      fileOut->Close();                                                                     
   }   

   // FIXME: get adaptive result out in some way
   if (nPoints <= 0 || poiMin >= poiMax){
     result.Clear();
     std::cout << _legend 
	       << "can't return the adaptive scan result yet, returning empty for now"
	       << std::endl;
   }

   return result;
}



Double_t
LimitCalc::GetExpectedPValue( RooStats::HypoTestInverterResult * pResult,
			      Int_t index,
			      Double_t nSigma ){
  //
  // Find a p-value in the expected p-value sampling distribution
  // that corresponds to a specified deviation from median,
  // for a POI value specified by index
  // Sampling distributions are taken from the provided result object
  //
  
  double p[1];
  double q[1];
  p[0] = ROOT::Math::normal_cdf(nSigma,1);
  RooStats::SamplingDistribution * s = pResult->GetExpectedPValueDist(index);
  const std::vector<double> & values = s->GetSamplingDistribution();
  double * x = const_cast<double *>(&values[0]); // cast for TMath::Quantiles
  TMath::Quantiles(values.size(), 1, x, q, p, false);
  delete s;

  return q[0];
}



// internal routine to run the inverter
// uses dataset from the class mpData
RooStats::HypoTestInverterResult * 
LimitCalc::RunInverter( int    npoints,
			double poimin,
			double poimax, 
			int    nSbToys,
			int    nBToys,
			bool   useCls ){

  std::string _legend = "[LimitCalc::RunInverter]: ";

  // check data
   if (!mpData) { 
     Error("LimitCalc::RunInverter","dataset not found");
     return 0;
   }
   
   // check model config
   if (!mpSbModel) {
     Error("LimitCalc::RunInverter","S+B ModelConfig does not exist...");
     return 0;
   }

   // check model pdf
   if (!mpSbModel->GetPdf()) { 
     Error("LimitCalc::RunInverter","S+B model has no pdf...");
     return 0;
   }

   // check POI
   if (!mpSbModel->GetParametersOfInterest()) {
     Error("LimitCalc::RunInverter","S+B model has no POI...");
     return 0;
   }

   // check S+B snapshot
   if (!mpSbModel->GetSnapshot() ) { 
      Info("LimitCalc::RunInverter","S+B model has no snapshot  - make one using model POI");
      mpSbModel->SetSnapshot( *mpSbModel->GetParametersOfInterest() );
   }

   // check B-only model config
   if (!mpBModel || mpBModel == mpSbModel) {
      Info("LimitCalc::RunInverter","The background model does not exist...");
      Info("LimitCalc::RunInverter","Copy it from S+B ModelConfig and set POI to zero");
      mpBModel = (RooStats::ModelConfig*) mpSbModel->Clone();
      mpBModel->SetName(TString("SbModel")+TString("_with_poi_0"));      
      RooRealVar * var = dynamic_cast<RooRealVar*>(mpBModel->GetParametersOfInterest()->first());
      if (!var) return 0;
      double oldval = var->getVal();
      var->setVal(0);
      mpBModel->SetSnapshot( RooArgSet(*var)  );
      var->setVal(oldval);
   }
   else { 
     // check B-only snapshot
     if (!mpBModel->GetSnapshot() ) { 
       Info("LimitCalc::RunInverter","B-only model has no snapshot  - make one using model poi and 0 values ");
       RooRealVar * var = dynamic_cast<RooRealVar*>(mpBModel->GetParametersOfInterest()->first());
       if (var) { 
	 double oldval = var->getVal();
	 var->setVal(0);
	 mpBModel->SetSnapshot( RooArgSet(*var)  );
	 var->setVal(oldval);
       }
       else { 
	 Error("LimitCalc::RunInverter","B-only model has no valid poi...");
	 return 0;
       }         
     }
   }


   RooStats::SimpleLikelihoodRatioTestStat slrts(*mpSbModel->GetPdf(),*mpBModel->GetPdf());
   if (mpSbModel->GetSnapshot()) slrts.SetNullParameters(*mpSbModel->GetSnapshot());
   if (mpBModel->GetSnapshot()) slrts.SetAltParameters(*mpBModel->GetSnapshot());

   // ratio of profile likelihood - need to pass snapshot for the alt
   RooStats::RatioOfProfiledLikelihoodsTestStat 
     ropl(*mpSbModel->GetPdf(), *mpBModel->GetPdf(), mpBModel->GetSnapshot());
   ropl.SetSubtractMLE(false);
   
   RooStats::ProfileLikelihoodTestStat profll(*mpSbModel->GetPdf());
   if (mTestStatType == 3) profll.SetOneSided(1);
   if (mOptimize){ 
      profll.SetReuseNLL(true);
      slrts.setReuseNLL(true);
   }

   RooRealVar * pMu = dynamic_cast<RooRealVar*>(mpSbModel->GetParametersOfInterest()->first());
   assert(pMu != 0);
   RooStats::MaxLikelihoodEstimateTestStat maxll(*mpSbModel->GetPdf(),*pMu); 

   RooStats::TestStatistic * testStat = &slrts;
   if (mTestStatType == 1) testStat = &ropl;
   if (mTestStatType == 2 || mTestStatType == 3) testStat = &profll;
   if (mTestStatType == 4) testStat = &maxll;
  
   
   RooStats::HypoTestCalculatorGeneric *  hc = 0;
   if (mInverterCalcType == 0) hc = new RooStats::FrequentistCalculator(*mpData, *mpBModel, *mpSbModel);
   else hc = new RooStats::HybridCalculator(*mpData, *mpBModel, *mpSbModel);

   RooStats::ToyMCSampler * toymcs = (RooStats::ToyMCSampler*)hc->GetTestStatSampler();
   if(mpSbModel->GetPdf()->canBeExtended()){
     // if the PDF is extended, number of events per toy
     // will be taken from the PDF normalization
     std::cout << _legend
	       << "will take number of entries per toy from S+B model PDF"
	       << std::endl;
   }
   else{
     // PDF is not extended

     if(mNEventsPerToy > 0){
       // number of event per toy was specified
       toymcs->SetNEventsPerToy(mNEventsPerToy);
       std::cout << _legend
		 << "number of entries per toy explicitely set to "
		 << mNEventsPerToy
		 << std::endl;
     }
     else if(mpData->numEntries()==1){
       toymcs->SetNEventsPerToy(1);
       std::cout << _legend
		 << "guessing that this must be a counting experiment, "
		 << std::endl
		 << _legend
		 << "number of entries per toy explicitely set to 1"
		 << std::endl
		 << _legend
		 << "if you wish otherwise, set it with SetNEventsPerToy()"
		 << std::endl;
     }
     else{
       std::cout << _legend
		 << "number of entries per toy is undefined"
		 << std::endl;
     }
   }

   toymcs->SetTestStatistic(testStat);
   if (mOptimize){
     // Lorenzo: works only of b pdf and s+b pdf are the same
     if (mpBModel->GetPdf() == mpSbModel->GetPdf() ) 
       toymcs->SetUseMultiGen(true);
   }


   if (mInverterCalcType == 1) { 
     RooStats::HybridCalculator *hhc = (RooStats::HybridCalculator*) hc;
      hhc->SetToys(nSbToys,nBToys); 

      // remove global observables from ModelConfig
      mpBModel->SetGlobalObservables( RooArgSet() );
      mpSbModel->SetGlobalObservables( RooArgSet() );

      // check for nuisance prior pdf in case of nuisance parameters 
      if (mpBModel->GetNuisanceParameters() || mpSbModel->GetNuisanceParameters() ){
	RooAbsPdf * pNuisPdf = 0; 
	if (mNuisPriorName.length()!=0) pNuisPdf = mpWs->pdf(mNuisPriorName.c_str());
	// use prior defined first in bModel (then in SbModel)
	if (!pNuisPdf)  { 
            Info("StandardHypoTestInvDemo","No nuisance pdf given for the HybridCalculator - try to use the prior pdf from the model");
            pNuisPdf = (mpBModel->GetPriorPdf() ) ?  mpBModel->GetPriorPdf() : mpSbModel->GetPriorPdf();
         }
         if (!pNuisPdf) { 
            Error("StandardHypoTestInvDemo","Cannnot run Hybrid calculator because no prior on the nuisance parameter is specified");
            return 0;
         }
         const RooArgSet * cpNuisParams = (mpBModel->GetNuisanceParameters() ) ? mpBModel->GetNuisanceParameters() : mpSbModel->GetNuisanceParameters();
         RooArgSet * pNp = pNuisPdf->getObservables(*cpNuisParams);
         if (pNp->getSize() == 0) { 
            Warning("StandardHypoTestInvDemo","Prior nuisance does not depend on nuisance parameters. They will be smeared in their full range");
         }
         delete pNp;
         hhc->ForcePriorNuisanceAlt(*pNuisPdf);
         hhc->ForcePriorNuisanceNull(*pNuisPdf);
      }
   } 
   else 
     ((RooStats::FrequentistCalculator*) hc)->SetToys(nSbToys,nBToys); 

   // Get the result
   RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);


   TStopwatch tw; tw.Start(); 
   const RooArgSet * poiSet = mpSbModel->GetParametersOfInterest();
   RooRealVar *poi = (RooRealVar*)poiSet->first();

   // fit the data first

   mpSbModel->GetPdf()->fitTo(*mpData, 
			      RooFit::Verbose(0),
			      RooFit::PrintLevel(-1),
			      RooFit::Warnings(0),
			      RooFit::PrintEvalErrors(-1));

   double poihat  = poi->getVal();

   RooStats::HypoTestInverter calc(*hc);
   calc.SetConfidenceLevel(0.95);

   calc.UseCLs(useCls);
   calc.SetVerbose(true);


   // can speed up using proof-lite
   if (mUseProof && mNProofWorkers > 1) { 
     RooStats::ProofConfig pc(*mpWs, mNProofWorkers, "", kFALSE);
     toymcs->SetProofConfig(&pc);    // enable proof
   }

   
   if (npoints > 0) {
      if (poimin >= poimax) { 
         // if no min/max given scan between MLE and +4 sigma 
         poimin = int(poihat);
         poimax = int(poihat +  4 * poi->getError());
      }
      std::cout << "Doing a fixed scan in interval : " << poimin << " , " << poimax << std::endl;
      calc.SetFixedScan(npoints,poimin,poimax);
   }
   else { 
     //poi->setMax(10*int( (poihat+ 10 *poi->getError() )/10 ) );
     std::cout << "Doing an  automatic scan in interval : " << poi->getMin() << " , " << poi->getMax() << std::endl;
   }

   RooStats::HypoTestInverterResult * r = 0;

   if (mTestMode){
     // test new functionality here
     
     // binary search for expected limit
     double _p = 0;
     double _cls = 0.05;
     double _precision = 0.005;
     double _sigma = 0.0;

     // establish starting bounds
     // FIXME: check that low and high cover the interval
     std::pair<double,double> _high;
     std::pair<double,double> _low;
     _high.first = GetFirstPoiMax();
     _low.first= GetFirstPoiMin();
     calc.RunOnePoint(_low.first);
     r = calc.GetInterval();
     _low.second = GetExpectedPValue(r, r->ArraySize()-1, _sigma);
     calc.RunOnePoint(_high.first);
     r = calc.GetInterval();
     _high.second = GetExpectedPValue(r, r->ArraySize()-1, _sigma);

     // begin binary search
     double _current_poi;
     while ( fabs(_p - _cls) > _precision ){
       // predict the next point
     std::cout << "DEBUG3******" << std::endl;
       std::cout << _low.first << std::cout;
       std::cout << _low.second << std::cout;
       std::cout << _high.first << std::cout;
       std::cout << _high.second << std::cout;
       _current_poi = _low.first + (_high.first-_low.first)*(_low.second-_cls)/(_low.second-_high.second);
       calc.RunOnePoint(_current_poi);
       r = calc.GetInterval();
       _p = GetExpectedPValue(r, r->ArraySize()-1, 0.0);
       if (_p > _cls){
	 _low.first = _current_poi;
	 _low.second = _p;
       }
       else{
	 _high.first = _current_poi;
	 _high.second = _p;
       }
     }

     calc.RunOnePoint(0.15);
     r = calc.GetInterval();
     std::cout << "ArraySize: " << r->ArraySize() << std::endl;
     std::cout << "CLsb: " << r->CLsplusb(0) << std::endl;
     std::cout << "CLb: " << r->CLb(0) << std::endl;
     std::cout << "CLs: " << r->CLs(0) << std::endl;

     //double _p = GetExpectedPValue(r, 0, nSigma );
     std::cout << "exp median: " << GetExpectedPValue(r, 0, 2) << std::endl;
     std::cout << "exp median: " << GetExpectedPValue(r, 0, 1) << std::endl;
     std::cout << "exp median: " << GetExpectedPValue(r, 0, 0) << std::endl;
     std::cout << "exp median: " << GetExpectedPValue(r, 0, -1) << std::endl;
     std::cout << "exp median: " << GetExpectedPValue(r, 0, -2) << std::endl;

     // stop here for testing
     return r;
   }

   r = calc.GetInterval();

   if (mRebuildSamplingDist) {
     calc.SetCloseProof(1);
     RooStats::SamplingDistribution * pLimDist = calc.GetUpperLimitDistribution(true,mNToysToRebuild);
     if (pLimDist) { 
       std::cout << "expected up limit " << pLimDist->InverseCDF(0.5) << " +/- " 
		 << pLimDist->InverseCDF(0.16) << "  " 
		 << pLimDist->InverseCDF(0.84) << "\n"; 
     }
     else 
       std::cout << "ERROR : failed to re-build distributions " << std::endl; 
   }

   //update r to a new re-freshed copied
   r = calc.GetInterval();

   return r; 
}



//--------> global functions --------------------------------------
//
LimitResult limit( const char * method,
		   const char * inFileName,
		   const char * workspaceName,
		   const char * datasetName ){
  //
  // Do one of the prepackaged limit calculations
  //
  std::string _legend = "[limit]: ";

  // instantiate calculator
  LimitCalc * pCalc = LimitCalc::GetInstance();

  // load workspace
  if (inFileName && workspaceName){
    pCalc->LoadWorkspace(inFileName, workspaceName);
  }

  // load dataset
  if (datasetName){
    pCalc->LoadData("observed_data");
  }

  LimitResult limitResult;
  if (!method || std::string(method).find("no_limit") != std::string::npos){
    std::cout << _legend
	      << "no limit calculation requested, doing nothing"
	      << std::endl;
  }
  else if (std::string(method).find("cls") != std::string::npos){
    limitResult = pCalc->GetClsLimit(0, 1000, true);
  }
  else if (std::string(method).find("plr") != std::string::npos){
    //limitResult = pCalc->GetPlrLimit(0, 1000, true);
    std::cout << _legend
	      << "profile likelihood ratio"
	      << std::endl;
  }
  else{
    std::cout << _legend
	      << "method " << method << "is unknown, exiting"
	      << std::endl;
    std::exit(-1);
  }

  return limitResult;
}



