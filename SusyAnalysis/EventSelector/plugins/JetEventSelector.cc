#include "SusyAnalysis/EventSelector/interface/JetEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>
#include <sstream>

//________________________________________________________________________________________
JetEventSelector::JetEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  corrStep_(    pset.getParameter<std::string>("correction") ),
  corrFlavour_( pset.getParameter<std::string>("flavour")    ),
  minEt_(  pset.getParameter< std::vector<double> >("minEt"        ) ),
  maxEta_( pset.getParameter< std::vector<double> >("maxEta"       ) ),
  maxFem_( pset.getParameter< std::vector<double> >("maxEMFraction") )
{

  /// definition of variables to be cached
  defineVariable("NumberOfJets");
  for ( size_t i=0; i<minEt_.size(); ++i ) {
    std::ostringstream strEt;
    strEt << "Jet" << i << "Et";
    defineVariable(strEt.str());
    std::ostringstream strEta;
    strEta << "Jet" << i << "Eta";
    defineVariable(strEta.str());
    std::ostringstream strFem;
    strFem << "Jet" << i << "EMfraction";
    defineVariable(strFem.str());
  }

  edm::LogInfo("JetEventSelector") << "constructed with \n"
				   << "  jetTag    = " << jetTag_ << "\n"
				   << "  min #jets = " << minEt_.size();
}


//________________________________________________________________________________________
bool
JetEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();

  //FIXME: what about checking at construction time?
  if ( minEt_.size()!=maxEta_.size() ||
       maxFem_.size()!=maxEta_.size() ) {
    edm::LogError("JetEventSelector") << "Inconsistent length of vector of cut values";
    return false;
  }

  // get the jets
  edm::Handle<pat::JetCollection> jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("JetEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }
  //
  // check number of jets
  //
  setVariable(0,jetHandle->size());
  if ( jetHandle->size()<minEt_.size() )  return false;
  //
  // sort jets by corrected Et,
  // 
  std::vector<float> correctedEts;
  correctedEts.reserve(jetHandle->size());
  for ( size_t i=0; i<jetHandle->size(); ++i ) {
    const pat::Jet& jet = (*jetHandle)[i];
    float et = jet.et();
    std::string corrstep = corrStep_;
    if ( corrStep_ != jet.corrStep() || corrFlavour_ != jet.corrFlavour() )
      et *= jet.corrFactor( corrstep, corrFlavour_ );
    correctedEts.push_back(et);
  }
  std::vector<size_t> etSorted = 
    IndexSorter< std::vector<float> >(correctedEts,true)();
  //
  // check cuts (assume that jets are sorted by Et)
  //
  bool result(true);
  for ( unsigned int i=0; i<minEt_.size(); ++i ) {
    unsigned int j = etSorted[i];
    float EMFRAC=0;
    if ((*jetHandle)[j].isCaloJet()) EMFRAC=(*jetHandle)[j].emEnergyFraction();
    if ((*jetHandle)[j].isPFJet()) EMFRAC=(*jetHandle)[j].neutralEmEnergyFraction()+
      (*jetHandle)[j].chargedEmEnergyFraction();
    if ( correctedEts[j]<minEt_[i] ||
	 fabs((*jetHandle)[j].eta())>maxEta_[i]||
	 EMFRAC>maxFem_[i] ) {
      LogTrace("JetEventSelector") << "JetEventSelector: failed at jet " << (i+1);
      result = false;
     }
    setVariable(3*i+1,correctedEts[j]);
    setVariable(3*i+2,(*jetHandle)[j].eta());
    setVariable(3*i+3,EMFRAC);
   }
   LogTrace("JetEventSelector") << "JetEventSelector: all jets passed";
   return result;
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, JetEventSelector, "JetEventSelector");
