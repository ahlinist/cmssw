/*
*\Author:  O.Iorio
*
*
*
*\version  $Id: SingleTopSystematicsTreesDumper.cc,v 1.4 2011/05/12 16:35:59 oiorio Exp $ 
*/
// This analyzer dumps the histograms for all systematics listed in the cfg file 
//
//
//

#define DEBUG    0 // 0=false
#define MC_DEBUG 0 // 0=false   else -> dont process preselection
#define C_DEBUG  0 // currently debuging

#include "TopQuarkAnalysis/SingleTop/interface/SingleTopSystematicsTreesDumper.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>
//#include "CommonTools/CandUtils/interface/Booster.h"
#include <sstream> //libreria per usare stringstream

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "TopQuarkAnalysis/SingleTop/interface/EquationSolver.h"

SingleTopSystematicsTreesDumper::SingleTopSystematicsTreesDumper(const edm::ParameterSet& iConfig)
{
  //MCLightQuarkProducer   = iConfig.getParameter<InputTag>("MCLightQuarkProducer");
  systematics = iConfig.getUntrackedParameter<std::vector<std::string> >("systematics"); 
  rate_systematics = iConfig.getUntrackedParameter<std::vector<std::string> >("rateSystematics"); 
  //Channel information
  
  channelInfo = iConfig.getParameter<edm::ParameterSet>("channelInfo"); 
  //Cross section, name and number of events 
  channel = channelInfo.getUntrackedParameter<string>("channel");
  crossSection = channelInfo.getUntrackedParameter<double>("crossSection");
  originalEvents = channelInfo.getUntrackedParameter<double>("originalEvents");
  finalLumi = channelInfo.getUntrackedParameter<double>("finalLumi");
  MTWCut = channelInfo.getUntrackedParameter<double>("MTWCut",50);
  RelIsoCut = channelInfo.getUntrackedParameter<double>("RelIsoCut",0.1);
  loosePtCut = channelInfo.getUntrackedParameter<double>("loosePtCut",30); 


  leptonsPt_ =  iConfig.getParameter< edm::InputTag >("leptonsPt");
  leptonsPhi_ =  iConfig.getParameter< edm::InputTag >("leptonsPhi");
  leptonsEta_ =  iConfig.getParameter< edm::InputTag >("leptonsEta");
  leptonsEnergy_ =  iConfig.getParameter< edm::InputTag >("leptonsEnergy");
  leptonsCharge_ =  iConfig.getParameter< edm::InputTag >("leptonsCharge");
  leptonsRelIso_ =  iConfig.getParameter< edm::InputTag >("leptonsRelIso");

  leptonsID_ =  iConfig.getParameter< edm::InputTag >("leptonsID");

  leptonsFlavour_ =  iConfig.getUntrackedParameter< std::string >("leptonsFlavour");

  looseMuonsRelIso_ =  iConfig.getParameter< edm::InputTag >("looseMuonsRelIso");
  looseElectronsRelIso_ =  iConfig.getParameter< edm::InputTag >("looseElectronsRelIso");

  jetsEta_ =  iConfig.getParameter< edm::InputTag >("jetsEta");
  jetsPt_ =  iConfig.getParameter< edm::InputTag >("jetsPt");
  jetsPhi_ =  iConfig.getParameter< edm::InputTag >("jetsPhi");
  jetsEnergy_ =  iConfig.getParameter< edm::InputTag >("jetsEnergy");
  
  jetsBTagAlgo_ =  iConfig.getParameter< edm::InputTag >("jetsBTagAlgo");
  jetsAntiBTagAlgo_ =  iConfig.getParameter< edm::InputTag >("jetsAntiBTagAlgo");
  jetsFlavour_ =  iConfig.getParameter< edm::InputTag >("jetsFlavour");

  METPhi_ =  iConfig.getParameter< edm::InputTag >("METPhi");
  METPt_ =  iConfig.getParameter< edm::InputTag >("METPt");
  
  //  UnclMETPx_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPx");
  //  UnclMETPy_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPy");
  UnclMETPx_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPx");
  UnclMETPy_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPy");
  
  jetsCorrTotal_ =  iConfig.getParameter< edm::InputTag >("jetsCorrTotal");

  doBScan_  =  iConfig.getUntrackedParameter< bool >("doBScan",false); 
  doQCD_  =  iConfig.getUntrackedParameter< bool >("doQCD",true); 
  //  jetsPF_ =  iConfig.getParameter< edm::InputTag >("patJets");
  
  systematics.push_back("noSyst");
  
  Service<TFileService> fs;

  bScanSteps =1*doBScan_;
  bTagThreshold =3.41;

  TFileDirectory SingleTopSystematics = fs->mkdir( "systematics_histograms" );
  TFileDirectory SingleTopTrees = fs->mkdir( "systematics_trees" );
  
  std::vector<std::string> all_syst = systematics;
   
  for(size_t i = 0; i < rate_systematics.size();++i){
    all_syst.push_back(rate_systematics.at(i));  
  }
  
  for(size_t i = 0; i < all_syst.size();++i){
    
    string syst = all_syst[i];
    
    string treename = (channel+"_"+syst);
    string treenameQCD = (channel+"_"+syst+"QCD");
    
    trees[syst] = new TTree(treename.c_str(),treename.c_str()); 
    
    trees[syst]->Branch("eta",&etaTree);
    trees[syst]->Branch("costhetalj",&cosTree);
    trees[syst]->Branch("topMass",&topMassTree);
    trees[syst]->Branch("mtwMass",&mtwMassTree);
    
    trees[syst]->Branch("charge",&chargeTree);
    trees[syst]->Branch("runid",&runTree);
    trees[syst]->Branch("lumiid",&lumiTree);
    trees[syst]->Branch("eventid",&eventTree);
    trees[syst]->Branch("weight",&weightTree);

    //Extra info
    
    trees[syst]->Branch("leptonPt",&lepPt);
    trees[syst]->Branch("leptonPz",&lepPz);
    trees[syst]->Branch("leptonPhi",&lepPhi);
    trees[syst]->Branch("leptonRelIso",&lepRelIso);
    
    trees[syst]->Branch("fJetPt",&fJetPt);
    trees[syst]->Branch("fJetE",&fJetE);
    trees[syst]->Branch("fJetPz",&fJetPz);
    trees[syst]->Branch("fJetPhi",&fJetPhi);
    
    trees[syst]->Branch("bJetPt",&bJetPt);
    trees[syst]->Branch("bJetE",&bJetE);
    trees[syst]->Branch("bJetPz",&bJetPz);
    trees[syst]->Branch("bJetPhi",&bJetPhi);
    
    trees[syst]->Branch("metPt",&metPt);
    trees[syst]->Branch("metPhi",&metPhi);
    
    trees[syst]->Branch("topPt",&topPt);
    trees[syst]->Branch("topPhi",&topPhi);
    trees[syst]->Branch("topPz",&topPz);
    trees[syst]->Branch("topE",&topE);

    trees[syst]->Branch("totalEnergy",&totalEnergy);
    trees[syst]->Branch("totalMomentum",&totalMomentum);


    treesWSample[syst] = new TTree((treename+"WSample").c_str(),(treename+"WSample").c_str()); 
    
    treesWSample[syst]->Branch("etaLowBTag",&etaTree);
    treesWSample[syst]->Branch("etaHighBTag",&etaTree2);
    treesWSample[syst]->Branch("costhetalj",&cosTree);
    treesWSample[syst]->Branch("topMass",&topMassTree);
    treesWSample[syst]->Branch("mtwMass",&mtwMassTree);
    
    treesWSample[syst]->Branch("charge",&chargeTree);
    treesWSample[syst]->Branch("runid",&runTree);
    treesWSample[syst]->Branch("lumiid",&lumiTree);
    treesWSample[syst]->Branch("eventid",&eventTree);
    treesWSample[syst]->Branch("weight",&weightTree);
    
    //Extra info

    treesWSample[syst]->Branch("leptonPt",&lepPt);
    treesWSample[syst]->Branch("leptonPz",&lepPz);
    treesWSample[syst]->Branch("leptonPhi",&lepPhi);
    treesWSample[syst]->Branch("leptonRelIso",&lepRelIso);
    
    treesWSample[syst]->Branch("fJetPt",&fJetPt);
    treesWSample[syst]->Branch("fJetE",&fJetE);
    treesWSample[syst]->Branch("fJetPz",&fJetPz);
    treesWSample[syst]->Branch("fJetPhi",&fJetPhi);
    
    treesWSample[syst]->Branch("bJetPt",&bJetPt);
    treesWSample[syst]->Branch("bJetE",&bJetE);
    treesWSample[syst]->Branch("bJetPz",&bJetPz);
    treesWSample[syst]->Branch("bJetPhi",&bJetPhi);
    
    treesWSample[syst]->Branch("metPt",&metPt);
    treesWSample[syst]->Branch("metPhi",&metPhi);
    
    treesWSample[syst]->Branch("topPt",&topPt);
    treesWSample[syst]->Branch("topPhi",&topPhi);
    treesWSample[syst]->Branch("topPz",&topPz);
    treesWSample[syst]->Branch("topE",&topE);
    
    treesWSample[syst]->Branch("totalEnergy",&totalEnergy);
    treesWSample[syst]->Branch("totalMomentum",&totalMomentum);

    //QCD

    treesQCD[syst] = new TTree(treenameQCD.c_str(),treenameQCD.c_str()); 
    
    treesQCD[syst]->Branch("eta",&etaTree);
    treesQCD[syst]->Branch("costhetalj",&cosTree);
    treesQCD[syst]->Branch("topMass",&topMassTree);
    treesQCD[syst]->Branch("mtwMass",&mtwMassTree);
    treesQCD[syst]->Branch("leptonRelIso",&lepRelIso);
    


    treesQCD[syst]->Branch("charge",&chargeTree);
    treesQCD[syst]->Branch("runid",&runTree);
    treesQCD[syst]->Branch("lumiid",&lumiTree);
    treesQCD[syst]->Branch("eventid",&eventTree);
    treesQCD[syst]->Branch("weight",&weightTree);

    //W Sample QCD

    treesWSampleQCD[syst] = new TTree((treename+"WSampleQCD").c_str(),(treename+"WSampleQCD").c_str()); 
    
    treesWSampleQCD[syst]->Branch("etaLowBTag",&etaTree);
    treesWSampleQCD[syst]->Branch("etaHighBTag",&etaTree2);
    treesWSampleQCD[syst]->Branch("costhetalj",&cosTree);
    treesWSampleQCD[syst]->Branch("topMass",&topMassTree);
    treesWSampleQCD[syst]->Branch("mtwMass",&mtwMassTree);
    treesWSampleQCD[syst]->Branch("leptonRelIso",&lepRelIso);
    

    treesWSampleQCD[syst]->Branch("charge",&chargeTree);
    treesWSampleQCD[syst]->Branch("runid",&runTree);
    treesWSampleQCD[syst]->Branch("lumiid",&lumiTree);
    treesWSampleQCD[syst]->Branch("eventid",&eventTree);
    treesWSampleQCD[syst]->Branch("weight",&weightTree);

    //W Sample    

  }
  
  for(size_t i = 0; i < systematics.size();++i){
    
    string syst = systematics[i];
    
    string treename = (channel+"_"+syst);
    
    for(size_t step = 0; step < bScanSteps;++step){
      
      stringstream ssstep;
      ssstep << step;
      string sstep = ssstep.str();
      treesScan[step][syst] = new TTree((treename+"_step_"+sstep).c_str(),(treename+"_step_"+sstep).c_str()); 

      treesScan[step][syst]->Branch("etaLowBTag",&etaTree);
      treesScan[step][syst]->Branch("etaHighBTag",&etaTree2);
      treesScan[step][syst]->Branch("costhetalj",&cosTree);
      treesScan[step][syst]->Branch("topMass",&topMassTree);
      treesScan[step][syst]->Branch("mtwMass",&mtwMassTree);
      
      treesScan[step][syst]->Branch("highBTag",&highBTagTree);
      treesScan[step][syst]->Branch("lowBTag",&lowBTagTree);
      
      treesScan[step][syst]->Branch("charge",&chargeTree);
      treesScan[step][syst]->Branch("runid",&runTree);
      treesScan[step][syst]->Branch("lumiid",&lumiTree);
      treesScan[step][syst]->Branch("eventid",&eventTree);
      treesScan[step][syst]->Branch("weight",&weightTree);
      
    }
  }
  
  JEC_PATH = "CondFormats/JetMETObjects/data/";
  fip = edm::FileInPath(JEC_PATH+"Spring10_Uncertainty_AK5PF.txt");
  jecUnc = new JetCorrectionUncertainty(fip.fullPath());
  JES_SW = 0.015;
  JES_b_cut = 0.02;
  JES_b_overCut = 0.03;

  //  cout<< "I work for now but I do nothing. But again, if you gotta do nothing, you better do it right. To prove my good will I will provide you with somse numbers later."<<endl;
}

void SingleTopSystematicsTreesDumper::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  
  iEvent.getByLabel(leptonsEta_,leptonsEta);
  iEvent.getByLabel(leptonsPt_,leptonsPt);
  iEvent.getByLabel(leptonsPhi_,leptonsPhi);
  iEvent.getByLabel(leptonsEnergy_,leptonsEnergy);
  iEvent.getByLabel(leptonsCharge_,leptonsCharge);
  iEvent.getByLabel(leptonsRelIso_,leptonsRelIso);
  iEvent.getByLabel(leptonsID_,leptonsID);
  
  iEvent.getByLabel(looseElectronsRelIso_,looseElectronsRelIso);
  iEvent.getByLabel(looseMuonsRelIso_,looseMuonsRelIso);
  
  iEvent.getByLabel(jetsEta_,jetsEta);
  iEvent.getByLabel(jetsPt_,jetsPt);
  iEvent.getByLabel(jetsPhi_,jetsPhi);

  iEvent.getByLabel(jetsEnergy_,jetsEnergy);
  iEvent.getByLabel(jetsBTagAlgo_,jetsBTagAlgo);
  iEvent.getByLabel(jetsAntiBTagAlgo_,jetsAntiBTagAlgo);
  iEvent.getByLabel(jetsFlavour_,jetsFlavour);
  iEvent.getByLabel(jetsCorrTotal_,jetsCorrTotal);
  iEvent.getByLabel(METPhi_,METPhi);
  iEvent.getByLabel(METPt_,METPt);

  iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHPT",perfHP);
  iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHEL",perfHE);
  


  iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHPT",perfHP);
  iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHEL",perfHE);
  
  BinningPointByMap measurePoint;
   
  
  float metPx = 0; 
  float metPy = 0;
  
  metPx = METPt->at(0)*cos(METPhi->at(0));
  metPy = METPt->at(0)*sin(METPhi->at(0));

  float metPxTmp = metPx; 
  float metPyTmp = metPy;



  size_t nLeptons = leptonsPt->size();
  size_t nJets = jetsPt->size();
  
  double WeightLumi = finalLumi*crossSection/originalEvents;
  double BTagWeight = 1;
  double BTagWeightWSample = 1;
  double BTagWeightTTSample = 1;
  double Weight = 1;
  double MTWValue =0;
  double MTWValueQCD =0;
  double RelIsoQCDCut = 0.1;
  
  float ptCut = 30;  
  if(channel=="Data")WeightLumi=1;
  
  for(size_t s = 0; s < systematics.size();++s){
    string syst_name =  systematics.at(s);

    Weight = WeightLumi;
    BTagWeight = 1;
    BTagWeightWSample = 1;
    BTagWeightTTSample = 1;
    
    //Setup for systematics

    
    double TCHP_CTag = BScaleFactor("TCHP_C",syst_name);
    double TCHP_BTag = BScaleFactor("TCHP_B",syst_name);
    
    double TCHE_CTag = BScaleFactor("TCHE_C",syst_name);
    double TCHE_BTag = BScaleFactor("TCHE_B",syst_name);

    //    double TCHP_MisTag = 1.4; //
    //    double TCHE_MisTag = 1.0; //DummyValues, to be changed in the next part of the code
    
    //    cout << " TCHP_BTag should be 0.9 "<< TCHP_BTag <<endl;    
      
    leptons.clear();
    leptonsQCD.clear();
    jets.clear();
    bjets.clear();
    antibjets.clear();
    //    loosejets.clear();

    MTWValue =0;
    metPx = metPxTmp; 
    metPy = metPyTmp;

    
    int lowBTagTreePosition=-1;
    lowBTagTree = 99999;
    
    int highBTagTreePosition=-1;
    highBTagTree = -9999;


    if(syst_name == "UnclusteredMETUp"){
      iEvent.getByLabel(UnclMETPx_,UnclMETPx);
      iEvent.getByLabel(UnclMETPy_,UnclMETPy);
      metPx+= (*UnclMETPx) *0.1;
      metPy+= (*UnclMETPy) *0.1;
    }
    if(syst_name == "UnclusteredMETDown"){
      iEvent.getByLabel(UnclMETPx_,UnclMETPx);
      iEvent.getByLabel(UnclMETPy_,UnclMETPy);
      metPx-= (*UnclMETPx) *0.1;
      metPy-= (*UnclMETPy) *0.1;
    }
    
    //    cout <<" BTagWeight before "<< BTagWeight << endl;

    float eta;
    float ptCorr;
    int flavour;
    double unc =0;
    
    //    cout << " test 0 " << endl;
    //Loops to apply systematics on jets-leptons
    
    if(doQCD_){
      for(size_t i = 0;i<nLeptons;++i){
	//   float leptonPx = cos(leptonsPhi->at(i))* leptonsPt->at(i);
	//   float leptonPy = sin(leptonsPhi->at(i))* leptonsPt->at(i);
	
	float leptonRelIso = leptonsRelIso->at(i);
	lepRelIso = leptonRelIso;
	if(leptonRelIso < RelIsoQCDCut )continue;
	
	float leptonPt = leptonsPt->at(i);
	float leptonPhi = leptonsPhi->at(i);
	float leptonEta = leptonsEta->at(i);
	//      float leptonPt = sqrt(leptonPx*leptonPx + leptonPy*leptonPy);
	
	//      float leptonP = sqrt( (leptonPt*leptonPt) + (leptonPz*leptonPz));
	float leptonE = leptonsEnergy->at(i);
	leptonsQCD.push_back(math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE));
      }
      //leptons.clear();
    }
    
    for(size_t i = 0;i<nLeptons;++i){
      
      //cout << "test lepton flavour "<< leptonsFlavour_ <<endl;
      float leptonRelIso = leptonsRelIso->at(i);
      lepRelIso = leptonRelIso;
      if(leptonRelIso>RelIsoCut)continue;
      if(leptonsFlavour_ == "electron"  ) {
	if(leptonsID->size()==0)cout<< "warning requiring ele id of collection which has no entries! Check the leptonsFlavour parameter "<<endl;
	//	cout << " debug flavour "<< leptonsFlavour_ <<" leptons ID " << leptonsID->at(i)<<endl;
	float leptonID = leptonsID->at(i);
	if (!(leptonID==5 || leptonID ==7))continue;
	
      }
      
      float leptonPt = leptonsPt->at(i);
      float leptonPhi = leptonsPhi->at(i);
      float leptonEta = leptonsEta->at(i);
      //      float leptonPt = sqrt(leptonPx*leptonPx + leptonPy*leptonPy);
      
      //      float leptonP = sqrt( (leptonPt*leptonPt) + (leptonPz*leptonPz));
      float leptonE = leptonsEnergy->at(i);
      leptons.push_back(math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE));
    }
    
    //Part of the effective selection and filling
    if(leptons.size()!=1 && (leptonsQCD.size()!=1 && doQCD_) )return;
    
    for(size_t i = 0;i<nJets;++i){
      //eta = jetsEta->at(i);
      eta = jetsEta->at(i);
      ptCorr = jetsPt->at(i);
      flavour = jetsFlavour->at(i);
      
      bool passesPtCut = ptCorr>ptCut;
      //bool passesLoosePtCut = ptCorr>loosePtCut;
      if(passesPtCut && syst_name != "JESUp" && syst_name != "JESDown") jets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i),jetsEta->at(i),jetsPhi->at(i),jetsEnergy->at(i) ) ); 
      //if(passesLoosePtCut && syst_name != "JESUp" && syst_name != "JESDown") loosejets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i),jetsPhi->at(i),jetsEta->at(i),jetsEnergy->at(i)) );   
      else if(syst_name == "JESUp"){
	unc = jetUncertainty( eta,  ptCorr, flavour);
	passesPtCut = ptCorr * (1+unc) >ptCut;
	//passesLoosePtCut = ptCorr * (1+unc) > loosePtCut;
	metPx-=(jetsPt->at(i)*cos(jetsPhi->at(i)))*unc;
	metPy-=(jetsPt->at(i)*sin(jetsPhi->at(i)))*unc;
	if(passesPtCut) jets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i) * (1+unc),jetsEta->at(i), jetsPhi->at(i), jetsEnergy->at(i) * (1+unc)) ); 
	//if(passesLoosePtCut) loosejets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i) * (1+unc),jetsPhi->at(i)*(1+unc),jetsEta->at(i)*(1+unc),jetsEnergy->at(i)*(1+unc)) );   
      }
      else if(syst_name == "JESDown"){
	unc = jetUncertainty( eta,  ptCorr, flavour);
	passesPtCut = ptCorr * (1-unc) > ptCut;
	//passesLoosePtCut = ptCorr * (1-unc) > loosePtCut;
	metPx-=-(jetsPt->at(i)*cos(jetsPhi->at(i)))*unc;
	metPy-=-(jetsPt->at(i)*sin(jetsPhi->at(i)))*unc;
	if(passesPtCut) jets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i) * (1-unc),jetsEta->at(i),jetsPhi->at(i),jetsEnergy->at(i)*(1-unc)) ); 
	//if(passesLoosePtCut) loosejets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i) * (1-unc),jetsPhi->at(i)*(1-unc),jetsEta->at(i)*(1-unc),jetsEnergy->at(i)*(1-unc)) );   
      }
      //      if(passesPtCut) cout <<" jet "<< i <<" passes pt cut, flavour "<< abs(flavour)<< " syst " << syst_name << " pt "<< ptCorr<< " pt with unc "<< jets.back().pt() <<" unc "<< unc << endl;
      
      bool passesBTag = jetsBTagAlgo->at(i)>bTagThreshold;
      bool passesAntiBTag = jetsAntiBTagAlgo->at(i)<1.7;
      //bool passesAntiBTag = jetsBTagAlgo->at(i)<bTagThreshold;
      
      if(passesPtCut && passesBTag) {
	
	bjets.push_back(jets.back()); 
	
	if(abs(flavour)==4) BTagWeight*=TCHP_CTag ;
	if(abs(flavour)==5) BTagWeight*=TCHP_BTag ;
	if(abs(flavour)<4 && abs(flavour)>0){
	  
	  double etaMin =  min(fabs(eta),(float)2.3999);
	  double ptMin =  min(jets.back().pt(),998.0);
	  
	  measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
	  measurePoint.insert(BinningVariables::JetEt,ptMin);
	  
	  double eff =(perfHP->getResult(PerformanceResult::BTAGLEFF,measurePoint));
	  double SF = (perfHP->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
	  double SFErr = (perfHP->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	  
	  BTagWeight*=  MisTagScaleFactor("TCHP_L",syst_name,SF,eff,SFErr);
	  measurePoint.reset();
	  //	  cout <<" jet "<< i <<" passes direct btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << " eff "<<  eff<<" SF "<< SF << " sf unc "<< SFErr <<endl;
	}
	//	cout <<" jet "<< i <<" passes direct btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << endl;
      }
      if(passesPtCut && passesAntiBTag){
	
	antibjets.push_back(jets.back());
	if(abs(flavour)==4) BTagWeight*=TCHE_CTag ;
	if(abs(flavour)==5) BTagWeight*=TCHE_BTag ;
	if(abs(flavour)<4 && abs(flavour)>0){
	  
	  double etaMin =  min(fabs(eta),(float)2.3999);
	  double ptMin =  min(jets.back().pt(),998.0);
	  
	  measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
	  measurePoint.insert(BinningVariables::JetEt,ptMin);
	  
	  double eff =(perfHE->getResult(PerformanceResult::BTAGLEFF,measurePoint));
	  double SF = (perfHE->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
	  double SFErr = (perfHE->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	  
	  BTagWeight*=  MisTagScaleFactor("TCHE_L",syst_name,SF,eff,SFErr);
	  
	  measurePoint.reset();
	  //	  cout <<" jet "<< i <<" passes anti-btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << " eff "<<  eff<<" SF "<< SF << " sf unc "<< SFErr <<endl;
	}
      }
      
      //      if(!passesLoosePtCut)continue;
      if(passesPtCut && jetsBTagAlgo->at(i) > highBTagTree){
	highBTagTree=jetsBTagAlgo->at(i);
	highBTagTreePosition=jets.size()-1;
      } 
      
      //if(!passesPtCut)continue;
      if(passesPtCut && jetsBTagAlgo->at(i) < lowBTagTree){
	lowBTagTree=jetsBTagAlgo->at(i);
	lowBTagTreePosition=jets.size()-1;
      }
    }
    

    

    if (doQCD_){
      //      cout << " qcd lep size " <<leptonsQCD.size()<< " non qcd lep size" <<looseMuonsRelIso->size()+ looseElectronsRelIso->size()<< endl;
      if(leptonsQCD.size()==1 ){
	//&& (looseMuonsRelIso->size()+ looseElectronsRelIso->size())==0){
	metPt = sqrt(metPx*metPx+metPy*metPy);
	MTWValueQCD =  sqrt((leptonsQCD.at(0).pt()+metPt)*(leptonsQCD.at(0).pt()+metPt)  -(leptonsQCD.at(0).px()+metPx)*(leptonsQCD.at(0).px()+metPx) -(leptonsQCD.at(0).py()+metPy)*(leptonsQCD.at(0).py()+metPy));
	

	if( jets.size()==2 && bjets.size()==1 && antibjets.size()==1){
	  
	  Weight*=BTagWeight;
	  
	  math::PtEtaPhiELorentzVector top = top4Momentum(leptonsQCD.at(0),bjets.at(0),metPx,metPy);
	  float fCosThetaLJ =  cosThetaLJ(leptonsQCD.at(0), antibjets.at(0), top);
	  
	  runTree = iEvent.eventAuxiliary().run();
	  lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	  eventTree = iEvent.eventAuxiliary().event();
	  weightTree = Weight;
	  
	  etaTree = fabs(antibjets.at(0).eta());
	  cosTree = fCosThetaLJ;
	  topMassTree = top.mass();
	  mtwMassTree = MTWValueQCD;
	  chargeTree = leptonsCharge->at(0) ; 
	  
	  treesQCD[syst_name]->Fill();

	}

	
	if( lowBTagTreePosition > -1 && highBTagTreePosition > -1 && jets.size() ==2 &&  bjets.size()==0 ){
	  if(highBTagTreePosition == lowBTagTreePosition)continue;
	  
	  math::PtEtaPhiELorentzVector top = top4Momentum(leptonsQCD.at(0),jets.at(highBTagTreePosition),metPx,metPy);
	  
	  float fCosThetaLJ =  cosThetaLJ(leptonsQCD.at(0),jets.at(lowBTagTreePosition),top);
	  
	  runTree = iEvent.eventAuxiliary().run();
	  lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	  eventTree = iEvent.eventAuxiliary().event();
	  weightTree = Weight;
	  
	  etaTree = fabs(jets.at(lowBTagTreePosition).eta());
	  etaTree2 = fabs(jets.at(highBTagTreePosition).eta());
	  cosTree = fCosThetaLJ;
	  topMassTree = top.mass();
	  mtwMassTree = MTWValueQCD;
	  chargeTree = leptonsCharge->at(0);
	  
	  treesWSampleQCD[syst_name]->Fill();

	}
      }
      //      cout << "inside doqcd 4 "<<endl;
      
    }

    if(leptons.size()!=1)continue;
    
    metPt = sqrt(metPx*metPx+metPy*metPy);
    MTWValue =  sqrt((leptons.at(0).pt()+metPt)*(leptons.at(0).pt()+metPt)  -(leptons.at(0).px()+metPx)*(leptons.at(0).px()+metPx) -(leptons.at(0).py()+metPy)*(leptons.at(0).py()+metPy));
    
    if(leptonsFlavour_=="muon") if( (leptons.size() == 1 && (looseMuonsRelIso->size( )+ looseElectronsRelIso->size())>1))   continue;//Loose muons size always >=1 due to presence of tight muon 
    if(leptonsFlavour_=="electron"){
      if((looseMuonsRelIso->size())>0) continue;//Electrons +1 loose muon vetoed. Loose electron veto is replaced by z veto (not done here yet)
    }
    
    //W control Sample
    if( lowBTagTreePosition > -1 && highBTagTreePosition > -1 && jets.size() ==2 &&  bjets.size()==0 ){
      if(highBTagTreePosition == lowBTagTreePosition)continue;
      
      math::PtEtaPhiELorentzVector top = top4Momentum(leptons.at(0),jets.at(highBTagTreePosition),metPx,metPy);
      
      float fCosThetaLJ =  cosThetaLJ(leptons.at(0),jets.at(lowBTagTreePosition),top);
      
      runTree = iEvent.eventAuxiliary().run();
      lumiTree = iEvent.eventAuxiliary().luminosityBlock();
      eventTree = iEvent.eventAuxiliary().event();
      weightTree = Weight;

      etaTree = fabs(jets.at(lowBTagTreePosition).eta());
      etaTree2 = fabs(jets.at(highBTagTreePosition).eta());
      cosTree = fCosThetaLJ;
      topMassTree = top.mass();
      mtwMassTree = MTWValue;
      chargeTree = leptonsCharge->at(0);

      lepPt = leptons.at(0).pt();
      lepPz = leptons.at(0).pz();
      lepPhi = leptons.at(0).phi();
      
      bJetPt = jets.at(highBTagTreePosition).pt();
      bJetE = jets.at(highBTagTreePosition).energy();
      bJetPz = jets.at(highBTagTreePosition).pz();
      bJetPhi = jets.at(highBTagTreePosition).phi();

      fJetPt = jets.at(lowBTagTreePosition).pt();
      fJetE = jets.at(lowBTagTreePosition).energy();
      fJetPz = jets.at(lowBTagTreePosition).pz();
      fJetPhi = jets.at(lowBTagTreePosition).phi();

      topPt = top.pt();
      topE = top.energy();
      topPz = top.pz();
      topPhi = top.phi();
      
      totalEnergy = (top+jets.at(lowBTagTreePosition)).energy();
      totalMomentum = (top+jets.at(lowBTagTreePosition)).P();
      

      metPt = METPt->at(0);
      metPhi = METPhi->at(0);
      
      treesWSample[syst_name]->Fill();
    }
        
    //Signal sample
    if( jets.size()==2 && bjets.size()==1 && antibjets.size()==1){
      
      //      cout << " passes cuts pre-mtw, syst " << syst_name << " b tag weight " <<  BTagWeight<< " Weight " << Weight  <<endl;
      
      Weight*=BTagWeight;
      
      math::PtEtaPhiELorentzVector top = top4Momentum(leptons.at(0),bjets.at(0),metPx,metPy);
      float fCosThetaLJ =  cosThetaLJ(leptons.at(0), antibjets.at(0), top);

      runTree = iEvent.eventAuxiliary().run();
      lumiTree = iEvent.eventAuxiliary().luminosityBlock();
      eventTree = iEvent.eventAuxiliary().event();
      weightTree = Weight;

      etaTree = fabs(antibjets.at(0).eta());
      cosTree = fCosThetaLJ;
      topMassTree = top.mass();
      mtwMassTree = MTWValue;
      chargeTree = leptonsCharge->at(0) ; 

      lepPt = leptons.at(0).pt();
      lepPz = leptons.at(0).pz();
      lepPhi = leptons.at(0).phi();
      
      bJetPt = bjets.at(0).pt();
      bJetE = bjets.at(0).energy();
      bJetPz = bjets.at(0).pz();
      bJetPhi = bjets.at(0).phi();

      fJetPt = antibjets.at(0).pt();
      fJetE = antibjets.at(0).energy();
      fJetPz = antibjets.at(0).pz();
      fJetPhi = antibjets.at(0).phi();

      topPt = top.pt();
      topE = top.energy();
      topPz = top.pz();
      topPhi = top.phi();
      
      totalEnergy = (top+antibjets.at(0)).energy();
      totalMomentum = (top+antibjets.at(0)).P();
      
      metPt = METPt->at(0);
      metPhi = METPhi->at(0);

      trees[syst_name]->Fill();

      if(leptonsFlavour_ =="electron" )cout << " passes cuts pre-mtw, syst " << syst_name << " top mass "<< top.mass() << " cosTheta* "<< fCosThetaLJ << " fjetEta " << fabs(antibjets.at(0).eta()) << " Weight "  << Weight << " B Weight "<< BTagWeight <<endl;
      
    }

    //B-tag scanned sample
    if(doBScan_){
      if( jets.size()==2 && antibjets.size()==1){
	if( lowBTagTreePosition > -1 && highBTagTreePosition > -1 && jets.size() ==2 /*&&  bjets.size()==0 */){
	  if(highBTagTreePosition == lowBTagTreePosition)continue;
	  for( size_t step = 1; step <= bScanSteps; ++step){
	    //    if(highBTagTree < (bTagThreshold-(step*1.41)))continue;
	    
	    //      cout << " passes cuts pre-mtw, syst " << syst_name << " b tag weight " <<  BTagWeight<< " Weight " << Weight  <<endl;
	    
	    Weight*=BTagWeight;
	    math::PtEtaPhiELorentzVector top = top4Momentum(leptons.at(0),jets.at(highBTagTreePosition),metPx,metPy);
	    
	    //      cout << " test 3 " << endl;
	    
	    float fCosThetaLJ =  cosThetaLJ(leptons.at(0),jets.at(lowBTagTreePosition),top);
	    
	    runTree = iEvent.eventAuxiliary().run();
	    lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	    eventTree = iEvent.eventAuxiliary().event();
	    weightTree = Weight;
	    
	    //cout << " test 4 " << endl;
	    etaTree = fabs(jets.at(lowBTagTreePosition).eta());
	    etaTree2 = fabs(jets.at(highBTagTreePosition).eta());
	    cosTree = fCosThetaLJ;
	    topMassTree = top.mass();
	    mtwMassTree = MTWValue;
	    chargeTree = leptonsCharge->at(0);
	    /*
	    lepPt = leptons.at(0).pt();
	    lepPz = leptons.at(0).pz();
	    lepPhi = leptons.at(0).phi();
	    
	    bJetPt = jets.at(highBTagTreePosition).pt();
	    bJetE = jets.at(highBTagTreePosition).energy();
	    bJetPz = jets.at(highBTagTreePosition).pz();
	    bJetPhi = jets.at(highBTagTreePosition).phi();
	    
	    fJetPt = jets.at(lowBTagTreePosition).pt();
	    fJetE = jets.at(lowBTagTreePosition).energy();
	    fJetPz = jets.at(lowBTagTreePosition).pz();
	    fJetPhi = jets.at(lowBTagTreePosition).phi();
	    
	    topPt = top.pt();
	    topE = top.energy();
	    topPz = top.pz();
	    topPhi = top.phi();
	    
	    totalEnergy = (top+jets.at(lowBTagTreePosition)).energy();
	    totalMomentum = (top+jets.at(lowBTagTreePosition)).P();
	    
	    
	    metPt = METPt->at(0);
	    metPhi = METPhi->at(0);
	    */
	    //	    cout << " high b tag "<< highBTagTree<< " top mass "<< topMassTree << endl;
	    treesScan[step-1][syst_name]->Fill();            
	  }
	}
      }
    }
  }
}


float SingleTopSystematicsTreesDumper::cosThetaLJ(math::PtEtaPhiELorentzVector lepton, math::PtEtaPhiELorentzVector jet, math::PtEtaPhiELorentzVector top){
  
  math::PtEtaPhiELorentzVector boostedLepton = ROOT::Math::VectorUtil::boost(lepton,top.BoostToCM());
  math::PtEtaPhiELorentzVector boostedJet = ROOT::Math::VectorUtil::boost(jet,top.BoostToCM());

  return  ROOT::Math::VectorUtil::CosTheta(boostedJet.Vect(),boostedLepton.Vect());
  
}

math::PtEtaPhiELorentzVector SingleTopSystematicsTreesDumper::top4Momentum(math::PtEtaPhiELorentzVector lepton, math::PtEtaPhiELorentzVector jet, float metPx, float metPy){
  return top4Momentum(lepton.px(),lepton.py(),lepton.pz(),lepton.energy(),jet.px(),jet.py(),jet.pz(),jet.energy(),metPx,metPy);
}

math::PtEtaPhiELorentzVector SingleTopSystematicsTreesDumper::top4Momentum(float leptonPx, float leptonPy, float leptonPz, float leptonE, float jetPx, float jetPy, float jetPz,float jetE, float metPx, float metPy){
  float lepton_Pt = sqrt( (leptonPx*leptonPx)+  (leptonPy*leptonPy) );
  
  math::XYZTLorentzVector neutrino = NuMomentum(leptonPx,leptonPy,leptonPz,lepton_Pt,leptonE,metPx,metPy);//.at(0);;
    
  math::XYZTLorentzVector lep(leptonPx,leptonPy,leptonPz,leptonE);
  math::XYZTLorentzVector jet(jetPx,jetPy,jetPz,jetE);
  
  math::XYZTLorentzVector top = lep + jet + neutrino;
  return math::PtEtaPhiELorentzVector(top.pt(),top.eta(),top.phi(),top.E());  
}

math::XYZTLorentzVector SingleTopSystematicsTreesDumper::NuMomentum(float leptonPx, float leptonPy, float leptonPz, float leptonPt, float leptonE, float metPx, float metPy ){

    
  double  mW = 80.38;
  
  math::XYZTLorentzVector result;
  
  //  double Wmt = sqrt(pow(Lepton.et()+MET.pt(),2) - pow(Lepton.px()+metPx,2) - pow(leptonPy+metPy,2) );
    
  double MisET2 = (metPx*metPx + metPy*metPy);
  double mu = (mW*mW)/2 + metPx*leptonPx + metPy*leptonPy;
  double a  = (mu*leptonPz)/(leptonE*leptonE - leptonPz*leptonPz);
  double a2 = TMath::Power(a,2);
  double b  = (TMath::Power(leptonE,2.)*(MisET2) - TMath::Power(mu,2.))/(TMath::Power(leptonE,2) - TMath::Power(leptonPz,2));
  double pz1(0),pz2(0),pznu(0);
  int nNuSol(0);

  math::XYZTLorentzVector p4nu_rec;
  math::XYZTLorentzVector p4W_rec;
  math::XYZTLorentzVector p4b_rec;
  math::XYZTLorentzVector p4Top_rec;
  math::XYZTLorentzVector p4lep_rec;    
  
  p4lep_rec.SetPxPyPzE(leptonPx,leptonPy,leptonPz,leptonE);
  
  math::XYZTLorentzVector p40_rec(0,0,0,0);
  
  if(a2-b > 0 ){
    //if(!usePositiveDeltaSolutions_)
    //  {
    //	result.push_back(p40_rec);
    //	return result;
    //	}
    double root = sqrt(a2-b);
    pz1 = a + root;
    pz2 = a - root;
    nNuSol = 2;     
    
    //    if(usePzPlusSolutions_)pznu = pz1;    
    //    if(usePzMinusSolutions_)pznu = pz2;
    //if(usePzAbsValMinimumSolutions_){
    pznu = pz1;
    if(fabs(pz1)>fabs(pz2)) pznu = pz2;
    //}
    
    
    double Enu = sqrt(MisET2 + pznu*pznu);
    
    p4nu_rec.SetPxPyPzE(metPx, metPy, pznu, Enu);
    
    //    result =.push_back(p4nu_rec);
    result = p4nu_rec;
    
  }
  else{
    
    // if(!useNegativeDeltaSolutions_){
    //result.push_back(p40_rec);
    //  return result;
    //    }
    //    double xprime = sqrt(mW;
    

      double ptlep = leptonPt,pxlep=leptonPx,pylep=leptonPy,metpx=metPx,metpy=metPy;
      
      double EquationA = 1;
      double EquationB = -3*pylep*mW/(ptlep);
      double EquationC = mW*mW*(2*pylep*pylep)/(ptlep*ptlep)+mW*mW-4*pxlep*pxlep*pxlep*metpx/(ptlep*ptlep)-4*pxlep*pxlep*pylep*metpy/(ptlep*ptlep);
      double EquationD = 4*pxlep*pxlep*mW*metpy/(ptlep)-pylep*mW*mW*mW/ptlep;
      
      std::vector<long double> solutions = EquationSolve<long double>((long double)EquationA,(long double)EquationB,(long double)EquationC,(long double)EquationD);
      
      std::vector<long double> solutions2 = EquationSolve<long double>((long double)EquationA,-(long double)EquationB,(long double)EquationC,-(long double)EquationD);
      
      
      double deltaMin = 14000*14000;
      double zeroValue = -mW*mW/(4*pxlep); 
      double minPx=0;
      double minPy=0;
      
      //    std::cout<<"a "<<EquationA << " b " << EquationB  <<" c "<< EquationC <<" d "<< EquationD << std::endl; 
      
   //  if(usePxMinusSolutions_){
	for( int i =0; i< (int)solutions.size();++i){
	  if(solutions[i]<0 ) continue;
	  double p_x = (solutions[i]*solutions[i]-mW*mW)/(4*pxlep); 
	  double p_y = ( mW*mW*pylep + 2*pxlep*pylep*p_x -mW*ptlep*solutions[i])/(2*pxlep*pxlep);
	  double Delta2 = (p_x-metpx)*(p_x-metpx)+(p_y-metpy)*(p_y-metpy); 
	  
      //      std::cout<<"intermediate solution1 met x "<<metpx << " min px " << p_x  <<" met y "<<metpy <<" min py "<< p_y << std::endl; 

      if(Delta2< deltaMin && Delta2 > 0){deltaMin = Delta2;
      minPx=p_x;
      minPy=p_y;}
      //     std::cout<<"solution1 met x "<<metpx << " min px " << minPx  <<" met y "<<metpy <<" min py "<< minPy << std::endl; 
      }
	
	//    } 
	
	//if(usePxPlusSolutions_){
      for( int i =0; i< (int)solutions2.size();++i){
	if(solutions2[i]<0 ) continue;
	double p_x = (solutions2[i]*solutions2[i]-mW*mW)/(4*pxlep); 
	double p_y = ( mW*mW*pylep + 2*pxlep*pylep*p_x +mW*ptlep*solutions2[i])/(2*pxlep*pxlep);
	double Delta2 = (p_x-metpx)*(p_x-metpx)+(p_y-metpy)*(p_y-metpy); 
	//  std::cout<<"intermediate solution2 met x "<<metpx << " min px " << minPx  <<" met y "<<metpy <<" min py "<< minPy << std::endl; 
	if(Delta2< deltaMin && Delta2 > 0){deltaMin = Delta2;
	  minPx=p_x;
	  minPy=p_y;}
	//	std::cout<<"solution2 met x "<<metpx << " min px " << minPx  <<" met y "<<metpy <<" min py "<< minPy << std::endl; 
      }
      //}
  
    double pyZeroValue= ( mW*mW*pxlep + 2*pxlep*pylep*zeroValue);
    double delta2ZeroValue= (zeroValue-metpx)*(zeroValue-metpx) + (pyZeroValue-metpy)*(pyZeroValue-metpy);
    
    if(deltaMin==14000*14000)return result;    
    //    else std::cout << " test " << std::endl;

    if(delta2ZeroValue < deltaMin){
      deltaMin = delta2ZeroValue;
      minPx=zeroValue;
      minPy=pyZeroValue;}
  
    //    std::cout<<" MtW2 from min py and min px "<< sqrt((minPy*minPy+minPx*minPx))*ptlep*2 -2*(pxlep*minPx + pylep*minPy)  <<std::endl;
    ///    ////Y part   

    double mu_Minimum = (mW*mW)/2 + minPx*pxlep + minPy*pylep;
    double a_Minimum  = (mu_Minimum*leptonPz)/(leptonE*leptonE - leptonPz*leptonPz);
    pznu = a_Minimum;
  
    //if(!useMetForNegativeSolutions_){
      double Enu = sqrt(minPx*minPx+minPy*minPy + pznu*pznu);
      p4nu_rec.SetPxPyPzE(minPx, minPy, pznu , Enu);
  
      //    }
      //    else{
      //      pznu = a;
      //      double Enu = sqrt(metpx*metpx+metpy*metpy + pznu*pznu);
      //      p4nu_rec.SetPxPyPzE(metpx, metpy, pznu , Enu);
      //    }
    
      //      result.push_back(p4nu_rec);
      result = p4nu_rec;
  }
  return result;    
}



double SingleTopSystematicsTreesDumper::BScaleFactor(string algo,string syst_name){

  double bcentral =0.9;  
  double berr = 0.15;
  double cerr =0.3;
  double tcheeff =0.7;
  
  if(syst_name == "BTagUp"){
    if(algo == "TCHP_B"){
      return bcentral+berr;
    }
    if(algo == "TCHP_C"){
      return bcentral+cerr;
    }
    
    if(algo == "TCHE_B"){
      return (1-tcheeff*(bcentral+berr))/(1-tcheeff);
    }
    
    if(algo == "TCHE_C"){
      return (1-tcheeff*(bcentral+cerr))/(1-tcheeff);
    }
    
  }
  
  if(syst_name == "BTagDown"){
    if(algo == "TCHP_B"){
      return bcentral-berr;
    }
    if(algo == "TCHP_C"){
      return bcentral-cerr;
    }
  
    if(algo == "TCHE_B"){
      return (1-tcheeff*(bcentral-berr))/(1-tcheeff);
    }
    if(algo == "TCHE_C"){
      return (1-tcheeff*(bcentral-cerr))/(1-tcheeff);
    }
  }

  if(algo == "TCHP_B"){
    return bcentral;
  }
  if(algo == "TCHP_C"){
    return bcentral;
  }
  if(algo == "TCHE_B"){
    return (1-tcheeff*(bcentral))/(1-tcheeff);
  }
  if(algo == "TCHE_C"){
    return (1-tcheeff*(bcentral))/(1-tcheeff);
  }
    
  return 0.9;
}

double SingleTopSystematicsTreesDumper::MisTagScaleFactor(string algo,string syst_name,double sf, double eff, double sferr){
  double mistagcentral = sf;  
  double mistagerr = sferr;
  double tcheeff = eff;

  
  if(syst_name == "MisTagUp"){
    if(algo == "TCHP_L"){
      return mistagcentral+mistagerr;
    }
    if(algo == "TCHE_L"){
      return (1-tcheeff)/(1-tcheeff/(mistagcentral+mistagerr));
    }
    
  }
  
  if(syst_name == "MisTagDown"){
    if(algo == "TCHP_L"){
      return mistagcentral-mistagerr;
    }
    if(algo == "TCHE_L"){
      return (1-tcheeff)/(1-tcheeff/(mistagcentral-mistagerr));
    }
  }

  if(algo == "TCHP_L"){
    return mistagcentral;
  }
  if(algo == "TCHE_L"){
    return (1-tcheeff)/(1-tcheeff/(mistagcentral));
  }
  
  return 0.9;


}


double SingleTopSystematicsTreesDumper::jetUncertainty(double eta, double ptCorr, int flavour){
  jecUnc->setJetEta(eta); 
  jecUnc->setJetPt(ptCorr);
  double JetCorrection = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
  bool cut = ptCorr> 50 && ptCorr < 200 && fabs(eta) < 2.0;
  // JES_SW = 0.015;                                                                                                                                 
  //  double JES_PU=0.75*0.8*2.2/ptCorr;
  double JES_PU=0.; //We are using pfNoPU
  double JES_b=0;
  if(abs(flavour)==5){
    if(cut) JES_b = JES_b_cut;
    else JES_b = JES_b_overCut;
  }
  //    float JESUncertaintyTmp = sqrt(JESUncertainty*JESUncertainty + JetCorrection*JetCorrection);                                                 
  return sqrt(JES_b*JES_b + JES_PU*JES_PU +JES_SW*JES_SW + JetCorrection*JetCorrection);
}

void SingleTopSystematicsTreesDumper::endJob(){
  
  //part for rate systematics

  for(size_t i = 0; i < rate_systematics.size();++i){
    string syst = rate_systematics[i];
    string treename = (channel+"_"+syst);

    cout<< " endjob"  << syst<< " 0 "<<endl;
    
    trees[syst]->CopyAddresses(trees["noSyst"]);
    treesWSample[syst]->CopyAddresses(treesWSample["noSyst"]);

    trees[syst]->CopyEntries(trees["noSyst"]); 
    treesWSample[syst]->CopyEntries(trees["noSyst"]); 
    

        cout<< " endjob"  << syst<< " 1 "<<endl;

    /*  for(size_t step = 0; step < bScanSteps;++step){
      treesScan[step][syst]->CopyAddresses(treesScan[step]["noSyst"]); 
      treesScan[step][syst]->CopyEntries(treesScan[step]["noSyst"]); 
    }
    */
    
    //modify the weight by a constant factor    
    double tmpWeight = 0;
    double weightSF = 1.;
    
    TBranch * b = trees["noSyst"]->GetBranch("weight");
    int entries = b->GetEntries();
    b->SetAddress(&tmpWeight);    


    cout<< " endjob"  << syst<< " 2 "<<endl;
    
    trees[syst]->GetBranch("weight")->Reset();
    trees[syst]->GetBranch("weight")->SetAddress(&tmpWeight);
    

    cout<< " endjob"  << syst<< " 3 "<<endl;
    
    for(int t =0; t < entries ; ++t){
      b->GetEntry(t);
      tmpWeight*=weightSF;
      trees[syst]->GetBranch("weight")->Fill();
      
    }
    

    
    b->SetAddress(&weightTree);
    trees[syst]->GetBranch("weight")->SetAddress(&weightTree);
    

    
    //    cout<< " syst "<< syst<< " weights entries "<<  entries <<endl;

  }
}
  



//define this as a plug-in
DEFINE_FWK_MODULE(SingleTopSystematicsTreesDumper);
