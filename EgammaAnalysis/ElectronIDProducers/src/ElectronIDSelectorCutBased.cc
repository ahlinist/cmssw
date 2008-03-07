#include "EgammaAnalysis/ElectronIDProducers/interface/ElectronIDSelectorCutBased.h"

ElectronIDSelectorCutBased::ElectronIDSelectorCutBased (const edm::ParameterSet& conf) : conf_ (conf) 
{
  doPtdrId_ = conf_.getParameter<bool> ("doPtdrId");
  doCutBased_ = conf_.getParameter<bool> ("doCutBased");
  
  if (doPtdrId_) 
    ptdrAlgo_ = new PTDRElectronID ();
  if (doCutBased_) 
    cutBasedAlgo_ = new CutBasedElectronID ();

  if (doPtdrId_ && doCutBased_) 	 	     
     throw (std::runtime_error ("\n\nElectronIDSelectorCutBased: you cannot choose both PTDR algo and CutBased Algo.\n\n")) ;
}

ElectronIDSelectorCutBased::~ElectronIDSelectorCutBased () 
{
  if (doPtdrId_) 
    delete ptdrAlgo_ ;
  if (doCutBased_) 
    delete cutBasedAlgo_ ;
}

void ElectronIDSelectorCutBased::newEvent (const edm::Event& e, const edm::EventSetup& c)
{
  if (doPtdrId_) 
    ptdrAlgo_->setup (conf_);
  if (doCutBased_)  
    cutBasedAlgo_->setup (conf_);
}

double ElectronIDSelectorCutBased::operator () (const reco::GsfElectron & electron, const edm::Event& event) 
{
  if (doPtdrId_) 
  	return static_cast<double>(ptdrAlgo_->result (& (electron), event)) ;
  if (doCutBased_) 
  	return static_cast<double>(cutBasedAlgo_->result (& (electron), event)) ;

  return 0. ;
}
