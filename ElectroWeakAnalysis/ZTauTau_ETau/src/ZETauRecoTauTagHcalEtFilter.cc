#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagHcalEtFilter.h"

#include "TLorentzVector.h"


using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoTauTagHcalEtFilter::ZETauRecoTauTagHcalEtFilter(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
  m_HtotEt = iConfig.getParameter<double>("HtotOverPtLtrCut");
  m_OutTrHhotDEta=iConfig.getParameter<double>("OutTrHhotDEta");
  m_PtLtr = iConfig.getParameter<double>("PtLdgTr");
  m_MinN = iConfig.getParameter<int>("MinN");
  
  produces<IsolatedTauTagInfoCollection>("TauTag");
  produces<LorentzVectorCollection>("TauTag");
}

ZETauRecoTauTagHcalEtFilter::~ZETauRecoTauTagHcalEtFilter(){ }

bool ZETauRecoTauTagHcalEtFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{

  edm::ESHandle<MagneticField> field;
  iES.get<IdealMagneticFieldRecord>().get(field);
  edm::ESHandle<Propagator> propagator;
  iES.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", propagator);

  auto_ptr<IsolatedTauTagInfoCollection> product_TauTag(new IsolatedTauTagInfoCollection);
  auto_ptr<LorentzVectorCollection> product_TauTagVec(new LorentzVectorCollection);
  
  Handle<IsolatedTauTagInfoCollection> tauTagHandle;
  iEvent.getByLabel( m_TauTag , tauTagHandle );  
  IsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();

  int nObjects=0;
  for(;tautagit!=tauTagHandle->end();++tautagit)
    {
      
      
      const reco::TrackRef leadTk = tautagit->leadingSignalTrack(0.1,m_PtLtr);
      if(!leadTk)continue;
      const reco::CaloJet* cj = dynamic_cast<reco::CaloJet*>(const_cast<reco::Jet*>((tautagit->jet().get())));
      const vector<CaloTowerRef> TauJetTowers = cj->getConstituents();
      double HCalTowerEtMax = -10.;double HCalTowerEtMaxEta = 0.;
      double HCalTowerEtTotal=0.;double HCal3x3Et=0.;
      
      GlobalPoint trackAtEcal=PropagateTrackToCalo(*leadTk,*field,*propagator);
      double trackAtEcalEta=trackAtEcal.eta();
      double trackAtEcalPhi=trackAtEcal.phi();
      double absTrackAtEcalEta=fabs(trackAtEcal.eta());
      
      
      for(unsigned int iTower = 0; iTower < TauJetTowers.size(); iTower++) {
        CaloTowerRef t = TauJetTowers[iTower];
	double deta=fabs(trackAtEcalEta-t->eta());double dphi=fabs(trackAtEcalPhi-t->phi());
	if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
	if(sqrt(dphi*dphi+deta*deta)<0.184){
	  HCal3x3Et+=t->hadEt();
	  if( t->hadEt() >= HCalTowerEtMax ){
	    HCalTowerEtMax = t->hadEt();
	    HCalTowerEtMaxEta = t->eta();
	  }
        }
	//HCalTowerEtTotal+=t->hadEt();
      }
      
      double OutTrHhotDEta = fabs(HCalTowerEtMaxEta-trackAtEcalEta);
      //cout<<"LALA:::::::::::::::"<<trackAtEcalEta<<endl;
      double leadTkPt;
      
      leadTkPt=leadTk->pt();
      //if(HCalTowerEtTotal/leadTkPt>m_HotHCalEt)
      //if(HCalTowerEtMax>m_HotHCalEt)
      
      if ( absTrackAtEcalEta < 0.018 || (absTrackAtEcalEta>0.423 && absTrackAtEcalEta<0.461) || 
	   (absTrackAtEcalEta>0.770 && absTrackAtEcalEta<0.806) || 
	   (absTrackAtEcalEta>1.127 && absTrackAtEcalEta<1.163) || 
	   (absTrackAtEcalEta>1.460 && absTrackAtEcalEta<1.558) )continue;
      
      if(HCal3x3Et/leadTkPt>m_HtotEt&&OutTrHhotDEta<m_OutTrHhotDEta)
	{
	  LorentzVector tau(tautagit->jet().get()->px(),tautagit->jet().get()->py(),tautagit->jet().get()->pz(),tautagit->jet().get()->energy());
	  product_TauTag->push_back((*tautagit));
	  product_TauTagVec->push_back(tau);
	  nObjects++;
	}
    }

  bool accept =false;
  if(nObjects>=m_MinN)
   {
     iEvent.put(product_TauTag,"TauTag"); 
     iEvent.put(product_TauTagVec,"TauTag");
     accept=true;
   }
  return accept;
}

