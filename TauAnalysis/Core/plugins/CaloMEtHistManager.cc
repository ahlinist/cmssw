#include "TauAnalysis/Core/plugins/CaloMEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

CaloMEtHistManager::CaloMEtHistManager(const edm::ParameterSet& cfg)
  : MEtHistManager(cfg)
{
  //std::cout << "<CaloMEtHistManager::CaloMEtHistManager>:" << std::endl;
}

CaloMEtHistManager::~CaloMEtHistManager()
{
//--- nothing to be done yet...
}

void CaloMEtHistManager::bookHistogramsImp()
{
  //std::cout << "<CaloMEtHistManager::bookHistogramsImp>:" << std::endl;
  
  MEtHistManager::bookHistogramsImp();

  hRAWplusJESplusMUONplusTAU_MEtPt_ = book1D("RAWplusJESplusMUONplusTAU_MEtPt", "RAWplusJESplusMUONplusTAU_MEtPt", 75, 0., 150.);
  hRAWplusJESplusMUONplusTAU_MEtPhi_ = book1D("RAWplusJESplusMUONplusTAU_MEtPhi", "RAWplusJESplusMUONplusTAU_MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hRAWplusJESplusMUONplusTAU_MEtPx_ = book1D("RAWplusJESplusMUONplusTAU_MEtPx", "RAWplusJESplusMUONplusTAU_MEtPx", 150, -150., 150.);
  hRAWplusJESplusMUONplusTAU_MEtPy_ = book1D("RAWplusJESplusMUONplusTAU_MEtPy", "RAWplusJESplusMUONplusTAU_MEtPy", 150, -150., 150.);
  
  hRAWplusJESplusMUON_MEtPt_ = book1D("RAWplusJESplusMUON_MEtPt", "RAWplusJESplusMUON_MEtPt", 75, 0., 150.);
  hRAWplusJESplusMUON_MEtPhi_ = book1D("RAWplusJESplusMUON_MEtPhi", "RAWplusJESplusMUON_MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hRAWplusJESplusMUON_MEtPx_ = book1D("RAWplusJESplusMUON_MEtPx", "RAWplusJESplusMUON_MEtPx", 150, -150., 150.);
  hRAWplusJESplusMUON_MEtPy_ = book1D("RAWplusJESplusMUON_MEtPy", "RAWplusJESplusMUON_MEtPy", 150, -150., 150.);
  
  hRAWplusMUONplusTAU_MEtPt_ = book1D("RAWplusMUONplusTAU_MEtPt", "RAWplusMUONplusTAU_MEtPt", 75, 0., 150.);
  hRAWplusMUONplusTAU_MEtPhi_ = book1D("RAWplusMUONplusTAU_MEtPhi", "RAWplusMUONplusTAU_MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hRAWplusMUONplusTAU_MEtPx_ = book1D("RAWplusMUONplusTAU_MEtPx", "RAWplusMUONplusTAU_MEtPx", 150, -150., 150.);
  hRAWplusMUONplusTAU_MEtPy_ = book1D("RAWplusMUONplusTAU_MEtPy", "RAWplusMUONplusTAU_MEtPy", 150, -150., 150.);
  
  hRAW_MEtPt_ = book1D("RAW_MEtPt", "RAW_MEtPt", 75, 0., 150.);
  hRAW_MEtPhi_ = book1D("RAW_MEtPhi", "RAW_MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hRAW_MEtPx_ = book1D("RAW_MEtPx", "RAW_MEtPx", 150, -150., 150.);
  hRAW_MEtPy_ = book1D("RAW_MEtPy", "RAW_MEtPy", 150, -150., 150.);
  
  hRAW_MEtSignificance_ = book1D("RAW_MEtSignificance", "RAW_MEtSignificance", 101, -0.5, 100.05);

  hRAWplusJES_MEtPt_ = book1D("RAWplusJES_MEtPt", "RAWplusJES_MEtPt", 75, 0., 150.);
  hRAWplusJES_MEtPhi_ = book1D("RAWplusJES_MEtPhi", "RAWplusJES_MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hRAWplusJES_MEtPx_ = book1D("RAWplusJES_MEtPx", "RAWplusJES_MEtPx", 150, -150., 150.);
  hRAWplusJES_MEtPy_ = book1D("RAWplusJES_MEtPy", "RAWplusJES_MEtPy", 150, -150., 150.);
  
  hMUON_MExCorrection_ = book1D("MUON_MExCorrection", "MUON_MExCorrection", 150, -150., 150.);
  hMUON_MEyCorrection_ = book1D("MUON_MEyCorrection", "MUON_MEyCorrection", 150, -150., 150.);
  hTAU_MExCorrection_ = book1D("TAU_MExCorrection", "TAU_MExCorrection", 150, -150., 150.);
  hTAU_MEyCorrection_ = book1D("TAU_MEyCorrection", "TAU_MEyCorrection", 150, -150., 150.);
  hJES_MExCorrection_ = book1D("JES_MExCorrection", "JES_MExCorrection", 150, -150., 150.);
  hJES_MEyCorrection_ = book1D("JES_MEyCorrection", "JES_MEyCorrection", 150, -150., 150.);
  
  hRAWplusJESplusMUONplusTAUMEtPtDiffGen_ = book1D("RAWplusJESplusMUONplusTAUMEtPtDiffGen", "RAWplusJESplusMUONplusTAUMEtPtDiffGen", 100, -50., +50.);
  hRAWplusJESplusMUONplusTAUMEtPtResGen_ = book1D("RAWplusJESplusMUONplusTAUMEtPtResGen", "RAWplusJESplusMUONplusTAUMEtPtResGen", 100, -5.0, +5.0);
  hRAWplusJESplusMUONplusTAUMEtPtRecVsGen_ = book2D("RAWplusJESplusMUONplusTAUMEtPtRecVsGen", "RAWplusJESplusMUONplusTAUMEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hRAWplusJESplusMUONplusTAUMEtPhiDiffGen_ = book1D("RAWplusJESplusMUONplusTAUMEtPhiDiffGen", "RAWplusJESplusMUONplusTAUMEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hRAWplusJESplusMUONplusTAUMEtPhiRecVsGen_ = book2D("RAWplusJESplusMUONplusTAUMEtPhiRecVsGen", "RAWplusJESplusMUONplusTAUMEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  
  hRAWplusJESplusMUONMEtPtDiffGen_ = book1D("RAWplusJESplusMUONMEtPtDiffGen", "RAWplusJESplusMUONMEtPtDiffGen", 100, -50., +50.);
  hRAWplusJESplusMUONMEtPtResGen_ = book1D("RAWplusJESplusMUONMEtPtResGen", "RAWplusJESplusMUONMEtPtResGen", 100, -5.0, +5.0);
  hRAWplusJESplusMUONMEtPtRecVsGen_ = book2D("RAWplusJESplusMUONMEtPtRecVsGen", "RAWplusJESplusMUONMEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hRAWplusJESplusMUONMEtPhiDiffGen_ = book1D("RAWplusJESplusMUONMEtPhiDiffGen", "RAWplusJESplusMUONMEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hRAWplusJESplusMUONMEtPhiRecVsGen_ = book2D("RAWplusJESplusMUONMEtPhiRecVsGen", "RAWplusJESplusMUONMEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  
  hRAWplusJESMEtPtDiffGen_ = book1D("RAWplusJESMEtPtDiffGen", "RAWplusJESMEtPtDiffGen", 100, -50., +50.);
  hRAWplusJESMEtPtResGen_ = book1D("RAWplusJESMEtPtResGen", "RAWplusJESMEtPtResGen", 100, -5.0, +5.0);
  hRAWplusJESMEtPtRecVsGen_ = book2D("RAWplusJESMEtPtRecVsGen", "RAWplusJESMEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hRAWplusJESMEtPhiDiffGen_ = book1D("RAWplusJESMEtPhiDiffGen", "RAWplusJESMEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hRAWplusJESMEtPhiRecVsGen_ = book2D("RAWplusJESMEtPhiRecVsGen", "RAWplusJESMEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  
  hRAWMEtPtDiffGen_ = book1D("RAWMEtPtDiffGen", "RAWMEtPtDiffGen", 100, -50., +50.);
  hRAWMEtPtResGen_ = book1D("RAWMEtPtResGen", "RAWMEtPtResGen", 100, -5.0, +5.0);
  hRAWMEtPtRecVsGen_ = book2D("RAWMEtPtRecVsGen", "RAWMEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hRAWMEtPhiDiffGen_ = book1D("RAWMEtPhiDiffGen", "RAWMEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hRAWMEtPhiRecVsGen_ = book2D("RAWMEtPhiRecVsGen", "RAWMEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  
  hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Pt_ = book1D("GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Pt", "GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Pt", 150, -150.0, 150.0);
  hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Phi_ = book1D("GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Phi", "GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());
  hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Px_ = book1D("GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Px", "GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Px", 150, -150.0, 150.0);
  hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Py_ = book1D("GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Py", "GenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Py", 150, -150.0, 150.0);
  
  hGenMEtDeltaRAWplusJESplusMUONMEt_Pt_ = book1D("GenMEtDeltaRAWplusJESplusMUONMEt_Pt", "GenMEtDeltaRAWplusJESplusMUONMEt_Pt", 150, -150.0, 150.0);
  hGenMEtDeltaRAWplusJESplusMUONMEt_Phi_ = book1D("GenMEtDeltaRAWplusJESplusMUONMEt_Phi", "GenMEtDeltaRAWplusJESplusMUONMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());
  hGenMEtDeltaRAWplusJESplusMUONMEt_Px_ = book1D("GenMEtDeltaRAWplusJESplusMUONMEt_Px", "GenMEtDeltaRAWplusJESplusMUONMEt_Px", 150, -150.0, 150.0);
  hGenMEtDeltaRAWplusJESplusMUONMEt_Py_ = book1D("GenMEtDeltaRAWplusJESplusMUONMEt_Py", "GenMEtDeltaRAWplusJESplusMUONMEt_Py", 150, -150.0, 150.0);
  
  hGenMEtDeltaRAWplusJESMEt_Pt_ = book1D("GenMEtDeltaRAWplusJESMEt_Pt", "GenMEtDeltaRAWplusJESMEt_Pt", 150, -150.0, 150.0);
  hGenMEtDeltaRAWplusJESMEt_Phi_ = book1D("GenMEtDeltaRAWplusJESMEt_Phi", "GenMEtDeltaRAWplusJESMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());
  hGenMEtDeltaRAWplusJESMEt_Px_ = book1D("GenMEtDeltaRAWplusJESMEt_Px", "GenMEtDeltaRAWplusJESMEt_Px", 150, -150.0, 150.0);
  hGenMEtDeltaRAWplusJESMEt_Py_ = book1D("GenMEtDeltaRAWplusJESMEt_Py", "GenMEtDeltaRAWplusJESMEt_Py", 150, -150.0, 150.0);
  
  hGenMEtDeltaRAWMEt_Pt_ = book1D("GenMEtDeltaRAWMEt_Pt", "GenMEtDeltaRAWMEt_Pt", 150, -150.0, 150.0);
  hGenMEtDeltaRAWMEt_Phi_ = book1D("GenMEtDeltaRAWMEt_Phi", "GenMEtDeltaRAWMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());
  hGenMEtDeltaRAWMEt_Px_ = book1D("GenMEtDeltaRAWMEt_Px", "GenMEtDeltaRAWMEt_Px", 150, -150.0, 150.0);
  hGenMEtDeltaRAWMEt_Py_ = book1D("GenMEtDeltaRAWMEt_Py", "GenMEtDeltaRAWMEt_Py", 150, -150.0, 150.0);
}

void CaloMEtHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<CaloMEtHistManager::fillHistogramsImp>:" << std::endl; 

  MEtHistManager::fillHistogramsImp(evt, es, evtWeight);

  edm::Handle<std::vector<pat::MET> > patMETs;
  getCollection(evt, metSrc_, patMETs);

  double metSignificance = -1.;
  if ( metSignificanceSrc_.label() != "" ) {
     edm::Handle<edm::View<reco::CaloMET> > recoMETs;
     evt.getByLabel(metSignificanceSrc_, recoMETs);

     if ( recoMETs->size() == 1 ) metSignificance = recoMETs->begin()->metSignificance();
  }

  if ( patMETs->size() == 1 ) {
    const pat::MET& theEventMET = (*patMETs->begin());

    double RAWplusJESplusMUONplusTAU_MEtPt = theEventMET.pt();
    double RAWplusJESplusMUONplusTAU_MEtPhi = theEventMET.phi();
    double RAWplusJESplusMUONplusTAU_MEtPx = RAWplusJESplusMUONplusTAU_MEtPt * cos(RAWplusJESplusMUONplusTAU_MEtPhi);
    double RAWplusJESplusMUONplusTAU_MEtPy = RAWplusJESplusMUONplusTAU_MEtPt * sin(RAWplusJESplusMUONplusTAU_MEtPhi);
    math::XYZTLorentzVector RAWplusJESplusMUONplusTAU_MetVector(RAWplusJESplusMUONplusTAU_MEtPx , RAWplusJESplusMUONplusTAU_MEtPy , 0 , 0);
    hRAWplusJESplusMUONplusTAU_MEtPt_->Fill(RAWplusJESplusMUONplusTAU_MEtPt, evtWeight);
    hRAWplusJESplusMUONplusTAU_MEtPhi_->Fill(RAWplusJESplusMUONplusTAU_MEtPhi, evtWeight);
    hRAWplusJESplusMUONplusTAU_MEtPx_->Fill(RAWplusJESplusMUONplusTAU_MEtPx, evtWeight);
    hRAWplusJESplusMUONplusTAU_MEtPy_->Fill(RAWplusJESplusMUONplusTAU_MEtPy, evtWeight);

    double RAWplusJESplusMUON_MEtPt = theEventMET.uncorrectedPt(pat::MET::uncorrTAU);
    double RAWplusJESplusMUON_MEtPhi = theEventMET.uncorrectedPhi(pat::MET::uncorrTAU);
    double RAWplusJESplusMUON_MEtPx = RAWplusJESplusMUON_MEtPt * cos(RAWplusJESplusMUON_MEtPhi);
    double RAWplusJESplusMUON_MEtPy = RAWplusJESplusMUON_MEtPt * sin(RAWplusJESplusMUON_MEtPhi);
    math::XYZTLorentzVector RAWplusJESplusMUON_MetVector(RAWplusJESplusMUON_MEtPx , RAWplusJESplusMUON_MEtPy , 0 , 0);
    hRAWplusJESplusMUON_MEtPt_->Fill(RAWplusJESplusMUON_MEtPt, evtWeight);
    hRAWplusJESplusMUON_MEtPhi_->Fill(RAWplusJESplusMUON_MEtPhi, evtWeight);
    hRAWplusJESplusMUON_MEtPx_->Fill(RAWplusJESplusMUON_MEtPx, evtWeight);
    hRAWplusJESplusMUON_MEtPy_->Fill(RAWplusJESplusMUON_MEtPy, evtWeight);

    double RAWplusMUONplusTAU_MEtPt = theEventMET.uncorrectedPt(pat::MET::uncorrJES);
    double RAWplusMUONplusTAU_MEtPhi = theEventMET.uncorrectedPhi(pat::MET::uncorrJES);
    double RAWplusMUONplusTAU_MEtPx = RAWplusMUONplusTAU_MEtPt * cos(RAWplusMUONplusTAU_MEtPhi);
    double RAWplusMUONplusTAU_MEtPy = RAWplusMUONplusTAU_MEtPt * sin(RAWplusMUONplusTAU_MEtPhi);
    math::XYZTLorentzVector RAWplusMUONplusTAU_MetVector(RAWplusMUONplusTAU_MEtPx , RAWplusMUONplusTAU_MEtPy , 0 , 0);
    hRAWplusMUONplusTAU_MEtPt_->Fill(RAWplusMUONplusTAU_MEtPt, evtWeight);
    hRAWplusMUONplusTAU_MEtPhi_->Fill(RAWplusMUONplusTAU_MEtPhi, evtWeight);
    hRAWplusMUONplusTAU_MEtPx_->Fill(RAWplusMUONplusTAU_MEtPx, evtWeight);
    hRAWplusMUONplusTAU_MEtPy_->Fill(RAWplusMUONplusTAU_MEtPy, evtWeight);

    double RAW_MEtPt = theEventMET.uncorrectedPt(pat::MET::uncorrALL);
    double RAW_MEtPhi = theEventMET.uncorrectedPhi(pat::MET::uncorrALL);
    double RAW_MEtPx = RAW_MEtPt * cos(RAW_MEtPhi);
    double RAW_MEtPy = RAW_MEtPt * sin(RAW_MEtPhi);
    math::XYZTLorentzVector RAW_MetVector(RAW_MEtPx , RAW_MEtPy , 0 , 0);
    hRAW_MEtPt_->Fill(RAW_MEtPt, evtWeight);
    hRAW_MEtPhi_->Fill(RAW_MEtPhi, evtWeight);
    hRAW_MEtPx_->Fill(RAW_MEtPx, evtWeight);
    hRAW_MEtPy_->Fill(RAW_MEtPy, evtWeight);

    hRAW_MEtSignificance_->Fill(metSignificance, evtWeight);

    math::XYZTLorentzVector deltaTAU_MetVector = RAWplusJESplusMUONplusTAU_MetVector - RAWplusJESplusMUON_MetVector;
    math::XYZTLorentzVector deltaJES_MetVector = RAWplusJESplusMUONplusTAU_MetVector - RAWplusMUONplusTAU_MetVector;
    math::XYZTLorentzVector deltaMUON_MetVector = RAWplusJESplusMUONplusTAU_MetVector - deltaJES_MetVector - deltaTAU_MetVector - RAW_MetVector;

    math::XYZTLorentzVector RAWplusJES_MetVector = RAW_MetVector + deltaJES_MetVector;
    double RAWplusJES_MEtPt = RAWplusJES_MetVector.Pt();
    double RAWplusJES_MEtPhi = RAWplusJES_MetVector.Phi();
    double RAWplusJES_MEtPx = RAWplusJES_MetVector.Px();
    double RAWplusJES_MEtPy = RAWplusJES_MetVector.Py();
    hRAWplusJES_MEtPt_->Fill(RAWplusJES_MEtPt, evtWeight);
    hRAWplusJES_MEtPhi_->Fill(RAWplusJES_MEtPhi, evtWeight);
    hRAWplusJES_MEtPx_->Fill(RAWplusJES_MEtPx, evtWeight);
    hRAWplusJES_MEtPy_->Fill(RAWplusJES_MEtPy, evtWeight);

    hMUON_MExCorrection_->Fill(deltaMUON_MetVector.Px(), evtWeight);
    hMUON_MEyCorrection_->Fill(deltaMUON_MetVector.Py(), evtWeight);
    hTAU_MExCorrection_->Fill(deltaTAU_MetVector.Px(), evtWeight);
    hTAU_MEyCorrection_->Fill(deltaTAU_MetVector.Py(), evtWeight);
    hJES_MExCorrection_->Fill(deltaJES_MetVector.Px(), evtWeight);
    hJES_MEyCorrection_->Fill(deltaJES_MetVector.Py(), evtWeight);

    //MUONplusTAU_MEtPhi = (MUONplusTAU_MEtPhi>=0) ? MUONplusTAU_MEtPhi : (MUONplusTAU_MEtPhi + (2.0 * TMath::Pi()));

    if ( theEventMET.genMET() ) {

      hRAWplusJESplusMUONplusTAUMEtPtDiffGen_->Fill(RAWplusJESplusMUONplusTAU_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hRAWplusJESplusMUONplusTAUMEtPtResGen_->Fill((RAWplusJESplusMUONplusTAU_MEtPt - theEventMET.genMET()->pt())/TMath::Sqrt(theEventMET.genMET()->pt()), evtWeight);
      hRAWplusJESplusMUONplusTAUMEtPtRecVsGen_->Fill(theEventMET.genMET()->pt(), RAWplusJESplusMUONplusTAU_MEtPt, evtWeight);
      hRAWplusJESplusMUONplusTAUMEtPhiDiffGen_->Fill(RAWplusJESplusMUONplusTAU_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hRAWplusJESplusMUONplusTAUMEtPhiRecVsGen_->Fill(theEventMET.genMET()->phi(), RAWplusJESplusMUONplusTAU_MEtPhi, evtWeight);
      hRAWplusJESplusMUONMEtPtDiffGen_->Fill(RAWplusJESplusMUON_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hRAWplusJESplusMUONMEtPtResGen_->Fill((RAWplusJESplusMUON_MEtPt - theEventMET.genMET()->pt())/TMath::Sqrt(theEventMET.genMET()->pt()), evtWeight);
      hRAWplusJESplusMUONMEtPtRecVsGen_->Fill(theEventMET.genMET()->pt(), RAWplusJESplusMUON_MEtPt, evtWeight);
      hRAWplusJESplusMUONMEtPhiDiffGen_->Fill(RAWplusJESplusMUON_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hRAWplusJESplusMUONMEtPhiRecVsGen_->Fill(theEventMET.genMET()->phi(), RAWplusJESplusMUON_MEtPhi, evtWeight);
      hRAWplusJESMEtPtDiffGen_->Fill(RAWplusJES_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hRAWplusJESMEtPtResGen_->Fill((RAWplusJES_MEtPt - theEventMET.genMET()->pt())/TMath::Sqrt(theEventMET.genMET()->pt()), evtWeight);
      hRAWplusJESMEtPtRecVsGen_->Fill(theEventMET.genMET()->pt(), RAWplusJES_MEtPt, evtWeight);
      hRAWplusJESMEtPhiDiffGen_->Fill(RAWplusJES_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hRAWplusJESMEtPhiRecVsGen_->Fill(theEventMET.genMET()->phi(), RAWplusJES_MEtPhi, evtWeight);
      hRAWMEtPtDiffGen_->Fill(RAW_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hRAWMEtPtResGen_->Fill((RAW_MEtPt - theEventMET.genMET()->pt())/TMath::Sqrt(theEventMET.genMET()->pt()), evtWeight);
      hRAWMEtPtRecVsGen_->Fill(theEventMET.genMET()->pt(), RAW_MEtPt, evtWeight);
      hRAWMEtPhiDiffGen_->Fill(RAW_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hRAWMEtPhiRecVsGen_->Fill(theEventMET.genMET()->phi(), RAW_MEtPhi, evtWeight);

      hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Pt_->Fill(RAWplusJESplusMUONplusTAU_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Px_->Fill(RAWplusJESplusMUONplusTAU_MEtPx - theEventMET.genMET()->px(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Py_->Fill(RAWplusJESplusMUONplusTAU_MEtPy - theEventMET.genMET()->py(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONMEt_Pt_->Fill(RAWplusJESplusMUON_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONMEt_Px_->Fill(RAWplusJESplusMUON_MEtPx - theEventMET.genMET()->px(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONMEt_Py_->Fill(RAWplusJESplusMUON_MEtPy - theEventMET.genMET()->py(), evtWeight);
      hGenMEtDeltaRAWplusJESMEt_Pt_->Fill(RAWplusJES_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hGenMEtDeltaRAWplusJESMEt_Px_->Fill(RAWplusJES_MEtPx - theEventMET.genMET()->px(), evtWeight);
      hGenMEtDeltaRAWplusJESMEt_Py_->Fill(RAWplusJES_MEtPy - theEventMET.genMET()->py(), evtWeight);
      hGenMEtDeltaRAWMEt_Pt_->Fill(RAW_MEtPt - theEventMET.genMET()->pt(), evtWeight);
      hGenMEtDeltaRAWMEt_Px_->Fill(RAW_MEtPx - theEventMET.genMET()->px(), evtWeight);
      hGenMEtDeltaRAWMEt_Py_->Fill(RAW_MEtPy - theEventMET.genMET()->py(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONplusTAUMEt_Phi_->Fill(RAWplusJESplusMUONplusTAU_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hGenMEtDeltaRAWplusJESplusMUONMEt_Phi_->Fill(RAWplusJESplusMUON_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hGenMEtDeltaRAWplusJESMEt_Phi_->Fill(RAWplusJES_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
      hGenMEtDeltaRAWMEt_Phi_->Fill(RAW_MEtPhi - theEventMET.genMET()->phi(), evtWeight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, CaloMEtHistManager, "CaloMEtHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, CaloMEtHistManager, "CaloMEtHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<CaloMEtHistManager> CaloMEtAnalyzer;

DEFINE_FWK_MODULE(CaloMEtAnalyzer);

