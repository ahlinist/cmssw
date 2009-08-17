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
// $Id: RPCTriggerValidation.cc,v 1.1 2009/08/05 14:47:46 fruboes Exp $
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

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <algorithm>
RPCTriggerValidation::L1MuonCandLocalInfo::L1MuonCandLocalInfo(L1MuRegionalCand const & cand) :
  _charge(cand.chargeValue()),
  _bx(cand.bx()),
  _ptCode(cand.pt_packed() ),
  _tower(cand.eta_packed()),
  _phi(cand.phiValue()),
  _eta(cand.etaValue())
{

  _phi-=3.14159265;
}


RPCTriggerValidation::RPCTriggerValidation(const edm::ParameterSet& iConfig) :
      m_l1CollectionsVec(iConfig.getParameter< std::vector< edm::InputTag > >("L1CandsCollections")),
      m_inColMC(iConfig.getParameter<edm::InputTag >("MCCollection")),
      m_outputDirectory(iConfig.getParameter<std::string >("outputDirectory"))
{

}


RPCTriggerValidation::~RPCTriggerValidation()
{
 

}


void
RPCTriggerValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

   Handle<CandidateView> mcHandle; // can hold reco::Muon or genParticle
   iEvent.getByLabel(m_inColMC, mcHandle);

   std::vector<GenMuonLocalInfo> gens;

   std::cout << "#######" << std::endl << std::endl;
   for(size_t i = 0; i < mcHandle->size(); ++i) {
      CandidateBaseRef ref = mcHandle->refAt(i);
      if ( ref->pdgId() == 13 || ref->pdgId() == -13) {
         gens.push_back(GenMuonLocalInfo(ref));
   //      std::cout << ref->pdgId() << " " << gens.rbegin()->charge() << std::endl;
      }
   }

   
   std::vector<L1MuonCandLocalInfo> l1s;
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

   assignCandidatesToGens(gens,l1s); 

   std::cout << "Assigned L1s: " << std::endl;
   std::vector<GenMuonLocalInfo>::iterator itGen = gens.begin();
   std::vector<GenMuonLocalInfo>::iterator itGenE = gens.end();
   for (;itGen!=itGenE;++itGen)
   {
       std::cout << *itGen << std::endl;
       std::vector<L1MuonCandLocalInfo>::iterator it = itGen->_l1cands.begin();
       std::vector<L1MuonCandLocalInfo>::iterator itE = itGen->_l1cands.end();
       for (;it!=itE;++it) {
         std::cout << *it << std::endl;

       }
   }


  {
     std::cout << "Unassigned L1s: " << std::endl;
     std::vector<L1MuonCandLocalInfo>::iterator it = l1s.begin();
     std::vector<L1MuonCandLocalInfo>::iterator itE =  l1s.end();
     for (;it!=itE;++it) {
        std::cout << *it << std::endl;
     }
  }

}

namespace tmf {

  bool SortL1Candidates(const RPCTriggerValidation::L1MuonCandLocalInfo & l1, 
                        const RPCTriggerValidation::L1MuonCandLocalInfo & l2 ) 
  {

     return l1.ptCode() < l2.ptCode();

  }

  bool SortGens(const RPCTriggerValidation::GenMuonLocalInfo & g1, 
                const RPCTriggerValidation::GenMuonLocalInfo & g2) 
  {

     return g1.pt() < g2.pt();    

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
  while (it!=l1cands.end())
  {
    double drMin = -1;
    std::vector<GenMuonLocalInfo>::iterator itGenMin =  gens.end();
    std::vector<GenMuonLocalInfo>::iterator itGen = gens.begin();
    std::vector<GenMuonLocalInfo>::iterator itGenE = gens.end();
    for (;itGen!=itGenE;++itGen)
    {
       double dr = reco::deltaR(*itGen,*it);
       std::cout << "))))))))))->" << drMin  << " " << dr << std::endl; 
       if (dr < drMin || drMin < 0) { 
         drMin = dr;
         itGenMin = itGen;
       }

    }
    // TODO check if there is no genCandidate close to choosen one, if so choose the one with proper pt
    // Problem:  {a,b} - L1 cands, {C,D} - Gens, what if both a and b got assigned to C? How to handle this
    // TODO make this configurable
    std::cout << "))))))))))->" << drMin << std::endl; 
    if (drMin < 0.3 && drMin > 0 ) {
       itGenMin->_l1cands.push_back(*it);
       it = l1cands.erase(it);
    } else {
      ++it;
    }

    

  }


}

void 
RPCTriggerValidation::beginJob()
{
}

void 
RPCTriggerValidation::endJob() {
}


std::ostream& operator<<( std::ostream& os, const RPCTriggerValidation::GenMuonLocalInfo& g1 ) {
              os << "Gen phi " << g1.phi()
                 << " eta "  << g1.eta()
                 << " pt " << g1.pt()
                 << " ch " << g1.charge();
              return os;
}

std::ostream& operator<<( std::ostream& os, const RPCTriggerValidation::L1MuonCandLocalInfo& l1 ) {
              os << "L1Cand phi " << l1.phi() 
                 << " eta "  << l1.eta()
                 << " bx "  << l1.bx()
                 << " ptCode " << l1.ptCode()
                 << " ch " << l1.charge()
                 << " tw " << l1.tower();
              return os;      
}


DEFINE_FWK_MODULE(RPCTriggerValidation);
