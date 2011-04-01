// contains the older method to get the HLT information
#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <TLorentzVector.h>

#include "QCDAnalysis/HighPtJetAnalysis/interface/ProcessedTreeProducer.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

using namespace edm;
using namespace reco;
using namespace std;

ProcessedTreeProducer::ProcessedTreeProducer(edm::ParameterSet const& cfg) 
{
  mPFJetsName      = cfg.getParameter<std::string>          ("pfjets");
  mCaloJetsName    = cfg.getParameter<std::string>          ("calojets");
  mPFJECservice    = cfg.getParameter<std::string>          ("pfjecService");
  mCaloJECservice  = cfg.getParameter<std::string>          ("calojecService");
  mPFPayloadName   = cfg.getParameter<std::string>          ("PFPayloadName");
  mCaloPayloadName = cfg.getParameter<std::string>          ("CaloPayloadName");
  mCaloJetID       = cfg.getParameter<std::string>          ("calojetID");
  mCaloJetExtender = cfg.getParameter<std::string>          ("calojetExtender");
  mNPFJETS_MAX     = cfg.getParameter<int>                  ("nPFJets");
  mNCaloJETS_MAX   = cfg.getParameter<int>                  ("nCaloJets");
  mGoodVtxNdof     = cfg.getParameter<double>               ("goodVtxNdof");
  mGoodVtxZ        = cfg.getParameter<double>               ("goodVtxZ"); 
  mPreselection    = cfg.getParameter<bool>                 ("preselDijet");
  mIsMCarlo        = cfg.getUntrackedParameter<bool>        ("isMCarlo",false);
  mGenJetsName     = cfg.getUntrackedParameter<std::string> ("genjets","");
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducer::beginJob() 
{
  mTree = fs->make<TTree>("ProcessedTree","ProcessedTree");
  buildTree();
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducer::endJob() 
{

}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducer::initEvent()
{
  mEvent.runNo              = -999;
  mEvent.evtNo              = -999;
  mEvent.lumi               = -999;
  mEvent.bunch              = -999;
  mEvent.nVtx               = -999;
  mEvent.isPVgood           = -999;
  mEvent.PVz                = -999;
  mEvent.PVx                = -999;
  mEvent.PVy                = -999;
  mEvent.PVndof             = -999;
  mEvent.pthat              = -999;
  mEvent.weight             = -999;
  mEvent.pfRawMass          = -999;
  mEvent.pfCorMass          = -999;
  mEvent.pfCorMassUp        = -999;
  mEvent.pfCorMassDo        = -999;
  mEvent.pfDeta             = -999;
  mEvent.pfYmax             = -999;
  mEvent.caloRawMass        = -999;
  mEvent.caloCorMass        = -999;
  mEvent.caloCorMassUp      = -999;
  mEvent.caloCorMassDo      = -999;
  mEvent.caloDeta           = -999;
  mEvent.caloYmax           = -999;
  mEvent.pfmet              = -999;
  mEvent.pfsumet            = -999;
  mEvent.pfmet_over_sumet   = -999;
  mEvent.calomet            = -999;
  mEvent.calosumet          = -999;
  mEvent.calomet_over_sumet = -999;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducer::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 
  //Clear all the vectors stored in tree
  clearTreeArrays();
  //initialize the event variables
  initEvent();
  //-------------- Basic Event Info ------------------------------
  mEvent.runNo = event.id().run();
  mEvent.evtNo = event.id().event();
  mEvent.lumi  = event.luminosityBlock();
  mEvent.bunch = event.bunchCrossing();
  //-------------- Vertex Info -----------------------------------
  Handle<reco::VertexCollection> recVtxs;
  event.getByLabel("offlinePrimaryVertices",recVtxs);
  mEvent.nVtx = recVtxs->size();
  int VtxGood(0);
  for(VertexCollection::const_iterator i_vtx = recVtxs->begin(); i_vtx != recVtxs->end(); i_vtx++) {
    int index = i_vtx-recVtxs->begin();
    if (index == 0) {
      mEvent.PVx    = i_vtx->x();
      mEvent.PVy    = i_vtx->y();
      mEvent.PVz    = i_vtx->z();
      mEvent.PVndof = i_vtx->ndof();
    }
    if (!(i_vtx->isFake()) && i_vtx->ndof() >= mGoodVtxNdof && fabs(i_vtx->z()) <= mGoodVtxZ) {
      if (index == 0) {
        mEvent.isPVgood = 1;
      }
      VtxGood++;
    }
  }
  mEvent.nVtxGood = VtxGood;
  //-------------- Generator Info -------------------------------------
  Handle<GenEventInfoProduct> hEventInfo;
  if (mIsMCarlo) { 
    event.getByLabel("generator", hEventInfo);
    mEvent.pthat  = hEventInfo->binningValues()[0];
    mEvent.weight = hEventInfo->weight();
  }
  //---------------- Jets ---------------------------------------------
  mPFJEC   = JetCorrector::getJetCorrector(mPFJECservice,iSetup);
  mCALOJEC = JetCorrector::getJetCorrector(mCaloJECservice,iSetup);
  edm::ESHandle<JetCorrectorParametersCollection> PFJetCorParColl;
  iSetup.get<JetCorrectionsRecord>().get(mPFPayloadName,PFJetCorParColl); 
  JetCorrectorParameters const& PFJetCorPar = (*PFJetCorParColl)["Uncertainty"];
  mPFUnc = new JetCorrectionUncertainty(PFJetCorPar);
  edm::ESHandle<JetCorrectorParametersCollection> CaloJetCorParColl;
  iSetup.get<JetCorrectionsRecord>().get(mCaloPayloadName,CaloJetCorParColl);    
  JetCorrectorParameters const& CaloJetCorPar = (*CaloJetCorParColl)["Uncertainty"];
  mCALOUnc = new JetCorrectionUncertainty(CaloJetCorPar);

  Handle<GenJetCollection>  genjets;
  Handle<PFJetCollection>   pfjets;
  Handle<CaloJetCollection> calojets;
  Handle<JetExtendedAssociation::Container> calojetExtender;
  Handle<ValueMap<reco::JetID> > calojetID;
  event.getByLabel(mPFJetsName,pfjets);
  event.getByLabel(mCaloJetsName,calojets);
  event.getByLabel(mCaloJetExtender,calojetExtender);
  event.getByLabel(mCaloJetID,calojetID);
  if (mIsMCarlo)
    event.getByLabel(mGenJetsName,genjets);
  vector<PFJET_VAR> all_pfjets;
  vector<CALOJET_VAR> all_calojets;
  PFJET_VAR aux_pfjet;
  CALOJET_VAR aux_calojet;
  int npfjets(0),ncalojets(0);
  for(PFJetCollection::const_iterator i_pfjet = pfjets->begin(); i_pfjet != pfjets->end(); i_pfjet++) {
    int index = i_pfjet-pfjets->begin();
    edm::RefToBase<reco::Jet> pfjetRef(edm::Ref<PFJetCollection>(pfjets,index));
    double scale = mPFJEC->correction(*i_pfjet,pfjetRef,event,iSetup);
    mPFUnc->setJetEta(i_pfjet->eta());
    mPFUnc->setJetPt(scale * i_pfjet->pt());
    double unc = mPFUnc->getUncertainty(true);
    aux_pfjet.rawPt = i_pfjet->pt();
    aux_pfjet.corPt = scale*i_pfjet->pt();
    aux_pfjet.eta   = i_pfjet->eta();
    aux_pfjet.y     = i_pfjet->y();
    aux_pfjet.phi   = i_pfjet->phi();
    aux_pfjet.rawE  = i_pfjet->energy();
    aux_pfjet.corE  = scale*i_pfjet->energy();
    aux_pfjet.m     = i_pfjet->mass();
    aux_pfjet.jec   = scale;
    aux_pfjet.jecUnc= unc;
    aux_pfjet.chf   = i_pfjet->chargedHadronEnergyFraction();
    aux_pfjet.nhf   = (i_pfjet->neutralHadronEnergy() + i_pfjet->HFHadronEnergy())/i_pfjet->energy();
    aux_pfjet.phf   = i_pfjet->photonEnergyFraction();
    aux_pfjet.elf   = i_pfjet->electronEnergyFraction();
    aux_pfjet.chm   = i_pfjet->chargedHadronMultiplicity();
    aux_pfjet.nhm   = i_pfjet->neutralHadronMultiplicity();
    aux_pfjet.phm   = i_pfjet->photonMultiplicity();
    aux_pfjet.elm   = i_pfjet->electronMultiplicity();
    aux_pfjet.npr   = i_pfjet->chargedMultiplicity() + i_pfjet->neutralMultiplicity();
    aux_pfjet.passLooseID = 0;
    bool looseID  = (aux_pfjet.npr>1 && aux_pfjet.phf<0.99 && aux_pfjet.nhf<0.99 && ((fabs(aux_pfjet.eta)<=2.4 && aux_pfjet.elf<0.99 && aux_pfjet.chf>0 && aux_pfjet.chm>0) || fabs(aux_pfjet.eta)>2.4)) ;
    if (looseID)
      aux_pfjet.passLooseID = 1;    
    if (mIsMCarlo) {
      GenJetCollection::const_iterator i_matched;
      float rmin(999);
      for(GenJetCollection::const_iterator i_gen = genjets->begin(); i_gen != genjets->end(); i_gen++) {
        double deltaR = reco::deltaR(*i_pfjet,*i_gen);
        if (deltaR < rmin) {
          rmin = deltaR;
          i_matched = i_gen;
        }
      }
      aux_pfjet.genR   = rmin;
      aux_pfjet.genE   = i_matched->energy();
      aux_pfjet.genPt  = i_matched->pt();
      aux_pfjet.genEta = i_matched->eta();
      aux_pfjet.genPhi = i_matched->phi();
      aux_pfjet.genM   = i_matched->mass();
    }
    all_pfjets.push_back(aux_pfjet);
    npfjets++;
  }
  sort(all_pfjets.begin(),all_pfjets.end(),sort_pfjets);
  for(int i=0;i<min(mNPFJETS_MAX,npfjets);i++) {
      mPFJets[i] = all_pfjets[i];
  }
  if (npfjets > 1) {
    TLorentzVector pfcorP4[2],pfcorP4Up[2],pfcorP4Do[2],pfrawP4[2];
    pfrawP4[0].SetPtEtaPhiE(mPFJets[0].rawPt,mPFJets[0].eta,mPFJets[0].phi,mPFJets[0].rawE);
    pfrawP4[1].SetPtEtaPhiE(mPFJets[1].rawPt,mPFJets[1].eta,mPFJets[1].phi,mPFJets[1].rawE);
    mEvent.pfRawMass = (pfrawP4[0]+pfrawP4[1]).M();

    pfcorP4[0].SetPtEtaPhiE(mPFJets[0].corPt,mPFJets[0].eta,mPFJets[0].phi,mPFJets[0].corE);
    pfcorP4[1].SetPtEtaPhiE(mPFJets[1].corPt,mPFJets[1].eta,mPFJets[1].phi,mPFJets[1].corE);
    mEvent.pfCorMass = (pfcorP4[0]+pfcorP4[1]).M();

    double ss0 = mPFJets[0].jecUnc;
    double ss1 = mPFJets[1].jecUnc;
    pfcorP4Up[0].SetPtEtaPhiE((1+ss0)*mPFJets[0].corPt,mPFJets[0].eta,mPFJets[0].phi,(1+ss0)*mPFJets[0].corE);
    pfcorP4Up[1].SetPtEtaPhiE((1+ss1)*mPFJets[1].corPt,mPFJets[1].eta,mPFJets[1].phi,(1+ss1)*mPFJets[1].corE);
    mEvent.pfCorMassUp = (pfcorP4Up[0]+pfcorP4Up[1]).M();

    pfcorP4Do[0].SetPtEtaPhiE((1-ss0)*mPFJets[0].corPt,mPFJets[0].eta,mPFJets[0].phi,(1-ss0)*mPFJets[0].corE);
    pfcorP4Do[1].SetPtEtaPhiE((1-ss1)*mPFJets[1].corPt,mPFJets[1].eta,mPFJets[1].phi,(1-ss1)*mPFJets[1].corE);
    mEvent.pfCorMassDo = (pfcorP4Do[0]+pfcorP4Do[1]).M();

    mEvent.pfDeta = mPFJets[0].eta - mPFJets[1].eta;
    mEvent.pfYmax = max(fabs(mPFJets[0].y),fabs(mPFJets[1].y));
  }

  for(CaloJetCollection::const_iterator i_calojet = calojets->begin(); i_calojet != calojets->end(); i_calojet++) {
    int index = i_calojet-calojets->begin();
    edm::RefToBase<reco::Jet> calojetRef(edm::Ref<CaloJetCollection>(calojets,index));
    double scale = mCALOJEC->correction(*i_calojet,calojetRef,event,iSetup);
    mCALOUnc->setJetEta(i_calojet->eta());
    mCALOUnc->setJetPt(scale * i_calojet->pt());
    double unc = mCALOUnc->getUncertainty(true);
    aux_calojet.rawPt    = i_calojet->pt();
    aux_calojet.corPt    = scale*i_calojet->pt();
    aux_calojet.eta      = i_calojet->eta();
    aux_calojet.y        = i_calojet->y();
    aux_calojet.phi      = i_calojet->phi();
    aux_calojet.rawE     = i_calojet->energy();
    aux_calojet.corE     = scale*i_calojet->energy();
    aux_calojet.m        = i_calojet->mass();
    aux_calojet.jec      = scale;
    aux_calojet.jecUnc   = unc;
    aux_calojet.emf      = i_calojet->emEnergyFraction();
    aux_calojet.n90hits  = int((*calojetID)[calojetRef].n90Hits);
    aux_calojet.fHPD     = (*calojetID)[calojetRef].fHPD;
    aux_calojet.fRBX     = (*calojetID)[calojetRef].fRBX;
    aux_calojet.nTrkVtx  = JetExtendedAssociation::tracksAtVertexNumber(*calojetExtender,*i_calojet);
    aux_calojet.nTrkCalo = JetExtendedAssociation::tracksAtCaloNumber(*calojetExtender,*i_calojet);
    bool looseID  = ((i_calojet->emEnergyFraction() > 0.01 || fabs(i_calojet->eta()) > 2.6) && (aux_calojet.n90hits > 1) && (aux_calojet.fHPD < 0.98));
    aux_calojet.passLooseID = 0;
    if (looseID) 
      aux_calojet.passLooseID = 1;
    if (mIsMCarlo) {
      GenJetCollection::const_iterator i_matched;
      float rmin(999);
      for(GenJetCollection::const_iterator i_gen = genjets->begin(); i_gen != genjets->end(); i_gen++) {
        double deltaR = reco::deltaR(*i_calojet,*i_gen);
        if (deltaR < rmin) {
          rmin = deltaR;
          i_matched = i_gen;
        }
      }
      aux_calojet.genR   = rmin;
      aux_calojet.genE   = i_matched->energy();
      aux_calojet.genPt  = i_matched->pt();
      aux_calojet.genEta = i_matched->eta();
      aux_calojet.genPhi = i_matched->phi();
      aux_calojet.genM   = i_matched->mass();
    }
    all_calojets.push_back(aux_calojet);
    ncalojets++;
  }
  
  sort(all_calojets.begin(),all_calojets.end(),sort_calojets);
  for(int i=0;i<min(mNCaloJETS_MAX,ncalojets);i++) {
    mCaloJets[i] = all_calojets[i];
  }
  if (ncalojets > 1) {
    TLorentzVector calocorP4[2],calocorP4Up[2],calocorP4Do[2],calorawP4[2];
    calorawP4[0].SetPtEtaPhiE(mCaloJets[0].rawPt,mCaloJets[0].eta,mCaloJets[0].phi,mCaloJets[0].rawE);
    calorawP4[1].SetPtEtaPhiE(mCaloJets[1].rawPt,mCaloJets[1].eta,mCaloJets[1].phi,mCaloJets[1].rawE);
    mEvent.caloRawMass = (calorawP4[0]+calorawP4[1]).M();

    calocorP4[0].SetPtEtaPhiE(mCaloJets[0].corPt,mCaloJets[0].eta,mCaloJets[0].phi,mCaloJets[0].corE);
    calocorP4[1].SetPtEtaPhiE(mCaloJets[1].corPt,mCaloJets[1].eta,mCaloJets[1].phi,mCaloJets[1].corE);
    mEvent.caloCorMass = (calocorP4[0]+calocorP4[1]).M();

    double ss0 = mCaloJets[0].jecUnc;
    double ss1 = mCaloJets[1].jecUnc;
    calocorP4Up[0].SetPtEtaPhiE((1+ss0)*mCaloJets[0].corPt,mCaloJets[0].eta,mCaloJets[0].phi,(1+ss0)*mCaloJets[0].corE);
    calocorP4Up[1].SetPtEtaPhiE((1+ss1)*mCaloJets[1].corPt,mCaloJets[1].eta,mCaloJets[1].phi,(1+ss1)*mCaloJets[1].corE);
    mEvent.caloCorMassUp = (calocorP4Up[0]+calocorP4Up[1]).M();

    calocorP4Do[0].SetPtEtaPhiE((1-ss0)*mCaloJets[0].corPt,mCaloJets[0].eta,mCaloJets[0].phi,(1-ss0)*mCaloJets[0].corE);
    calocorP4Do[1].SetPtEtaPhiE((1-ss1)*mCaloJets[1].corPt,mCaloJets[1].eta,mCaloJets[1].phi,(1-ss1)*mCaloJets[1].corE);
    mEvent.caloCorMassDo = (calocorP4Do[0]+calocorP4Do[1]).M();

    mEvent.caloDeta = mCaloJets[0].eta - mCaloJets[1].eta;
    mEvent.caloYmax = max(fabs(mCaloJets[0].y),fabs(mCaloJets[1].y));
  }
  //---------------- met ---------------------------------------------
  Handle<PFMETCollection> pfmet;
  Handle<CaloMETCollection> calomet;
  event.getByLabel("pfMet",pfmet);
  event.getByLabel("met",calomet);
  mEvent.pfmet   = (*pfmet)[0].et();
  mEvent.pfsumet = (*pfmet)[0].sumEt();
  mEvent.pfmet_over_sumet = (*pfmet)[0].et()/(*pfmet)[0].sumEt();
  mEvent.calomet   = (*calomet)[0].et();
  mEvent.calosumet = (*calomet)[0].sumEt();
  mEvent.calomet_over_sumet = (*calomet)[0].et()/(*calomet)[0].sumEt();
  //-------------- fill the tree -------------------------------------  
  if (mPreselection) {
    if (npfjets > 1 || ncalojets > 1) {
      mTree->Fill();
    }
  }
  else {
    mTree->Fill();
  } 
  delete mPFUnc;
  delete mCALOUnc;
}
//////////////////////////////////////////////////////////////////////////////////////////
ProcessedTreeProducer::~ProcessedTreeProducer() 
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducer::buildTree() 
{
  char name[1000];
  mTree->Branch("event", &mEvent, "runNo/I:evtNo:lumi:bunch:nVtx:nVtxGood:isPVgood:PVndof/F:PVx:PVy:PVz:pfCorMass:pfCorMassUp:pfCorMassDo:pfRawMass:caloCorMass:caloCorMassUp:caloCorMassDo:caloRawMass:pfDeta:caloDeta:pfYmax:caloYmax:pfmet:pfsumet:pfmet_over_sumet:calomet:calosumet:calomet_over_sumet:pthat:weight");
  for(int i=0;i<mNPFJETS_MAX;i++) {
    sprintf(name,"pfjet%d",i+1);
    mTree->Branch(name, &mPFJets[i].passLooseID,"passLooseID/I:npr:chm:nhm:phm:elm:jec/F:jecUnc:rawPt:corPt:eta:y:phi:rawE:corE:m:chf:nhf:phf:elf:genR:genPt:genEta:genPhi:genE:genM:corRsp:rawRsp");
  } 
  for(int i=0;i<mNCaloJETS_MAX;i++) {
    sprintf(name,"calojet%d",i+1);
    mTree->Branch(name, &mCaloJets[i].passLooseID,"passLooseID/I:n90hits:nTrkVtx:nTrkCalo:jec/F:jecUnc:rawPt:corPt:eta:y:phi:rawE:corE:m:emf:fHPD:fRBX:genR:genPt:genEta:genPhi:genE:genM:corRsp:rawRsp");
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducer::clearTreeArrays() 
{
  for(int i=0;i<mNPFJETS_MAX;i++) {
    mPFJets[i].passLooseID = -999;
    mPFJets[i].npr         = -999;
    mPFJets[i].chm         = -999;
    mPFJets[i].nhm         = -999;
    mPFJets[i].phm         = -999;
    mPFJets[i].elm         = -999;
    mPFJets[i].rawPt       = -999;
    mPFJets[i].corPt       = -999;
    mPFJets[i].eta         = -999;
    mPFJets[i].y           = -999;
    mPFJets[i].phi         = -999;
    mPFJets[i].rawE        = -999;
    mPFJets[i].corE        = -999;
    mPFJets[i].m           = -999;
    mPFJets[i].jec         = -999;
    mPFJets[i].jecUnc      = -999;
    mPFJets[i].chf         = -999;
    mPFJets[i].nhf         = -999;
    mPFJets[i].phf         = -999;
    mPFJets[i].elf         = -999;
    mPFJets[i].genR        = -999;
    mPFJets[i].genPt       = -999;
    mPFJets[i].genEta      = -999;
    mPFJets[i].genPhi      = -999;
    mPFJets[i].genE        = -999;
    mPFJets[i].genM        = -999;
    mPFJets[i].corRsp      = -999;
    mPFJets[i].rawRsp      = -999;
  }
  for(int i=0;i<mNCaloJETS_MAX;i++) {
    mCaloJets[i].passLooseID = -999;
    mCaloJets[i].n90hits     = -999;
    mCaloJets[i].nTrkVtx     = -999;
    mCaloJets[i].nTrkCalo    = -999;
    mCaloJets[i].rawPt       = -999;
    mCaloJets[i].corPt       = -999;
    mCaloJets[i].eta         = -999;
    mCaloJets[i].y           = -999;
    mCaloJets[i].phi         = -999;
    mCaloJets[i].rawE        = -999;
    mCaloJets[i].corE        = -999;
    mCaloJets[i].m           = -999;
    mCaloJets[i].jec         = -999;
    mCaloJets[i].jecUnc      = -999;
    mCaloJets[i].emf         = -999;
    mCaloJets[i].fHPD        = -999;
    mCaloJets[i].fRBX        = -999;
    mCaloJets[i].genR        = -999;
    mCaloJets[i].genPt       = -999;
    mCaloJets[i].genEta      = -999;
    mCaloJets[i].genPhi      = -999;
    mCaloJets[i].genE        = -999;
    mCaloJets[i].genM        = -999;
    mCaloJets[i].corRsp      = -999;
    mCaloJets[i].rawRsp      = -999;
  }
}
