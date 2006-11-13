#include "AnalysisExamples/SusyAnalysis/interface/ObjectCleaner.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

ObjectCleaner::ObjectCleaner(vector<MrParticle*>& Data, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers):
RecoData(Data), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
DEBUGLVL(0),
clean_dRSSelecmax(10.), 
clean_deltaRElecJetmax(0.5), clean_elecbyJetEratio(0.9),
clean_dROSelecmax(0.2),
clean_dPhiJetMETmin(0.), clean_dR12min(0.5), clean_dR21min(0.5)
{};

ObjectCleaner::ObjectCleaner(vector<MrParticle*>& Data, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers, edm::ParameterSet param):
RecoData(Data), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
DEBUGLVL(0)
{
clean_dRSSelecmax = param.getParameter<double>("clean_dRSSelecmax") ;
clean_deltaRElecJetmax = param.getParameter<double>("clean_deltaRElecJetmax") ;
clean_elecbyJetEratio = param.getParameter<double>("clean_elecbyJetEratio") ;
clean_dROSelecmax = param.getParameter<double>("clean_dROSelecmax") ;
clean_dPhiJetMETmin = param.getParameter<double>("clean_dPhiJetMETmin") ;
clean_dR12min = param.getParameter<double>("clean_dR12min") ;
clean_dR21min = param.getParameter<double>("clean_dR21min") ;
};

//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanElectron(int ichk)
{
  // Verifies the electron identification quality
  // Still to be done ****

 if (ichk < 0){return false;}

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::DuplicateElectron(int ichk)
{
  // Checks for duplicate electrons
 
 bool isDuplicate = false;
 if (ichk < 0){return isDuplicate;}

 MrParticle * recopart = RecoData[ichk];
 if (recopart->particleType() != 1){return false;}

 const Electron* elecand = RecoData[ichk]->electronCandidate();
// cout << " pointer to elec cand " << elecand;
 float elecmom = elecand->p();
// cout << " momentum " << elecmom << endl;
 const SuperCluster* supercluster = &(*(elecand->superCluster()));
// cout << " pointer to supercluster " << supercluster;
 float elenergy =(elecand->superCluster())->energy();
// cout << " energy " << elenergy << endl;

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
    if (RecoData[j]->particleType() == 1){

      if (recopart->charge() == RecoData[j]->charge() ){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRSSelecmax){
   
          const SuperCluster* newsuper =&(*( RecoData[j]->electronCandidate()->superCluster()));
//          cout << " pointer to new supercluster " << newsuper << endl;
          if (newsuper == supercluster){
            float newmom = RecoData[j]->electronCandidate()->p();
//            cout << " new momentum " << newmom << endl;
            if (fabs(elenergy-elecmom) >= fabs(elenergy-newmom)){
              if (DEBUGLVL >= 2){
                cout << " Electron " << ichk 
                     << " duplicate: DeltaR = " << deltaR
                     << endl;
              }
              isDuplicate =  true;
              break;
            }
          }
        }
      }
    }
   }
      
 }

 return isDuplicate;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanMuon(int ichk)
{
  // Verifies the muon identification quality

 if (ichk < 0){return false;}

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanTau(int ichk)
{
  // Verifies the hadronic tau identification quality

 if (ichk < 0){return false;}

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanPhoton(int ichk)
{
  // Verifies the photon identification quality

 if (ichk < 0){return false;}

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanJet(int ichk)
{
  // Verifies the jet reconstruction quality
  // and rejects jets made from electrons
  // (electrons should be filled in RecoData before cleaning the jets)   

  if (ichk < 0){return false;}
  
  MrParticle * recopart = RecoData[ichk];

  for (unsigned int j = 0; j < RecoData.size(); j++) {

   // veto jets with E<p
   if(RecoData[j]->particleType() >= 5){
     if(RecoData[j]->energy() < RecoData[j]->p() ){
      if (DEBUGLVL >= 1){
        cout << "  Jet = " << j 
             << "has E<p: E = " << RecoData[j]->energy()
             << ", p = " << RecoData[j]->p() << endl;
      }
      return false;
     }
   }

   // veto electron jets 
   if(RecoData[j]->particleType() == 1){
    float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                             recopart->phi(), RecoData[j]->phi());
    
    if( deltaR < clean_deltaRElecJetmax && 
      ( RecoData[j]->energy() / recopart->energy() ) > clean_elecbyJetEratio) {
      if (DEBUGLVL >= 2){
        cout << "  Elec = " << j << " E = " << RecoData[j]->energy()
             << " DRejet = " << deltaR
             << " Ee/Ej = " << RecoData[j]->energy() / recopart->energy() << endl;
      }
      return false;
    }
   }
  } 
  // veto jets with too large Fem
  // (but what if correponding electron or photon does not exist?)

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanEvent()
{
 // To veto events from beam halo, cosmics or noise
 // will also need the primary vertex
 // tests on Fem and Ftrk to be added (careful for mu-mu and e-e)


  if (RecoData.size() <= 0){
    return false;
  }

  if (TrackData->size() <=0){
    return false;
  }

  if (CaloTowerData->size() <=0){
    return false;
  }



return true;

}
//------------------------------------------------------------------------------

bool ObjectCleaner::ConvertedPhoton(int i, int j)
{
 // Checks whether 2 OS electrons come from photon conversion
 // returns true if converted photon
 // here only test on DeltaR (like Catania analysis)
 // could also make a cut on invariant mass

  if (i < 0 || j < 0){return false;}

  if (RecoData[i]->charge()*RecoData[j]->charge() == -1){
    float deltaR = GetDeltaR(RecoData[i]->eta(), RecoData[j]->eta(), 
                             RecoData[i]->phi(), RecoData[j]->phi());
    if (deltaR < clean_dROSelecmax){
      return true;
    }
  }
  else{
    return false;
  }
  //dummy
  return false;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanMET(float met[])
{
// The MET should not be aligned with any jet
// and should not be along one and opposite to the other of the 2 leading jets
// to veto QCD events with jet "mismeasurements"
// (see Jet + MET analysis in ptdr2)

  float met_x = met[0];
  float met_y = met[1];
  if (met_x == 0. && met_y == 0.){return false;}
  float met_phi = atan2(met_y,met_x);

  float etmax1 = 0.;
  float etmax2 = 0.;
  int imax1 = -1;
  int imax2 = -1;
  for(unsigned int i = 0; i < RecoData.size(); i++){
   if(RecoData[i]->particleType() >= 5){
     float dPhi =  DeltaPhi(RecoData[i]->phi(), met_phi);
     if (dPhi < clean_dPhiJetMETmin) {
       return false;
     }
     if(RecoData[i]->pt() > etmax1){
       etmax2 = etmax1;
       imax2 = imax1;
       etmax1 = RecoData[i]->pt();
       imax1 = i;
     } else if (RecoData[i]->pt() > etmax2){
       etmax2 = RecoData[i]->pt();
       imax2 = i;
     }
    }
   }
   if (imax2 >=0){
     float dPhi1 = DeltaPhi(RecoData[imax1]->phi(), met_phi );
     float dPhi2 = DeltaPhi(RecoData[imax2]->phi(), met_phi );
     float pi = 3.141592654;
     float r12 = sqrt(dPhi1*dPhi1 + (pi-dPhi2)*(pi-dPhi2) );
     float r21 = sqrt(dPhi2*dPhi2 + (pi-dPhi1)*(pi-dPhi1) );
     if (r12 < clean_dR12min){return false;}
     if (r21 < clean_dR21min){return false;}
   }

 return true;

}

//------------------------------------------------------------------------------

int ObjectCleaner::FindNearestJet(int ichk)
{
// Looks for the nearest jet in deltaR to a given object
// and returns its RecoData index 
// returns -1 if no nearest jet

  int iJetMin = -1;
  if (ichk < 0){return iJetMin;}
  
  float deltaRmin = 999.;
  for(int i = 0; i < (int) RecoData.size(); i++){
   if(RecoData[i]->particleType() >= 5){
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

float ObjectCleaner::GetPtwrtJet(int ichk, int iJet)
{
// Computes the Pt of object ichk wrt the direction of object iJet
  
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

float ObjectCleaner::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float ObjectCleaner::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


