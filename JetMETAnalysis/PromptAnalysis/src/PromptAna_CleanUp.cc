#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_CleanUp.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;
using namespace reco;

PromptAna_CleanUp::PromptAna_CleanUp(const edm::ParameterSet& iConfig) 
{
  //Get Input Tags
  HcalNoiseRBXCollectionTag = iConfig.getParameter<edm::InputTag>("HcalNoiseRBXCollection");
  HcalNoiseSummaryTag       = iConfig.getParameter<edm::InputTag>("HcalNoiseSummary");
  theJetCollectionLabel     = iConfig.getParameter<edm::InputTag>("JetCollectionLabel");
  prefix                    = iConfig.getParameter<std::string>  ("Prefix"  );
  suffix                    = iConfig.getParameter<std::string>  ("Suffix"  );

  produces <std::vector<bool> >   ( prefix + "JetIDMinimal"  + suffix );
  produces <std::vector<bool> >   ( prefix + "JetIDLoose"  + suffix );
  produces <std::vector<bool> >   ( prefix + "JetIDTight"  + suffix );
  produces <std::vector<bool> >   ( prefix + "HcalNoiseFilterTight"  + suffix );
  produces <std::vector<bool> >   ( prefix + "HcalNoiseFilterLoose"  + suffix );
  
  jetID = new reco::helper::JetIDHelper(iConfig.getParameter<ParameterSet>("JetIDParams"));
}

void PromptAna_CleanUp::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<bool> >  f_jetID_Minimal                                ( new std::vector<bool>()  ) ;
  std::auto_ptr<std::vector<bool> >  f_jetID_Loose                                  ( new std::vector<bool>()  ) ;
  std::auto_ptr<std::vector<bool> >  f_jetID_Tight                                  ( new std::vector<bool>()  ) ;
  std::auto_ptr<std::vector<bool> >  f_hcalnoise_Tight                              ( new std::vector<bool>()  ) ;
  std::auto_ptr<std::vector<bool> >  f_hcalnoise_Loose                              ( new std::vector<bool>()  ) ;

  // ==========================================================
  //
  edm::Handle<HcalNoiseRBXCollection> HRBXCollection;
  iEvent.getByLabel(HcalNoiseRBXCollectionTag,HRBXCollection);
  
  edm::Handle<HcalNoiseSummary> HNoiseSummary;
  iEvent.getByLabel(HcalNoiseSummaryTag,HNoiseSummary);

  edm::Handle<reco::CaloJetCollection> caloJets;
  iEvent.getByLabel(theJetCollectionLabel, caloJets);
  // ==========================================================
  // JetID 

  //
  // --- Minimal cuts
  //
  bool bJetIDMinimal=true;
  int nj=0;
  for (reco::CaloJetCollection::const_iterator cal = caloJets->begin(); 
       cal!=caloJets->end(); ++cal){
    
    jetID->calculate(iEvent, *cal);
    nj++;
    if (cal->pt()>10.){
      if (fabs(cal->eta())<=2.6 && 
	  cal->emEnergyFraction()<=0.01) bJetIDMinimal=false;
    }
  }

  f_jetID_Minimal -> push_back(bJetIDMinimal);

  //
  // --- Loose cuts
  //
  bool bJetIDLoose=true;
  for (reco::CaloJetCollection::const_iterator cal = caloJets->begin(); 
       cal!=caloJets->end(); ++cal){
    jetID->calculate(iEvent, *cal);
    if (cal->pt()>10.){
      //
      // for all regions
      if (jetID->n90Hits()<2)  bJetIDLoose=false; 
      if (jetID->fHPD()>=0.98) bJetIDLoose=false; 
      //
      // for non-forward
      if (fabs(cal->eta())<2.55){
	if (cal->emEnergyFraction()<=0.01) bJetIDLoose=false; 
      }
      // for forward
      else {
	if (cal->emEnergyFraction()<=-0.9) bJetIDLoose=false; 
	if (cal->pt()>80.){
	if (cal->emEnergyFraction()>= 1.0) bJetIDLoose=false; 
	}
      } // forward vs non-forward
    }   // pt>10 GeV/c
  }     // calor-jets loop

  f_jetID_Loose -> push_back(bJetIDLoose);

  //
  // --- Tight cuts
  //
  bool bJetIDTight=true;
  bJetIDTight=bJetIDLoose;
  for (reco::CaloJetCollection::const_iterator cal = caloJets->begin(); 
       cal!=caloJets->end(); ++cal){
    jetID->calculate(iEvent, *cal);
    if (cal->pt()>25.){
      //
      // for all regions
      if (jetID->fHPD()>=0.95) bJetIDTight=false; 
      //
      // for 1.0<|eta|<1.75
      if (fabs(cal->eta())>=1.00 && fabs(cal->eta())<1.75){
	if (cal->pt()>80. && cal->emEnergyFraction()>=1.) bJetIDTight=false; 
      }
      //
      // for 1.75<|eta|<2.55
      else if (fabs(cal->eta())>=1.75 && fabs(cal->eta())<2.55){
	if (cal->pt()>80. && cal->emEnergyFraction()>=1.) bJetIDTight=false; 
      }
      //
      // for 2.55<|eta|<3.25
      else if (fabs(cal->eta())>=2.55 && fabs(cal->eta())<3.25){
	if (cal->pt()< 50.                   && cal->emEnergyFraction()<=-0.3) bJetIDTight=false; 
	if (cal->pt()>=50. && cal->pt()< 80. && cal->emEnergyFraction()<=-0.2) bJetIDTight=false; 
	if (cal->pt()>=80. && cal->pt()<340. && cal->emEnergyFraction()<=-0.1) bJetIDTight=false; 
	if (cal->pt()>=340.                  && cal->emEnergyFraction()<=-0.1 
                                             && cal->emEnergyFraction()>=0.95) bJetIDTight=false; 
      }
      //
      // for 3.25<|eta|
      else if (fabs(cal->eta())>=3.25){
	if (cal->pt()< 50.                   && cal->emEnergyFraction()<=-0.3
                                             && cal->emEnergyFraction()>=0.90) bJetIDTight=false; 
	if (cal->pt()>=50. && cal->pt()<130. && cal->emEnergyFraction()<=-0.2
                                             && cal->emEnergyFraction()>=0.80) bJetIDTight=false; 
	if (cal->pt()>=130.                  && cal->emEnergyFraction()<=-0.1 
                                             && cal->emEnergyFraction()>=0.70) bJetIDTight=false; 
      }
    }   // pt>10 GeV/c
  }     // calor-jets loop

  f_jetID_Tight -> push_back(bJetIDTight);

  // ==========================================================
  // HCAL Noise filter

  f_hcalnoise_Loose -> push_back(HNoiseSummary->passLooseNoiseFilter());
  f_hcalnoise_Tight -> push_back(HNoiseSummary->passTightNoiseFilter());
  
  // ==========================================================


  iEvent.put(  f_jetID_Minimal                                          ,      prefix + "JetIDMinimal"  + suffix );
  iEvent.put(  f_jetID_Loose                                            ,      prefix + "JetIDLoose"  + suffix );
  iEvent.put(  f_jetID_Tight                                            ,      prefix + "JetIDTight"  + suffix );
  iEvent.put(  f_hcalnoise_Loose                                        ,      prefix + "HcalNoiseFilterLoose"  + suffix );
  iEvent.put(  f_hcalnoise_Tight                                        ,      prefix + "HcalNoiseFilterTight"  + suffix );
}
// ***********************************************************
void PromptAna_CleanUp::endJob() {

  delete jetID;

}
