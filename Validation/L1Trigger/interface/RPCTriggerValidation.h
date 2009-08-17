#ifndef Validation_L1Trigger_RPCTriggerValidation_H
#define Validation_L1Trigger_RPCTriggerValidation_H
// -*- C++ -*-
//
// Package:    RPCTriggerValidation
// Class:      RPCTriggerValidation
// 
/**\class RPCTriggerValidation RPCTriggerValidation.h Validation/L1Trigger/interface/RPCTriggerValidation.h

 Description: Validation module for RPCTrigger

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tomasz Maciej Frueboes
//         Created:  Wed Aug  5 16:03:51 CEST 2009
// $Id: RPCTriggerValidation.h,v 1.1 2009/08/05 14:47:45 fruboes Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <vector>
//
// class decleration
//

#include <iosfwd>
#include <iostream>

class L1MuRegionalCand;

class RPCTriggerValidation : public edm::EDAnalyzer {
   public:
      explicit RPCTriggerValidation(const edm::ParameterSet&);
      ~RPCTriggerValidation();

      struct L1MuonCandLocalInfo {
         L1MuonCandLocalInfo(L1MuRegionalCand const & cand);
         private:
           int _charge;
           int _bx;
           int _ptCode;
           int _tower;
           float _phi;
           float _eta; 
         public:
           float eta() const {return _eta;};
           float phi() const {return _phi;};
           int ptCode() const {return  _ptCode;};
           int bx()  const {return _bx;};
           int charge()  const {return _charge;};    
           int tower() const {return _tower;};


      };

      struct GenMuonLocalInfo {
        GenMuonLocalInfo(reco::CandidateBaseRef ref) : _charge( ref->charge()), 
                                                       _pt( ref->pt()), 
                                                       _phi( ref->phi() ), 
                                                       _eta( - ref->eta()) {} ;
        private:
          int _charge;
          float _pt;
          float _phi; 
          float _eta;
        public:
          float eta() const {return _eta;};
          float phi() const {return _phi;};
          float pt() const {return _pt; };
          int charge() const {return _charge;};
     	  std::vector<L1MuonCandLocalInfo> _l1cands;





      };

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      void assignCandidatesToGens( std::vector<GenMuonLocalInfo> & gens, 
                                   std::vector<L1MuonCandLocalInfo> & l1cands);

      std::vector<edm::InputTag > m_l1CollectionsVec; 
      edm::InputTag m_inColMC;       /// input tag for reco::muon (DQMOffline) or genParticles (Validation)
      std::string m_outputDirectory; /// where to put DQM histos

};


std::ostream& operator<<( std::ostream& os, const RPCTriggerValidation::L1MuonCandLocalInfo& l1 );
std::ostream& operator<<( std::ostream& os, const RPCTriggerValidation::GenMuonLocalInfo& l1 );

#endif

