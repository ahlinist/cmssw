#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/ElectronProducer.h"


//adapted from DQMOffline/Trigger/src/HLTTauRefProducer by {bachtis,berryhil,kap01}
//minimal modifications for this application by gfball

ElectronProducer::ElectronProducer(const edm::ParameterSet& iConfig) {
  e_ = iConfig.getParameter<edm::InputTag>("ElectronCollection");
  e_idAssocProd_ = iConfig.getParameter<edm::InputTag>("IdCollection");
  e_ctfTrackCollection_= iConfig.getParameter<edm::InputTag>("TrackCollection");
  ptMinElectron_= iConfig.getParameter<double>("ptMin");
  e_doID_ = iConfig.getParameter<bool>("doID");
  e_doTrackIso_ = iConfig.getParameter<bool>("doTrackIso");
  e_trackMinPt_= iConfig.getParameter<double>("ptMinTrack");
  e_lipCut_= iConfig.getParameter<double>("lipMinTrack");
  e_minIsoDR_= iConfig.getParameter<double>("InnerConeDR");
  e_maxIsoDR_= iConfig.getParameter<double>("OuterConeDR");
  e_isoMaxSumPt_= iConfig.getParameter<double>("MaxIsoVar");
  e_fromZ_=iConfig.getParameter<bool>("doElecFromZ");
  e_zMmin_= iConfig.getParameter<double>("MinZwindow");
  e_zMmax_= iConfig.getParameter<double>("MaxZwindow");
  e_fromZet_ = iConfig.getParameter<double>("ElecEtFromZcut");
  e_etaMax_ = iConfig.getParameter<double>("EtaMax");

  produces<LorentzVectorCollection>("Electrons");
}

ElectronProducer::~ElectronProducer() {}

void ElectronProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<LorentzVectorCollection> product_Electrons(new LorentzVectorCollection);
  //Retrieve the collections
  //std::cout<<"Entering ElectronProducer"<<std::endl;
  edm::Handle<edm::ValueMap<float> > pEleID;
  iEvent.getByLabel(e_idAssocProd_,pEleID);
  if (!pEleID.isValid()){
    edm::LogInfo("")<< "Error! Can't get electronID by label. ";
  }
  
  edm::Handle<reco::TrackCollection> pCtfTracks;
  iEvent.getByLabel(e_ctfTrackCollection_, pCtfTracks);
  if (!pCtfTracks.isValid()) {
    edm::LogInfo("")<< "Error! Can't get " << e_ctfTrackCollection_.label() << " by label. ";
    return;
  }
  const reco::TrackCollection * ctfTracks = pCtfTracks.product();
  edm::Handle<reco::GsfElectronCollection> electrons;
  if(iEvent.getByLabel(e_,electrons)) {
    //std::cout<<"Got Collections"<<std::endl;
    //std::cout<<electrons->size()<<" electrons in Collection"<<std::endl;
    for(size_t i=0;i<electrons->size();++i) {
      edm::Ref<reco::GsfElectronCollection> electronRef(electrons,i);
      float idDec=0.;
      if(e_doID_){
	idDec=(*pEleID)[electronRef];
      }else idDec=1.;
      if((*electrons)[i].pt()>ptMinElectron_&&fabs((*electrons)[i].eta())<e_etaMax_&&idDec) {
	if(e_doTrackIso_){
	  double isolation_value_ele = ElectronTrkIsolation(ctfTracks,(*electrons)[i]);
	  if(isolation_value_ele<e_isoMaxSumPt_){
	    LorentzVector vec((*electrons)[i].px(),(*electrons)[i].py(),(*electrons)[i].pz(),(*electrons)[i].energy());
	    product_Electrons->push_back(vec);
	  } 
	}
	else{ 
	  LorentzVector vec((*electrons)[i].px(),(*electrons)[i].py(),(*electrons)[i].pz(),(*electrons)[i].energy());
	  product_Electrons->push_back(vec);
	}
      }
    }
  }
  if(e_fromZ_) {
    //std::cout<<"Starting ElectronsFromZ"<<std::endl;
    if (product_Electrons->size()==2) {
      LorentzVector e1 = product_Electrons->at(0);
      LorentzVector e2 = product_Electrons->at(1);
      double zmass = (e1+e2).M();
      if (zmass > e_zMmin_ && zmass < e_zMmax_) {
	if (e1.Et()>e_fromZet_ && e2.Et()>e_fromZet_) {
	  //std::cout<<"Putting ElectronsFromZ"<<std::endl;
	  iEvent.put(product_Electrons,"Electrons");
	  return;
	}
      }
    }
    //std::cout<<"Emptying collection, failed ElectronsFromZ"<<std::endl;
    product_Electrons->clear();
    iEvent.put(product_Electrons,"Electrons");
    return;
  } else {
    iEvent.put(product_Electrons,"Electrons");
    return;
  }

}

double ElectronProducer::ElectronTrkIsolation(const reco::TrackCollection* ctfTracks,const reco::GsfElectron& electron)
{
  reco::TrackCollection::const_iterator tr = ctfTracks->begin();
  double sum_of_pt_ele=0;
  for(;tr != ctfTracks->end();++tr)
    {
      double lip = electron.gsfTrack()->dz() - tr->dz();
      if(tr->pt() > e_trackMinPt_ && fabs(lip) < e_lipCut_){
	double dphi=fabs(tr->phi()-electron.trackMomentumAtVtx().phi());
	if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
	double deta=fabs(tr->eta()-electron.trackMomentumAtVtx().eta());
	double dr_ctf_ele = sqrt(deta*deta+dphi*dphi);
	if((dr_ctf_ele>e_minIsoDR_) && (dr_ctf_ele<e_maxIsoDR_)){
	  double cft_pt_2 = (tr->pt())*(tr->pt());
	  sum_of_pt_ele += cft_pt_2;
	}
      }
    }
  double isolation_value_ele = sum_of_pt_ele/(electron.trackMomentumAtVtx().Rho()*electron.trackMomentumAtVtx().Rho());
  return isolation_value_ele;
}


DEFINE_ANOTHER_FWK_MODULE(ElectronProducer);
