#include "AnalysisExamples/SusyAnalysis/interface/UserAnalysis.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

UserAnalysis::UserAnalysis(Config_t * theConfig) : 
myConfig(theConfig), EventData(0)
{
  
  // get parameters for UserAnalysis
  useranalysis_params = (*myConfig).useranalysis_params;
  // load the parameters
  user_metMin = useranalysis_params.getParameter<double>("user_metMin") ;
  
  cout << endl;
  cout << "UserAnalysis parameters:" << endl;
  cout << " user_metMin = " << user_metMin << endl;

   // initialize histograms

  // book histograms for multiplicities of leptons and jets
   hLeptonMult = new TH1D( "LeptonMult", "Multiplicity of leptons", 15, 0.0, 15.0);
   hElectronMult = new TH1D( "ElectronMult", "Multiplicity of electrons", 10, 0.0, 10.0);
   hMuonMult = new TH1D( "MuonMult", "Multiplicity of muons", 10, 0.0, 10.0);
   hPhotonMult = new TH1D( "PhotonMult", "Multiplicity of photons", 10, 0.0, 10.0);
   hJetMult = new TH1D( "JetMult", "Multiplicity of jets", 30, 0.0, 30.0);
   hBJetMult = new TH1D( "BJetMult", "Multiplicity of b-jets", 15, 0.0, 15.0);

  // book histograms for PT of the leptons and the 3 leading jets
   hPtElec = new TH1D( "PtElec", "Pt spectrum of electrons", 50, 0.0, 1000.0);
   hPtMuon = new TH1D( "PtMuon", "Pt spectrum of muons", 50, 0.0, 1000.0);
   hPtJet1 = new TH1D( "PtJet1", "Pt spectrum of 1st jet", 50, 0.0, 1000.0);
   hPtJet2 = new TH1D( "PtJet2", "Pt spectrum of 2nd jet", 50, 0.0, 1000.0);
   hPtJet3 = new TH1D( "PtJet3", "Pt spectrum of 3rd jet", 50, 0.0, 1000.0);

  // book histograms for MET, ETSum and HT
   hMissingETmc = new TH1D( "MissingETmc", "Missing transverse energyfrom MC", 100, 0.0, 1000.0);
   hMissingET = new TH1D( "MissingET", "Missing transverse energy", 100, 0.0, 1000.0);
   hEtSum = new TH1D( "ETsum", "Transverse energy sum", 100, 0.0, 2000.0);
   hHT = new TH1D( "HT", "Transverse energy sum", 100, 0.0, 2000.0);
   hHemiMass = new TH1D( "HemiMass", "Hemisphere mass", 100, 0.0, 1000.0);

  // book histograms for ET distribution of jets correctly and wrongly assigned to hemispheres
   hJetGoodEt = new TH1D( "JetGoodEt", "Et spectrum of Jets correctly ID'ed", 80, 0.0, 800.0);
   hJetWrongEt = new TH1D( "JetWrongEt", "Et spectrum of Jets wrongly ID'ed", 80, 0.0, 800.0);
   
  // Initialize user counters
  nTotEvtSelUser = 0;




}

// Called from Destructor:

void UserAnalysis::endAnalysis() {

// Print the user statistics here
 
 cout << endl;
 cout << "User statistics:" << endl;
 
 cout << " number of User selected events = " << nTotEvtSelUser << endl;

 return;
 
} 
 


//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------
// this method checks whether the L1 trigger is accepted
bool UserAnalysis::L1Driver(MrEvent* EventData)
{ 
  
  // this module is dummy and accepts the OR of all L1 trigger bits
  // note that, if the L1 trigger bit vector does not exist, the event is accepted
  // (behaviour can be changed below)
  
  unsigned int fired(0);
  bool acceptedL1 = false;
  
  std::vector<int> bits = EventData->l1Bits();
  //  cout << "TriggerProcessor: bits = ";
  if (bits.size() > 0){
    for(unsigned int i=0; i<bits.size(); i++) {
      //    cout << bits[i];
      if(bits[i]) fired++;
    }
  }
  //  cout << endl;
  
  if(fired > 0) acceptedL1 = true;
  //cout << "TriggerProcessor: acceptedL1 = " << (int) acceptedL1 << endl;
  EventData->setTriggeredL1(acceptedL1);
  
  return acceptedL1;

}

//------------------------------------------------------------------------------
// this method checks whether the HLT is accepted
bool UserAnalysis::HLTDriver(MrEvent* EventData)
{ 
   
  // this module is dummy and accepts the OR of all HLT trigger bits
  // note that, if the HLT trigger bit vector does not exist, the event is accepted
  // (behaviour can be changed below)


  unsigned int fired(0);
  bool acceptedHLT = false;
  
  std::vector<int> bits = EventData->hltBits();
  //  cout << "TriggerProcessor: bits = ";
  if (bits.size() > 0){
    for(unsigned int i=0; i<bits.size(); i++) {
      //    cout << bits[i];
      if(bits[i]) fired++;
    }
  }
  //  cout << endl;
  
  if(fired > 0) acceptedHLT = true;
  //cout << "TriggerProcessor: acceptedHLT = " << (int) acceptedHLT << endl;
  EventData->setTriggeredHLT(acceptedHLT);
  return acceptedHLT;

}

//------------------------------------------------------------------------------
// this method is called for every event
void UserAnalysis::doAnalysis(MrEvent* theEventData)
{
  EventData = theEventData;
  std::vector<MrParticle*> & RecoData = *(EventData->recoData());
  //std::vector<MrParticle*> & MCData = *(EventData->mcData());
  
  
  
    // This produces some simple plots as examples
  
  // Put your event selection here *****************
    // Overall condition on MET (to illustrate usage of parameters)
    float metchk = EventData->metRecoilMod();
    if (metchk < user_metMin){return;}
    
    // count number of selected events for final statistics
    nTotEvtSelUser++;


    // make multiplicity plots
    int NElectrons =0;
    int NMuons =0;
    int NPhotons =0;
    int NJets=0;
    int NBJets=0;

    for (int i = 0; i < (int) RecoData.size(); i++){
      if (RecoData[i]->particleType() == 1) { NElectrons++;}
      if (RecoData[i]->particleType() == 2) { NMuons++;}
      if (RecoData[i]->particleType() == 4) { NPhotons++;}
      if (RecoData[i]->particleType() == 5 || RecoData[i]->particleType() == 6) {
	NJets++;
        if (RecoData[i]->particleType() == 6){
          NBJets++;
	}  	  
      }    
    }
    hLeptonMult->Fill(NElectrons+NMuons);
    hElectronMult->Fill(NElectrons);
    hMuonMult->Fill(NMuons);
    hPhotonMult->Fill(NPhotons);
    hJetMult->Fill(NJets);
    hBJetMult->Fill(NBJets);

    // plot the pT of the leptons and the 3 leading jets
    for(int i = 0; i < (int) RecoData.size() ;i++){
      if (RecoData[i]->particleType() == 1){
        hPtElec->Fill(RecoData[i]->pt());
      }
      if (RecoData[i]->particleType() == 2){
        hPtMuon->Fill(RecoData[i]->pt());
      }
    }
    int nJets = EventData->numJets();
    if (nJets > 3){nJets = 3;}
    for (int iJet = 1; iJet <= nJets; iJet++){
      float ptjet = EventData->ptJet(iJet);
      if (iJet == 1){hPtJet1->Fill(ptjet);}
      if (iJet == 2){hPtJet2->Fill(ptjet);}
      if (iJet == 3){hPtJet3->Fill(ptjet);}
    }
    
    if (nJets > 0){
      // make plots of Missing energy from MC and recoil
      float metmc = EventData->metMCMod();
      hMissingETmc->Fill(metmc);
      float metrec = EventData->metRecoilMod();
      hMissingET->Fill(metrec);
  
      // make a plot of ETsum and HT (using the MET from recoil)
      float etSum = EventData->etSumRecoil();
      float ht = EventData->htRecoil();
      hEtSum->Fill(etSum);
      hHT->Fill(ht);
  
      // make a plot of the largest hemisphere mass
      float mass1 = EventData->hemiMass1();
      float mass2 = EventData->hemiMass2();
      float mass = mass1 > mass2 ? mass1 : mass2;
      hHemiMass->Fill(mass);
    }
    
//    cout << " Now look at hemispheres, njets = " << nJets << endl;
    // pT distribution of the 2 leading jets correctly and wrongly assigned to hemispheres
    // example of access to the hemisphere information
    if (nJets >= 2) {
      int firstsusymother[2] = {-1, -1};
//    cout << " pointer to EventData " << EventData << endl;
      firstsusymother[0] = EventData->indexMatchedSusyMother1();
      firstsusymother[1] = EventData->indexMatchedSusyMother2();
//    cout << " firstsusymother done " << endl;
      int indjet1 = EventData->indexRecoJet(1);
      int indjet2 = EventData->indexRecoJet(2);
//    cout << " indjet done " << indjet1 << " " << indjet2 << endl;
      int hemijet1 = RecoData[indjet1]->hemisphere();
      int hemijet2 = RecoData[indjet2]->hemisphere();
//    cout << " hemijet done " << endl;
      int susymotherjet1 = FindTopSusyMother(RecoData[indjet1]->partonIndex());
      int susymotherjet2 = FindTopSusyMother(RecoData[indjet2]->partonIndex());
//    cout << " susymotherjet done " << endl;
      if (hemijet1 > 0) {
        float ptjet1 = EventData->ptJet(1);
        if (susymotherjet1 > 0 && susymotherjet1 == firstsusymother[hemijet1-1]) {
          hJetGoodEt->Fill(ptjet1);
        }
        else{
          hJetWrongEt->Fill(ptjet1);
        }
      }
      if (hemijet2 > 0) {
        float ptjet2 = EventData->ptJet(2);
        if (susymotherjet2 > 0 && susymotherjet2 == firstsusymother[hemijet2-1]) {
           hJetGoodEt->Fill(ptjet2);
        }
        else{
           hJetWrongEt->Fill(ptjet2);
        }
      }
    }
//  cout << " filled SUSYMother hgrs " << endl;

  
  
  
  
  return;
}

//------------------------------------------------------------------------------

int UserAnalysis::FindProducedSusyParticles(int firstsusy[])
{ // finds the produced susy particles
  // and returns their index IN MCData into an array
  std::vector<MrParticle*> & MCData = *(EventData->mcData());

  int nfound = 0;
  for (int j = 0; j < (int) MCData.size(); j++){
    if (abs(MCData[j]->pid()) > 1000000 &&
        abs(MCData[MCData[j]->motherIndex()]->pid()) < 1000000){
      firstsusy[nfound] = j;
      nfound++;
    }
  }

  return nfound;

}

//------------------------------------------------------------------------------

int UserAnalysis::FindTopSusyMother(int current)
{ // goes up to highest susy particle
  // current = index of current object in the MCData vector
  //using MrParticle class;
  std::vector<MrParticle*> & MCData = *(EventData->mcData());

 if(current <2) return -1;
 int mother = MCData[current]->motherIndex();
 if(mother <2){
   return -1;
 } else if (abs(MCData[mother]->pid()) < 22 && abs(MCData[current]->pid()) > 1000000) {
    return current; 
 } else {
    return FindTopSusyMother(mother);
 }

}

//------------------------------------------------------------------------------

int UserAnalysis::FindLowSusyMother(int current)
{ // goes up to the first squark (including stop) or gluino on the path
  // current = index of current object in the MCData vector
  //using MrParticle class;
 std::vector<MrParticle*> & MCData = *(EventData->mcData());

 if(current <2) return -1;
 int mother = MCData[current]->motherIndex();
 if(mother <2){
   return -1;
 } else if ( (abs(MCData[current]->pid()) >= 1000001 && abs(MCData[current]->pid()) <= 1000006) || 
      (abs(MCData[current]->pid()) >= 2000001 && abs(MCData[current]->pid()) <= 2000006) ||
        abs(MCData[current]->pid()) == 1000021 ) {
    return current; 
 } else {
    return FindLowSusyMother(mother);
 }
}

//------------------------------------------------------------------------------

int UserAnalysis::FirstSMParton(int current)
{ // goes up to the first quark produced in squark or gluino decay
  // current = index of current object in the MCData vector
  //using MrParticle class;
  std::vector<MrParticle*> & MCData = *(EventData->mcData());


 if(current <2) return -1;
 int mother = MCData[current]->motherIndex();
 if(mother <2){
   return -1;
 } else if ( (abs(MCData[mother]->pid()) >= 1000001 && abs(MCData[mother]->pid()) <= 1000006) || 
      (abs(MCData[mother]->pid()) >= 2000001 && abs(MCData[mother]->pid()) <= 2000006) ||
        abs(MCData[mother]->pid()) == 1000021 ) {
   if(abs(MCData[current]->pid()) >= 1 && abs(MCData[current]->pid()) <= 6 ){
    return current; 
    } else {return -1;}
 } else {
    return FirstSMParton(mother);
 }
}


//------------------------------------------------------------------------------

bool UserAnalysis::ComesFromSquark(int imc)
{
 // returns true if the LowSusyMother is a squark
 // imc = index of current object in the MCData vector
 //using MrParticle class;
  std::vector<MrParticle*> & MCData = *(EventData->mcData());


 int mother = FindLowSusyMother(imc);
 if (mother<2) {return false;}
 if ((abs(MCData[mother]->pid()) >= 1000001 && abs(MCData[mother]->pid()) <= 1000006) || 
     (abs(MCData[mother]->pid()) >= 2000001 && abs(MCData[mother]->pid()) <= 2000006) ) {
   return true;
  } else { return false;}
}

//------------------------------------------------------------------------------

bool UserAnalysis::ComesFromGluino(int imc)
{
 // returns true if the LowSusyMother is a gluino
 // imc = index of current object in the MCData vector
 //using MrParticle class;
 std::vector<MrParticle*> & MCData = *(EventData->mcData());

 int mother = FindLowSusyMother(imc);
 if (mother<2) {return false;}
 if (abs(MCData[mother]->pid()) == 1000021)  {
  return true;
 } else { return false;}
}

//------------------------------------------------------------------------------

int UserAnalysis::FindNearestJet(int ichk)
{
// Looks for the nearest jet in deltaR to a given object
// and returns its RecoData index 
// returns -1 if no nearest jet

  std::vector<MrParticle*> & RecoData = *(EventData->recoData());

  int iJetMin = -1;
  if (ichk < 0){return iJetMin;}
  
  float deltaRmin = 999.;
  for(int i = 0; i < (int) RecoData.size(); i++){
   if(RecoData[i]->particleType() >= 5
      && RecoData[i]->particleType() <= 7){
    float deltaR = GetDeltaR(RecoData[ichk]->eta(), RecoData[i]->eta(), 
                             RecoData[ichk]->phi(), RecoData[i]->phi());
    if (deltaR < deltaRmin && i != ichk){
      deltaRmin = deltaR;
      iJetMin = i;
    }
   }
  }
  
  return iJetMin;

}

//------------------------------------------------------------------------------

float UserAnalysis::GetPtwrtJet(int ichk, int iJet)
{
// Computes the Pt of object ichk wrt the direction of object iJet
    std::vector<MrParticle*> & RecoData = *(EventData->recoData());
  
    if (ichk < 0 || iJet < 0){return -1.;}
    
    float plwrtJet = (RecoData[ichk]->px()*RecoData[iJet]->px() +
                      RecoData[ichk]->py()*RecoData[iJet]->py() +
                      RecoData[ichk]->pz()*RecoData[iJet]->pz() )
                    / RecoData[iJet]->p();
//    cout << " plwrtJet = " << plwrtJet
//         << " cosTheta = " << plwrtJet/RecoData[ichk]->p() << endl;
    float ptwrtJet = sqrt(RecoData[ichk]->p()*RecoData[ichk]->p() -
                          plwrtJet*plwrtJet);
//    cout << " ptwrtJet = " << ptwrtJet << endl;

 return ptwrtJet;

}

//------------------------------------------------------------------------------

void UserAnalysis::AddToJet(int ichk)
{ // adds an object to its nearest jet
  std::vector<MrParticle*> & RecoData = *(EventData->recoData());
 
  int iJet = FindNearestJet(ichk);
  
  if (ichk >= 0 && iJet >= 0) {
   RecoData[iJet]->setPx(RecoData[iJet]->px() + RecoData[ichk]->px());
   RecoData[iJet]->setPy(RecoData[iJet]->py() + RecoData[ichk]->py());
   RecoData[iJet]->setPz(RecoData[iJet]->pz() + RecoData[ichk]->pz());
   RecoData[iJet]->setEnergy(RecoData[iJet]->energy() + RecoData[ichk]->energy());
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
  }

  return;
}

//------------------------------------------------------------------------------

float UserAnalysis::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float UserAnalysis::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


