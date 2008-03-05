#include "AnalysisExamples/SusyAnalysis/interface/EffProcessor.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

EffProcessor::EffProcessor(Config_t * aConfig): 
myConfig(aConfig), 
DEBUGLVL(0)
{

}
//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

bool EffProcessor::EffDriver(MrEvent* theEventData)
{
  
  EventData = theEventData;
  std::vector<MrParticle*> & RecoData = *(EventData->recoData());

  if (DEBUGLVL >= 1){
    cout << "Computing efficiencies " << endl;
  }

  // ******************************************************** 
  // Make simple checks on existence of data
   if (RecoData.size() <= 0){
     if (DEBUGLVL >= 1){
       cout << " No Reco Data in event, abandoned " << endl;
     }
     return false;
   }

  // ******************************************************** 
  // Compute the trigger efficiency and fakes and store it in MrEvent

   myEffTrigger = new EffTrigger(myConfig);
   float effTrig = myEffTrigger->GetEffTrigger(EventData);
   EventData->setEffTrig(effTrig);
   
   // compute probability for this event to have a fake e or mu
   myEffLeptonFakes = new EffLeptonFakes(myConfig);
   float fakeElProb = myEffLeptonFakes->GetLeptonFakes(1, EventData);
   EventData->setFakeElProb(fakeElProb);
   float fakeMuProb = myEffLeptonFakes->GetLeptonFakes(2, EventData);
   EventData->setFakeMuProb(fakeMuProb);

   if (DEBUGLVL >= 2){
     cout << endl;
     cout << " Trigger efficiency = " << effTrig << endl;
     cout << " Fake probability for El = " << fakeElProb
          << ", for Mu = " << fakeMuProb << endl;
   }
   
   delete myEffTrigger;
   delete myEffLeptonFakes;
   

  // ******************************************************** 
  // Compute the efficiency of Reco+ID and fake probability 
  //  and store it in MrParticle

   myEffLeptonID = new EffLeptonID(myConfig);
   myEffLeptonToBeFake = new EffLeptonToBeFake(myConfig);
   
   for (int i=0; i< (int) RecoData.size(); ++i){
     int idpart = 0;
     if (RecoData[i]->particleType() == 10) {idpart = 1;}
     else if (RecoData[i]->particleType() == 20) {idpart = 2;}
     if (idpart > 0){
       float pt = RecoData[i]->pt();
       float eta = RecoData[i]->eta();
       // compute the Reco+ID efficiency
       float effID = myEffLeptonID->GetLeptonEfficiency(idpart, pt, eta);
       RecoData[i]->setEffID(effID);
       // compute the probability for a given lepton to ba a fake
       float effFake = myEffLeptonToBeFake->GetLeptonFakes(i, EventData);
       RecoData[i]->setEffToBeFake(effFake);
       if (DEBUGLVL >= 2){
         cout << " Lepton pT = " << pt 
              << " type = " << RecoData[i]->particleType()
              << " Effic ID = " << effID << " Fake = " << effFake << endl;
       }
     }
   }
   
   delete myEffLeptonID;
   delete myEffLeptonToBeFake;
   
    
  return true;
}


//------------------------------------------------------------------------------


