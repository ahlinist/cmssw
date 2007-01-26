#ifndef MrEvent_h
#define MrEvent_h

/*  \class MrEvent
*
*  Event class to hold global event quantities and entries to the MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: December 2006 
*/


// system include files
#include <vector>
#include <iostream>

// user include files
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 

using namespace reco;
using namespace std;

class MrEvent {

public:

// constructor
MrEvent() : pMcData(0), pRecoData(0), jetsprocessed(false)
{
  TrigL1 = 0;
  TrigHLT = 0;
  MetMC.SetXYZ(0.,0.,0.);
  MetCalo.SetXYZ(0.,0.,0.);
  MetRecoil.SetXYZ(0.,0.,0.);
  for (int i=0; i<6; i++){
    HemiAxis1.push_back(0.);
    HemiAxis2.push_back(0.);
  }
  IndexMatchedSusyMother1 = -1;
  IndexMatchedSusyMother2 = -1;
};

// destructor
virtual ~MrEvent(){};

// access methods
// the pointers to MCData and RecoData
std::vector<MrParticle*>* mcData(){return pMcData;}
std::vector<MrParticle*>* recoData(){return pRecoData;}
// Triggering status
int triggeredL1(void){return TrigL1;}
int triggeredHLT(void){return TrigHLT;}
// Missing Et as 3-vector
math::XYZVector metMCvect(){return MetMC;}
math::XYZVector metCalovect(){return MetCalo;}
math::XYZVector metRecoilvect(){return MetRecoil;}
// Missing Et magnitude
float metMCMod(){return sqrt(MetMC.perp2());}
float metCaloMod(){return sqrt(MetCalo.perp2());}
float metRecoilMod(){return sqrt(MetRecoil.perp2());}
// Hemisphere axes as Nx, Ny, Nz, P, E (with Ni = Pi/P), same as Hemisphere class
std::vector<float> hemiAxis1(){return HemiAxis1;}
std::vector<float> hemiAxis2(){return HemiAxis2;}
// Hemisphere masses
float hemiMass1(){return sqrt(HemiAxis1[4]*HemiAxis1[4]-HemiAxis1[3]*HemiAxis1[3]);}
float hemiMass2(){return sqrt(HemiAxis2[4]*HemiAxis2[4]-HemiAxis2[3]*HemiAxis2[3]);}
int indexMatchedSusyMother1(){return IndexMatchedSusyMother1;}
int indexMatchedSusyMother2(){return IndexMatchedSusyMother2;}
// Quantities derived from the RecoData
int numJets(){if(!jetsprocessed) {GetLeadingJets();} ; 
//      cout << "MrEvent: numJets = " << NumJets << " indices stored " << LeadJets[0] << " " << LeadJets[1] << endl;
      return NumJets;}
int indexRecoJet(int iJet){if(!jetsprocessed) {GetLeadingJets();} ;
//      cout << "MrEvent: index returned " << LeadJets[iJet] << endl;
      if(iJet <= NumJets){return LeadJets[iJet-1];}else{return -1;}}
float ptJet(int iJet) {if(!jetsprocessed) {GetLeadingJets();} ;
//      cout << "MrEvent: ptJet indices stored " << LeadJets[0] << " " << LeadJets[1] << endl;
      if(iJet <= NumJets){return (*pRecoData)[LeadJets[iJet-1]]->pt();}else{return 0.;}}
float ptSumLeadJets(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets;}
float etSumCalo(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets + metCaloMod();}
float etSumRecoil(){if(!jetsprocessed) {GetLeadingJets();} ;
//      cout << "MrEvent: etSumRecoil indices stored " << LeadJets[0] << " " << LeadJets[1] << endl;
      return PtSumLeadJets + metRecoilMod();}
float htCalo(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets - ptJet(1) + metCaloMod();}
float htRecoil(){if(!jetsprocessed) {GetLeadingJets();} ;
//      cout << "MrEvent: htRecoil indices stored " << LeadJets[0] << " " << LeadJets[1] << endl;
      return PtSumLeadJets - ptJet(1) + metRecoilMod();}


// set methods
void setMCData(std::vector<MrParticle*>* mcdata){pMcData = mcdata;}
void setRecoData(std::vector<MrParticle*>* recodata){pRecoData = recodata;}
void setTriggeredL1(int trig){TrigL1 = trig;}
void setTriggeredHLT(int trig){TrigHLT = trig;}
void setMetMC(math::XYZVector metMC) {MetMC = metMC;}
void setMetCalo(math::XYZVector metCalo) {MetCalo = metCalo; }
void setMetRecoil(math::XYZVector metRecoil) {MetRecoil = metRecoil;}
void setHemiAxis1(std::vector<float> axis){HemiAxis1 = axis;}
void setHemiAxis2(std::vector<float> axis){HemiAxis2 = axis;}
void setIndexMatchedSusyMother1(int index){IndexMatchedSusyMother1 = index;}
void setIndexMatchedSusyMother2(int index){IndexMatchedSusyMother2 = index;}

private:

// data members
std::vector<MrParticle*>* pMcData; 
std::vector<MrParticle*>* pRecoData;
int TrigL1, TrigHLT;
math::XYZVector MetMC, MetCalo, MetRecoil;
std::vector<float> HemiAxis1, HemiAxis2;
int IndexMatchedSusyMother1, IndexMatchedSusyMother2;
static const int Njetsmax = 4;
int LeadJets[Njetsmax], NumJets;
float PtSumLeadJets;
bool jetsprocessed;

// methods
void GetLeadingJets(){
// saves the indices of the 4 leading jets
// and computes the scalar PtSum of the leading jets
  
  int nsaved = -1;
  for(int i = 0; i < (int) pRecoData->size() ;i++){
    if ((*pRecoData)[i]->particleType() == 5 
       || (*pRecoData)[i]->particleType() == 6){
      if (nsaved < 0){
        nsaved++;
        LeadJets[nsaved] = i;
      }
      else if ((*pRecoData)[LeadJets[nsaved]]->energy() < (*pRecoData)[i]->energy()){
        for (int j=0; j <= nsaved; j++){
          if ((*pRecoData)[LeadJets[j]]->energy() < (*pRecoData)[i]->energy()){
            if (nsaved == Njetsmax-1){nsaved--;}
            for (int k=nsaved; k >= j; k--){
              LeadJets[k+1] = LeadJets[k];
            }
            LeadJets[j] = i;
            nsaved++;
          }
        }
      } else if (nsaved < Njetsmax-1){
        nsaved++;
        LeadJets[nsaved] = i;
      }
    }
  }
  
  PtSumLeadJets = 0.;
  for(int i = 0; i <= nsaved ;i++){
    PtSumLeadJets += (*pRecoData)[LeadJets[i]]->pt();
  }

  NumJets = nsaved+1;
  jetsprocessed = true; 
  return;
}

};
#endif
