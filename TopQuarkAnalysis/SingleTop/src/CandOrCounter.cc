/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: CandOrCounter.cc $ 
 */


#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "TopQuarkAnalysis/SingleTop/interface/CandOrCounter.h"


CandOrCounter::CandOrCounter(const edm::ParameterSet& iConfig){
  cand1_ = iConfig.getParameter<edm::InputTag>("src1");
  cand2_ = iConfig.getParameter<edm::InputTag>("src2");
  minNum_ = iConfig.getParameter<int>("minNumber");
  maxNum_ = iConfig.getParameter<int>("maxNumber");
}


bool CandOrCounter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup){

edm::Handle<edm::View<reco::Candidate> > cand1;
iEvent.getByLabel(cand1_,cand1);

edm::Handle<edm::View<reco::Candidate> > cand2;
iEvent.getByLabel(cand2_,cand2);

 int num = (int)(cand2->size() + cand1->size());  
 return ( (num >= minNum_) && (num <= maxNum_));
}

CandOrCounter::~CandOrCounter(){;}

DEFINE_FWK_MODULE( CandOrCounter );
