#include "AnalysisExamples/SusyAnalysis/interface/MCProcessor.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

MCProcessor::MCProcessor(MrEvent* pEvtData):
myConfig(0), myEventData(pEvtData), MCData(*(myEventData->mcData())), 
GenData(*(myEventData->genData())), DEBUGLVL(0), ana_elecPtMin2(10.), ana_muonPtMin2(10.),
ana_tauPtMin2(5.), ana_photonPtMin2(20.), ana_jetPtMin2(30.),
mc_numEvtPrnt(20), mc_PhotCalFac(1.), mc_JetCalFac(1.7), mc_JetDeltaRIC(0.5),
iso_MethodElec(1100), iso_ElCalDRin(0.), iso_ElCalDRout(0.1), iso_ElCalSeed(0.1), 
iso_ElTkDRin(0.), iso_ElTkDRout(0.1), iso_ElTkSeed(0.1), iso_ElIsoValue(0.5),
iso_MethodMuon(1100), iso_MuCalDRin(0.), iso_MuCalDRout(0.1), iso_MuCalSeed(0.1),
iso_MuTkDRin(0.), iso_MuTkDRout(0.1), iso_MuTkSeed(0.1), iso_MuIsoValue(0.5),
iso_MethodTau(1100), iso_TauCalDRin(0.), iso_TauCalDRout(0.1), iso_TauCalSeed(0.1),
iso_TauTkDRin(0.), iso_TauTkDRout(0.1), iso_TauTkSeed(0.1), iso_TauIsoValue(0.5),
iso_MethodPhot(1100), iso_PhCalDRin(0.), iso_PhCalDRout(0.1), iso_PhCalSeed(0.1), 
iso_PhTkDRin(0.), iso_PhTkDRout(0.1), iso_PhTkSeed(0.1), iso_PhIsoValue(0.5)
{}

MCProcessor::MCProcessor(MrEvent* pEvtData,
             Config_t * aConfig): 
myConfig(aConfig), myEventData(pEvtData), MCData(*(myEventData->mcData())), 
GenData(*(myEventData->genData())), DEBUGLVL(0)
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
  
  // get isolation parameters
  isolator_params = (*myConfig).isolator_params;
  
  iso_MethodElec = isolator_params.getParameter<int>("iso_MethodElec") ;
  iso_ElCalDRin = isolator_params.getParameter<double>("iso_ElCalDRin") ;
  iso_ElCalDRout = isolator_params.getParameter<double>("iso_ElCalDRout") ;
  iso_ElCalSeed = isolator_params.getParameter<double>("iso_ElCalSeed") ;
  iso_ElTkDRin = isolator_params.getParameter<double>("iso_ElTkDRin") ;
  iso_ElTkDRout = isolator_params.getParameter<double>("iso_ElTkDRout") ;
  iso_ElTkSeed = isolator_params.getParameter<double>("iso_ElTkSeed") ;
  iso_ElIsoValue = isolator_params.getParameter<double>("iso_ElIsoValue") ;
  iso_MethodMuon = isolator_params.getParameter<int>("iso_MethodMuon") ;
  iso_MuCalDRin = isolator_params.getParameter<double>("iso_MuCalDRin") ;
  iso_MuCalDRout = isolator_params.getParameter<double>("iso_MuCalDRout") ;
  iso_MuCalSeed = isolator_params.getParameter<double>("iso_MuCalSeed") ;
  iso_MuTkDRin = isolator_params.getParameter<double>("iso_MuTkDRin") ;
  iso_MuTkDRout = isolator_params.getParameter<double>("iso_MuTkDRout") ;
  iso_MuTkSeed = isolator_params.getParameter<double>("iso_MuTkSeed") ;
  iso_MuIsoValue = isolator_params.getParameter<double>("iso_MuIsoValue") ;
  iso_MethodTau = isolator_params.getParameter<int>("iso_MethodTau") ;
  iso_TauCalDRin = isolator_params.getParameter<double>("iso_TauCalDRin") ;
  iso_TauCalDRout = isolator_params.getParameter<double>("iso_TauCalDRout") ;
  iso_TauCalSeed = isolator_params.getParameter<double>("iso_TauCalSeed") ;
  iso_TauTkDRin = isolator_params.getParameter<double>("iso_TauTkDRin") ;
  iso_TauTkDRout = isolator_params.getParameter<double>("iso_TauTkDRout") ;
  iso_TauTkSeed = isolator_params.getParameter<double>("iso_TauTkSeed") ;
  iso_TauIsoValue = isolator_params.getParameter<double>("iso_TauIsoValue") ;
  iso_MethodPhot = isolator_params.getParameter<int>("iso_MethodPhot") ;
  iso_PhCalDRin = isolator_params.getParameter<double>("iso_PhCalDRin") ;
  iso_PhCalDRout = isolator_params.getParameter<double>("iso_PhCalDRout") ;
  iso_PhCalSeed = isolator_params.getParameter<double>("iso_PhCalSeed") ;
  iso_PhTkDRin = isolator_params.getParameter<double>("iso_PhTkDRin") ;
  iso_PhTkDRout = isolator_params.getParameter<double>("iso_PhTkDRout") ;
  iso_PhTkSeed = isolator_params.getParameter<double>("iso_PhTkSeed") ;
  iso_PhIsoValue = isolator_params.getParameter<double>("iso_PhIsoValue") ;
 
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
   
  // make MC summary printout of the particles for the various status flags from MCData

  MakeMCStatusInfo();
  
  // handle the MC particles in the GenData
  
  // if found to be tau, set the type and count jets and taus
  MakeGenJetInfo();
  
  // check isolation using charged pions and kaons
  MakeMCIsoInfo();
  // defines a lepton as isolated if it comes from a W or Z or ...
//  MakeMCSusyMothIso();

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
{ // Counts the number of partons for the various status values
  // for hadrons, it counts the quarks and gluons
     
     int nMCelec[] = {0, 0, 0};
     int nMCmuon[] = {0, 0, 0};
     int nMCtau[] = {0, 0, 0};
     int nMCphot[] = {0, 0, 0};
     int nMCjet[]  = {0, 0, 0};
                                                                                                                                        
     int nMCelecBigEta[] = {0, 0, 0};
     int nMCmuonBigEta[] = {0, 0, 0};
     int nMCtauBigEta[] = {0, 0, 0};
     int nMCphotBigEta[] = {0, 0, 0};
     int nMCjetBigEta[]  = {0, 0, 0};

     vector<int> jetIndex;
     
     int istatus = 0;
     for(int i = 0; i < (int) MCData.size(); i++){
      if (abs(MCData[i]->pid()) == 11){
       if ( fabs(MCData[i]->eta()) < ana_elecEtaMax && MCData[i]->pt() > ana_elecPtMin2){   
        istatus = MCData[i]->status() - 1;
        nMCelec[istatus]++;
       }
       if ( fabs(MCData[i]->eta()) > ana_elecEtaMax && MCData[i]->pt() > ana_elecPtMin2){
         nMCelecBigEta[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 13){
       if ( fabs(MCData[i]->eta()) < ana_muonEtaMax && MCData[i]->pt() > ana_muonPtMin2){   
        istatus = MCData[i]->status() - 1;
        nMCmuon[istatus]++;
       }
       if ( fabs(MCData[i]->eta()) > ana_muonEtaMax && MCData[i]->pt() > ana_muonPtMin2){
         nMCmuonBigEta[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 15){
       if ( fabs(MCData[i]->eta()) < ana_tauEtaMax && MCData[i]->pt() > ana_tauPtMin2){   
        istatus = MCData[i]->status() - 1;
        nMCtau[istatus]++;
       }
       if ( fabs(MCData[i]->eta()) > ana_tauEtaMax && MCData[i]->pt() > ana_tauPtMin2){
         nMCtauBigEta[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 22){
       if ( fabs(MCData[i]->eta()) < ana_photonEtaMax && MCData[i]->pt() > mc_PhotCalFac*ana_photonPtMin2){   
        istatus = MCData[i]->status() - 1;
        nMCphot[istatus]++;
       }
       if ( fabs(MCData[i]->eta()) > ana_photonEtaMax && MCData[i]->pt() > mc_PhotCalFac*ana_photonPtMin2){
         nMCphotBigEta[istatus]++;
       }
      }
      
      // one way : use the quark/gluons 
      
      if (abs(MCData[i]->pid()) == 21 || 
           (abs(MCData[i]->pid()) > 0 && abs(MCData[i]->pid()) < 6)){
       if ( fabs(MCData[i]->eta()) < ana_jetEtaMax && 
            MCData[i]->pt() > mc_JetCalFac*ana_jetPtMin2){   
        istatus = MCData[i]->status() - 1;
        nMCjet[istatus]++;
        jetIndex.push_back(i);
       }
       if ( fabs(MCData[i]->eta()) > ana_jetEtaMax && MCData[i]->pt() > mc_JetCalFac*ana_jetPtMin2){
         nMCjetBigEta[istatus]++;
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

     nMCElecBigEtaTrue = nMCelecBigEta[0];
     nMCMuonBigEtaTrue = nMCmuonBigEta[0];
     nMCTauBigEtaTrue = nMCtauBigEta[1];
     nMCPhotBigEtaTrue = nMCphotBigEta[0];
     nMCJetBigEtaTrue = nMCjetBigEta[1];


                                                            
}

//------------------------------------------------------------------------------
void MCProcessor::MakeGenJetInfo()
{ // Counts the number of hadrons and taus using GenJets

  int nMCJet = 0, nMCJetBigEta = 0;
  int nMCTau = 0, nMCTauBigEta = 0;
 
  // second way : use genjets
  for(int i = 0; i < (int) GenData.size(); i++){
    if(IsTauJet(i) && IsMCTauIso(i)) {GenData[i]->setParticleType(30);} 
    
    int ptype = GenData[i]->particleType()/10;
    if(ptype == 3){
       if ( fabs(GenData[i]->eta()) < ana_tauEtaMax && 
           GenData[i]->pt() > ana_tauPtMin2){  
         nMCTau++;    
      } else if (fabs(GenData[i]->eta()) > ana_tauEtaMax && 
           GenData[i]->pt() > ana_tauPtMin2) {nMCTauBigEta++;} 
    } 
    if (ptype == 5){
      if ( fabs(GenData[i]->eta()) < ana_jetEtaMax && 
           GenData[i]->pt() > ana_jetPtMin2){  
         nMCJet++;    
      } else if (fabs(GenData[i]->eta()) > ana_jetEtaMax && 
           GenData[i]->pt() > ana_jetPtMin2) {nMCJetBigEta++;}  
    }
  }
 
   nMCJetTrue = nMCJet;
   nMCJetBigEtaTrue = nMCJetBigEta;
   nMCTauTrue = nMCTau;
   nMCTauBigEtaTrue = nMCTauBigEta;
   
}

//------------------------------------------------------------------------------
bool MCProcessor::IsTauJet(int indexInGenData)
{
// testing
  return false;
// checks if a GenJet is a tau jet
  if(GenData[indexInGenData]->particleType()< 50 ||
     GenData[indexInGenData]->particleType()>=80) {return false;}
// store MCData particles that are the constituents of the GenJet in an array
  std::vector <const GenParticle*> constituents = 
    dynamic_cast<const GenJet*>(GenData[indexInGenData]->jetCandidate())->getGenConstituents();
  std::vector <int> indexinMCData;
  for (int i=0; i< (int) constituents.size(); ++i) {
    for (int j=0; j< (int) MCData.size(); ++j){
      if ( fabs(MCData[j]->eta() - constituents[i]->eta()) < 0.01 
         && fabs(MCData[j]->pt() - constituents[i]->pt()) < 0.1 
         && fabs(MCData[j]->phi() - constituents[i]->phi()) < 0.01){
            indexinMCData.push_back(j);
            break;      
      }
    }
  } 

// check whether the mother or grandmother of the particles in the array is tau

  for (int i=0; i< (int) indexinMCData.size(); ++i) { 
     int indm = MCData[indexinMCData[i]]->motherIndex();
     int pidm = MCData[indm]->pid();
     int indgm = MCData[indm]->motherIndex();
     int pidgm = MCData[indgm]->pid();
     
     if (abs(pidm) == 15){
       GenData[indexInGenData]->setPartonIndex(indm);
       return true;
     } else if (abs(pidgm) == 15) {
       GenData[indexInGenData]->setPartonIndex(indgm);
       return true;
     }
  }

 return false;
 
}

//------------------------------------------------------------------------------

void MCProcessor::MakeMCIsoInfo()
{ 
  // Check isolation of leptons and photons from GenData
  
  int nMCElecIso = 0, nMCMuonIso = 0, nMCphotIso = 0;
  int nMCElecIsoBigEta = 0, nMCMuonIsoBigEta = 0, nMCphotIsoBigEta = 0;
  
  for (int i = 0; i < (int) GenData.size(); ++i) {
    int ptype = GenData[i]->particleType()/10;
    if (ptype == 1 && IsMCIso(i) ){
      if (fabs(GenData[i]->eta()) < ana_elecEtaMax && 
          GenData[i]->pt() > ana_elecPtMin2){nMCElecIso++;}
      if (fabs(GenData[i]->eta()) > ana_elecEtaMax && 
          GenData[i]->pt() > ana_elecPtMin2){nMCElecIsoBigEta++;}
    } else if (ptype == 2 && IsMCIso(i) ) {
      if (fabs(GenData[i]->eta()) < ana_muonEtaMax && 
          GenData[i]->pt() > ana_muonPtMin2){nMCMuonIso++;}
      if (fabs(GenData[i]->eta()) > ana_muonEtaMax && 
          GenData[i]->pt() > ana_muonPtMin2){nMCMuonIsoBigEta++;}
    } else if (ptype == 4 && IsMCIso(i) ) {
      if (fabs(GenData[i]->eta()) < ana_photonEtaMax && 
          GenData[i]->pt() > ana_photonPtMin2){nMCphotIso++;}
      if (fabs(GenData[i]->eta()) > ana_photonEtaMax && 
          GenData[i]->pt() > ana_photonPtMin2){nMCphotIsoBigEta++;}
    }
  }
  
  nMCElecTrue = nMCElecIso;
  nMCMuonTrue = nMCMuonIso;
  nMCPhotTrue = nMCphotIso;
  
  nMCElecBigEtaTrue = nMCElecIsoBigEta;
  nMCMuonBigEtaTrue = nMCMuonIsoBigEta;
  nMCPhotBigEtaTrue = nMCphotIsoBigEta;
    
}

//------------------------------------------------------------------------------

bool MCProcessor::IsMCIso(int indexInGenData)
{ 
 // Checks whether a particle of given index in GenData is isolated
 //  comparison is made with charged pions and kaons from status 1 in MCData
 // Using MrParticle class;

  GenData[indexInGenData]->setParticleIso(true);
  if (indexInGenData < 0) {return true;}
  int ichk = GenData[indexInGenData]->partonIndex();
  if (ichk < 0) {return true;}
  int ptype = MCData[ichk]->particleType()/10;
  if (ptype != 1 && ptype != 2) {return true;}
  float eta = MCData[ichk]->eta();
  float phi = MCData[ichk]->phi();
  
  int iso_MethodObj = 0;
  float iso_ObjDRin = 0., iso_ObjDRout = 0., iso_ObjSeed = 0.,
        iso_ObjIsoValue = 0.;
  int idet = 0;
  if (ptype == 1){
    iso_MethodObj = iso_MethodElec;
    idet = (iso_MethodObj - 2000) / 100;
    if (idet == 1){
      iso_ObjDRin = iso_ElCalDRin;
      iso_ObjDRout = iso_ElCalDRout;
    } else {
      iso_ObjDRin = iso_ElTkDRin;
      iso_ObjDRout = iso_ElTkDRout;
    }
    iso_ObjSeed = iso_ElTkSeed;
    iso_ObjIsoValue = iso_ElIsoValue;
  }
  else if (ptype == 2){
    iso_MethodObj = iso_MethodMuon;
    idet = (iso_MethodObj - 2000) / 100;
    if (idet == 1){
      iso_ObjDRin = iso_MuCalDRin;
      iso_ObjDRout = iso_MuCalDRout;
    } else {
      iso_ObjDRin = iso_MuTkDRin;
      iso_ObjDRout = iso_MuTkDRout;
    }
    iso_ObjSeed = iso_MuTkSeed;
    iso_ObjIsoValue = iso_MuIsoValue;
  }
  
  // Decode the isolation method:
  // idet = 1 for calo iso, = 2 for tracker iso, = 3 for weighted sum
  // itra = 1 for Et sum, = 2 for E sum, = 3 for #objects sum
  // ival = 1 if cut on relative value of Et/E sum, = 2 if absolute value
  int itra = (iso_MethodObj / 10) % 10;
  int ival = (iso_MethodObj % 10);
  
  float pSubtr = 0.;
  float ptest = 0.;
  if (itra == 1) {
    ptest = MCData[ichk]->pt();
    pSubtr = ptest;
  } else if (itra == 2) {
    ptest = MCData[ichk]->p();
    pSubtr = ptest;
  } else if (itra == 3) {
    ptest = 1.;
    pSubtr = 1.;
  }
  int ntkTrkSubtr = 1;
  
  float pSum = 0.;
  for (int i = 0; i < (int) MCData.size(); ++i) {            
    int pid = abs(MCData[i]->pid());
    int status = MCData[i]->status();
    if ( status == 1 && i != ichk &&
        (pid == 211 || pid == 321) &&
//         MCData[i]->p() > iso_ObjSeed){
         MCData[i]->pt() > 1.0){
      float DR = GetDeltaR(eta, MCData[i]->eta(), phi, MCData[i]->phi());
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_ObjDRin && DR <= iso_ObjDRout){
        if (itra == 1){
          pSum += MCData[i]->pt();
        } else if (itra == 2){
          pSum += MCData[i]->p();
        } else if (itra == 3){
          pSum = (int) pSum + 1;
        }
      }
    }
  }
  
  if (itra < 3 && ival == 1) {
    if (iso_ObjDRin == 0.) {pSum = (pSum-pSubtr) / ptest;}
    else {pSum = pSum / ptest;}
  } else if (itra < 3 && ival == 2) {
    if (iso_ObjDRin == 0.) {pSum -= pSubtr;}
  } else if (itra == 3) {
    if (iso_ObjDRin == 0.) {pSum = (int) pSum - ntkTrkSubtr;}
  }
      
  GenData[indexInGenData]->setParticleIsoValue(pSum);
  bool isIsolated = true;
  if (pSum > iso_ObjIsoValue){isIsolated = false;}
  GenData[indexInGenData]->setParticleIso(isIsolated);
  int indexInMCData = GenData[indexInGenData]->partonIndex();
  if (indexInMCData >= 0) {
    MCData[indexInMCData]->setParticleIsoValue(pSum);
    MCData[indexInMCData]->setParticleIso(isIsolated);
  }
  
  return isIsolated;
  
}

//------------------------------------------------------------------------------

bool MCProcessor::IsMCTauIso(int indexInGenData)
{ 
 // Checks whether a tau of given index in GenData is isolated w.r.t. particles in MCData
 //  comparison is made with charged pions and kaons from status 1
 // Using MrParticle class;

  GenData[indexInGenData]->setParticleIso(true);
  if (indexInGenData < 0) {return true;}
  float eta = GenData[indexInGenData]->eta();
  float phi = GenData[indexInGenData]->phi();
  
  // Decode the isolation method:
  // idet = 1 for calo iso, = 2 for tracker iso, = 3 for weighted sum
  // itra = 1 for Et sum, = 2 for E sum, = 3 for #objects sum
  // ival = 1 if cut on relative value of Et/E sum, = 2 if absolute value
  int idet = (iso_MethodTau - 2000) / 100;
  int itra = (iso_MethodTau / 10) % 10;
  int ival = (iso_MethodTau % 10);
  
  float iso_TauDRin = 0., iso_TauDRout = 0., iso_TauSeed = 0.;
  if (idet == 1){
    iso_TauDRin = iso_TauCalDRin;
    iso_TauDRout = iso_TauCalDRout;
    iso_TauSeed = iso_TauCalSeed;
  } else {
    iso_TauDRin = iso_TauTkDRin;
    iso_TauDRout = iso_TauTkDRout;
    iso_TauSeed = iso_TauTkSeed;
  }
  
  float ptest = 0.;
  if (itra == 1) {
    ptest = GenData[indexInGenData]->pt();
  } else if (itra == 2) {
    ptest = GenData[indexInGenData]->p();
  } else if (itra == 3) {
    ptest = 1.;
  }
  
  float pSum = 0.;
  for (int i = 0; i < (int) MCData.size(); ++i) {            
    int pid = abs(MCData[i]->pid());
    int status = MCData[i]->status();
    if ( status == 1 &&
        (pid == 211 || pid == 321) &&
         MCData[i]->p() > iso_TauSeed){
      float DR = GetDeltaR(eta, MCData[i]->eta(), phi, MCData[i]->phi());
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_TauDRin && DR <= iso_TauDRout){
        if (itra == 1){
          pSum += MCData[i]->pt();
        } else if (itra == 2){
          pSum += MCData[i]->p();
        } else if (itra == 3){
          pSum = (int) pSum + 1;
        }
      }
    }
  }
  
  if (itra < 3 && ival == 1) {
    pSum = pSum / ptest;
  }
      
  GenData[indexInGenData]->setParticleIsoValue(pSum);
  bool isIsolated = true;
  if (pSum > iso_TauIsoValue){isIsolated = false;}
  GenData[indexInGenData]->setParticleIso(isIsolated);
  int indexInMCData = GenData[indexInGenData]->partonIndex();
  if (indexInMCData >= 0) {
    MCData[indexInMCData]->setParticleIsoValue(pSum);
    MCData[indexInMCData]->setParticleIso(isIsolated);
  }
  
  return isIsolated;
  
}

//------------------------------------------------------------------------------

void MCProcessor::MakeMCSusyMothIso()
{ 
  // Check isolation of leptons and taus from GenData
  // call them isolated of they come from a W or Z or a SUSY direct parent
  
  int nMCElecIso = 0, nMCMuonIso = 0, nMCTauIso = 0;
  int nMCElecIsoBigEta = 0, nMCMuonIsoBigEta = 0, nMCTauIsoBigEta = 0;
  
  for (int i = 0; i < (int) GenData.size(); ++i) {
    bool isIso = false;
    int ptype = GenData[i]->particleType()/10;
    if (ptype == 1 || ptype == 2 || ptype == 3){
//    cout << "Lepton found " << endl;
      int indMCData = GenData[i]->partonIndex();
      int indm = MCData[indMCData]->motherIndex();
      int pidm = abs(MCData[indm]->pid());
//      cout << " Mother pid = " << pidm << endl;
      if (pidm == 11 || pidm == 13) {
        indm = MCData[indm]->motherIndex();
        pidm = abs(MCData[indm]->pid());
//        cout << " Grandmother pid = " << pidm << endl;
      }
      if (pidm == 23 || pidm == 24 || pidm == 15 || pidm > 1000000) {
        isIso = true;
        if (ptype == 1 ) {
          if (fabs(GenData[i]->eta()) < ana_elecEtaMax && 
              GenData[i]->pt() > ana_elecPtMin2){nMCElecIso++;}
          if (fabs(GenData[i]->eta()) > ana_elecEtaMax && 
              GenData[i]->pt() > ana_elecPtMin2){nMCElecIsoBigEta++;}
        } else if (ptype == 2 ) {
          if (fabs(GenData[i]->eta()) < ana_muonEtaMax && 
              GenData[i]->pt() > ana_muonPtMin2){nMCMuonIso++;}
          if (fabs(GenData[i]->eta()) > ana_muonEtaMax && 
              GenData[i]->pt() > ana_muonPtMin2){nMCMuonIsoBigEta++;}
        } else if (ptype == 3 ) {
          if (fabs(GenData[i]->eta()) < ana_tauEtaMax && 
              GenData[i]->pt() > ana_tauPtMin2){nMCTauIso++;}
          if (fabs(GenData[i]->eta()) > ana_tauEtaMax && 
              GenData[i]->pt() > ana_tauPtMin2){nMCTauIsoBigEta++;}
        }
      }
      GenData[i]->setParticleIso(isIso);
      GenData[i]->setParticleIsoValue(0.);
      int indexInMCData = GenData[i]->partonIndex();
      if (indexInMCData >= 0) {
        MCData[indexInMCData]->setParticleIsoValue(0.);
        MCData[indexInMCData]->setParticleIso(isIso);
      }
    }
  }
  
  nMCElecTrue = nMCElecIso;
  nMCMuonTrue = nMCMuonIso;
  nMCTauTrue = nMCTauIso;
  
  nMCElecBigEtaTrue = nMCElecIsoBigEta;
  nMCMuonBigEtaTrue = nMCMuonIsoBigEta;
  nMCTauBigEtaTrue = nMCTauIsoBigEta;
    
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


