#include "SusyAnalysis/EventSelector/interface/BJetEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <vector>

//________________________________________________________________________________________
BJetEventSelector::BJetEventSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset) {
   // input collection
   jetTag_ = pset.getParameter<edm::InputTag> ("jetTag");
   // jet et and eta cuts
   minEt_ = pset.getParameter<double> ("minEt");
   maxEta_ = pset.getParameter<double> ("maxEta");
   // name of tagger
   tagLabel_ = pset.getParameter<std::string> ("tagLabel");
   // lower cuts on discriminator (defines also min. nr. of jets)
   minTag_ = pset.getParameter<std::vector<double> > ("minTag");

   /// definition of variables to be cached
   defineVariable("NumberOfJets");
   for (size_t i = 0; i < minTag_.size(); ++i) {
      std::ostringstream strTag;
      strTag << "Jet" << i << "Bdisc";
      defineVariable(strTag.str());
   }

   edm::LogInfo("BJetEventSelector") << "constructed with \n" << "  jetTag = " << jetTag_ << "\n" << "  tagger = "
            << tagLabel_ << "\n" << "  min #jets = " << minTag_.size();
}

//________________________________________________________________________________________
bool BJetEventSelector::select(const edm::Event& event) const {

   // Reset cached variables
   resetVariables();

   // Get the jets
   edm::Handle<edm::View<pat::Jet> > jetHandle;
   event.getByLabel(jetTag_, jetHandle);
   if (!jetHandle.isValid()) {
      edm::LogWarning("BJetEventSelector") << "No Jet results for InputTag " << jetTag_;
      return false;
   }

   // Get discriminator values
   std::vector<float> discriminators;
   discriminators.reserve(jetHandle->size());
   for (unsigned int i = 0; i < jetHandle->size(); ++i) {
      const pat::Jet& jet = (*jetHandle)[i];
      if (jet.et() > minEt_ && fabs(jet.eta()) < maxEta_)
         discriminators.push_back(jet.bDiscriminator(tagLabel_));
   }

   // Check number of jets (discriminators will have default value if too small)
   setVariable(0, discriminators.size());
   if (discriminators.size() < minTag_.size())
      return false;
   std::sort(discriminators.begin(), discriminators.end(), std::greater<float>());

   // Check selection
   bool result(true);
   for (unsigned int i = 0; i < minTag_.size(); ++i) {
      if (discriminators[i] < minTag_[i]) {
         LogDebug("BJetEventSelector") << "failed at jet " << (i + 1);
         result = false;
      }
      setVariable(i + 1, discriminators[i]);
   }
   LogDebug("BJetEventSelector") << "all jets passed";

   return result;

}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, BJetEventSelector, "BJetEventSelector");
