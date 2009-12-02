#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_CleanUp.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;
using namespace reco;

PromptAna_CleanUp::PromptAna_CleanUp(const edm::ParameterSet& iConfig) 
{
  //Get Input Tags
  HcalNoiseSummaryTag  = iConfig.getParameter<edm::InputTag>("HcalNoiseSummary");
  prefix               = iConfig.getParameter<std::string>  ("Prefix"  );
  suffix               = iConfig.getParameter<std::string>  ("Suffix"  );

  produces < bool>   ( prefix + "HcalNoiseFilterHighLevel"  + suffix );
  produces < bool>   ( prefix + "HcalNoiseFilterTight"  + suffix );
  produces < bool>   ( prefix + "HcalNoiseFilterLoose"  + suffix );

  // the status with which the filter failed, 0 means no failure
  produces <int>   ( prefix + "NoiseFilterStatus" + suffix);
  produces <int>   ( prefix + "NoiseType" + suffix);

  // quantities to calculate EM fraction and charge fraction
  // of the event (|eta|<3.0)
  produces <double> ( prefix + "EventEMEnergy" + suffix);
  produces <double> ( prefix + "EventHadEnergy" + suffix);
  produces <double> ( prefix + "EventTrackEnergy" + suffix);
  produces <double> ( prefix + "EventEMFraction" + suffix);
  produces <double> ( prefix + "EventChargeFraction" + suffix);
  
  // minimum/maximum/RMS rechit time
  // rechit energy>10 GeV or 25 GeV
  produces <double> ( prefix + "Min10GeVHitTime" + suffix);
  produces <double> ( prefix + "Max10GeVHitTime" + suffix);
  produces <double> ( prefix + "Rms10GeVHitTime" + suffix);
  produces <double> ( prefix + "Min25GeVHitTime" + suffix);
  produces <double> ( prefix + "Max25GeVHitTime" + suffix);
  produces <double> ( prefix + "Rms25GeVHitTime" + suffix);

  // # of hits with E>10 GeV or 25 GeV
  produces <int>   ( prefix + "Num10GeVHits" + suffix);
  produces <int>   ( prefix + "Num25GeVHits" + suffix);
  
  // E(2TS), E(10TS), and E(2TS)/E(10TS) for the minimum E(2TS)/E(10TS) found in an RBX in the event
  // the total energy in the RBX must be > 20 GeV
  produces <double> ( prefix + "MinE2TS" + suffix);
  produces <double> ( prefix + "MinE10TS" + suffix);
  produces <double> ( prefix + "MinE2Over10TS" + suffix);
  
  // largest number of zeros found in a single RBX in the event
  produces <int>   ( prefix + "MaxZeros" + suffix);
  
  // largest number of hits in a single HPD/RBX in the event
  produces <int>   ( prefix + "MaxHPDHits" + suffix);
  produces <int>   ( prefix + "MaxRBXHits" + suffix);
  
  // smallest EMF found in an HPD/RBX in the event
  // the total energy in the HPD/RBX must be >20 GeV
  produces <double> ( prefix + "MinHPDEMF" + suffix);
  produces <double> ( prefix + "MinRBXEMF" + suffix);
  
  // number of "problematic" RBXs
  produces <int>   ( prefix + "NumProblematicRBXs" + suffix);
}

void PromptAna_CleanUp::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<bool>  f_hcalnoise_HighLevel  ( new bool()  );
  std::auto_ptr<bool>  f_hcalnoise_Tight      ( new bool()  );
  std::auto_ptr<bool>  f_hcalnoise_Loose      ( new bool()  );
  std::auto_ptr<int>   f_noiseFilterStatus ( new int() );
  std::auto_ptr<int>   f_noiseType ( new int() );
  std::auto_ptr<double> f_eventEMEnergy ( new double() );
  std::auto_ptr<double> f_eventHadEnergy ( new double() );
  std::auto_ptr<double> f_eventTrackEnergy ( new double() );
  std::auto_ptr<double> f_eventEMFraction ( new double() );
  std::auto_ptr<double> f_eventChargeFraction ( new double() );
  std::auto_ptr<double> f_min10GeVHitTime ( new double() );
  std::auto_ptr<double> f_max10GeVHitTime ( new double() );
  std::auto_ptr<double> f_rms10GeVHitTime ( new double() );
  std::auto_ptr<double> f_min25GeVHitTime ( new double() );
  std::auto_ptr<double> f_max25GeVHitTime ( new double() );
  std::auto_ptr<double> f_rms25GeVHitTime ( new double() );
  std::auto_ptr<int>   f_num10GeVHits ( new int() );
  std::auto_ptr<int>   f_num25GeVHits ( new int() );
  std::auto_ptr<double> f_minE2TS ( new double() );
  std::auto_ptr<double> f_minE10TS ( new double() );
  std::auto_ptr<double> f_minE2Over10TS ( new double() );
  std::auto_ptr<int>   f_maxZeros ( new int() );
  std::auto_ptr<int>   f_maxHPDHits ( new int() );
  std::auto_ptr<int>   f_maxRBXHits ( new int() );
  std::auto_ptr<double> f_minHPDEMF ( new double() );
  std::auto_ptr<double> f_minRBXEMF ( new double() );
  std::auto_ptr<int>   f_numProblematicRBXs ( new int() );

  // ==========================================================
  // HCAL Noise filter
  edm::Handle<HcalNoiseSummary> HNoiseSummary;
  iEvent.getByLabel(HcalNoiseSummaryTag,HNoiseSummary);

  *f_hcalnoise_HighLevel.get() = HNoiseSummary->passHighLevelNoiseFilter();
  *f_hcalnoise_Tight.get()     = HNoiseSummary->passTightNoiseFilter();
  *f_hcalnoise_Loose.get()     = HNoiseSummary->passLooseNoiseFilter();
  *f_noiseFilterStatus.get()=HNoiseSummary->noiseFilterStatus();
  *f_noiseType.get()=HNoiseSummary->noiseType();
  *f_eventEMEnergy.get()=HNoiseSummary->eventEMEnergy();
  *f_eventHadEnergy.get()=HNoiseSummary->eventHadEnergy();
  *f_eventTrackEnergy.get()=HNoiseSummary->eventTrackEnergy();
  *f_eventEMFraction.get()=HNoiseSummary->eventEMFraction();
  *f_eventChargeFraction.get()=HNoiseSummary->eventChargeFraction();
  *f_min10GeVHitTime.get()=HNoiseSummary->min10GeVHitTime();
  *f_max10GeVHitTime.get()=HNoiseSummary->max10GeVHitTime();
  *f_rms10GeVHitTime.get()=HNoiseSummary->rms10GeVHitTime();
  *f_min25GeVHitTime.get()=HNoiseSummary->min25GeVHitTime();
  *f_max25GeVHitTime.get()=HNoiseSummary->max25GeVHitTime();
  *f_rms25GeVHitTime.get()=HNoiseSummary->rms25GeVHitTime();
  *f_num10GeVHits.get()=HNoiseSummary->num10GeVHits();
  *f_num25GeVHits.get()=HNoiseSummary->num25GeVHits();
  *f_minE2TS.get()=HNoiseSummary->minE2TS();
  *f_minE10TS.get()=HNoiseSummary->minE10TS();
  *f_minE2Over10TS.get()=HNoiseSummary->minE2Over10TS();
  *f_maxZeros.get()=HNoiseSummary->maxZeros();
  *f_maxHPDHits.get()=HNoiseSummary->maxHPDHits();
  *f_maxRBXHits.get()=HNoiseSummary->maxRBXHits();
  *f_minHPDEMF.get()=HNoiseSummary->minHPDEMF();
  *f_minRBXEMF.get()=HNoiseSummary->minRBXEMF();
  *f_numProblematicRBXs.get()=HNoiseSummary->numProblematicRBXs();
  
  iEvent.put( f_hcalnoise_HighLevel         ,   prefix + "HcalNoiseFilterHighLevel"  + suffix );
  iEvent.put( f_hcalnoise_Tight             ,   prefix + "HcalNoiseFilterTight"  + suffix );
  iEvent.put( f_hcalnoise_Loose             ,   prefix + "HcalNoiseFilterLoose"  + suffix );
  iEvent.put( f_noiseFilterStatus           ,   prefix + "NoiseFilterStatus" + suffix );
  iEvent.put( f_noiseType                   ,   prefix + "NoiseType" + suffix );
  iEvent.put( f_eventEMEnergy               ,   prefix + "EventEMEnergy" + suffix );
  iEvent.put( f_eventHadEnergy              ,   prefix + "EventHadEnergy" + suffix );
  iEvent.put( f_eventTrackEnergy            ,   prefix + "EventTrackEnergy" + suffix );
  iEvent.put( f_eventEMFraction             ,   prefix + "EventEMFraction" + suffix );
  iEvent.put( f_eventChargeFraction         ,   prefix + "EventChargeFraction" + suffix );
  iEvent.put( f_min10GeVHitTime             ,   prefix + "Min10GeVHitTime" + suffix );
  iEvent.put( f_max10GeVHitTime             ,   prefix + "Max10GeVHitTime" + suffix );
  iEvent.put( f_rms10GeVHitTime             ,   prefix + "Rms10GeVHitTime" + suffix );
  iEvent.put( f_min25GeVHitTime             ,   prefix + "Min25GeVHitTime" + suffix );
  iEvent.put( f_max25GeVHitTime             ,   prefix + "Max25GeVHitTime" + suffix );
  iEvent.put( f_rms25GeVHitTime             ,   prefix + "Rms25GeVHitTime" + suffix );
  iEvent.put( f_num10GeVHits                ,   prefix + "Num10GeVHits" + suffix );
  iEvent.put( f_num25GeVHits                ,   prefix + "Num25GeVHits" + suffix );
  iEvent.put( f_minE2TS                     ,   prefix + "MinE2TS" + suffix );
  iEvent.put( f_minE10TS                    ,   prefix + "MinE10TS" + suffix );
  iEvent.put( f_minE2Over10TS               ,   prefix + "MinE2Over10TS" + suffix );
  iEvent.put( f_maxZeros                    ,   prefix + "MaxZeros" + suffix );
  iEvent.put( f_maxHPDHits                  ,   prefix + "MaxHPDHits" + suffix );
  iEvent.put( f_maxRBXHits                  ,   prefix + "MaxRBXHits" + suffix );
  iEvent.put( f_minHPDEMF                   ,   prefix + "MinHPDEMF" + suffix );
  iEvent.put( f_minRBXEMF                   ,   prefix + "MinRBXEMF" + suffix );
  iEvent.put( f_numProblematicRBXs          ,   prefix + "NumProblematicRBXs" + suffix );
}
