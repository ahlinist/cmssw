#include "RecoTauTag/CombinedTauTag/interface/CombinedTauTag.h"

// ------------ method called to produce the data  ------------
void CombinedTauTag::produce(Event& iEvent, const EventSetup& iSetup) {
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
    (*theCombinedTauTagLikelihoodRatio).setCalibrationInterface(theCalibrationInterface);
    (*theCombinedTauTagAlg).setCombinedTauTagLikelihoodRatio(theCombinedTauTagLikelihoodRatio);
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
  
  Handle<IsolatedTauTagInfoCollection> theIsolatedTauTagInfoCollection;
  iEvent.getByLabel(IsolatedTauTagSrc_, theIsolatedTauTagInfoCollection);   

  std::auto_ptr<JetTagCollection>             tagCollection;
  std::auto_ptr<CombinedTauTagInfoCollection> extCollection( new CombinedTauTagInfoCollection() );
  if (theIsolatedTauTagInfoCollection.product()->empty()) {
    tagCollection.reset( new JetTagCollection() );
  } else {
    RefToBaseProd<reco::Jet> rtbp(theIsolatedTauTagInfoCollection->begin()->jet());
    tagCollection.reset( new JetTagCollection(RefToBaseProd<reco::Jet>(rtbp)) );
  
    for (unsigned int i = 0; i < theIsolatedTauTagInfoCollection->size(); ++i) {  
      pair<float,CombinedTauTagInfo> tag = theCombinedTauTagAlg->tag(Ref<IsolatedTauTagInfoCollection>(theIsolatedTauTagInfoCollection, i), myPV, iEvent, iSetup);
      tagCollection->setValue(i, tag.first);    
      extCollection->push_back(tag.second);
    }
  }
  iEvent.put(tagCollection); 
  iEvent.put(extCollection);
}
DEFINE_FWK_MODULE(CombinedTauTag);
