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
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 

using namespace reco;
using namespace std;

class MrEvent {

public:

// constructor
MrEvent() : pMcData(0), pRecoData(0), TrackData(0), VertexData(0), CaloTowerData(0), jetsprocessed(false)
{
  TrigL1 = 0;
  TrigHLT = 0;
  EventQuality = 0;
  IndexPVx = -1;
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
// the pointers to the vertex collection, track collection and calotower collection
const TrackCollection * trackCollection() {return TrackData;}
const VertexCollection * vertexCollection() {return VertexData;}
const CaloTowerCollection * caloTowerCollection() {return CaloTowerData;}
// event and run number
int run() {return RunNumber;}
int event() {return EventNumber;}
// Triggering status
int triggeredL1(){return TrigL1;}
int triggeredHLT(){return TrigHLT;}
// quality index (0 if good event; 1 or higher for pathologies)
int quality() {return EventQuality;}
// Primary vertex chisquared, ndof and normalized chisquared (chisq = -1. if no primary vertex)
int indexPrimaryVx() {return IndexPVx;}
float chi2(){return Chi2;}
float ndof(){return Ndof;}
float normalizedChi2(){return NormalizedChi2;}
// Primary vertex position and errors
float pvx(){return PVx;}
float pvy(){return PVy;}
float pvz(){return PVz;}
float pvr(){return sqrt(PVx*PVx + PVy*PVy);}
float pvdx(){return PVdx;}
float pvdy(){return PVdy;}
float pvdz(){return PVdz;}
// Primary vertex number of associated tracks and their scalar pt sum
int pvnTracks(){return PvnTracks;}
float pvPtsum(){return PvPtsum;}
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
// Index in MCData of the partons matching the hemispheres
int indexMatchedSusyMother1(){return IndexMatchedSusyMother1;}
int indexMatchedSusyMother2(){return IndexMatchedSusyMother2;}

// Quantities derived from the RecoData
// Number of jets (up to 15 max) stored in decreasing p_T
int numJets(){if(!jetsprocessed) {GetLeadingJets();} ; 
      return NumJets;}
// Index in RecoData of the a jet (iJet = 1 to numJets)
int indexRecoJet(int iJet){if(!jetsprocessed) {GetLeadingJets();} ;
      if(iJet <= NumJets){return LeadJets[iJet-1];}else{return -1;}}
// p_T of stored jet iJet
float ptJet(int iJet) {if(!jetsprocessed) {GetLeadingJets();} ;
      if(iJet <= NumJets){return (*pRecoData)[LeadJets[iJet-1]]->pt();}else{return 0.;}}
// p_t sum of the three leading jets
float ptSumLeadJets(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets;}
// E_t sum of the three leading jets plus the MET from calorimeter
float etSumCalo(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets + metCaloMod();}
// E_t sum of the three leading jets plus the MET from recoil
float etSumRecoil(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets + metRecoilMod();}
// Ht: E_t sum of the 2nd and 3rd leading jets plus the MET from calorimeter
float htCalo(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets - ptJet(1) + metCaloMod();}
// Ht: E_t sum of the 2nd and 3rd leading jets plus the MET from recoil
float htRecoil(){if(!jetsprocessed) {GetLeadingJets();} ;
      return PtSumLeadJets - ptJet(1) + metRecoilMod();}


// set methods
void setMCData(std::vector<MrParticle*>* mcdata){pMcData = mcdata;}
void setRecoData(std::vector<MrParticle*>* recodata){pRecoData = recodata;}
void setTrackCollection(const TrackCollection * tc)  {TrackData = tc;}
void setVertexCollection(const VertexCollection * vc) {VertexData = vc;}
void setCaloTowerCollection(const CaloTowerCollection * cc) {CaloTowerData = cc;}
void setRun(int run) {RunNumber = run;}
void setEvent(int ev) {EventNumber = ev;}
void setTriggeredL1(int trig){TrigL1 = trig;}
void setTriggeredHLT(int trig){TrigHLT = trig;}
void setQuality(int qual) {EventQuality = qual;}
void setIndexPrimaryVx(int index) {IndexPVx = index;}
void setChi2(float chi){Chi2 = chi;}
void setNdof(float nd){Ndof = nd;}
void setNormalizedChi2(float chin){NormalizedChi2 = chin;}
void setPVx(float x){PVx = x;}
void setPVy(float y){PVy = y;}
void setPVz(float z){PVz = z;}
void setPVdx(float dx){PVdx = dx;}
void setPVdy(float dy){PVdy = dy;}
void setPVdz(float dz){PVdz = dz;}
void setPvnTracks(int nt){PvnTracks = nt;}
void setPvPtsum(float pt){PvPtsum = pt;}
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
const TrackCollection * TrackData;
const VertexCollection * VertexData;
const CaloTowerCollection * CaloTowerData;


int RunNumber, EventNumber;
int TrigL1, TrigHLT;
int EventQuality;
float Chi2, Ndof, NormalizedChi2, PVx, PVy, PVz, PVdx, PVdy, PVdz, PvPtsum;
int IndexPVx, PvnTracks;
math::XYZVector MetMC, MetCalo, MetRecoil;
std::vector<float> HemiAxis1, HemiAxis2;
int IndexMatchedSusyMother1, IndexMatchedSusyMother2;
static const int Njetsmax = 15;
int LeadJets[Njetsmax], NumJets;
float PtSumLeadJets;
bool jetsprocessed;



// methods
void GetLeadingJets(){
// saves the indices of the 15 leading jets
// and computes the scalar PtSum of the leading jets
  
  int nsaved = -1;
  for(int i = 0; i < (int) pRecoData->size() ;i++){
    if ((*pRecoData)[i]->particleType() == 5 
       || (*pRecoData)[i]->particleType() == 6){
      if (nsaved < 0){
        nsaved++;
        LeadJets[nsaved] = i;
      }
      else if ((*pRecoData)[LeadJets[nsaved]]->pt() < (*pRecoData)[i]->pt()){
        for (int j=0; j <= nsaved; j++){
          if ((*pRecoData)[LeadJets[j]]->pt() < (*pRecoData)[i]->pt()){
            if (nsaved == Njetsmax-1){nsaved--;}
            for (int k=nsaved; k >= j; k--){
              LeadJets[k+1] = LeadJets[k];
            }
            LeadJets[j] = i;
            nsaved++; 
            break;
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
    if (i <= 3){
      PtSumLeadJets += (*pRecoData)[LeadJets[i]]->pt();
    }
  }

  NumJets = nsaved+1;
  jetsprocessed = true; 
  return;
}

};
#endif
