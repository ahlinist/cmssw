#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"

#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

#include <memory>
#include <string>
#include <iostream>

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>

using namespace edm;
using namespace reco; 
using namespace std;

class PFTauTagTest : public EDAnalyzer {
public:
  explicit PFTauTagTest(const ParameterSet&);
  ~PFTauTagTest() {}
  virtual void analyze(const Event& iEvent,const EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
private:
  string PFIsolatedTauTagProd_;
  int nEvent;
  vector<float> nEventsUsed;
  vector<float> nEventsRiso;
  int nEventTaggedJets;
};

PFTauTagTest::PFTauTagTest(const ParameterSet& iConfig){
  PFIsolatedTauTagProd_        = iConfig.getParameter<string>("PFIsolatedTauTagProd");
  nEvent=0;
  nEventTaggedJets=0;
  nEventsRiso.reserve(6);
  nEventsUsed.reserve(6);
  for(int i=0;i<6;i++){
    nEventsRiso[i]=0.;
    nEventsUsed[i]=0.;
  }
}

void PFTauTagTest::beginJob(){}

void PFTauTagTest::analyze(const Event& iEvent, const EventSetup& iSetup){
  cout<<endl;
  cout<<"********"<<endl;
  cout<<"Event number "<<nEvent++<<endl;
  
  Handle<JetTagCollection> jetTagHandle;
  iEvent.getByLabel(PFIsolatedTauTagProd_,jetTagHandle);
  const JetTagCollection& myJetTagCollection=*(jetTagHandle.product()); 
  cout<<"Found "<<myJetTagCollection.size()<<" had. tau-jet candidates"<<endl;
  
  bool useOnlyChargedHadrCand=true;
  float Rmatch=0.1;
  float Rsig=0.07;
  bool useOnlyChargedHadrforleadPFCand=true;
  float pT_LT=6.;
  float pT_min=1.;
  int it=0;
  for (JetTagCollection::const_iterator iJetTag=myJetTagCollection.begin();iJetTag!=myJetTagCollection.end();++iJetTag) {
    PFIsolatedTauTagInfoRef thePFIsolatedTauTagInfo=(*iJetTag).tagInfoRef().castTo<PFIsolatedTauTagInfoRef>();    
    if(it==0){
      for(int ii=0;ii<6;ii++){
	nEventsUsed[ii]++;
	float Riso=ii*0.05+0.2;
	double mydiscriminator;
	if (useOnlyChargedHadrCand) mydiscriminator=(*thePFIsolatedTauTagInfo).discriminatorByIsolPFChargedHadrCandsN(Rmatch,Rsig,Riso,useOnlyChargedHadrforleadPFCand,pT_LT,pT_min);
	else mydiscriminator=(*thePFIsolatedTauTagInfo).discriminatorByIsolPFCandsN(Rmatch,Rsig,Riso,useOnlyChargedHadrforleadPFCand,pT_LT,pT_min);
	if(mydiscriminator==1.) nEventsRiso[ii]++;
      }
    }
    it++;
    //Prints out some quantities
    cout<<"***"<<endl;
    cout<<"Jet Number "<<it<<endl;
    cout<<"Discriminator from jetTag = "<<(*iJetTag).discriminator()<<endl;
    PFCandidateRef theleadPFCand;
    if (useOnlyChargedHadrforleadPFCand) theleadPFCand=(*thePFIsolatedTauTagInfo).leadPFChargedHadrCand(Rmatch,pT_LT);
    else theleadPFCand=(*thePFIsolatedTauTagInfo).leadPFCand(Rmatch,pT_LT);
    cout<<"# PF charged hadr. cand's "<<(*thePFIsolatedTauTagInfo).PFChargedHadrCands().size()<<endl;
    cout<<"# PF neutral hadr. cand's "<<(*thePFIsolatedTauTagInfo).PFNeutrHadrCands().size()<<endl;
    cout<<"# PF gamma cand's "<<(*thePFIsolatedTauTagInfo).PFGammaCands().size()<<endl;
    if(!theleadPFCand) cout<<"No Lead PFCand "<<endl;
    else{
      cout<<"Lead PFCand pt "<<(*theleadPFCand).pt()<<endl;
      math::XYZVector momentum = (*theleadPFCand).momentum();
      int mySignalPFCandsN;
      if (useOnlyChargedHadrCand) mySignalPFCandsN=(int)(*thePFIsolatedTauTagInfo).PFChargedHadrCandsInCone(momentum,Rsig,pT_min).size();
      else mySignalPFCandsN=(int)(*thePFIsolatedTauTagInfo).PFCandsInCone(momentum,Rsig,pT_min).size();
      cout<<"Number of SignalPFCand's = "<<mySignalPFCandsN<<endl;
    }
  }    
}
void PFTauTagTest::endJob(){
  TH1F effVsRiso("eff","Eff",6,0.2,0.5);
  int ibin=0;  
  for(int ii=0;ii<6;ii++){
    if(nEventsUsed[ii]>0) ibin=ii+1;
    float eff=nEventsRiso[ii]/nEventsUsed[ii];
    effVsRiso.SetBinContent(ibin,eff);
  }
  
  //Write the histos in a rootfile
  TFile hFile("histo1.root","RECREATE");
  (effVsRiso).Write();
  
  //Make a plot and save the jpg
  gROOT->SetBatch();	
  gROOT->SetStyle("Plain");
  TCanvas c;
  effVsRiso.SetMaximum(1.01);
  effVsRiso.GetXaxis()->SetTitle("Isolation Cone");
  effVsRiso.GetYaxis()->SetTitle("Efficiency");
  effVsRiso.SetFillColor(kRed);
  effVsRiso.SetLineWidth(2);
  effVsRiso.Draw();
  c.SaveAs("eff.jpg");
}
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(PFTauTagTest);
