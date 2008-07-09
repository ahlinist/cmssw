#include "SusyAnalysis/EventSelector/interface/MetEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "SusyAnalysis/EventSelector/interface/uncorrectionTypeMET.h"

#include <vector>

MetEventSelector::MetEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset)  {
  // input collection
  metTag_ = pset.getParameter<edm::InputTag>("metTag");
  // lower cut on MET
  minMet_ = pset.getParameter<double>("minMET");

//   uncorrString_ = pset.getParameter<std::string>("uncorrType");

//   uncorrect_  = uncorrectionType(uncorrString_);
  // uncorrection type
  uncorrType_ = pat::uncorrectionTypeMET(pset.getParameter<std::string>("uncorrType"));


 // definition of the variable to be cached
  defineVariable("MET");

  edm::LogInfo("MetEventSelector") << "constructed with \n"
				   << "  metTag = " << metTag_ << "\n"
				   << "  minMET = " << minMet_;
}

// /// sorry ugly hack needs to be changed once pat::MET changes but still safer then to use indices
// bool MetEventSelector::uncorrectionType (const std::string& correctionName)
// {
//   if(correctionName == "uncorrALL"){
//     uncorrType =  pat::MET::UncorectionType(pat::MET::uncorrALL);
//     return true;
//   }
//   if(correctionName == "uncorrJES"){
//     uncorrType = pat::MET::UncorectionType(pat::MET::uncorrJES);
//     return true;
//   }
//   if(correctionName == "uncorrMUON"){
//     uncorrType = pat::MET::UncorectionType(pat::MET::uncorrMUON);
//     return true;
//   }
//   else { 
//     edm::LogInfo("MetEventSelector")<< "given uncorrection not valid return corrected MET";
//     return false; 
//   }
// }


bool
MetEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();
  //
  // get the MET result
  //
  edm::Handle< edm::View<pat::MET> > metHandle;
  event.getByLabel(metTag_, metHandle);
  if ( !metHandle.isValid() ) {
    edm::LogWarning("MetEventSelector") << "No Met results for InputTag " << metTag_;
    return false;
  }
  
 
  //
  // sanity check on collection
  //
  if ( metHandle->size()!=1 ) {
    edm::LogWarning("MetEventSelector") << "MET collection size is " 
					<< metHandle->size() << " instead of 1";
    return false;
  }

  //get the uncorrected/corrected MET
  float myMET = -10;
  
//   if(uncorrect_ == true) myMET = metHandle->front().uncorrectedPt(uncorrType);
//   else myMET = metHandle->front().et(); //if given string not known correct everything
  
  myMET = uncorrType_==pat::MET::uncorrMAXN ?
    metHandle->front().et() : metHandle->front().uncorrectedPt(uncorrType_);

  //
  // apply cut
  //
 
  setVariable(0,myMET);
  LogTrace("MetEventSelector") << "result = " << (myMET > minMet_);
  return myMET > minMet_;
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MetEventSelector, "MetEventSelector");

