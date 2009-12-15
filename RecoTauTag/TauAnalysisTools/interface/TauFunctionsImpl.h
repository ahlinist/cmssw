#ifndef RecoTauTag_TauAnalysisTools_TauFunctionImpl_h
#define RecoTauTag_TauAnalysisTools_TauFunctionImpl_h

/*
 * TauFunctionsImpl.h
 *
 * Author: Evan K. Friis, UC Davis (friis@physics.ucdavis.edu)
 *
 * Created: Mon Nov  9 14:15:09 PST 2009
 *
 * Implementation header of the various Tau function utilities in the
 * TauFunction family.
 *
 * Include this function to get convienent typedef access to the supported
 * types.
 * 
 * To add support for other types you must add the approrpriate information
 * to RecoTauTag/TauAnalysisTools/src/TauFunctionsImpl.cc which will make
 * the implementation available to the linker.
 *
 */

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "RecoTauTag/TauAnalysisTools/interface/TauDiscriminatorFunction.h"
#include "RecoTauTag/TauAnalysisTools/interface/TauStringFunction.h"
#include "RecoTauTag/TauAnalysisTools/interface/TauMatchStringFunction.h"
#include "RecoTauTag/TauAnalysisTools/interface/TauDecayModeFunction.h"

typedef TauStringFunction<reco::Candidate> CandidateStringFunction; 
typedef TauStringFunction<reco::PFTau>     PFTauStringFunction;     
typedef TauStringFunction<reco::CaloTau>   CaloTauStringFunction;   
typedef TauStringFunction<reco::GenJet>    GenJetStringFunction;    
typedef TauStringFunction<reco::PFJet>     PFJetStringFunction;     
typedef TauStringFunction<reco::CaloJet>   CaloJetStringFunction;   
typedef TauStringFunction<pat::Tau>        PATTauStringFunction;    

typedef TauMatchStringFunction<reco::Candidate> MatchedToCandidateStringFunction; 
typedef TauMatchStringFunction<reco::PFTau>     MatchedToPFTauStringFunction;     
typedef TauMatchStringFunction<reco::CaloTau>   MatchedToCaloTauStringFunction;   
typedef TauMatchStringFunction<reco::GenJet>    MatchedToGenJetStringFunction;    
typedef TauMatchStringFunction<reco::PFJet>     MatchedToPFJetStringFunction;     
typedef TauMatchStringFunction<reco::CaloJet>   MatchedToCaloJetStringFunction;   
typedef TauMatchStringFunction<pat::Tau>        MatchedToPATTauStringFunction;    

typedef TauDiscriminatorFunction<reco::PFTau, reco::PFTauDiscriminator> PFTauDiscriminatorFunction;
typedef TauDiscriminatorFunction<reco::CaloTau, reco::CaloTauDiscriminator> CaloTauDiscriminatorFunction;

// Forward declarations for helper functions to determine type
namespace TauHelpers {

   void tauFuncNotSupported(std::string type, std::string message);
   // Helper functions that return the correct function by type
   TauFunction* tauStringFuncFromType(const std::string &type, const std::string &expr);
   TauFunction* tauMatchStringFuncFromType(const std::string &type, 
         const std::string &expr, const edm::Handle<reco::CandViewMatchMap> *matchView);
}


#endif

