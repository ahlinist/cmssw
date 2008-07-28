#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SusyAnalysis/EventSelector/interface/MuonEventSelector.h"

//________________________________________________________________________________________
MuonEventSelector::MuonEventSelector (const edm::ParameterSet& pset ) :
  SusyEventSelector(pset),
  muonTag_(pset.getParameter<edm::InputTag>       ("muonTag")),
  minPt_  (pset.getParameter<std::vector<double> >("minPt")),
  maxEta_ (pset.getParameter<std::vector<double> >("maxEta")),
  minIso_ (pset.getParameter<std::vector<double> >("minIso"))
{
  // Define variables to store
  // defineVariable("myVariable");
  defineVariable("NumberOfMuons");
  for(size_t i = 0; i < minPt_.size(); i++)
  {
    std::ostringstream strCharge;
    std::ostringstream strPt;
    std::ostringstream strEta;
    std::ostringstream strIso;

    strCharge << "Muon" << i << "Charge";
    strPt     << "Muon" << i << "Pt";
    strEta    << "Muon" << i << "Eta";
    strIso    << "Muon" << i << "Iso";
    defineVariable(strCharge.str());
    defineVariable(strPt.str    ());
    defineVariable(strEta.str   ());
    defineVariable(strIso.str   ());
  }

  edm::LogInfo("MuonEventSelector") << "constructed with " << std::endl
    << "muonTag     = " << muonTag_      << std::endl
    << "min # muons = " << minPt_.size() << std::endl;
}

//________________________________________________________________________________________
bool
MuonEventSelector::select (const edm::Event& event) const
{
  // Reset cached variables
  resetVariables();

  if(minPt_.size() != maxEta_.size() ||
     minPt_.size() != minIso_.size())
  {
    edm::LogError("MuonEventSelector") << "Inconsistent length of vector of cut values";

    return false;
  }

  // Get muons
  edm::Handle<std::vector<pat::Muon> > muonHandle;

  event.getByLabel(muonTag_, muonHandle);
  if(!muonHandle.isValid())
  {
    edm::LogWarning("MuonEventSelector") << "No muon results for InputTag " << muonTag_;

    return false;
  }

  // Compute variables

  // Store variables
  // setVariable("myVariable",myValue);

  // Cut on number of muons
  setVariable(0, muonHandle->size());
  if(muonHandle->size() < minPt_.size()) return false;

  // Cut on remaining variables
  bool result(true);

  for(unsigned int i = 0; i < minPt_.size(); i++)
  {
    if((*muonHandle)[i].pt() < minPt_[i] ||
       fabs((*muonHandle)[i].eta()) > maxEta_[i] ||
       (*muonHandle)[i].pt()/((*muonHandle)[i].pt() + (*muonHandle)[i].trackIso()) < minIso_[i])
    {
      LogTrace("MuonEventSelector") << "MuonEventSelector: failed at muon " << (i + 1);
      result = false;
    }

    setVariable(4*i + 1, (*muonHandle)[i].charge());
    setVariable(4*i + 2, (*muonHandle)[i].pt());
    setVariable(4*i + 3, (*muonHandle)[i].eta());
    setVariable(4*i + 4, (*muonHandle)[i].pt()/((*muonHandle)[i].pt() + (*muonHandle)[i].trackIso()));
  }

  // Return selection
  // return (myValue > minValue);
  if(result) LogTrace("MuonEventSelector") << "MuonEventSelector: all muons passed";

  return result;
}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"

DEFINE_EDM_PLUGIN(EventSelectorFactory, MuonEventSelector, "MuonEventSelector");

