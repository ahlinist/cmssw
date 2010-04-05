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
// $Id: RPCTriggerValidation.h,v 1.7 2010/03/29 18:05:59 dbart Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <L1Trigger/RPCTrigger/interface/RPCConst.h>
#include <L1Trigger/RPCTrigger/interface/RPCLogCone.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "RPCTriggerValidationStruct.h"
#include <DataFormats/MuonReco/interface/MuonTime.h>
#include <DataFormats/MuonReco/interface/Muon.h>



#include <vector>
#include <string>
//
// class decleration
//

#include <iosfwd>
#include <iostream>

using namespace RPCTriggerValidationStruct;

class L1MuRegionalCand;

class RPCTriggerValidation : public edm::EDAnalyzer {
   public:
      explicit RPCTriggerValidation(const edm::ParameterSet&);
      ~RPCTriggerValidation();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      void assignCandidatesToGens( std::vector<GenMuonLocalInfo> & gens, 
                                   std::vector<L1MuonCandLocalInfo> & l1cands);
				   
				   	
      MonitorElement * nomEta, * denomEta, * ghost, * unassigned,* nomPt, * denomPt,*alltrig,*trig;
      std::vector<MEResolution> _meResolutionVec; 
      std::vector<MEDistribution> _meDistributionVec;
      std::vector<METiming> _meTimingVec;
      std::vector<MEEfficiency> _meEfficiencyVec;
      

      
      std::vector<edm::InputTag > m_l1CollectionsVec; 
      edm::InputTag m_inColMC;       /// input tag for reco::muon (DQMOffline) or genParticles (Validation)
      std::string m_outputDirectory; /// where to put DQM histos
      std::string m_outputFile;
      DQMStore * dqm;
      double deltaRThreshold;
      
      bool m_L1MuonFromReco;
       
	bool m_GlobalMuon;
	bool m_StandAloneMuon;
	bool m_takeGMT;
};


std::ostream& operator<<( std::ostream& os, const RPCTriggerValidationStruct::L1MuonCandLocalInfo& l1 );
std::ostream& operator<<( std::ostream& os, const RPCTriggerValidationStruct::GenMuonLocalInfo& l1 );

#endif

