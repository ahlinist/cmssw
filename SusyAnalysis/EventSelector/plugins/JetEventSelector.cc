#include "SusyAnalysis/EventSelector/interface/JetEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>
#include <sstream>

//________________________________________________________________________________________
JetEventSelector::JetEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  minPt_(  pset.getParameter< std::vector<double> >("minPt"        ) ),
  maxEta_( pset.getParameter< std::vector<double> >("maxEta"       ) ),
  minFem_( pset.getParameter< std::vector<double> >("minEMFraction") ),
  maxFem_( pset.getParameter< std::vector<double> >("maxEMFraction") ),
  minN90_( pset.getParameter<int>("minTowersN90")),
  minfHPD_(pset.getParameter<double>("minfHPD"))
{

  /// definition of variables to be cached
  defineVariable("NumberOfJets");
  for ( size_t i=0; i<minPt_.size(); ++i ) {
    std::ostringstream strPt;
    strPt << "Jet" << i << "Pt";
    defineVariable(strPt.str());
    std::ostringstream strEta;
    strEta << "Jet" << i << "Eta";
    defineVariable(strEta.str());
    std::ostringstream strFem;
    strFem << "Jet" << i << "EMfraction";
    defineVariable(strFem.str());
  }

  edm::LogInfo("JetEventSelector") << "constructed with \n"
				   << "  jetTag    = " << jetTag_ << "\n"
				   << "  min #jets = " << minPt_.size();
}


//________________________________________________________________________________________
bool
JetEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();

  //FIXME: what about checking at construction time?
  if ( minPt_.size()!=maxEta_.size() ||
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
  if ( jetHandle->size()<minPt_.size() )  return false;
  //
  // check cuts (assume that jets are sorted by Et)
  //
  bool result(false);
  unsigned int j =0;
  for ( unsigned int i=0; i<minPt_.size(); ++i ) {

    //not re-sorted at the momont!
    float EMFRAC=0;
    if ((*jetHandle)[i].isCaloJet()) EMFRAC=(*jetHandle)[i].emEnergyFraction();
    if ((*jetHandle)[i].isPFJet()) EMFRAC=(*jetHandle)[i].neutralEmEnergyFraction()+
      (*jetHandle)[i].chargedEmEnergyFraction();
    
    
    if((*jetHandle)[i].jetID().n90Hits <= minN90_)  continue;
    if((*jetHandle)[i].jetID().fHPD >= minfHPD_ ) continue;
    
    if (  (*jetHandle)[i].pt()>=minPt_[j] &&
	 fabs((*jetHandle)[i].eta())<=maxEta_[j] && 
	 ((EMFRAC<=maxFem_[i] && EMFRAC>=minFem_[j]) || fabs((*jetHandle)[i].eta()) > 2.6))      //check EMF only |eta|<2.6
      {
	setVariable(3*j+1,(*jetHandle)[i].pt());
	setVariable(3*j+2,(*jetHandle)[i].eta());
	setVariable(3*j+3,EMFRAC);
	++j;
      }
    if (j==minPt_.size()) {
      result=true;
      break;
    }

  }
  if (result) LogTrace("JetEventSelector") << "JetEventSelector: all jets passed";
  else        LogTrace("JetEventSelector") << "JetEventSelector: failed";
  return result;
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, JetEventSelector, "JetEventSelector");
