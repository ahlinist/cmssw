#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SusyAnalysis/EventSelector/interface/WeightSelector.h"

//________________________________________________________________________________________
WeightSelector::WeightSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), weightTag_(pset.getParameter<edm::InputTag> ("weightTag")) {

   // Define variables to store
   //defineVariable("PrescaleWeight");

}

//________________________________________________________________________________________
bool WeightSelector::select(const edm::Event& event) const {

   // Reset cached variables
   resetVariables();

   // Compute variables

   edm::Handle<double> event_weight;
   event.getByLabel(weightTag_, event_weight);
   double weight = (event_weight.isValid() ? (*event_weight) : 1.0);

   // Store variables
   //setVariable("PrescaleWeight", weight);

   // Return selection
   return (weight > 0.);

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, WeightSelector, "WeightSelector");
