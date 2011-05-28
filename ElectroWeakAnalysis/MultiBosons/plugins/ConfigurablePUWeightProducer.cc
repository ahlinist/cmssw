#ifndef ElectroWeakAnalysis_MultiBosons_ConfigurablePUWeightProducer_h
#define ElectroWeakAnalysis_MultiBosons_ConfigurablePUWeightProducer_h

#include <memory>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/PythonParameterSet/interface/BoostPython.h"

#include "TFile.h"
#include "TH1D.h"

namespace vgamma {
  
  class ConfigurablePUWeightProducer : public edm::EDProducer {
  public:

    explicit ConfigurablePUWeightProducer(const edm::ParameterSet& iConfig);    
    virtual ~ConfigurablePUWeightProducer();
    
    void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) ;
    
  private:
    edm::InputTag src_ ;
    std::map<std::string,std::vector<double> > mcNPUprobs_;
    std::map<std::string,std::pair<TH1D*, double> > puData_;
  };
  
  ConfigurablePUWeightProducer::ConfigurablePUWeightProducer(const edm::ParameterSet& iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")) {
    std::vector<std::string> psetNames;
    iConfig.getParameterSetNames(psetNames,true);      
    
    for( std::vector<std::string>::const_iterator iSet = psetNames.begin();
	 iSet != psetNames.end(); ++iSet){
      edm::ParameterSet conf = iConfig.getParameterSet(*iSet);
      
      edm::FileInPath dataNPUSrc_ = conf.getParameter<edm::FileInPath>("dataNPUSrc");
      std::vector<double> mcprobs = conf.getParameter<std::vector<double> >("mcNPUprobs");
      
      if ( dataNPUSrc_.location() != edm::FileInPath::Unknown ) {
	
	TH1D* PUdata_ = NULL;
	double normalization = 0.0;
	
	// get data estimated PU distribution
	TFile *temp = TFile::Open(dataNPUSrc_.fullPath().c_str());
	PUdata_ = (TH1D*)(temp->Get("pileup")->Clone());
	temp->Close();
	delete temp;
	
	//create normalization of MC PU distribution
	for(unsigned npu = 0; npu < mcNPUprobs_.size(); ++npu)
	  normalization += PUdata_->GetBinContent(PUdata_->GetXaxis()->FindBin(npu));
	
	puData_[*iSet] = std::make_pair(PUdata_,normalization);	
	mcNPUprobs_[*iSet] = mcprobs;
	produces<std::vector<double> >(*iSet);// this is the corresponding pileup weight w.r.t. the data given
      } else {
	throw cms::Exception("Configuration") << "File: " 
					      << dataNPUSrc_.relativePath() 
					      << " does not exist!" << std::endl;
      }
    }
  }
  
  ConfigurablePUWeightProducer::~ConfigurablePUWeightProducer() {  
    std::map<std::string,std::pair<TH1D*, double> >::const_iterator i;
    for(i = puData_.begin(); i != puData_.end(); ++i)
      delete i->second.first;
  }

  void ConfigurablePUWeightProducer::produce(edm::Event & iEvent, 
					     const edm::EventSetup & iSetup) {
    
    // read input collection
    edm::Handle<std::vector<PileupSummaryInfo> > infos;
    iEvent.getByLabel(src_, infos);
    
    //loop over pileup configurations
    std::map<std::string,std::pair<TH1D*, double> >::const_iterator i;
    for(i = puData_.begin(); i != puData_.end(); ++i){
      // create the product
      std::auto_ptr<std::vector<double> > product(new std::vector<double>);
      
      // prepare room for output
      product->reserve(infos->size());
      for ( std::vector<PileupSummaryInfo>::const_iterator It = infos->begin() ;
	    It != infos->end() ;
	    ++It ) {
	TH1D* h = i->second.first;

	int npu = It->getPU_NumInteractions();
	double weight = h->GetBinContent(h->GetXaxis()->FindBin(npu))/mcNPUprobs_[i->first][npu];  
     
	weight /= i->second.second; // normalize the weight to unity
	product->push_back(weight);
      }
        
      iEvent.put( product, i->first );
    }// loop over pileup configurations
  }
}

typedef vgamma::ConfigurablePUWeightProducer ConfigurablePUWeightProducer;

DEFINE_FWK_MODULE( ConfigurablePUWeightProducer );
#endif
