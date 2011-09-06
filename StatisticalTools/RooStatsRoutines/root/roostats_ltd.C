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
  LimitCalc();
  LimitCalc( UInt_t randomSeed );
  ~LimitCalc();

  RooStats::ModelConfig * LoadModelConfig( std::string mcName );
  void SetSbModelConfig( RooStats::ModelConfig * pSbModelConfig );
  void SetBModelConfig( RooStats::ModelConfig * pBModelConfig );

  RooWorkspace * LoadWorkspace( std::string wsFileName,
				std::string wsName );
  RooWorkspace * GetWorkspace(){ return mpWs;}

  RooAbsData * GetData( std::string datasetName){return mpData;};
  void SetData( RooAbsData * data );
  RooAbsData * LoadData( std::string datasetName );

  // compute limits with HypoTestInverter
  // uses: workspace, sbmodel, bmodel,
  //       calc type, test stat type
  LimitResult ComputeInverterLimit( bool useCls,
				    int nPoints,
				    double poiMin,  // use default is poimin >= poimax
				    double poiMax,
				    int nToys );
private:

  void init( UInt_t seed ); //  to be called by constructor

  Double_t GetRandom( std::string pdfName, std::string varName );
  Long64_t FindLowBoundary(std::vector<Double_t> * pCdf, Double_t value);
  Long64_t FindHighBoundary(std::vector<Double_t> * pCdf, Double_t value);

  // internal routine to run the inverter
  // uses: workspace, mpData, sbmodel, bmodel, 
  //       calc type, test stat type
  RooStats::HypoTestInverterResult * RunInverter( int nPoints,
						  double poiMin,
						  double poiMax, 
						  int nToys,
						  bool useCls );
 
  RooStats::MCMCInterval * GetMcmcInterval(double confLevel,
					   int nIter,
					   int nBurnIn,
					   double leftSideTailFraction,
					   int nBins);

  void PlotMcmcPosterior( std::string plotFileName );
  double PrintMcmcUpperLimit( std::string limitFileName = "" );

  Double_t RoundUpperBound(Double_t bound);

  // attributes
  int mInverterCalcType;
  int mTestStatType;

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
  bool doPlotHypoTestResult;
  bool useProof;
  bool optimize;
  bool writeResult;
  int nProofWorkers;

};



// default constructor
LimitCalc::LimitCalc(){
  init(0);
}


LimitCalc::LimitCalc(UInt_t randomSeed){
  init(randomSeed);
}


void LimitCalc::init(UInt_t randomSeed){

  // 0 - freq, 1 - hybrid
  mInverterCalcType = 0;

  // 0 - SimpleLikelihood (LEP), 
  // 1 - Profile likelihood ratio (Tevatron),
  // 2 - One-sided profile likelihood,
  // 3 - Two-sided profile likelihood
  mTestStatType = 2;

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
  if (randomSeed == 0){
    mRandom.SetSeed();
    UInt_t _seed = mRandom.GetSeed();
    UInt_t _pid = gSystem->GetPid();
    std::cout << "[LimitCalc]: random seed: " << _seed << std::endl;
    std::cout << "[LimitCalc]: process ID: " << _pid << std::endl;
    _seed = 31*_seed+_pid;
    std::cout << "[LimitCalc]: new random seed (31*seed+pid): " << _seed << std::endl;
    mRandom.SetSeed(_seed);
    
    // set RooFit random seed (it has a private copy)
    RooRandom::randomGenerator()->SetSeed(_seed);
  }
  else{
    std::cout << "[LimitCalc]: random seed: " << randomSeed << std::endl;
    mRandom.SetSeed(randomSeed);
    
    // set RooFit random seed (it has a private copy)
    RooRandom::randomGenerator()->SetSeed(randomSeed);
  }

  // HypoTestInverter attributes (CLs, FC...)
  doPlotHypoTestResult =  false; 
  useProof =  false;
  optimize =  false;
  writeResult =  false;
  nProofWorkers =  1;

}


LimitCalc::~LimitCalc(){
  delete mpWs;
  delete mpData;
  delete mpBayesCalc;
  delete mpSimpleInterval;
  delete mpMcmcInterval;
  delete mpFcCalc;
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

  RooWorkspace * _ws = (RooWorkspace *)p_infile->Get(wsName.c_str());
  if (!_ws){
    std::cout << _legend 
	      << "no workspace " << wsName << " found!"
	      << std::endl;
    return 0;
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

  // delete current dataset (or null)
  delete mpData;

  mpData = (RooAbsData *)data->Clone();

  if (!mpData){
    std::cout << _legend 
	      << "no dataset found!"
	      << std::endl;

    mpData = 0;
  }

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
//std::vector<Double_t>
//GetClsLimits(RooWorkspace * pWs,
//	     const char * modelSBName,
//	     const char * modelBName,
//	     const char * dataName,
//	     int calculatorType,  // calculator type
//	     int testStatType, // test stat type
//	     bool useCls,
//	     int npoints,
//	     double poimin,  // use default is poimin >= poimax
//	     double poimax,
//	     int ntoys,
//	     std::string suffix)
{

  //
  // Return a vector of numbers (terrible design, I know) ordered as
  //  - observed limit
  //  - observed limit error
  //  - expected limit median
  //  - expected limit -1 sigma
  //  - expected limit +1 sigma
  //  - expected limit -2 sigma
  //  - expected limit +2 sigma
  //

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

    plotHypoTestResult   plot result of tests at each point (TS distributions) 
    useProof = true;
    writeResult = true;
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
    r = RunInverter( nPoints, poiMin, poiMax,  nToys, useCls );    
    if (!r) { 
      std::cerr << "Error running the HypoTestInverter - Exit " << std::endl;
      return result;
    }
  }
      		

   double upperLimit = r->UpperLimit();
   double ulError = r->UpperLimitEstimatedError();
   //result.push_back(upperLimit);
   //result.push_back(ulError);
   result.SetObservedLimit(upperLimit);
   result.SetObservedLimitError(ulError);


   //std::cout << "The computed upper limit is: " << upperLimit << " +/- " << ulError << std::endl;
 
   //   const int nEntries = r->ArraySize();

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

   if (plotHypoTestResult) { 
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
   //std::cout << " expected limit (median) " << q[0] << std::endl;
   //std::cout << " expected limit (-1 sig) " << q[1] << std::endl;
   //std::cout << " expected limit (+1 sig) " << q[2] << std::endl;
   //std::cout << " expected limit (-2 sig) " << q[3] << std::endl;
   //std::cout << " expected limit (+2 sig) " << q[4] << std::endl;
   result.SetExpectedLimit(q[0]);
   result.SetOneSigmaLowRange(q[1]);
   result.SetOneSigmaHighRange(q[2]);
   result.SetTwoSigmaLowRange(q[3]);
   result.SetTwoSigmaHighRange(q[4]);


   if (mpWs != NULL && writeResult) {

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

   return result;
}


// internal routine to run the inverter
// uses dataset from the class mpData
RooStats::HypoTestInverterResult * 
LimitCalc::RunInverter( int npoints, double poimin, double poimax, 
			int ntoys, bool useCls ){
//HypoTestInverterResult *  RunInverter(RooWorkspace * w, const char * modelSBName, const char * modelBName, 
//                                  const char * dataName, int type,  int mTestStatType, 
//                                  int npoints, double poimin, double poimax, 
//                                  int ntoys, bool useCls ) 
//{

  //std::cout << "Running HypoTestInverter on the workspace " << mpWs->GetName() << std::endl;

   //mpWs->Print();


   if (!mpData) { 
     Error("LimitCalc::RunInverter","dataset not found");
     return 0;
   }

   
   if (!mpSbModel) {
     Error("LimitCalc::RunInverter","S+B ModelConfig does not exist...");
     return 0;
   }
   // check the model 
   if (!mpSbModel->GetPdf()) { 
     Error("LimitCalc::RunInverter","S+B model has no pdf...");
     return 0;
   }
   if (!mpSbModel->GetParametersOfInterest()) {
     Error("LimitCalc::RunInverter","S+B model has no POI...");
     return 0;
   }
   if (!mpSbModel->GetSnapshot() ) { 
      Info("LimitCalc::RunInverter","S+B model has no snapshot  - make one using model POI");
      mpSbModel->SetSnapshot( *mpSbModel->GetParametersOfInterest() );
   }


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
   if (optimize) profll.SetReuseNLL(true);

   RooStats::TestStatistic * testStat = &slrts;
   if (mTestStatType == 1) testStat = &ropl;
   if (mTestStatType == 2 || mTestStatType == 3) testStat = &profll;
  
   
   RooStats::HypoTestCalculatorGeneric *  hc = 0;
   if (mInverterCalcType == 0) hc = new RooStats::FrequentistCalculator(*mpData, *mpBModel, *mpSbModel);
   else hc = new RooStats::HybridCalculator(*mpData, *mpBModel, *mpSbModel);

   RooStats::ToyMCSampler *toymcs = (RooStats::ToyMCSampler*)hc->GetTestStatSampler();
   // FIXME:
   toymcs->SetNEventsPerToy(1);
   toymcs->SetTestStatistic(testStat);
   if (optimize) toymcs->SetUseMultiGen(true);


   if (mInverterCalcType == 1) { 
     RooStats::HybridCalculator *hhc = (RooStats::HybridCalculator*) hc;
      hhc->SetToys(ntoys,ntoys); 

      // check for nuisance prior pdf 
      //if (mpBModel->GetPriorPdf() && mpSbModel->GetPriorPdf() ) {
      //   hhc->ForcePriorNuisanceAlt(*mpBModel->GetPriorPdf());
      //   hhc->ForcePriorNuisanceNull(*mpSbModel->GetPriorPdf());
      //}
      RooAbsPdf * nuis_prior =  mpWs->pdf("nuis_prior");
      if (nuis_prior ) {
         hhc->ForcePriorNuisanceAlt(*nuis_prior);
         hhc->ForcePriorNuisanceNull(*nuis_prior);
      }
      else {
         if (mpBModel->GetNuisanceParameters() || mpSbModel->GetNuisanceParameters() ) {
            Error("GetClsLimits","Cannnot run Hybrid calculator because no prior on the nuisance parameter is specified");
            return 0;
         }
      }
   } 
   else 
     ((RooStats::FrequentistCalculator*) hc)->SetToys(ntoys,ntoys); 

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
   if (useProof && nProofWorkers > 1) { 
     RooStats::ProofConfig pc(*mpWs, nProofWorkers, "", kFALSE);
     toymcs->SetProofConfig(&pc);    // enable proof
   }

   
   if (npoints > 0) {
      if (poimin >= poimax) { 
         // if no min/max given scan between MLE and +4 sigma 
         poimin = int(poihat);
         poimax = int(poihat +  4 * poi->getError());
      }
      //std::cout << "Doing a fixed scan in interval : " << poimin << " , " << poimax << std::endl;
      calc.SetFixedScan(npoints,poimin,poimax);
   }
   else { 
      //poi->setMax(10*int( (poihat+ 10 *poi->getError() )/10 ) );
     //std::cout << "Doing an  automatic scan in interval : " << poi->getMin() << " , " << poi->getMax() << std::endl;
   }

   RooStats::HypoTestInverterResult * r = calc.GetInterval();

   return r; 
}
