#include "RecoTauTag/PFCombinedTauTag/interface/PFCombinedTauTag.h"

// ------------ method called to produce the data  ------------
void PFCombinedTauTag::produce(Event& iEvent, const EventSetup& iSetup) {
  Handle<PFIsolatedTauTagInfoCollection> thePFIsolatedTauTagInfoCollection;
  iEvent.getByLabel(PFIsolatedTauTagSrc_,thePFIsolatedTauTagInfoCollection);   
  JetTagCollection* baseCollection=new JetTagCollection();
  vector<PFCombinedTauTagInfo>* extCollection=new vector<PFCombinedTauTagInfo>();
  
  const EventSetupRecord& theEventSetupRcd=iSetup.get<CombinedTauTagRcd>();
  unsigned long long cacheId=theEventSetupRcd.cacheIdentifier();
  if(cacheId!=m_cacheId){
    ESHandle<CombinedTauTagCalibration> theCombinedTauTagCalibrationHandle;
    iSetup.get<CombinedTauTagRcd>().get(theCombinedTauTagCalibrationHandle);
    const CombinedTauTagCalibration* theCombinedTauTagCalibration=theCombinedTauTagCalibrationHandle.product();      
    CalibrationInterface<CategoryDataCondDB,CalibratedHistogram>* theCalibrationInterface=new CalibrationInterface<CategoryDataCondDB,CalibratedHistogram>;
    for(size_t i=0;i<theCombinedTauTagCalibration->data.size();i++){
      theCalibrationInterface->addEntry(CategoryDataCondDB((*theCombinedTauTagCalibration).data[i].category),(*theCombinedTauTagCalibration).data[i].histogram); 
    }    
    (*theLikelihoodRatio).setCalibrationInterface(theCalibrationInterface);
    (*thePFCombinedTauTagAlg).setLikelihoodRatio(theLikelihoodRatio);
  }
  m_cacheId=cacheId;
 
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
  iEvent.getByLabel(PVSrc_,vertices);
  const VertexCollection vertCollection=*(vertices.product());
  if(vertCollection.size()) myPV=*(vertCollection.begin());
  
  int iPFIsolatedTauTagInfo_key=0;
  for(PFIsolatedTauTagInfoCollection::const_iterator iPFIsolatedTauTagInfo=thePFIsolatedTauTagInfoCollection->begin();iPFIsolatedTauTagInfo!=thePFIsolatedTauTagInfoCollection->end();iPFIsolatedTauTagInfo++){
    //int i=iPFIsolatedTauTagInfo->jtaRef()->key.index();  // failure 
    //pair<JetTag,PFCombinedTauTagInfo> myPair=thePFCombinedTauTagAlg->tag(Ref<PFIsolatedTauTagInfoCollection>(thePFIsolatedTauTagInfoCollection,i),myPV,iEvent,iSetup); 
    pair<JetTag,PFCombinedTauTagInfo> myPair=thePFCombinedTauTagAlg->tag(Ref<PFIsolatedTauTagInfoCollection>(thePFIsolatedTauTagInfoCollection,iPFIsolatedTauTagInfo_key),myPV,iEvent,iSetup);
    baseCollection->push_back(myPair.first);    
    extCollection->push_back(myPair.second);
    ++iPFIsolatedTauTagInfo_key;
  }

  auto_ptr<PFCombinedTauTagInfoCollection> resultExt(extCollection); 
  OrphanHandle<PFCombinedTauTagInfoCollection> myPFCombinedTauTagInfoCollection=iEvent.put(resultExt); 
  int iJetTag_key=0;
  for(JetTagCollection::iterator ibaseCollection=baseCollection->begin();ibaseCollection!=baseCollection->end();ibaseCollection++){ 
    ibaseCollection->setTagInfo(RefToBase<BaseTagInfo>(PFCombinedTauTagInfoRef(myPFCombinedTauTagInfoCollection,iJetTag_key))); 
    iJetTag_key++; 
  }  
  auto_ptr<JetTagCollection> resultBase(baseCollection);
  iEvent.put(resultBase); 
}
