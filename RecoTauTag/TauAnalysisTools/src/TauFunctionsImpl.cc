#include "RecoTauTag/TauAnalysisTools/interface/TauFunctionsImpl.h"

// Instatiate functions to make them available to the linker
#include "RecoTauTag/TauAnalysisTools/src/TauStringFuction.cc"
template class TauStringFunction<reco::Candidate>;
template class TauStringFunction<reco::PFTau>;
template class TauStringFunction<reco::CaloTau>;
template class TauStringFunction<reco::GenJet>;
template class TauStringFunction<reco::PFJet>;
template class TauStringFunction<reco::CaloJet>;
template class TauStringFunction<pat::Tau>;
template class TauStringFunction<reco::PFTauDecayMode>;

#include "RecoTauTag/TauAnalysisTools/src/TauMatchStringFunction.cc"
template class TauMatchStringFunction<reco::Candidate>;
template class TauMatchStringFunction<reco::PFTau>;
template class TauMatchStringFunction<reco::CaloTau>;
template class TauMatchStringFunction<reco::GenJet>;
template class TauMatchStringFunction<reco::PFJet>;
template class TauMatchStringFunction<reco::CaloJet>;
template class TauMatchStringFunction<pat::Tau>;

#include "RecoTauTag/TauAnalysisTools/src/TauDiscriminatorFunction.cc"
template class TauDiscriminatorFunction<reco::PFTau, reco::PFTauDiscriminator>;
template class TauDiscriminatorFunction<reco::CaloTau, reco::CaloTauDiscriminator>;

// Helper functions to determine type
namespace TauHelpers {

   void tauFuncNotSupported(std::string type, std::string message)
   {
      // Datatype not handled
      throw cms::Exception(message) << "Unsupported TauStringFunction datatype: " 
         << type << ", options are:\n" 
         << "PFTau\n" 
         << "CaloTau\n"
         << "Candidate\n"
         << "GenJet\n"
         << "PFJet\n"
         << "CaloJet\n"
         << "PATTau\n";
   }

   // Helper functions that return the correct function by type
   TauFunction* tauStringFuncFromType(const std::string &type, const std::string &expr)
   {
      if (type == "PFTau" )
         return new PFTauStringFunction(expr);
      if (type == "CaloTau")
         return new CaloTauStringFunction(expr);
      if (type == "Candidate")
         return new CandidateStringFunction(expr);
      if (type == "GenJet")
         return new GenJetStringFunction(expr);
      if (type == "PFJet")
         return new PFJetStringFunction(expr);
      if (type == "CaloJet")
         return new CaloJetStringFunction(expr);
      if (type == "PATTau")
         return new PATTauStringFunction(expr);

      tauFuncNotSupported(type, "BadDataTypeTauStringFn");
      return NULL;
   }

   TauFunction* tauMatchStringFuncFromType(const std::string &type, 
         const std::string &expr, const edm::Handle<reco::CandViewMatchMap> *matchView)
   {
      if (type == "PFTau" )
         return new MatchedToPFTauStringFunction(expr, matchView);
      if (type == "CaloTau")
         return new MatchedToCaloTauStringFunction(expr, matchView);
      if (type == "Candidate")
         return new MatchedToCandidateStringFunction(expr, matchView);
      if (type == "GenJet")
         return new MatchedToGenJetStringFunction(expr, matchView);
      if (type == "PFJet")
         return new MatchedToPFJetStringFunction(expr, matchView);
      if (type == "CaloJet")
         return new MatchedToCaloJetStringFunction(expr, matchView);
      if (type == "PATTau")
         return new MatchedToPATTauStringFunction(expr, matchView);

      tauFuncNotSupported(type, "BadDataTypeTauMatchedStringFn");
      return NULL;
   }
}
