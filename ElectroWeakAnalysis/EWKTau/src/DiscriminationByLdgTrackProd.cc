#include "ElectroWeakAnalysis/EWKTau/interface/DiscriminationByLdgTrackProd.h"


using namespace reco; 
using namespace edm;
using namespace std;

//
// constructors and destructor
//
DiscriminationByLdgTrackProd::DiscriminationByLdgTrackProd(const edm::ParameterSet& iConfig)
{
  pfTauTag_ = iConfig.getParameter<InputTag>("PfTauTag");
  useTracksInstead_=iConfig.getParameter<bool>("UseTracksInstead");
  useChargedHadInstead_=iConfig.getParameter<bool>("UseChargeHadInstead");
  tauEtCut_=iConfig.getParameter<double>("TauEtCut");
  tauEtaCut_=iConfig.getParameter<double>("TauEtaCut");
  leadPtCut_=iConfig.getParameter<double>("LeadTrkPtCut");
  chooseSelType_=iConfig.getParameter<string>("ChooseSelectionType");
  produces<PFTauDiscriminatorByIsolation>("passKin");
  produces<PFTauDiscriminatorByIsolation>("foundLdgTrk");
  produces<PFTauDiscriminatorByIsolation>("passLdgTrkPt");
  
}


DiscriminationByLdgTrackProd::~DiscriminationByLdgTrackProd()
{
}


// ------------ method called on each new Event  ------------
void DiscriminationByLdgTrackProd::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  Handle<PFTauCollection> pfTaus;
  iEvent.getByLabel(pfTauTag_,pfTaus);

  auto_ptr<PFTauDiscriminatorByIsolation> pfTauDcrKin(new PFTauDiscriminatorByIsolation(PFTauRefProd(pfTaus)));
  auto_ptr<PFTauDiscriminatorByIsolation> pfTauDcrPtTr(new PFTauDiscriminatorByIsolation(PFTauRefProd(pfTaus)));
  auto_ptr<PFTauDiscriminatorByIsolation> pfTauDcrPtLdgTr(new PFTauDiscriminatorByIsolation(PFTauRefProd(pfTaus)));

  for(size_t iPFTau=0;iPFTau<pfTaus->size();++iPFTau) 
    {
      PFTauRef thePFTauRef(pfTaus,iPFTau);
      PFTau thePFTau=*thePFTauRef;
      
      if(thePFTau.et()>tauEtCut_&&fabs(thePFTau.eta())<tauEtaCut_)
	pfTauDcrKin->setValue(iPFTau,1);
      
      if(useTracksInstead_){
	const TrackRef theLeadTrk = thePFTau.leadTrack();
	if(!theLeadTrk)continue;
	pfTauDcrPtTr->setValue(iPFTau,1);
	if(theLeadTrk->pt()>leadPtCut_)
	  pfTauDcrPtLdgTr->setValue(iPFTau,1);
      }
      if(useChargedHadInstead_){
	if(!thePFTau.leadPFChargedHadrCand())continue;
	pfTauDcrPtTr->setValue(iPFTau,1);
	if(thePFTau.leadPFChargedHadrCand()->pt()>leadPtCut_)
	  pfTauDcrPtLdgTr->setValue(iPFTau,1);
      }
      
    }
  
  if(chooseSelType_=="passKin")iEvent.put(pfTauDcrKin,"passKin");
  if(chooseSelType_=="foundLdgTrk")iEvent.put(pfTauDcrPtTr,"foundLdgTrk");
  if(chooseSelType_=="passLdgTrkPt")iEvent.put(pfTauDcrPtLdgTr,"passLdgTrkPt");

}

