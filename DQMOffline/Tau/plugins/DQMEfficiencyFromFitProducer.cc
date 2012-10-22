/** \class DQMEfficiencyFromFitProducer
 *  
 *  Class to produce efficiency histograms by dividing nominator by denominator histograms
 *
 *  \author Mauro Verzetti
 */

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TH1.h>
#include "TMath.h"
#include <TH2.h>
#include <TCanvas.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>	// for itoa() call
#include <map>
using namespace std;

//Roofit
#include "RooFitResult.h"
#include "RooRealVar.h"
using namespace RooFit ;
#include "DQMOffline/Tau/interface/FitterAlgos.h"

//Regexp handling
#include "classlib/utils/RegexpMatch.h"
#include "classlib/utils/Regexp.h"

template<typename FittingAlgo>
class DQMEfficiencyFromFitProducer : public edm::EDAnalyzer
{
  typedef vector<edm::ParameterSet> vpset;
 public:
  explicit DQMEfficiencyFromFitProducer(const edm::ParameterSet&);
  virtual ~DQMEfficiencyFromFitProducer();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(){}
  virtual void endRun(const edm::Run& r, const edm::EventSetup& c);

private:
  lat::Regexp* buildRegex(const string & expr);
  void fitSlices(const pair<MonitorElement *, vector<MonitorElement *> > & );
  TH1F* project(TH2*,int, string);

  string moduleLabel_;
  edm::InputTag streamTag_;
  string refIdentifier_;
  bool fit_, useBinomialErrors_;
  double scaleFactor_,maxChi2Prob_;
  bool fill1D_,fill2D_;
  int fitsPerformed_,goodFits_;
  //std::vector<MonitorElement*> histoEfficiencyVector_;
};

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename FittingAlgo>
DQMEfficiencyFromFitProducer<FittingAlgo>::DQMEfficiencyFromFitProducer(const edm::ParameterSet& iConfig):
  moduleLabel_(iConfig.getParameter<std::string>("@module_label")),
  fit_(iConfig.getParameter<bool>("fit")),
  fill1D_(iConfig.getParameter<bool>("fill1D")),
  fill2D_(iConfig.getParameter<bool>("fill2D")),
  refIdentifier_(iConfig.getParameter<std::string>("refIdentifier")),
  maxChi2Prob_(iConfig.getParameter<double>("maxChi2Prob")),
  fitsPerformed_(0),
  goodFits_(0)
{
  streamTag_ = iConfig.exists("streamTag") ? iConfig.getParameter<edm::InputTag>("streamTag") : edm::InputTag("");
  scaleFactor_ = iConfig.exists("scaleFactor") ? iConfig.getParameter<double>("scaleFactor") : -1.;
  useBinomialErrors_  = iConfig.exists("useBinomialErrors") ? iConfig.getParameter<bool>("useBinomialErrors") : false;
}

template<typename FittingAlgo>
DQMEfficiencyFromFitProducer<FittingAlgo>::~DQMEfficiencyFromFitProducer() 
{
//--- nothing to be done yet
}

template<typename FittingAlgo>
void DQMEfficiencyFromFitProducer<FittingAlgo>::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

template<typename FittingAlgo>
lat::Regexp* DQMEfficiencyFromFitProducer<FittingAlgo>::buildRegex(const string & expr)
{
  lat::Regexp* rx=NULL;
  try
  {
    rx = new lat::Regexp(expr, 0, lat::Regexp::Wildcard);
    rx->study();
  }
  catch (lat::Error &e)
  {
    edm::LogError("buildRegex")<< "Invalid regular expression "<< expr << ": " << e.explain().c_str();
  }
  return rx;
}

template<typename FittingAlgo>
TH1F* DQMEfficiencyFromFitProducer<FittingAlgo>::project(TH2* h2,int binx, string name)
{
  /*What? do you really need a projection function?
    Yes because the ROOT one is f**ing buggy! and if you put in a loop the only things that comes out is segfaults and will to kill...
   */
  const int nBinsX = h2->GetNbinsX();
  const int nBinsY = h2->GetNbinsY();
  double hmin = h2->GetYaxis()->GetBinLowEdge(1);
  double hmax = h2->GetYaxis()->GetBinLowEdge(nBinsY) + h2->GetYaxis()->GetBinWidth(nBinsX);
  TH1F* ret = new TH1F(name.c_str(),h2->GetTitle(),nBinsY,hmin,hmax);
  int entries = 0;
  for(int bin=1; bin<nBinsY; ++bin){
    ret->SetBinContent(bin,h2->GetBinContent(binx,bin) );
    entries += h2->GetBinContent(binx,bin);
    ret->SetBinError(bin,TMath::Sqrt(h2->GetBinContent(binx,bin)) );
  }
  ret->SetEntries(entries);
  return ret;
}


template<typename FittingAlgo>
void DQMEfficiencyFromFitProducer<FittingAlgo>::fitSlices(const pair<MonitorElement *, vector<MonitorElement *> > & plotSet)
{
  //--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be plotted !!";
    return;
  }
  RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

  DQMStore& dqmStore_ = (*edm::Service<DQMStore>());

  TH2* refRisto = plotSet.first->getTH2F();
  const int nBinsX = refRisto->GetNbinsX();
  const int nTargets = plotSet.second.size();
  Measurement1D outputs[nBinsX][nTargets];
  // cout <<moduleLabel_ << "  Computing Efficiencies..." << endl;
  // cout << "reference: " << plotSet.first->getFullname() << endl;
  // for(int mePos = 0; mePos < nTargets; ++mePos)
  //   cout << "     test: " << plotSet.second[mePos]->getFullname() << endl;

  for(int i=1; i<=nBinsX; i++){
    bool refFitIsGood = true;
    double refVal = 1;
    double refErr = 0;
    if(scaleFactor_ == -1){
      string refFullName = plotSet.first->getFullname();
      string hname = refFullName.substr(refFullName.rfind("/") + 1);
      TH1F *refprj = project(refRisto,i, Form("ref_prjx_%i",i));
      refprj->SetTitle((hname+ "_Bin_"+Form("%i",i)).c_str());
      //refprj->Sumw2();
      fitsPerformed_++;
      fitinfo reffitresult = FittingAlgo::Fit(refprj);
      refFitIsGood = true; 
      goodFits_+= (refFitIsGood) ? 1 : 0;
      refVal = (refFitIsGood) ? reffitresult.nsig : -1;
      refErr = (refFitIsGood) ? reffitresult.err  : 0;
      //refprj->Delete();*/
      delete refprj;
    }//if(fitWithScale_){

    for(int mePos = 0; mePos < nTargets; ++mePos){
      if(refFitIsGood){
	//cout << "Fitting: " << plotSet.second[mePos]->getFullname() << " bin: " << i << endl;
	TH2* thisHisto = plotSet.second[mePos]->getTH2F();
	string refFullName = plotSet.second[mePos]->getFullname();
	string hname = refFullName.substr(refFullName.rfind("/") + 1);
	TH1F *prj = project(thisHisto,i, Form("this%i_prjx_%i",mePos,i));
	prj->SetTitle((hname+ "_Bin_"+Form("%i",i)).c_str());
	//prj->Sumw2();
	fitsPerformed_++;
	fitinfo fitresult = FittingAlgo::Fit(prj);
	bool thisFitIsGood = true;
	goodFits_+= (thisFitIsGood) ? 1 : 0;
	double thisVal = (thisFitIsGood) ? fitresult.nsig   : -1;
	double thisErr = (thisFitIsGood) ? fitresult.err : 0;
	prj->Delete();
	double ratio = (thisFitIsGood && refVal != 0) ? thisVal/refVal : -1;
	double err   = (thisFitIsGood && ratio  >  0) ? (1./ratio)*TMath::Sqrt(TMath::Power(refErr/refVal,2) + TMath::Power(thisErr/thisVal,2)) : 0.;
	if(useBinomialErrors_)
	  err   = (thisFitIsGood && ratio  >  0) ? TMath::Sqrt( TMath::Abs( ( (1.-2.*ratio)*thisErr*thisErr + ratio*ratio*refErr*refErr )/(refVal*refVal) ) ) : 0.;  //Used binomial error formula as in previous version
	Measurement1D binVal(ratio,err);
	outputs[i-1][mePos] = binVal;
      }//if(refFitIsGood){
      else outputs[i-1][mePos] = Measurement1D(-1,0);
      }//for(int mePos = 0; mePos < nTargets; ++mePos){

  }//for(int i=1; i<=nBinsX; i++){

  double hmin = refRisto->GetXaxis()->GetBinLowEdge(1);
  double hmax = refRisto->GetXaxis()->GetBinLowEdge(nBinsX) + refRisto->GetBinWidth(nBinsX);

  //dealing with strings, regex would help
  string refFullName = plotSet.first->getFullname();
  string generalDir = refFullName.substr(0,refFullName.find("RecoTauV/") + 9); // 9 = size of "RecoTauV/"
  string other    =  refFullName.substr(refFullName.find("RecoTauV/") + 9);
  string ProdAndStream = other.substr(0,other.find("_"));
  string var = other.substr(other.rfind("_vs_")+4,other.rfind("TauVisible") - other.rfind("_vs_")-4);
  string meMapName =(ProdAndStream+"_EffMap"+var).c_str();

  /*cout <<  "refFullName " << refFullName << endl
       << "   generalDir " << generalDir << endl
       << "   other    " << other    << endl
       << "   ProdAndStream " <<ProdAndStream << endl
       << "   var " << var << endl
       << "   meMapName "<< meMapName << endl
       << "   nTargets " << nTargets << endl;*/

  dqmStore_.setCurrentFolder(generalDir.c_str());
  MonitorElement *h2d = (fill2D_) ? dqmStore_.book2D( meMapName.c_str(), meMapName.c_str(),nBinsX,hmin,hmax,nTargets,0,nTargets) : NULL;
  for(int mePos = 0; mePos < nTargets; ++mePos){
    string mepath = plotSet.second[mePos]->getFullname();
    string folder = mepath.substr(0,mepath.rfind("/"));
    string meName = mepath.substr(mepath.rfind("/"));
    string discName = (meName.rfind("By") != string::npos) ? meName.substr(meName.rfind("By") + 2,meName.rfind("_vs_")-meName.rfind("By")-2) : "PFJetMatching";
    string me1dName = (discName+"Eff"+var);

    /*cout <<  "   mepath " << mepath << endl;
      cout <<  "   folder " << folder << endl;
      cout <<  "   meName " << meName << endl;
      cout <<  "   discName " << discName << endl;
      cout <<  "   me1dName " << me1dName << endl;*/

    dqmStore_.setCurrentFolder(folder );

    MonitorElement *h1d = (fill1D_) ? dqmStore_.book1D( (discName+"Eff"+var).c_str(), (discName+"Eff"+var).c_str(),nBinsX,hmin,hmax) : NULL;
    if(fill2D_) h2d->getTH2F()->GetYaxis()->SetBinLabel(mePos+1,discName.c_str());
    for(int binn = 0; binn < nBinsX; binn++){
      if(fill2D_){
	h2d->getTH2F()->SetBinContent(binn+1,mePos+1,outputs[binn][mePos].value());
	h2d->getTH2F()->SetBinError(binn+1,mePos+1,outputs[binn][mePos].error());
      }
      if(fill1D_){
	h1d->getTH1()->SetBinContent(binn+1,outputs[binn][mePos].value());
	h1d->getTH1()->SetBinError(binn+1,outputs[binn][mePos].error());
      }
    }
  }

}

template<typename FittingAlgo>
void DQMEfficiencyFromFitProducer<FittingAlgo>::endRun(const edm::Run& r, const edm::EventSetup& c)
{
  //std::cout << "<DQMEfficiencyFromFitProducer::endJob>:" << std::endl;

  //--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be plotted !!";
    return;
  }

  DQMStore& dqmStore_ = (*edm::Service<DQMStore>());
  vector<MonitorElement *> allOurMEs = dqmStore_.getAllContents("RecoTauV/");

  //Reference and Targets 
  map<MonitorElement *, vector<MonitorElement *> > effMap;
  vector<MonitorElement *> dummy;

  string streamRegex = (streamTag_ == edm::InputTag("")) ? "" : "*"+streamTag_.label() ;
  lat::Regexp* refregex = buildRegex("*RecoTauV/"+streamRegex+"*"+refIdentifier_+"*");
  
  //Find all the reference plots
  for(vector<MonitorElement *>::const_iterator element = allOurMEs.begin(); element != allOurMEs.end(); ++element){
    string pathname = (*element)->getFullname();
    if(refregex->match(pathname)) effMap[(*element)] = dummy; //to give initial (empty) value
  }
  delete refregex;

  //find all the plots connected to the same reference
  for(map<MonitorElement *, vector<MonitorElement *> >::iterator entry = effMap.begin(); entry != effMap.end(); ++entry){
    string refName = (scaleFactor_ == -1) ? entry->first->getFullname() : "";
    const string strregex = refName.substr(0,refName.find(refIdentifier_) ) + "*/*" + refName.substr(refName.find(refIdentifier_)+refIdentifier_.size() ); 
    lat::Regexp* targetregex = buildRegex(strregex);
    for(vector<MonitorElement *>::const_iterator element = allOurMEs.begin(); element != allOurMEs.end(); ++element){
      string pathname = (*element)->getFullname();
      if(refregex->match(pathname)) effMap[entry->first].push_back((*element)); //to give initial (empty) value
    }
    delete targetregex;
  }

  for(map<MonitorElement *, vector<MonitorElement *> >::iterator entry = effMap.begin(); entry != effMap.end(); ++entry)
    fitSlices(*entry);

  cout << "Fits performed: " << fitsPerformed_ << ",  " << goodFits_ << "are good" << endl;
  return;
}

#include "FWCore/Framework/interface/MakerMacros.h"

typedef DQMEfficiencyFromFitProducer<FitterAlgoExpBW> TauDQMEffFromFitExpBWProducer;
DEFINE_FWK_MODULE(TauDQMEffFromFitExpBWProducer);

typedef DQMEfficiencyFromFitProducer<NoFit> TauDQMEffFromNoFitProducer;
DEFINE_FWK_MODULE(TauDQMEffFromNoFitProducer);
