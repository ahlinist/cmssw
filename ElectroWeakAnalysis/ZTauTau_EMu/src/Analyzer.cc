#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "CSA07anl/Analyzer/interface/Analyzer.h"
Analyzer::Analyzer(const edm::ParameterSet& iConfig)
{
 
 
JETMAX=15;
MUONMAX=15;
ElectronMax=15;
 BMAX=200;

fOutputFileName = iConfig.getUntrackedParameter<string>("HistOutFile");
theECALIsoDepositLabel = iConfig.getUntrackedParameter<InputTag>( "ECALIsoDepositLabel");
theHCALIsoDepositLabel = iConfig.getUntrackedParameter<InputTag>( "HCALIsoDepositLabel");
theHOCALIsoDepositLabel = iConfig.getUntrackedParameter<InputTag>( "HOCALIsoDepositLabel");
theTrackerIsoDepositLabel = iConfig.getUntrackedParameter<InputTag>( "TrackerIsoDepositLabel");
muon_ = iConfig.getUntrackedParameter<InputTag>("muonLabel");
elec_ = iConfig.getUntrackedParameter<InputTag>("electronLabel");
hbhe_ = iConfig.getUntrackedParameter<InputTag>("hbheLabel");
eSC_  = iConfig.getUntrackedParameter<InputTag>("EScLabel");
eBC_  = iConfig.getUntrackedParameter<InputTag>("EBcLabel");
ctfTrk_ = iConfig.getUntrackedParameter<InputTag>("trackLabel");
electronProducer_=iConfig.getUntrackedParameter<std::string>("electronProducer");
electronIDAssocProducer_ = iConfig.getUntrackedParameter<std::string>("electronIDAssocProducer");


hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
JetMet = new TTree("jetmet","jetmet");

JetMet->Branch("Gen_MET",&Gen_MET,"Gen_MET/D");
JetMet->Branch("Rec_MET",&Rec_MET,"Rec_MET/D");
JetMet->Branch("Cor_MuMET",&Cor_MuMET,"Cor_MuMET/D");
JetMet->Branch("Cor_MuMETx",&Cor_MuMETx,"Cor_MuMETx/D");
JetMet->Branch("Cor_MuMETy",&Cor_MuMETy,"Cor_MuMETy/D");
JetMet->Branch("Rec_Scal_Et",&Rec_Scal_Et,"Rec_Scal_Et/D");
JetMet->Branch("Cor_Scal_Et",&Cor_Scal_Et,"Cor_Scal_Et/D");
JetMet->Branch("Cor_MET",&Cor_MET,"Cor_MET/D");
JetMet->Branch("Gen_MET_phi",&Gen_MET_phi,"Gen_MET_phi/D");
JetMet->Branch("Rec_MET_phi",&Rec_MET_phi,"Rec_MET_phi/D");
JetMet->Branch("Cor_MET_phi",&Cor_MET_phi,"Cor_MET_phi/D");

JetMet->Branch("Rec_Jet_size",&Rec_Jet_size,"Rec_Jet_size/I");
JetMet->Branch("Rec_Jet_phi",Rec_Jet_phi,"Rec_Jet_phi[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_eta",Rec_Jet_eta,"Rec_Jet_eta[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_pt",Rec_Jet_pt,"Rec_Jet_pt[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_et",Rec_Jet_et,"Rec_Jet_et[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_px",Rec_Jet_px,"Rec_Jet_px[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_pz",Rec_Jet_pz,"Rec_Jet_pz[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_py",Rec_Jet_py,"Rec_Jet_py[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_p",Rec_Jet_p,"Rec_Jet_p[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_HDfrac",Rec_Jet_HDfrac,"Rec_Jet_HDfrac[Rec_Jet_size]/D");
JetMet->Branch("Rec_Jet_EMfrac",Rec_Jet_EMfrac,"Rec_Jet_EMfrac[Rec_Jet_size]/D");

JetMet->Branch("Cor_Jet_size",&Cor_Jet_size,"Cor_Jet_size/I");
JetMet->Branch("Cor_Jet_phi",Cor_Jet_phi,"Cor_Jet_phi[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_eta",Cor_Jet_eta,"Cor_Jet_eta[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_pt",Cor_Jet_pt,"Cor_Jet_pt[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_et",Cor_Jet_et,"Cor_Jet_et[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_px",Cor_Jet_px,"Cor_Jet_px[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_pz",Cor_Jet_pz,"Cor_Jet_pz[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_py",Cor_Jet_py,"Cor_Jet_py[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_p",Cor_Jet_p,"Cor_Jet_p[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_HDfrac",Cor_Jet_HDfrac,"Cor_Jet_HDfrac[Cor_Jet_size]/D");
JetMet->Branch("Cor_Jet_EMfrac",Cor_Jet_EMfrac,"Cor_Jet_EMfrac[Cor_Jet_size]/D");

JetMet->Branch("Gen_Jet_size",&Gen_Jet_size,"Gen_Jet_size/I");
JetMet->Branch("Gen_Jet_pt",Gen_Jet_pt,"Gen_Jet_pt[Gen_Jet_size]/D");
JetMet->Branch("Gen_Jet_px",Gen_Jet_px,"Gen_Jet_px[Gen_Jet_size]/D");
JetMet->Branch("Gen_Jet_pz",Gen_Jet_pz,"Gen_Jet_pz[Gen_Jet_size]/D");
JetMet->Branch("Gen_Jet_py",Gen_Jet_py,"Gen_Jet_py[Gen_Jet_size]/D");
JetMet->Branch("Gen_Jet_p",Gen_Jet_p,"Gen_Jet_p[Gen_Jet_size]/D");
JetMet->Branch("Gen_Jet_phi",Gen_Jet_phi,"Gen_Jet_phi[Gen_Jet_size]/D");
JetMet->Branch("Gen_Jet_eta",Gen_Jet_eta,"Gen_Jet_eta[Gen_Jet_size]/D");

JetMet->Branch("nMuonCand",&nMuonCand,"nMuonCand/I");
JetMet->Branch("Muon_pt",Muon_pt,"Muon_pt[nMuonCand]/D");
JetMet->Branch("Muon_eta",Muon_eta,"Muon_eta[nMuonCand]/D");
JetMet->Branch("Muon_p",Muon_p,"Muon_p[nMuonCand]/D");
JetMet->Branch("Muon_px",Muon_px,"Muon_px[nMuonCand]/D");
JetMet->Branch("Muon_py",Muon_py,"Muon_py[nMuonCand]/D");
JetMet->Branch("Muon_pz",Muon_pz,"Muon_pz[nMuonCand]/D");
JetMet->Branch("Muon_phi",Muon_phi,"Muon_phi[nMuonCand]/D");
JetMet->Branch("Muon_theta",Muon_theta,"Muon_theta[nMuonCand]/D");
JetMet->Branch("Muon_charge",Muon_charge,"Muon_charge[nMuonCand]/D");
JetMet->Branch("MuonDz",&MuonDz,"MuonDz[nMuonCand]/D");
JetMet->Branch("MuonIP",&MuonIP,"MuonIP[nMuonCand]/D");
JetMet->Branch("Muonvty",&Muonvty,"Muonvty[nMuonCand]/D");
JetMet->Branch("Muonvtx",&Muonvtx,"Muonvtx[nMuonCand]/D");
JetMet->Branch("Muonvtz",&Muonvtz,"Muonvtz[nMuonCand]/D");



JetMet->Branch("nElectronCand",&nElectronCand,"nElectronCand/I");
JetMet->Branch("Electron_pt",Electron_pt,"Electron_pt[nElectronCand]/D");
JetMet->Branch("Electron_ID",Electron_ID,"Electron_ID[nElectronCand]/I");
JetMet->Branch("Electron_eta",Electron_eta,"Electron_eta[nElectronCand]/D");
JetMet->Branch("Electron_px",Electron_px,"Electron_px[nElectronCand]/D");
JetMet->Branch("Electron_py",Electron_py,"Electron_py[nElectronCand]/D");
JetMet->Branch("Electron_pz",Electron_pz,"Electron_pz[nElectronCand]/D");
JetMet->Branch("Electron_p",Electron_p,"Electron_p[nElectronCand]/D");
JetMet->Branch("Electron_phi",Electron_phi,"Electron_phi[nElectronCand]/D");
JetMet->Branch("Electron_theta",Electron_theta,"Electron_theta[nElectronCand]/D");
JetMet->Branch("Electron_charge",Electron_charge,"Electron_charge[nElectronCand]/D");
JetMet->Branch("Electron_HDovrEM",Electron_HDovrEM,"Electron_HDovrEM[nElectronCand]/D");

JetMet->Branch("ElectronDz",&ElectronDz,"ElectronDz[nElectronCand]/D");
JetMet->Branch("ElectronIP",&ElectronIP,"ElectronIP[nElectronCand]/D");
JetMet->Branch("Electronvty",&Electronvty,"Electronvty[nElectronCand]/D");
JetMet->Branch("Electronvtx",&Electronvtx,"Electronvtx[nElectronCand]/D");
JetMet->Branch("Electronvtz",&Electronvtz,"Electronvtz[nElectronCand]/D");


JetMet->Branch("nEvt",&nEvt,"nEvt/I");
JetMet->Branch("etadifference",&etadifference,"etadifference/D");
JetMet->Branch("dijetmass",&dijetmass,"dijetmass/D");
JetMet->Branch("deltaphi",&deltaphi,"deltaphi/D");
JetMet->Branch("coretadifference",&coretadifference,"coretadifference/D");
JetMet->Branch("cordijetmass",&cordijetmass,"cordijetmass/D");
JetMet->Branch("cordeltaphi",&cordeltaphi,"cordeltaphi/D");
JetMet->Branch("NV",&NV,"NV/D");
JetMet->Branch("corNV",&corNV,"corNV/D");


JetMet->Branch("EcalM_Tot",&EcalM_Tot,"EcalM_Tot[nMuonCand]/D");
JetMet->Branch("HcalM_Tot",&HcalM_Tot,"HcalM_Tot[nMuonCand]/D");
JetMet->Branch("HoM_Tot",&HoM_Tot,"HoM_Tot[nMuonCand]/D");  
JetMet->Branch("TrkM_Tot",&TrkM_Tot,"TrkM_Tot[nMuonCand]/D");
JetMet->Branch("EcalM_Frac",&EcalM_Frac,"EcalM_Frac[nMuonCand][5]/D");
JetMet->Branch("HcalM_Frac",&HcalM_Frac,"HcalM_Frac[nMuonCand][5]/D");
JetMet->Branch("HoM_Frac",&HoM_Frac,"HoM_Frac[nMuonCand][5]/D");
JetMet->Branch("TrkM_Frac",&TrkM_Frac,"TrkM_Frac[nMuonCand][5]/D");
JetMet->Branch("TrkIsoM_N",&TrkIsoM_N,"TrkIsoM_N[nMuonCand][5]/I");
JetMet->Branch("TrkIsoE_N",&TrkIsoE_N,"TrkIsoE_N[nElectronCand][5][5]/I");
JetMet->Branch("TrkIsoE_Pt",&TrkIsoE_Pt,"TrkIsoE_Pt[nElectronCand][5][5]/D");
JetMet->Branch("EcalIsoE_Pt",&EcalIsoE_Pt,"EcalIsoE_Pt[nElectronCand][5]/D");
JetMet->Branch("HcalIsoE_Pt",&HcalIsoE_Pt,"HcalIsoE_Pt[nElectronCand][5][5]/D");
JetMet->Branch("bjets",&bjets,"bjets/I");
JetMet->Branch("btagdisc",&btagdisc,"btagdisc[bjets]/D");
}


Analyzer::~Analyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

/// ------------ method called to for each event  ------------
void
Analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 
  nEvt=0;
  ++nEvt;
  
  double extRadius[5]={0.15,0.20,0.25,0.30,0.35};
  double intRadius[5]={0.0,0.02,0.03,0.04,0.05};

//*********************************************Muon Isolation***********************************************

edm::Handle<reco::MuIsoDepositAssociationMap> ecalIso;
iEvent.getByLabel(theECALIsoDepositLabel, ecalIso);
edm::Handle<reco::MuIsoDepositAssociationMap> hcalIso;
iEvent.getByLabel(theHCALIsoDepositLabel, hcalIso);
edm::Handle<reco::MuIsoDepositAssociationMap> hocalIso;
iEvent.getByLabel(theHOCALIsoDepositLabel, hocalIso);
edm::Handle<reco::MuIsoDepositAssociationMap> trackerIso;
iEvent.getByLabel(theTrackerIsoDepositLabel, trackerIso);
 delpx = 0;
 delpy = 0;
 delsumet = 0;    

//nMuonCand = 0;
Handle<MuonCollection> muons;
iEvent.getByLabel(muon_, muons);
for(size_t ocs=0;ocs<5;ocs++){
nMuonCand=0;
for (reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end()&& nMuonCand < MUONMAX; ++muon) {
  delpx += muon->px();
  delpy += muon->py();
  delsumet += muon->et();
   const reco::MuIsoDeposit& depEcal     = (*ecalIso)[muon->combinedMuon()];
   const reco::MuIsoDeposit& depHcal     = (*hcalIso)[muon->combinedMuon()];
   const reco::MuIsoDeposit& depHOcal   = (*hocalIso)[muon->combinedMuon()];
   const reco::MuIsoDeposit& depTracker = (*trackerIso)[muon->combinedMuon()];
   
Muon_charge[nMuonCand]=muon->charge();
Muon_pt[nMuonCand]=muon->pt();
Muon_eta[nMuonCand]=muon->eta();
Muon_p[nMuonCand]=muon->energy();
Muon_px[nMuonCand]=muon->px();
Muon_py[nMuonCand]=muon->py();
Muon_pz[nMuonCand]=muon->pz();
Muon_phi[nMuonCand]=muon->phi();
Muon_theta[nMuonCand]=muon->theta();
TrackRef trk = muon->combinedMuon();
MuonIP[nMuonCand] = trk->d0();
MuonDz[nMuonCand] = trk->dz();
Muonvtx[nMuonCand] = trk->vertex().x();
Muonvty[nMuonCand] = trk->vertex().y();
Muonvtz[nMuonCand] = trk->vertex().z();


//*******************************************Muon Isolation Parameters******************************
EcalM_Tot[nMuonCand] = depEcal.muonEnergy();
HcalM_Tot[nMuonCand] = depHcal.muonEnergy();
HoM_Tot[nMuonCand] = depHOcal.muonEnergy();
TrkM_Tot[nMuonCand] = depTracker.muonEnergy(); 
EcalM_Frac[nMuonCand][ocs] = depEcal.depositWithin(extRadius[ocs]);
HcalM_Frac[nMuonCand][ocs] = depHcal.depositWithin(extRadius[ocs]);
HoM_Frac[nMuonCand][ocs]   = depHOcal.depositWithin(extRadius[ocs]);
std::pair<double, int> sumPtAndNTracks = depTracker.depositAndCountWithin(extRadius[ocs]);
TrkM_Frac[nMuonCand][ocs] = sumPtAndNTracks.first;
TrkIsoM_N[nMuonCand][ocs] = sumPtAndNTracks.second;
nMuonCand++;
}
}

 


Handle<TrackCollection> tracks;
iEvent.getByLabel(ctfTrk_, tracks);
const reco::TrackCollection* trackCollection = tracks.product();

 Handle<PixelMatchGsfElectronCollection> electrons;
 iEvent.getByLabel(elec_, electrons);

// Get the  filtered objects
 edm::Handle< edm::View<reco::Candidate> > emcand;
 iEvent.getByLabel(elec_,emcand);
  edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
  iEvent.getByLabel(electronIDAssocProducer_, electronIDAssocHandle);
 
  nElectronCand=0;
  PixelMatchGsfElectronCollection::const_iterator electron;
  vector<PixelMatchGsfElectron>elec;
  vector<GsfTrack>gsf;
  elec.clear();
  double eid = -10;
   for (unsigned int i = 0; i < electrons->size(); i++){
   
    edm::Ref<reco::PixelMatchGsfElectronCollection> electronRef(electrons,i);
    // Find entry in electron ID map corresponding electron
    reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;
    electronIDAssocItr = electronIDAssocHandle->find(electronRef);
    const reco::ElectronIDRef& id = electronIDAssocItr->val;
    bool cutBasedID = id->cutBasedDecision(); 
    eid = cutBasedID;
    Electron_ID[nElectronCand] = eid;
      //for (electron = electrons->begin(); electron != electrons->end() && nElectronCand < ElectronMax; ++electron)

    elec.push_back((*electrons)[i]);
    Electron_charge[nElectronCand]=(*electrons)[i].charge();
    Electron_pt[nElectronCand]=(*electrons)[i].pt();
    Electron_eta[nElectronCand]=(*electrons)[i].eta();
    Electron_phi[nElectronCand]=(*electrons)[i].phi();
    Electron_p[nElectronCand]=(*electrons)[i].energy();
    Electron_px[nElectronCand]=(*electrons)[i].px();
    Electron_py[nElectronCand]=(*electrons)[i].py();
    Electron_pz[nElectronCand]=(*electrons)[i].pz();
    Electron_theta[nElectronCand]=(*electrons)[i].theta();
    Electron_HDovrEM[nElectronCand]=(*electrons)[i].hadronicOverEm();
    GsfTrackRef trk = (*electrons)[i].gsfTrack();
    Electronvtx[nElectronCand]= trk->vertex().x();
    Electronvty[nElectronCand]= trk->vertex().y();
    Electronvtz[nElectronCand]= trk->vertex().z();
    ElectronIP[nElectronCand]=trk->d0();
    ElectronDz[nElectronCand]=trk->dz();
    gsf.push_back(*trk);
   
    nElectronCand++;

}
//**********************************Electron Isolation******************************
edm::ESHandle<CaloGeometry> pG;
iSetup.get<IdealGeometryRecord>().get(pG);
const CaloGeometry* caloGeom = pG.product();


edm::Handle<reco::BasicClusterCollection> bcHandle;
iEvent.getByLabel(eBC_, bcHandle);
const reco::BasicClusterCollection* bcCollection = bcHandle.product();

edm::Handle<reco::SuperClusterCollection> scHandle;
iEvent.getByLabel(eSC_,scHandle);
const reco::SuperClusterCollection* scCollection = (scHandle.product());

edm::Handle<HBHERecHitCollection> hhitBarrelHandle;
iEvent.getByLabel(hbhe_, hhitBarrelHandle);
 HBHERecHitMetaCollection mhbhe =  HBHERecHitMetaCollection(*hhitBarrelHandle);




// Get the forward hcal hits
//edm::Handle<HFRecHitCollection> hhitEndcapHandle;
//iEvent.getByLabel("hfreco", hhitEndcapHandle);
//const HFRecHitCollection* hcalhitEndcapCollection = hhitEndcapHandle.product();

double trkptMin = 0.5;
double maxVTXdist = 0.1;
double ecalptMin = 0.5;
double hcalptMin = 0.5;
for(int ics=0;ics<5;ics++){
	for(int ocs=0;ocs<5;ocs++){

ElectronTkIsolation*  TrackN = new ElectronTkIsolation(extRadius[ocs], intRadius[ics], trkptMin, maxVTXdist,trackCollection);
EgammaEcalIsolation*   Eetsum = new EgammaEcalIsolation(extRadius[ocs],ecalptMin,bcCollection,scCollection);
EgammaHcalIsolation*   Hetsum = new EgammaHcalIsolation(extRadius[ocs], intRadius[ics],hcalptMin,caloGeom,&mhbhe);

for(int i =0;i<elec.size();i++)
			{


const reco::RecoCandidate* eleptr = (reco::RecoCandidate *) &(elec[i]);
TrkIsoE_Pt[i][ocs][ics]	 =  TrackN->getPtTracks(&(elec[i]));
TrkIsoE_N[i][ocs][ics] 	 =  TrackN->getNumberTracks(&(elec[i]));
EcalIsoE_Pt[i][ocs]      = Eetsum->getEcalEtSum(&(elec[i]));
HcalIsoE_Pt[i][ocs][ics] = Hetsum-> getHcalEtSum(&(elec[i]));

			}//ele lop
                                                             
		}//ocs loop
 
	}//ics loop


//**********************************************************************************



Rec_Jet_size=0;
Cor_Jet_size=0;
Gen_Jet_size=0;
Handle<CaloMETCollection>corcalomets;
iEvent.getByLabel("corMetType1Icone5",corcalomets);
const CaloMETCollection *corcalometcoll=corcalomets.product();
const CaloMET corcaloMET = corcalometcoll->front();
Cor_MET  = corcaloMET.pt();
Cor_MET_phi = corcaloMET.phi();
Cor_Scal_Et = corcaloMET.sumEt();

Cor_MuMETx = corcaloMET.px() - delpx;
Cor_MuMETy = corcaloMET.py() - delpy;
Cor_MuMET = sqrt(Cor_MuMETx*Cor_MuMETx+Cor_MuMETy*Cor_MuMETy);

Handle<CaloMETCollection>calomets;
iEvent.getByLabel("met",calomets);
const CaloMETCollection *calometcoll=calomets.product();
const CaloMET caloMET = calometcoll->front();
 Rec_Scal_Et = caloMET.sumEt();
 Rec_MET  = caloMET.pt();
 Rec_MET_phi = caloMET.phi();

Handle<GenMETCollection>genmet;
iEvent.getByLabel("genMet",genmet);
const GenMETCollection *genmetcoll=genmet.product();
const GenMET gMET = genmetcoll->front();
Gen_MET = gMET.pt();
Gen_MET_phi = gMET.phi();

Handle<GenJetCollection>genjet;
iEvent.getByLabel("iterativeCone5GenJetsNoNuBSM",genjet);
for( size_t j = 0; j < genjet->size(); j++)
{
if((*genjet)[j].pt() > 20.)
{
     Gen_Jet_pt[Gen_Jet_size]=(*genjet)[j].pt();
     Gen_Jet_eta[Gen_Jet_size]=(*genjet)[j].eta();
     Gen_Jet_phi[Gen_Jet_size]=(*genjet)[j].phi();
     Gen_Jet_px[Gen_Jet_size]=(*genjet)[j].px();
     Gen_Jet_py[Gen_Jet_size]=(*genjet)[j].py();
     Gen_Jet_pz[Gen_Jet_size]=(*genjet)[j].pz();
     Gen_Jet_p[Gen_Jet_size]=(*genjet)[j].p();
     Gen_Jet_size=Gen_Jet_size+1;
}
}

Handle<CaloJetCollection>calojet;
iEvent.getByLabel("iterativeCone5CaloJets",calojet);
     for( int j = 0; j < calojet->size(); j++)
       {
       if((*calojet)[j].pt() > 20.)
        {
    Rec_Jet_pt[Rec_Jet_size]=(*calojet)[j].pt();
    Rec_Jet_et[Rec_Jet_size]=(*calojet)[j].et();
    Rec_Jet_eta[Rec_Jet_size]=(*calojet)[j].eta();
    Rec_Jet_phi[Rec_Jet_size]=(*calojet)[j].phi();
    Rec_Jet_px[Rec_Jet_size]=(*calojet)[j].px();
    Rec_Jet_py[Rec_Jet_size]=(*calojet)[j].py();
    Rec_Jet_pz[Rec_Jet_size]=(*calojet)[j].pz();
    Rec_Jet_p[Rec_Jet_size]=(*calojet)[j].p();
    Rec_Jet_HDfrac[Rec_Jet_size]=(*calojet)[j].energyFractionHadronic();
    Rec_Jet_EMfrac[Rec_Jet_size]=(*calojet)[j].emEnergyFraction();
    Rec_Jet_size=Rec_Jet_size+1;
}
}
etadifference = 0;
NV = 0;
dijetmass = 0;
deltaphi = 0;
if(Rec_Jet_size>=2&&Rec_Jet_eta[0]*Rec_Jet_eta[1]<0.){
etadifference=fabs(Rec_Jet_eta[0]-Rec_Jet_eta[1]);
double mistwo = Rec_MET*Rec_MET;
double difftwo = (Rec_Jet_et[0]-Rec_Jet_et[1])*(Rec_Jet_et[0]-Rec_Jet_et[1]);
double twojettwo = Rec_Jet_et[1]*Rec_Jet_et[1];
NV = (mistwo-difftwo)/twojettwo;

double px = (Rec_Jet_px[0]+Rec_Jet_px[1])*(Rec_Jet_px[0]+Rec_Jet_px[1]);
double py = (Rec_Jet_py[0]+Rec_Jet_py[1])*(Rec_Jet_py[0]+Rec_Jet_py[1]);
double pz = (Rec_Jet_pz[0]+Rec_Jet_pz[1])*(Rec_Jet_pz[0]+Rec_Jet_pz[1]);
double p = (Rec_Jet_p[0]+Rec_Jet_p[1])*(Rec_Jet_p[0]+Rec_Jet_p[1]);
dijetmass = sqrt(p-px-py-pz);

recphi1 = Rec_Jet_phi[0];
recphi2 = Rec_Jet_phi[1];



  if(recphi1<0) recphi1+=TWOPI;
  if(recphi2<0) recphi2+=TWOPI;
  deltaphi=fabs(recphi1-recphi2);
  if(deltaphi>TWOPI) deltaphi-=TWOPI;
  if(deltaphi>PI) deltaphi=TWOPI-deltaphi;
}

//***********************************
Handle<CaloJetCollection>corcalojet;
iEvent.getByLabel("CorJetIcone5",corcalojet);
reco::CaloJetCollection GammaJetCorJetIcone5coll = *corcalojet;
std::sort(GammaJetCorJetIcone5coll.begin(), GammaJetCorJetIcone5coll.end(), PtSorter());

   for( int j = 0; j < GammaJetCorJetIcone5coll.size(); j++)
    {
     if(GammaJetCorJetIcone5coll[j].pt() > 20.)
     {
     
	 cout<<"Jet number====="<<j<<"pt====="<<GammaJetCorJetIcone5coll[j].pt()<<endl;

     
     Cor_Jet_pt[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].pt();
     Cor_Jet_et[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].et();
     Cor_Jet_eta[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].eta();
     Cor_Jet_phi[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].phi();
     Cor_Jet_px[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].px();
     Cor_Jet_py[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].py();
     Cor_Jet_pz[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].pz();
     Cor_Jet_p[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].p();
     Cor_Jet_HDfrac[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].energyFractionHadronic();
     Cor_Jet_EMfrac[Cor_Jet_size]=GammaJetCorJetIcone5coll[j].emEnergyFraction();
     Cor_Jet_size=Cor_Jet_size+1;
}
}

coretadifference = 0;
corNV = 0;
cordijetmass = 0;
cordeltaphi = 0;
if(Cor_Jet_size>=2&&Cor_Jet_eta[0]*Cor_Jet_eta[1]<0.){
coretadifference=fabs(Cor_Jet_eta[0]-Cor_Jet_eta[1]);

double cormistwo = Cor_MET*Cor_MET;
double cordifftwo = (Cor_Jet_et[0]-Cor_Jet_et[1])*(Cor_Jet_et[0]-Cor_Jet_et[1]);
double cortwojettwo = Cor_Jet_et[1]*Cor_Jet_et[1];
corNV = (cormistwo-cordifftwo)/cortwojettwo;

double corpx = (Cor_Jet_px[0]+Cor_Jet_px[1])*(Cor_Jet_px[0]+Cor_Jet_px[1]);
double corpy = (Cor_Jet_py[0]+Cor_Jet_py[1])*(Cor_Jet_py[0]+Cor_Jet_py[1]);
double corpz = (Cor_Jet_pz[0]+Cor_Jet_pz[1])*(Cor_Jet_pz[0]+Cor_Jet_pz[1]);
double corp = (Cor_Jet_p[0]+Cor_Jet_p[1])*(Cor_Jet_p[0]+Cor_Jet_p[1]);
cordijetmass = sqrt(corp-corpx-corpy-corpz);

correcphi1 = Cor_Jet_phi[0];
correcphi2 = Cor_Jet_phi[1];
  if(correcphi1<0) correcphi1+=TWOPI;
  if(correcphi2<0) correcphi2+=TWOPI;
cordeltaphi=fabs(correcphi1-correcphi2);
  if(cordeltaphi>TWOPI) cordeltaphi-=TWOPI;
  if(cordeltaphi>PI) cordeltaphi=TWOPI-cordeltaphi;
}
//********************B-tagging**********************************

Handle<JetTagCollection> jetsHandle;
iEvent.getByLabel("jetProbabilityJetTags", jetsHandle);
const JetTagCollection & Bjets = *(jetsHandle.product());

bjets = Bjets.size();
for (JetTagCollection::size_type i = 0; i < bjets; ++i) {
btagdisc[i] = Bjets[i].discriminator();
   }

JetMet->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void   
Analyzer::beginJob(const edm::EventSetup&)
{ 
 return;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Analyzer::endJob() {
  hOutputFile->Write() ;
  hOutputFile->Close() ;

  return;

}

//define this as a plug-in

