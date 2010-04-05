// -*- C++ -*-
//
// Package:    RPCTriggerValidation
// Class:      RPCTriggerValidation
// 
/**\class RPCTriggerValidation RPCTriggerValidation.cc TTT/RPCTriggerValidation/src/RPCTriggerValidation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tomasz Maciej Frueboes
//         Created:  Wed Aug  5 16:03:51 CEST 2009
// $Id: RPCTriggerValidation.cc,v 1.9 2010/04/05 16:14:52 dbart Exp $
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

#include "Validation/L1Trigger/interface/RPCTriggerValidation.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "L1Trigger/RPCTrigger/interface/RPCConst.h"
#include <L1Trigger/RPCTrigger/interface/RPCLogCone.h>
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include <DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h>
#include "DataFormats/Math/interface/deltaR.h"
#include <DataFormats/MuonReco/interface/MuonTime.h>
#include <DataFormats/MuonReco/interface/Muon.h>


#include <algorithm>

using namespace RPCTriggerValidationStruct;

int RPCTriggerValidationStruct::MEResolution::_NumberOfQuality;




RPCTriggerValidation::RPCTriggerValidation(const edm::ParameterSet& iConfig) :
      m_l1CollectionsVec(iConfig.getParameter< std::vector< edm::InputTag > >("L1CandsCollections")),
      m_inColMC(iConfig.getParameter<edm::InputTag >("MCCollection")),
      m_outputDirectory(iConfig.getParameter<std::string >("outputDirectory")),
      m_outputFile(iConfig.getParameter<std::string>("outputFile")),
      deltaRThreshold(iConfig.getParameter<double>("deltaRThreshold")),
      m_L1MuonFromReco(iConfig.getParameter<bool>("L1MuonFromReco")),
      m_GlobalMuon(iConfig.getParameter<bool>("GlobalMuon")),
      m_StandAloneMuon(iConfig.getParameter<bool>("StandAloneMuon")),
      m_takeGMT(iConfig.getParameter<bool>("takeGMT"))
      
      //etaMin(iConfig.getParameter<double>("etaMin")),
      //etaMax(iConfig.getParameter<double>("etaMax"))
{
	RPCTriggerValidationStruct::MEResolution::_NumberOfQuality=iConfig.getParameter<int>("NumberOfQuality");
         // DQMStore * dqm = 0;
   dqm = edm::Service<DQMStore>().operator->();
   if ( !dqm ) {
      throw cms::Exception("RPCTriggerValidation") << "Cannot get DQMStore \n";
   }
   
   dqm->setCurrentFolder(m_outputDirectory);
   nomEta = dqm->book1D("nomEta","RPCTrigger: Efficiency vs  #eta",100,-2.5,2.5);
   denomEta = dqm->book1D("denomEta","RPCTrigger: Efficiency vs #eta - denom",100,-2.5,2.5);
   trig = dqm->book1D("Trig","RPCTrigger: Trigger ",100,1,0);
   alltrig = dqm->book1D("AllTrig","RPCTrigger: All Trigger ",100,1,0);
   ghost = dqm->book1D("ghost","RPCTrigger: ghost",11,-0.5,10.5);
   unassigned = dqm->book1D("unassigned","RPCTrigger: Unassigned L1s",11,-0.5,10.5);
              
   std::vector<edm::ParameterSet> etaPtRanges = iConfig.getParameter< std::vector<edm::ParameterSet> > ("etaPtRanges");          
   std::vector<edm::ParameterSet>::iterator it = etaPtRanges.begin(); 
   std::vector<edm::ParameterSet>::iterator itE = etaPtRanges.end(); 
   // cms.PSet = ( etaL=cms.Double(-2.2), etaH=cms.Double(2.2), ptL=cms.Double(9.80), ptH=cms.Double(10.2) ),
   for (;it!=itE;++it){
      //std::cout << it->getParameter<double>("etaL")   << std::endl; 
      _meResolutionVec.push_back( MEResolution(*it,dqm) );     
   
   }
   
   std::vector<edm::ParameterSet> PtRanges = iConfig.getParameter< std::vector<edm::ParameterSet> > ("PtRanges");          
   it = PtRanges.begin(); 
   itE =PtRanges.end(); 
  
   for (;it!=itE;++it){
      
      _meDistributionVec.push_back( MEDistribution(*it,dqm) );     
     _meTimingVec.push_back( METiming(*it,dqm) );
   }
   
      std::vector<edm::ParameterSet> EtaRanges = iConfig.getParameter< std::vector<edm::ParameterSet> > ("EtaRanges");          
   it = EtaRanges.begin(); 
   itE =EtaRanges.end(); 
  
   for (;it!=itE;++it){
      
      _meEfficiencyVec.push_back( MEEfficiency(*it,dqm) );     
   
   }
  
}


RPCTriggerValidation::~RPCTriggerValidation()
{
 

}


void RPCTriggerValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;
   

   Handle<CandidateView> mcHandle; // can hold reco::Muon or genParticle
   iEvent.getByLabel(m_inColMC, mcHandle);
	bool cut;

   std::vector<GenMuonLocalInfo> gens;
    //std::cout << "#######" << std::endl << std::endl;
   for(size_t i = 0; i < mcHandle->size(); ++i) {

      CandidateBaseRef ref = mcHandle->refAt(i);

	cut=( !m_GlobalMuon||ref->isGlobalMuon()) &&( !m_StandAloneMuon ||ref->isStandAloneMuon());

      if((ref->pdgId() == 13 || ref->pdgId() == -13)&& cut){
      //if (ref->pdgId() == 13 || ref->pdgId() == -13) {
         gens.push_back(GenMuonLocalInfo(ref,iEvent));
   //      std::cout << ref->pdgId() << " " << gens.rbegin()->charge() << std::endl;
      }
   }

   std::vector<L1MuonCandLocalInfo> l1s;
  if(m_L1MuonFromReco){
   edm::Handle<L1MuGMTReadoutCollection> pCollection;
     iEvent.getByLabel(*(m_l1CollectionsVec.begin()),pCollection);
  
    
  L1MuGMTReadoutCollection const* gmtrc = pCollection.product();
  vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
  vector<L1MuGMTReadoutRecord>::const_iterator RRItr;


  for( RRItr = gmt_records.begin() ;
       RRItr != gmt_records.end() ;
       RRItr++ ) 
  {
    
   if (m_takeGMT) {
     std::vector<L1MuGMTExtendedCand> gmtCANDS = RRItr->getGMTCands();
     std::vector<L1MuGMTExtendedCand>::const_iterator gmtIT = gmtCANDS.begin();
     for(; gmtIT != gmtCANDS.end(); ++gmtIT) 
     {
       L1MuonCandLocalInfo tt(*gmtIT);
       std::cout << " Found: " << tt << std::endl;
       ///if (tt.tower() >= 0 ) {
       l1s.push_back(tt);
       //}
     }

   
   }
   else 
   {
     vector<vector<L1MuRegionalCand> > brlAndFwdCands;
     brlAndFwdCands.push_back(RRItr->getBrlRPCCands());
     brlAndFwdCands.push_back(RRItr->getFwdRPCCands());

     vector<vector<L1MuRegionalCand> >::iterator RPCTFCands = brlAndFwdCands.begin();
     for(; RPCTFCands!= brlAndFwdCands.end(); ++RPCTFCands)
     {
      
        for( vector<L1MuRegionalCand>::const_iterator 
            ECItr = RPCTFCands->begin() ;
            ECItr != RPCTFCands->end() ;
            ++ECItr ) 
        {
        	if(!ECItr->empty()) l1s.push_back(L1MuonCandLocalInfo(*ECItr) );
        }
     }

  }

}

}
else{
   std::vector< edm::InputTag >::iterator it = m_l1CollectionsVec.begin();
   std::vector< edm::InputTag >::iterator itE = m_l1CollectionsVec.end();
   for (;it!=itE;++it){
      Handle<std::vector <L1MuRegionalCand>  > l1Handle;
      iEvent.getByLabel(*it, l1Handle);

      for( vector<L1MuRegionalCand>::const_iterator   itL1 = l1Handle->begin() ;
                                                      itL1 != l1Handle->end() ;
                                                    ++itL1 ) 
      {
        l1s.push_back(L1MuonCandLocalInfo(*itL1) );

      }


   }

}
 alltrig->Fill(iEvent.run(),l1s.size()); //l1s.size()

   assignCandidatesToGens(gens,l1s); 

   //std::cout << "Assigned L1s: " << std::endl;
   std::vector<GenMuonLocalInfo>::iterator itGen = gens.begin();
   std::vector<GenMuonLocalInfo>::iterator itGenE = gens.end();
   for (;itGen!=itGenE;++itGen)
   {
     
     
     std::vector<MEResolution>::iterator itRes =  _meResolutionVec.begin();
     std::vector<MEResolution>::iterator itResE =  _meResolutionVec.end();
     for (;itRes!=itResE; ++itRes){
      itRes->fill(*itGen);
     }
     
     std::vector<MEDistribution>::iterator itDis =  _meDistributionVec.begin();
     std::vector<MEDistribution>::iterator itDisE =  _meDistributionVec.end();
     for (;itDis!=itDisE; ++itDis){
      itDis->fill(*itGen);
     }

     std::vector<METiming>::iterator itTim =  _meTimingVec.begin();
     std::vector<METiming>::iterator itTimE =  _meTimingVec.end();
     for (;itTim!=itTimE; ++itTim){
      itTim->fill(*itGen);
     }
     
     std::vector<MEEfficiency>::iterator itEff =  _meEfficiencyVec.begin();
     std::vector<MEEfficiency>::iterator itEffE =  _meEfficiencyVec.end();
     for (;itEff!=itEffE; ++itEff){
      itEff->fill(*itGen);
     }
     
     
// 	if(itGen->_l1cands.size()>1){
//         std::cout << *itGen << std::endl;
// 	}

	if (itGen->_l1cands.size()>0){
	 nomEta->Fill(itGen->eta());
	 trig->Fill(iEvent.run());
	// if(itGen->eta()>etaMin && itGen->eta()<etaMax ) nomPt->Fill(itGen->pt());
	}
      denomEta->Fill(itGen->eta());
      //if(itGen->eta()>etaMin && itGen->eta()<etaMax ) denomPt->Fill(itGen->pt());
      ghost->Fill(itGen->_l1cands.size());
       /*
       std::vector<L1MuonCandLocalInfo>::iterator it = itGen->_l1cands.begin();
       std::vector<L1MuonCandLocalInfo>::iterator itE = itGen->_l1cands.end();
       for (;it!=itE;++it) {
         std::cout << *it << std::endl;

       }*/
   }
	unassigned->Fill(l1s.size());

  /*
     std::cout << "Unassigned L1s: " << std::endl;
     std::vector<L1MuonCandLocalInfo>::iterator it = l1s.begin();
     std::vector<L1MuonCandLocalInfo>::iterator itE =  l1s.end();
     for (;it!=itE;++it) {
        std::cout << *it << std::endl;
	
    }*/
  

}

namespace tmf {

  bool SortL1Candidates(const L1MuonCandLocalInfo & l1, 
                        const L1MuonCandLocalInfo & l2 ) 
  {

     return l1.ptCode() > l2.ptCode();

  }

  bool SortGens(const GenMuonLocalInfo & g1, 
                const GenMuonLocalInfo & g2) 
  {

     return g1.pt() > g2.pt();    

  } 
  
  bool SortAssignedL1(const L1MuonCandLocalInfo & l1, 
                        const L1MuonCandLocalInfo & l2 ) 
  {

     return l1.quality() > l2.quality();    

  } 


}
/// Assigns L1 candidates to GenMuonLocalInfo
/// TODO make this configurable (eg DR)
void RPCTriggerValidation::assignCandidatesToGens( std::vector<GenMuonLocalInfo> & gens, 
                                                   std::vector<L1MuonCandLocalInfo> & l1cands)
{


  //sort both vectors wrt pt
  std::sort(gens.begin(),gens.end(), tmf::SortGens);
  std::sort(l1cands.begin(), l1cands.end(), tmf::SortL1Candidates);

  // for each l1 cand find closeset genCandidate
  std::vector<L1MuonCandLocalInfo>::iterator it = l1cands.begin();
  
  std::vector<GenMuonLocalInfo>::iterator itGenMin =  gens.end();
    std::vector<GenMuonLocalInfo>::iterator itGen = gens.begin();
    std::vector<GenMuonLocalInfo>::iterator itGenE = gens.end();
  
  while (it!=l1cands.end())
  {
    double drMin = -1;
     itGenMin =  gens.end();
     itGen = gens.begin();
     itGenE = gens.end();
    for (;itGen!=itGenE;++itGen)
    {
       double dr = reco::deltaR(*itGen,*it);
       //std::cout << "))))))))))->" << drMin  << " " << dr << std::endl; 
       if (dr < drMin || drMin < 0) { 
         drMin = dr;
         itGenMin = itGen;
       }

    }
    // TODO check if there is no genCandidate close to choosen one, if so choose the one with proper pt
    // Problem:  {a,b} - L1 cands, {C,D} - Gens, what if both a and b got assigned to C? How to handle this
    // TODO make this configurable
    //  std::cout << "))))))))))->" << drMin << std::endl; 
    if (drMin < deltaRThreshold && drMin > 0 ) {
       itGenMin->_l1cands.push_back(*it);
       it = l1cands.erase(it);
    } else {
      ++it;
    }

    

  }
  
	itGen = gens.begin();
     itGenE = gens.end();
    for (;itGen!=itGenE;++itGen)
    {
       std::sort(itGen->_l1cands.begin(), itGen->_l1cands.end(), tmf::SortAssignedL1);
    }


}

void 
RPCTriggerValidation::beginJob()
{
}

void 
RPCTriggerValidation::endJob() {

  nomEta->getTH1F()->Divide((denomEta->getTH1F()));
  //nomPt->getTH1F()->Divide((denomPt->getTH1F()));
  
  std::vector<MEEfficiency>::iterator itEff =  _meEfficiencyVec.begin();
     std::vector<MEEfficiency>::iterator itEffE =  _meEfficiencyVec.end();
     for (;itEff!=itEffE; ++itEff){
      itEff->dev();
     }
    
    
     std::vector<MEDistribution>::iterator itDis =  _meDistributionVec.begin();
     std::vector<MEDistribution>::iterator itDisE =  _meDistributionVec.end();
     for (;itDis!=itDisE; ++itDis){
      itDis->dev();
     }
    
    if (m_outputFile.size() != 0 && dqm ) dqm -> save(m_outputFile);            
}


std::ostream& operator<<( std::ostream& os, const RPCTriggerValidationStruct::GenMuonLocalInfo& g1 ) {
              os << "Gen phi " << g1.phi()
                 << " eta "  << g1.eta()
                 << " pt " << g1.pt()
                 << " ch " << g1.charge();
              if (g1._l1cands.size()>0){
                 os << "   first assigned l1: " << *(g1._l1cands.begin());
              os << "   last assigned l1: " << *(g1._l1cands.rbegin());
              }
                
            
              
              return os;
}

std::ostream& operator<<( std::ostream& os, const RPCTriggerValidationStruct::L1MuonCandLocalInfo& l1 ) {
              os << "L1Cand phi " << l1.phi() 
                 << " eta "  << l1.eta()
                 << " bx "  << l1.bx()
                 << " ptCode " << l1.ptCode()
		<<" quality "<<l1.quality();
                // << " ch " << l1.charge()
                // << " tw " << l1.tower();
              return os;      
}


const std::string RPCTriggerValidationStruct::MEDistribution::_tag[4] = {
     "NoL1Muon","L1Muon","L1GhostMuon","RefMuon"
    }; 

DEFINE_FWK_MODULE(RPCTriggerValidation);
