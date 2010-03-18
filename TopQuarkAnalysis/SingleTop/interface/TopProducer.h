#ifndef _Top_Producer_h
#define _Top_Producer_h



/**
 *\Class TopProducer
 *
 * \Author A. Orso M. Iorio
 * 
 *
 *\version  $Id: TopProducer.h,v 1.2 2010/03/18 11:34:07 oiorio Exp $
 *
 *
*/



#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/Run.h>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/ServiceRegistry/interface/Service.h" 


#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/PatCandidates/interface/UserData.h"
#include "PhysicsTools/PatAlgos/interface/PATUserDataHelper.h"


#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PATObject.h"

#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"

#include "TLorentzVector.h"
//#include "EquationSolver.h"


//class JetFlavourIdentifier;


namespace pat {

  class TopProducer : public edm::EDProducer {

    public:

      explicit TopProducer(const edm::ParameterSet & iConfig);
      ~TopProducer();
      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
    //       static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
    private:
    std::vector<TLorentzVector> Top4Momentum(const reco::Candidate & Lepton,const reco::Candidate & BJet,const reco::Candidate & MET);

    edm::InputTag electronsSrc_,muonsSrc_,jetsSrc_,METsSrc_;

    //Cuts for leptons
    double relIsoCutElectrons_,relIsoCutMuons_,jetMuonsIsoCut_;
    
    //Cuts for Jets
    double jetPtCorrectedCut_;

    //Cuts for BTags
    int nBtags_, nBtagVetos_;
    double bTagCutLoose_, bTagCutTight_;

    //Cuts for METs
    double metPt_;

    //Cuts for overall variables
    double wMtCut_; 
 



   //std::vector<std::string> triggernames;
      
  };
}


#endif
