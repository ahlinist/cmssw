#include "PhysicsTools/UtilAlgos/interface/CachingVariable.h"


#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
class L1BitComputer : public VariableComputer {
 public:
  L1BitComputer(CachingVariable::CachingVariableFactoryArg arg):VariableComputer(arg){
    src_=edm::Service<InputTagDistributorService>()->retrieve("src",arg.iConfig);
    for(int i = 0;i!=128;i++){
      std::stringstream ss;
      ss<<i;
      declare(ss.str());
    }

    for(int i = 0;i!=64;i++){
      std::stringstream ss;
      ss<<"TechTrig_";
      ss<<i;
      declare(ss.str());
    }


  }
    ~L1BitComputer(){};
    
    void compute(const edm::Event & iEvent) const{
      edm::Handle<L1GlobalTriggerReadoutRecord> l1Handle;
      iEvent.getByLabel(src_, l1Handle);
      if (l1Handle.failedToGet()) doesNotCompute();
      const DecisionWord & dWord = l1Handle->decisionWord();
      for(int i = 0;i!=128;i++){
	std::stringstream ss;
	ss<<i;
	double r=dWord.at(i);
	assign(ss.str(),r);
      }

      const TechnicalTriggerWord & tTWord = l1Handle->technicalTriggerWord();
      for(int i = 0;i!=64;i++){
	std::stringstream ss;
        ss<<"TechTrig_";
        ss<<i;
        double r=tTWord.at(i);
        assign(ss.str(),r);
      }

      
      
    }
 private:
    edm::InputTag src_;
};

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

class HLTBitComputer : public VariableComputer {
 public:
  HLTBitComputer(CachingVariable::CachingVariableFactoryArg arg):VariableComputer(arg){
    src_=edm::Service<InputTagDistributorService>()->retrieve("src",arg.iConfig);
    HLTConfigProvider provider;
    provider.init(src_.process());
    validTriggerNames_ =  provider.triggerNames();
    for (uint iT=0;iT!=validTriggerNames_.size();++iT){
      TString tname(validTriggerNames_[iT]);
      tname.ReplaceAll("HLT_","");//remove the "HLT_" prefix
      declare(std::string(tname));
    }
  }
    ~HLTBitComputer(){}
    void compute(const edm::Event & iEvent) const{
      edm::Handle<edm::TriggerResults> trh;
      iEvent.getByLabel(src_,trh);
      if (!trh.isValid()) doesNotCompute();
      triggerNames_.init(*trh);
      for (uint iT=0;iT!=validTriggerNames_.size();++iT){
	
	TString tname(validTriggerNames_[iT]);
	tname.ReplaceAll("HLT_","");
	double r=trh->accept(triggerNames_.triggerIndex(validTriggerNames_[iT]));
	assign(std::string(tname),r);
      }
      
    }
 private:
    edm::InputTag src_;
    std::vector<std::string> validTriggerNames_;
    mutable edm::TriggerNames triggerNames_;
};

class HLTBitVariable : public CachingVariable {
 public:
  HLTBitVariable(CachingVariableFactoryArg arg ) :
    CachingVariable("HLTBitVariable",arg.n,arg.iConfig),
    src_(edm::Service<InputTagDistributorService>()->retrieve("src",arg.iConfig)),
    bitName_(arg.n){ arg.m[arg.n]=this;}
    CachingVariable::evalType eval(const edm::Event & iEvent) const{
      edm::Handle<edm::TriggerResults> hltHandle;
      iEvent.getByLabel(src_, hltHandle);
      if (hltHandle.failedToGet()) return std::make_pair(false,0);
      edm::TriggerNames trgNames;
      trgNames.init(*hltHandle);
      unsigned int index = trgNames.triggerIndex(bitName_);
      if ( index==trgNames.size() ) return std::make_pair(false,0);
      else return std::make_pair(true, hltHandle->accept(index));

    }
 private:
  edm::InputTag src_;
  std::string bitName_;
};
