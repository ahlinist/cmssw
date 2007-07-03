#include "AnalysisExamples/SusyAnalysis/interface/MCProcessor.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

MCProcessor::MCProcessor(MrEvent* pEvtData):
myConfig(0), myEventData(pEvtData), MCData(*(myEventData->mcData())),
DEBUGLVL(0), ana_elecPtMin2(10.), ana_muonPtMin2(10.),
ana_tauPtMin2(5.), ana_photonPtMin2(20.), ana_jetPtMin2(30.),
mc_numEvtPrnt(20), mc_PhotCalFac(1.), mc_JetCalFac(1.7), mc_JetDeltaRIC(0.5)
{}

MCProcessor::MCProcessor(MrEvent* pEvtData,
             Config_t * aConfig): 
myConfig(aConfig), myEventData(pEvtData), MCData(*(myEventData->mcData())), DEBUGLVL(0)
{

  // get acceptance cuts
  acceptance_cuts =
    (*myConfig).acceptance_cuts;
    
 
   // load acceptance cuts
  ana_elecEtaMax = acceptance_cuts.getParameter<double>("ana_elecEtaMax") ;
  ana_muonEtaMax = acceptance_cuts.getParameter<double>("ana_muonEtaMax") ;
  ana_tauEtaMax = acceptance_cuts.getParameter<double>("ana_tauEtaMax") ;
  ana_photonEtaMax = acceptance_cuts.getParameter<double>("ana_photonEtaMax") ;
  ana_jetEtaMax = acceptance_cuts.getParameter<double>("ana_jetEtaMax") ;
  ana_elecPtMin2 = acceptance_cuts.getParameter<double>("ana_elecPtMin2") ;
  ana_muonPtMin2 = acceptance_cuts.getParameter<double>("ana_muonPtMin2") ;
  ana_tauPtMin2 = acceptance_cuts.getParameter<double>("ana_tauPtMin2") ;
  ana_photonPtMin2 = acceptance_cuts.getParameter<double>("ana_photonPtMin2") ;
  ana_jetPtMin2 = acceptance_cuts.getParameter<double>("ana_jetPtMin2") ;
  
  // get MC Processor parameters
  mcproc_params = (*myConfig).mcproc_params;
  
  mc_numEvtPrnt = mcproc_params.getParameter<int>("mc_numEvtPrnt") ;
  mc_PhotCalFac = mcproc_params.getParameter<double>("mc_PhotCalFac") ;
  mc_JetCalFac = mcproc_params.getParameter<double>("mc_JetCalFac") ;
  mc_JetDeltaRIC = mcproc_params.getParameter<double>("mc_JetDeltaRIC") ;
 
}
//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

bool MCProcessor::MCDriver()
{

  // make MC printout
  
  if (DEBUGLVL >= 2){
    if (mc_numEvtPrnt <= 0){mc_numEvtPrnt = 50;}
  }
   
  PrintMCInfo(mc_numEvtPrnt);
   
  // make MC summary printout of the particles for the various status flags

    MakeMCStatusInfo();

  // ******************************************************** 
  // Compute MET from MC and save it into EventData 

    math::XYZVector metrecoil = MetFromMC();
    myEventData->setMetMC(metrecoil);

    if (DEBUGLVL >= 1){
     float metMC =sqrt(metrecoil.perp2());
     cout << " MET from MC truth = " << metMC << endl;
     cout << endl;
    }
    

  return true;
}

//------------------------------------------------------------------------------

void MCProcessor::PrintMCInfo(int imax)
{ 
 // Prints a summary of MC information from the MCData vector
 // imax = maximum number of lines to print
 // Using MrParticle class;

  if (imax <= 0) {return;}
  cout << endl;
  cout << "MCData information : " << endl;
    int mcsize = (int) MCData.size();
    int nprnt = min (imax, mcsize);
    for (int i = 0; i < nprnt; i++) {            
      
      cout << "Index " << i
      << ", pid = " << MCData[i]->pid()
      << ", status = " << MCData[i]->status()
      << ", motherIndex = " << MCData[i]->motherIndex()
//      << ", px = " << MCData[i]->px()
//      << ", py = " << MCData[i]->py()
//      << ", pz = " << MCData[i]->pz() 
      << ", energy = " << MCData[i]->energy()
      << ", eta = " << MCData[i]->eta() 
      << ", phi = " << MCData[i]->phi()
      << endl;
            
    } 

    return;    
}

//------------------------------------------------------------------------------

void MCProcessor::MakeMCStatusInfo()
{ 
     
     int nMCelec[] = {0, 0, 0};
     int nMCmuon[] = {0, 0, 0};
     int nMCtau[] = {0, 0, 0};
     int nMCphot[] = {0, 0, 0};
     int nMCjet[]  = {0, 0, 0};
     
     vector<int> jetIndex;
     
     for(int i = 0; i < (int) MCData.size(); i++){
      if (abs(MCData[i]->pid()) == 11){
       if ( fabs(MCData[i]->eta()) < ana_elecEtaMax && MCData[i]->pt() > ana_elecPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCelec[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 13){
       if ( fabs(MCData[i]->eta()) < ana_muonEtaMax && MCData[i]->pt() > ana_muonPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCmuon[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 15){
       if ( fabs(MCData[i]->eta()) < ana_tauEtaMax && MCData[i]->pt() > ana_tauPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCtau[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 22){
       if ( fabs(MCData[i]->eta()) < ana_photonEtaMax && MCData[i]->pt() > mc_PhotCalFac*ana_photonPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCphot[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 21 || 
           (abs(MCData[i]->pid()) > 0 && abs(MCData[i]->pid()) < 6)){
       if ( fabs(MCData[i]->eta()) < ana_jetEtaMax && 
            MCData[i]->pt() > mc_JetCalFac*ana_jetPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCjet[istatus]++;
        jetIndex.push_back(i);
       }
      }
     }
     
     // Do not count partons too near in DeltaR
     if (jetIndex.size() > 1){
       for(int i=0; i<(int) jetIndex.size(); i++){
         if (jetIndex[i] >= 0){
           int istatus = MCData[jetIndex[i]]->status() - 1;
           float eta1 = MCData[jetIndex[i]]->eta();
           float phi1 = MCData[jetIndex[i]]->phi();
           for(int j=i+1; j<(int) jetIndex.size(); j++){
             if (jetIndex[j] >= 0 &&
                 istatus == MCData[jetIndex[j]]->status()-1){
               float eta2 = MCData[jetIndex[j]]->eta();
               float phi2 = MCData[jetIndex[j]]->phi();
               float deltaR = GetDeltaR(eta1, eta2, phi1, phi2);
               if (deltaR < mc_JetDeltaRIC){
                 nMCjet[istatus]--;
                 jetIndex[j] = -1;
               }
             }
           }
         }
       }
     }
     
     if (!jetIndex.empty() ){
       jetIndex.clear();
     }
     
     if (DEBUGLVL >= 1){
       cout << " MC particles inside final acceptance with " << endl;
       cout << "  status =     1,  2,  3" << endl;
       cout << "  electrons:   " 
            << nMCelec[0] << ", " << nMCelec[1] << ", " << nMCelec[2] << endl;
       cout << "  muons:       " 
            << nMCmuon[0] << ", " << nMCmuon[1] << ", " << nMCmuon[2] << endl;
       cout << "  taus:        " 
            << nMCtau[0] << ", " << nMCtau[1] << ", " << nMCtau[2] << endl;
       cout << "  photons:     " 
            << nMCphot[0] << ", " << nMCphot[1] << ", " << nMCphot[2] << endl;
       cout << "  jets:        " 
            << nMCjet[0] << ", " << nMCjet[1] << ", " << nMCjet[2] << endl;
     }
     
     nMCElecTrue = nMCelec[0];
     nMCMuonTrue = nMCmuon[0];
     nMCTauTrue = nMCtau[1];
     nMCPhotTrue = nMCphot[0];
     nMCJetTrue = nMCjet[1];
                                                            
}

//------------------------------------------------------------------------------

math::XYZVector MCProcessor::MetFromMC()
{ 
  // Computes the MET from the MC truth
  bool allvisEnergy = true;
  float metrecoil[] = {0., 0., 0.};

  // sum over all particles ignoring neutrinos and neutralino1
  if (allvisEnergy){
   for (int j = 0; j < (int) MCData.size(); j++){
     int pid = abs(MCData[j]->pid() );
     // assuming the neutralino1 is the LSP 
     if (MCData[j]->status() == 1
       && !(pid == 12 || pid == 14 || pid == 16 || pid == 1000022) ){
   
       metrecoil[0] -= MCData[j]->px();
       metrecoil[1] -= MCData[j]->py();
       metrecoil[2] -= MCData[j]->pz();
     }
   }
   metMCvector = math::XYZVector(metrecoil[0],metrecoil[1],metrecoil[2]);
  }
  
  // sum over visible particles inside acceptance
  else {
/*  get root error: e.g. "Symbol ana_jetEtaMax is not defined in current scope"
   for (int j = 0; j < (int) MCData.size(); j++){
     int pid = abs(MCData[j]->pid() );
     if (MCData[j]->status() == 1 && pid == 11){
       if (fabs(MCData[j]->eta()) < ana_elecEtaMax && MCData[j]->pt() > ana_elecPtMin2){
         metrecoil[0] -= MCData[j]->px();
         metrecoil[1] -= MCData[j]->py();
       }
     }

     if (MCData[j]->status() == 1 && pid == 13){
         ((pid >= 1 && pid <= 6) || pid == 21) ){
       if (fabs(MCData[j]->eta()) < ana_muonEtaMax && MCData[j]->pt() > ana_muonPtMin2){
         metrecoil[0] -= MCData[j]->px();
         metrecoil[1] -= MCData[j]->py();
       }
     }

     if (MCData[j]->status() == 2 && 
         ((pid >= 1 && pid <= 6) || pid == 21) ){
       if (fabs(MCData[j]->eta()) < ana_jetEtaMax && MCData[j]->pt() > ana_jetPtMin2){
         metrecoil[0] -= MCData[j]->px();
         metrecoil[1] -= MCData[j]->py();
       }
     }
   }
*/
  }
  
  return metMCvector;
}

//------------------------------------------------------------------------------

float MCProcessor::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float MCProcessor::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


