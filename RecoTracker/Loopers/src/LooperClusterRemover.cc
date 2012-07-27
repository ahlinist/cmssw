#include "RecoTracker/Loopers/interface/LooperClusterRemover.h"
#include "RecoTracker/Loopers/interface/Methods.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"

//
// constructors and destructor
//
LooperClusterRemover::LooperClusterRemover(const edm::ParameterSet& iConfig)
{
  //essentially this could be taken from the provenance, but whatever...
  stripClusters_ = iConfig.getParameter<edm::InputTag>("stripClusters");
  pixelClusters_ = iConfig.getParameter<edm::InputTag>("pixelClusters");
  
   //produce the mask of looper clusters
   produces<edm::ContainerMask<edmNew::DetSetVector<SiPixelCluster> > >();
   produces<edm::ContainerMask<edmNew::DetSetVector<SiStripCluster> > >();

   std::string whichOne = iConfig.getParameter<std::string>("algo");
   if (whichOne=="EveryNMethod")
        method_ = new LooperClusterRemoverMethod::EveryNMethod(iConfig);
   else if (whichOne=="FractionOfTruth")
       method_ = new LooperClusterRemoverMethod::FractionOfTruth(iConfig);
   else if (whichOne=="ReadFileIn")
     method_ = new LooperClusterRemoverMethod::ReadFileIn(iConfig);
   else if (whichOne=="LooperMethod")
     method_ = new LooperClusterRemoverMethod::LooperMethod(iConfig);
   else if (whichOne=="PerJet")
     method_ = new LooperClusterRemoverMethod::PerJet(iConfig);
   else {
     std::cout<<whichOne<<" is not recognized";
     method_=0;//and crash
   }

   // side produces the track candidate collection to be used for track fitting.
   produces<TrackCandidateCollection>();
   produces<reco::TrackCollection>();
   produces<reco::TrackExtraCollection>();
   produces<TrackingRecHitCollection>();
}


LooperClusterRemover::~LooperClusterRemover()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

void LooperClusterRemover::reset(edm::Event& iEvent){
  edm::Handle<edmNew::DetSetVector<SiStripCluster> > stripClusters;
  iEvent.getByLabel(stripClusters_, stripClusters);
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelClusters;
  iEvent.getByLabel(pixelClusters_, pixelClusters);
  
  prod_.collectedPixels.resize(pixelClusters->dataSize()); fill(prod_.collectedPixels.begin(), prod_.collectedPixels.end(), false);
  prod_.collectedStrips.resize(stripClusters->dataSize()); fill(prod_.collectedStrips.begin(), prod_.collectedStrips.end(), false);
}

void LooperClusterRemover::put(edm::Event& iEvent){
  edm::Handle<edmNew::DetSetVector<SiStripCluster> > stripClusters;
  iEvent.getByLabel(stripClusters_, stripClusters);
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > pixelClusters;
  iEvent.getByLabel(pixelClusters_, pixelClusters);

  std::auto_ptr<PixelMaskContainer> removedPixelClusterMask(
      new PixelMaskContainer(edm::RefProd<edmNew::DetSetVector<SiPixelCluster> >(pixelClusters),prod_.collectedPixels));
  std::auto_ptr<StripMaskContainer> removedStripClusterMask(
      new StripMaskContainer(edm::RefProd<edmNew::DetSetVector<SiStripCluster> >(stripClusters),prod_.collectedStrips));

  iEvent.put(removedPixelClusterMask);
  iEvent.put(removedStripClusterMask);
  if (prod_.tcOut.get())
    iEvent.put( prod_.tcOut );
  if (prod_.tOut.get())
    {
      iEvent.put(prod_.tOut);
      iEvent.put(prod_.teOut);
      iEvent.put(prod_.trhOut);
    }
}
// ------------ method called to produce the data  ------------
void
LooperClusterRemover::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //final products
  reset(iEvent);
  method_->run(iEvent,iSetup,
	       prod_);
  unsigned int nP=0,nS=0;
  for (unsigned int i=0;i!=prod_.collectedPixels.size();++i)    if (prod_.collectedPixels[i]) ++nP;
  for (unsigned int i=0;i!=prod_.collectedStrips.size();++i)    if (prod_.collectedStrips[i]) ++nS;
  
  edm::LogError("LooperClusterRemover")<<" rejecting "<<nP<<" pixels out of: "<<prod_.collectedPixels.size()<<"\n"
				       <<" rejecting "<<nS<<" stripd out of: "<<prod_.collectedStrips.size();
  if (prod_.tcOut.get())
    edm::LogError("LooperClusterRemover")<<" making "<<prod_.tcOut->size()<<" track candidates";
  if (prod_.tOut.get())
    edm::LogError("LooperClusterRemover")<<" making "<<prod_.tOut->size()<<" track";
  put(iEvent);
}

// ------------ method called once each job just before starting event loop  ------------
void 
LooperClusterRemover::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LooperClusterRemover::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
LooperClusterRemover::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
LooperClusterRemover::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
LooperClusterRemover::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
LooperClusterRemover::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LooperClusterRemover::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(LooperClusterRemover);
