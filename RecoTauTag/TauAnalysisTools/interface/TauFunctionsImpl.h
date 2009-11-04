#ifndef RecoTauTag_TauAnalysisTools_TauFunctionImpl_h
#define RecoTauTag_TauAnalysisTools_TauFunctionImpl_h

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

typedef TauStringFunction<reco::Candidate> CandidateStringFunction; 
typedef TauStringFunction<reco::PFTau>     PFTauStringFunction;     
typedef TauStringFunction<reco::CaloTau>   CaloTauStringFunction;   
typedef TauStringFunction<reco::GenJet>    GenJetStringFunction;    
typedef TauStringFunction<pat::Tau>        PATTauStringFunction;

typedef TauMatchStringFunction<reco::Candidate> MatchedToCandidateStringFunction; 
typedef TauMatchStringFunction<reco::PFTau>     MatchedToPFTauStringFunction;     
typedef TauMatchStringFunction<reco::CaloTau>   MatchedToCaloTauStringFunction;   
typedef TauMatchStringFunction<reco::GenJet>    MatchedToGenJetStringFunction;    
typedef TauMatchStringFunction<pat::Tau>        MatchedToPatTauStringFunction;    

typedef TauDiscriminatorFunctionImpl<reco::PFTau, reco::PFTauDiscriminator> PFTauDiscriminatorFunction;
typedef TauDiscriminatorFunctionImpl<reco::CaloTau, reco::CaloTauDiscriminator> CaloTauDiscriminatorFunction;

#endif

