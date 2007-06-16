#include "RecoTauTag/PFConeIsolation/interface/PFConeIsolation.h"

void PFConeIsolation::produce(Event& iEvent, const EventSetup& iSetup){
  Handle<GenericJetCollection> theGenericJetCollection;
  iEvent.getByLabel(PFJetmodule_,theGenericJetCollection);
  GenericJetRefVector theGenericJets;
  for(unsigned int i_BJ=0;i_BJ!=theGenericJetCollection->size();i_BJ++) { 
    theGenericJets.push_back(GenericJetRef(theGenericJetCollection,i_BJ));
  }
  // *** access to the PFCandidate's through Event would be temporary, access to the same objects would be through 'PFJet'.
  Handle<PFCandidateCollection> thePFCandidateCollection;
  iEvent.getByLabel(PFCandidatemodule_,thePFCandidateCollection);
  PFCandidateRefVector thePFCands;
  for(unsigned int i_PFCand=0;i_PFCand!=thePFCandidateCollection->size();i_PFCand++) { 
    thePFCands.push_back(PFCandidateRef(thePFCandidateCollection,i_PFCand));
  }
  // ***
  
  // query a sim/rec PV
  Vertex::Error PVError;
  PVError(0,0)=smearedPVsigmaX_*smearedPVsigmaX_;
  PVError(1,1)=smearedPVsigmaY_*smearedPVsigmaY_;
  PVError(2,2)=smearedPVsigmaZ_*smearedPVsigmaZ_;
  Vertex::Point PVPoint(0,0,0);
  Handle<SimVertexContainer> G4VtxContainer;
  iEvent.getByType(G4VtxContainer);
  if (G4VtxContainer.isValid() && G4VtxContainer->size()) {
    Vertex::Point SimPVPoint(RandGauss::shoot(G4VtxContainer->begin()->position().x(),smearedPVsigmaX_),  
			     RandGauss::shoot(G4VtxContainer->begin()->position().y(),smearedPVsigmaY_),  
			     RandGauss::shoot(G4VtxContainer->begin()->position().z(),smearedPVsigmaZ_));
    PVPoint=SimPVPoint;
  }
  Vertex myPV(PVPoint,PVError,1,1,1);
  
  Handle<VertexCollection> vertices;
  iEvent.getByLabel(PVmodule_,vertices);
  const VertexCollection vertCollection=*(vertices.product());
  if(vertCollection.size()) myPV=*(vertCollection.begin());
  
  PFIsolatedTauTagInfoCollection* extCollection=new PFIsolatedTauTagInfoCollection();
  
  unsigned int iGJ=0;
  for(GenericJetRefVector::const_iterator i_GJ=theGenericJets.begin();i_GJ!=theGenericJets.end();i_GJ++) { 
    PFCandidateRefVector thefilteredPFCands;
    for(PFCandidateRefVector::const_iterator i_PFCand=thePFCands.begin();i_PFCand!=thePFCands.end();i_PFCand++) { 
      if (ROOT::Math::VectorUtil::DeltaR((**i_GJ).momentum(),(**i_PFCand).momentum())<PFJetConeSize_) thefilteredPFCands.push_back((*i_PFCand));
    }
    GenericJetRef theGenericJetRef(theGenericJetCollection,iGJ);
    PFIsolatedTauTagInfo myPFIsolatedTauTagInfo=PFConeIsolationAlgo_->tag(theGenericJetRef,thefilteredPFCands,myPV); 
    extCollection->push_back(myPFIsolatedTauTagInfo);
    ++iGJ;
  }
  
  auto_ptr<PFIsolatedTauTagInfoCollection> resultExt(extCollection);  
  iEvent.put(resultExt);  
}
