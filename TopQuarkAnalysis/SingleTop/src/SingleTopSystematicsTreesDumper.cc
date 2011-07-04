/*
*\Author:  O.Iorio
*
*
*
*\version  $Id: SingleTopSystematicsTreesDumper.cc,v 1.9 2011/07/03 20:01:27 oiorio Exp $ 
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
//#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "DataFormats/Math/interface/deltaR.h"

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

  maxPtCut = iConfig.getUntrackedParameter<double>("maxPtCut",30);

  dataPUFile_ =  iConfig.getUntrackedParameter< std::string >("dataPUFile","pileUpDistr.root");
  mcPUFile_ =  iConfig.getUntrackedParameter< std::string >("mcPUFile","pileupdistr_TChannel.root");
  
  leptonsPt_ =  iConfig.getParameter< edm::InputTag >("leptonsPt");
  leptonsPhi_ =  iConfig.getParameter< edm::InputTag >("leptonsPhi");
  leptonsEta_ =  iConfig.getParameter< edm::InputTag >("leptonsEta");
  leptonsEnergy_ =  iConfig.getParameter< edm::InputTag >("leptonsEnergy");
  leptonsCharge_ =  iConfig.getParameter< edm::InputTag >("leptonsCharge");
  leptonsRelIso_ =  iConfig.getParameter< edm::InputTag >("leptonsRelIso");
  leptonsDB_ =  iConfig.getParameter< edm::InputTag >("leptonsDB");
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

  mode_ =  iConfig.getUntrackedParameter<std::string >("mode",""); 
  npv_ = iConfig.getParameter< edm::InputTag >("nvertices");//,"PileUpSync"); 
  
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
    trees[syst]->Branch("leptonEta",&lepEta);
    trees[syst]->Branch("leptonPhi",&lepPhi);
    trees[syst]->Branch("leptonRelIso",&lepRelIso);
    
    trees[syst]->Branch("fJetPt",&fJetPt);
    trees[syst]->Branch("fJetE",&fJetE);
    trees[syst]->Branch("fJetEta",&fJetEta);
    trees[syst]->Branch("fJetPhi",&fJetPhi);
    
    trees[syst]->Branch("bJetPt",&bJetPt);
    trees[syst]->Branch("bJetE",&bJetE);
    trees[syst]->Branch("bJetEta",&bJetEta);
    trees[syst]->Branch("bJetPhi",&bJetPhi);
    
    trees[syst]->Branch("metPt",&metPt);
    trees[syst]->Branch("metPhi",&metPhi);
    
    trees[syst]->Branch("topPt",&topPt);
    trees[syst]->Branch("topPhi",&topPhi);
    trees[syst]->Branch("topEta",&topEta);
    trees[syst]->Branch("topE",&topE);

    trees[syst]->Branch("ID",&electronID);

    trees[syst]->Branch("totalEnergy",&totalEnergy);
    trees[syst]->Branch("totalMomentum",&totalMomentum);

    trees[syst]->Branch("bJetFlavour",&bJetFlavourTree);


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

    treesWSample[syst]->Branch("ID",&electronID);


    //Extra info

    treesWSample[syst]->Branch("leptonPt",&lepPt);
    treesWSample[syst]->Branch("leptonEta",&lepEta);
    treesWSample[syst]->Branch("leptonPhi",&lepPhi);
    treesWSample[syst]->Branch("leptonRelIso",&lepRelIso);
    
    treesWSample[syst]->Branch("fJetPt",&fJetPt);
    treesWSample[syst]->Branch("fJetE",&fJetE);
    treesWSample[syst]->Branch("fJetEta",&fJetEta);
    treesWSample[syst]->Branch("fJetPhi",&fJetPhi);
    treesWSample[syst]->Branch("bJetFlavour",&bJetFlavourTree);
    
    treesWSample[syst]->Branch("bJetPt",&bJetPt);
    treesWSample[syst]->Branch("bJetE",&bJetE);
    treesWSample[syst]->Branch("bJetEta",&bJetEta);
    treesWSample[syst]->Branch("bJetPhi",&bJetPhi);

    treesWSample[syst]->Branch("highBTag",&highBTagTree);
    treesWSample[syst]->Branch("lowBTag",&lowBTagTree);
    
    treesWSample[syst]->Branch("metPt",&metPt);
    treesWSample[syst]->Branch("metPhi",&metPhi);
    
    treesWSample[syst]->Branch("topPt",&topPt);
    treesWSample[syst]->Branch("topPhi",&topPhi);
    treesWSample[syst]->Branch("topEta",&topEta);
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

    treesQCD[syst]->Branch("ID",&electronID);

    
    treesQCD[syst]->Branch("leptonPt",&lepPt);
    treesQCD[syst]->Branch("leptonEta",&lepEta);
    treesQCD[syst]->Branch("leptonPhi",&lepPhi);
    treesQCD[syst]->Branch("leptonRelIso",&lepRelIso);
    
    treesQCD[syst]->Branch("fJetPt",&fJetPt);
    treesQCD[syst]->Branch("fJetE",&fJetE);
    treesQCD[syst]->Branch("fJetEta",&fJetEta);
    treesQCD[syst]->Branch("fJetPhi",&fJetPhi);
    
    treesQCD[syst]->Branch("bJetPt",&bJetPt);
    treesQCD[syst]->Branch("bJetE",&bJetE);
    treesQCD[syst]->Branch("bJetEta",&bJetEta);
    treesQCD[syst]->Branch("bJetPhi",&bJetPhi);
    
    treesQCD[syst]->Branch("metPt",&metPt);
    treesQCD[syst]->Branch("metPhi",&metPhi);
    
    treesQCD[syst]->Branch("topPt",&topPt);
    treesQCD[syst]->Branch("topPhi",&topPhi);
    treesQCD[syst]->Branch("topEta",&topEta);
    treesQCD[syst]->Branch("topE",&topE);

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

    treesWSampleQCD[syst]->Branch("ID",&electronID);


    treesWSampleQCD[syst]->Branch("leptonPt",&lepPt);
    treesWSampleQCD[syst]->Branch("leptonEta",&lepEta);
    treesWSampleQCD[syst]->Branch("leptonPhi",&lepPhi);
    treesWSampleQCD[syst]->Branch("leptonRelIso",&lepRelIso);
    
    treesWSampleQCD[syst]->Branch("fJetPt",&fJetPt);
    treesWSampleQCD[syst]->Branch("fJetE",&fJetE);
    treesWSampleQCD[syst]->Branch("fJetEta",&fJetEta);
    treesWSampleQCD[syst]->Branch("fJetPhi",&fJetPhi);
    
    
    treesWSampleQCD[syst]->Branch("bJetPt",&bJetPt);
    treesWSampleQCD[syst]->Branch("bJetE",&bJetE);
    treesWSampleQCD[syst]->Branch("bJetEta",&bJetEta);
    treesWSampleQCD[syst]->Branch("bJetPhi",&bJetPhi);
    
    treesWSampleQCD[syst]->Branch("metPt",&metPt);
    treesWSampleQCD[syst]->Branch("metPhi",&metPhi);
    
    treesWSampleQCD[syst]->Branch("topPt",&topPt);
    treesWSampleQCD[syst]->Branch("topPhi",&topPhi);
    treesWSampleQCD[syst]->Branch("topEta",&topEta);
    treesWSampleQCD[syst]->Branch("topE",&topE);
    

 
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

      treesScan[step][syst]->Branch("bJetPt",&bJetPt);
      treesScan[step][syst]->Branch("bJetFlavour",&bJetFlavourTree);
      treesScan[step][syst]->Branch("fJetPt",&fJetPt);
      
      treesScan[step][syst]->Branch("charge",&chargeTree);
      treesScan[step][syst]->Branch("runid",&runTree);
      treesScan[step][syst]->Branch("lumiid",&lumiTree);
      treesScan[step][syst]->Branch("eventid",&eventTree);
      treesScan[step][syst]->Branch("weight",&weightTree);
      treesScan[step][syst]->Branch("leptonRelIso",&lepRelIso);

      treesScan[step][syst]->Branch("ID",&electronID);

      
      treesScan[step][syst]->Branch("ID",&electronID);

      treesScanQCD[step][syst] = new TTree((treename+"_stepQCD_"+sstep).c_str(),(treename+"_stepQCD_"+sstep).c_str()); 

      treesScanQCD[step][syst]->Branch("etaLowBTag",&etaTree);
      treesScanQCD[step][syst]->Branch("etaHighBTag",&etaTree2);
      treesScanQCD[step][syst]->Branch("costhetalj",&cosTree);
      treesScanQCD[step][syst]->Branch("topMass",&topMassTree);
      treesScanQCD[step][syst]->Branch("mtwMass",&mtwMassTree);
      
      treesScanQCD[step][syst]->Branch("highBTag",&highBTagTree);
      treesScanQCD[step][syst]->Branch("lowBTag",&lowBTagTree);
      
      treesScanQCD[step][syst]->Branch("charge",&chargeTree);
      treesScanQCD[step][syst]->Branch("runid",&runTree);
      treesScanQCD[step][syst]->Branch("lumiid",&lumiTree);
      treesScanQCD[step][syst]->Branch("eventid",&eventTree);
      treesScanQCD[step][syst]->Branch("weight",&weightTree);
      treesScanQCD[step][syst]->Branch("leptonRelIso",&lepRelIso);
      
    }
  }  
    
  JEC_PATH = "CondFormats/JetMETObjects/data/";
  fip = edm::FileInPath(JEC_PATH+"Spring10_Uncertainty_AK5PF.txt");
  jecUnc = new JetCorrectionUncertainty(fip.fullPath());
  JES_SW = 0.015;
  JES_b_cut = 0.02;
  JES_b_overCut = 0.03;
  
  std::string puhistoname = "pileUpDumper/PileUp"+channel;

  LumiWeights_ = edm::LumiReWeighting(
				      mcPUFile_,
				      dataPUFile_,
				      puhistoname,
				      std::string("pileup")  );

  //  cout<< "I work for now but I do nothing. But again, if you gotta do nothing, you better do it right. To prove my good will I will provide you with somse numbers later."<<endl;
}

void SingleTopSystematicsTreesDumper::analyze(const Event& iEvent, const EventSetup& iSetup)
{


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
  
  iEvent.getByLabel(leptonsEta_,leptonsEta);
  iEvent.getByLabel(leptonsPt_,leptonsPt);
  iEvent.getByLabel(leptonsPhi_,leptonsPhi);
  iEvent.getByLabel(leptonsEnergy_,leptonsEnergy);
  iEvent.getByLabel(leptonsCharge_,leptonsCharge);
  iEvent.getByLabel(leptonsRelIso_,leptonsRelIso);
  iEvent.getByLabel(leptonsID_,leptonsID);
  
  iEvent.getByLabel(looseElectronsRelIso_,looseElectronsRelIso);
  iEvent.getByLabel(looseMuonsRelIso_,looseMuonsRelIso);     
  
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

  size_t nLeptons = 0;//leptonsPt->size();
  
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
  //  float maxPtCut = maxPtCut_;

  double myWeight = 1.;

  if(channel != "Data"){
    iEvent.getByLabel(npv_,npv);
    myWeight = LumiWeights_.weight(*npv);
  }

  //edm::EventBase* const iEventB = dynamic_cast<edm::EventBase*>(&iEvent);
  //double MyWeight = LumiWeights_.weight( (*iEventB) );
  
  //double w = LumiWeights_.weight(iEvent);
  
  if(channel=="Data")WeightLumi=1;
  
  for(size_t s = 0; s < systematics.size();++s){
    string syst_name =  systematics.at(s);

    //Here the weight of the event is the weight
    //to normalize the sample to the luminosity 
    //required in the cfg
    Weight = WeightLumi;
    Weight *= myWeight;
    BTagWeight = 1;
    BTagWeightWSample = 1;
    BTagWeightTTSample = 1;
    
    //Setup for systematics

    //This is done according to old b-tagging prescriptions
    //Where the b(c) jet Scale Factors were taken 0.9 +-15%(30%)
    //Will have to be updated
    double TCHP_CTag = BScaleFactor("TCHP_C",syst_name);
    double TCHP_BTag = BScaleFactor("TCHP_B",syst_name);
    
    double TCHE_CTag = BScaleFactor("TCHE_C",syst_name);
    double TCHE_BTag = BScaleFactor("TCHE_B",syst_name);

    double TCHP_AntiCTag = AntiBScaleFactor("TCHP_C",syst_name);
    double TCHP_AntiBTag = AntiBScaleFactor("TCHP_B",syst_name);
    
    double TCHE_AntiCTag = AntiBScaleFactor("TCHE_C",syst_name);
    double TCHE_AntiBTag = AntiBScaleFactor("TCHE_B",syst_name);


    //Here we have vectors of weights 
    //to be associated with the 
    //b-jets selection in the sample according to algorythm X: 
    //a b-tag requirement implies a b_weight_tag_algoX,
    //a b-veto requirement implies a b_weight_antitag_algoX
    b_weight_tag_algo1.clear();
    b_weight_tag_algo2.clear();
    b_weight_antitag_algo1.clear();
    b_weight_antitag_algo2.clear();
    
    b_discriminator_value_tag_algo1.clear();
    b_discriminator_value_antitag_algo2.clear();
      
    //Clear the vector of objects to be used in the selection
    leptons.clear();
    leptonsQCD.clear();
    jets.clear();
    bjets.clear();
    antibjets.clear();
    
    //Define - initialize some variables
    MTWValue =0;
    metPx = metPxTmp; 
    metPy = metPyTmp;
    
    //position of lowest and highest b-tag used to chose the top candidate 
    int lowBTagTreePosition=-1;
    lowBTagTree = 99999;
    
    int highBTagTreePosition=-1;
    highBTagTree = -9999;

    int maxPtTreePosition=-1;
    maxPtTree = -99999;
    int minPtTreePosition=-1;
    minPtTree = 99999;

    //Taking the unclustered met previously evaluated 
    //and already present in the n-tuples
    //This is used for syst up and down
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
    
    
    //Define - initialize some variables
    float eta;
    float ptCorr;
    int flavour;
    double unc =0;
    
    //Loops to apply systematics on jets-leptons
    
    //Lepton loop
    nLeptons = leptonsPt->size();
    for(size_t i = 0;i<nLeptons;++i){
      
      float leptonRelIso = leptonsRelIso->at(i);
      lepRelIso = leptonRelIso;
      
      //Apply isolation cut
      if(leptonRelIso>RelIsoCut)continue;
      //if electron apply ID cuts
      if(leptonsFlavour_ == "electron"  ) {
	if(leptonsID->size()==0)cout<< "warning requiring ele id of collection which has no entries! Check the leptonsFlavour parameter "<<endl;
	float leptonID = leptonsID->at(i);
	//Legenda for eleId : 0 fail, 1 ID only, 2 iso Only, 3 ID iso only, 4 conv rej, 5 conv rej and ID, 6 conv rej and iso, 7 all 
	//Require Full ID selection
	if ((leptonID !=7)&&leptonID !=5)continue;
	electronID = leptonID;
	//This is to require conv rejection and ID but do not make requests on iso from id
	//	if (!(leptonID==5 || leptonID ==7))continue;
      }
      float leptonPt = leptonsPt->at(i);
      float leptonPhi = leptonsPhi->at(i);
      float leptonEta = leptonsEta->at(i);
      float leptonE = leptonsEnergy->at(i);
      //Build the lepton 4-momentum
      leptons.push_back(math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE));
  
     }

    //Jets loop
    for(size_t i = 0;i<nJets;++i){
      eta = jetsEta->at(i);
      ptCorr = jetsPt->at(i);
      flavour = jetsFlavour->at(i);

      //Pt cut
      bool passesPtCut = ptCorr>ptCut;
      
      //If systematics JES up/down we need to change the pt of the jet
      //consider if it passes the threshold or not
      if(passesPtCut && syst_name != "JESUp" && syst_name != "JESDown") jets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i),jetsEta->at(i),jetsPhi->at(i),jetsEnergy->at(i) ) ); 
      else if(syst_name == "JESUp"){
	unc = jetUncertainty( eta,  ptCorr, flavour);
	passesPtCut = ptCorr * (1+unc) >ptCut;
	metPx-=(jetsPt->at(i)*cos(jetsPhi->at(i)))*unc;
	metPy-=(jetsPt->at(i)*sin(jetsPhi->at(i)))*unc;
	if(passesPtCut) jets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i) * (1+unc),jetsEta->at(i), jetsPhi->at(i), jetsEnergy->at(i) * (1+unc)) ); 
      }
      else if(syst_name == "JESDown"){
	unc = jetUncertainty( eta,  ptCorr, flavour);
	passesPtCut = ptCorr * (1-unc) > ptCut;
	metPx-=-(jetsPt->at(i)*cos(jetsPhi->at(i)))*unc;
	metPy-=-(jetsPt->at(i)*sin(jetsPhi->at(i)))*unc;
	if(passesPtCut) jets.push_back(math::PtEtaPhiELorentzVector(jetsPt->at(i) * (1-unc),jetsEta->at(i),jetsPhi->at(i),jetsEnergy->at(i)*(1-unc)) ); 
      }
      //      if(passesPtCut) cout <<" jet "<< i <<" passes pt cut, flavour "<< abs(flavour)<< " syst " << syst_name << " pt "<< ptCorr<< " pt with unc "<< jets.back().pt() <<" unc "<< unc << endl;
      
      //Require the lepton jet DR > 0.3 veto
      if(passesPtCut && !leptons.empty()){
	for(size_t l =0;l<leptons.size();++l){
	  if( deltaR<math::PtEtaPhiELorentzVector,math::PtEtaPhiELorentzVector>(jets.back(),leptons.at(l))<0.3){
	    jets.pop_back();
	    continue;
	  }
	}
      }
      //b tag thresholds 
      
      double valueAlgo1 = jetsBTagAlgo->at(i);
      double valueAlgo2 = jetsAntiBTagAlgo->at(i);

      bool passesBTag = valueAlgo1  >bTagThreshold;
      bool passesAntiBTag = valueAlgo2 <1.7;

      if(!passesPtCut) continue;
      
      //max pt position:
      if(ptCorr > maxPtTree){ 
	maxPtTreePosition = jets.size()-1;
	maxPtTree = ptCorr;
      }
      //min pt position:
      if(ptCorr < minPtTree){ 
	minPtTreePosition = jets.size()-1;
	minPtTree = ptCorr;
      }
      
      //Passes firs algorythm (b tag requirement in the case of t-channel standard selection)
      if(passesBTag) {
	//Add to b-jet collection
	bjets.push_back(jets.back()); 

	//Apply different SFs if it is b,c or light jet
	if(abs(flavour)==4){ 
	  //Old prescriptions had a constant SF for 
	  //b(c) jets
	  BTagWeight*=TCHP_CTag ;
	  //Add to the weight vector
	  b_weight_tag_algo1.push_back(TCHP_CTag);
	}
	if(abs(flavour)==5){
	  BTagWeight*=TCHP_BTag ;
	  b_weight_tag_algo1.push_back(TCHP_BTag);
	}
	if((abs(flavour)<4 && abs(flavour)!=0) || abs(flavour) > 5 ){
	  
	  //Define the measure point coordinates in eta-phi
	  //to retrieve the value of scale factor and its error
	  double etaMin =  min(fabs(eta),(float)2.3999);
	  double ptMin =  min(jets.back().pt(),998.0);
	  
	  measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
	  measurePoint.insert(BinningVariables::JetEt,ptMin);
	  
	  double eff =(perfHP->getResult(PerformanceResult::BTAGLEFF,measurePoint));
	  double SF = (perfHP->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
	  double SFErr = (perfHP->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	  
	  //Function to retrieve the Mistag weight
	  //Given the DataBase SF and the systematics (up/down)
	  double mistagsf =  MisTagScaleFactor("TCHP_L",syst_name,SF,eff,SFErr); 
	  
	  //Apply mistag sf 
	  BTagWeight*= mistagsf;
	  b_weight_tag_algo1.push_back(mistagsf);
	  
	  measurePoint.reset();
	  //cout <<" jet "<< i <<" passes direct btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << " eff "<<  eff<<" SF "<< SF << " sf unc "<< SFErr <<endl;
	}
	//If no flavour was associated or it is a data sample, return b weight 1
	if(flavour == 0) b_weight_tag_algo1.push_back(1.); 
	//	cout <<" jet "<< i <<" passes direct btag, flavour "<< abs(flavour)<< " b weight " << b_weight_tag_algo1.back() << endl;
      }
      else{
	//If does not pass the b-trheshold:
	//Define a different weight 
	//To be use in case an explicit veto 
	//is required
	if(fabs(eta) > 2.5 && valueAlgo1 < -20) b_weight_antitag_algo1.push_back(1.);
	else{
	  if(abs(flavour)==4){ 
	    //Old prescriptions had a constant SF for 
	    //b(c) jets
	    //Notice that it requires the knowledge of b-efficiency on MC
	    //Taken from an AN, will be updated with new recipes
	    BTagWeight*=TCHP_AntiCTag ;
	    b_weight_antitag_algo1.push_back(TCHP_AntiCTag);
	  }
	  if(abs(flavour)==5){
	    BTagWeight*=TCHP_AntiBTag ;
	    b_weight_antitag_algo1.push_back(TCHP_AntiBTag);
	  }
	  if((abs(flavour)<4 && abs(flavour)!=0) || abs(flavour) > 5 ){
	    //Define the measure point coordinates in eta-phi
	    //to retrieve the value of scale factor and its error
	    
	    double etaMin =  min(fabs(eta),(float)2.3999);
	    double ptMin =  min(jets.back().pt(),998.0);
	    
	    measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
	    measurePoint.insert(BinningVariables::JetEt,ptMin);
	    
	    double eff =(perfHP->getResult(PerformanceResult::BTAGLEFF,measurePoint));
	    double SF = (perfHP->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
	    double SFErr = (perfHP->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	    
	    //Function to retrieve the Mistag weight
	    //Given the DataBase SF and the systematics (up/down)
	    double mistagsf =  AntiMisTagScaleFactor("TCHP_L",syst_name,SF,eff,SFErr); 
	    BTagWeight*= mistagsf;
	    b_weight_antitag_algo1.push_back(mistagsf);
	    
	    measurePoint.reset();
	    //	    cout <<" jet "<< i <<" passes direct anti btag algo1, flavour "<< abs(flavour)<< " b weight " << BTagWeight << " eff "<<  eff<<" SF "<< SF << " sf unc "<< SFErr <<endl;
	  }
	  //If no flavour was associated or it is a data sample, return b weight 1
	  if(flavour == 0) b_weight_antitag_algo1.push_back(1.); 
	  
	}
	if(passesAntiBTag){
	  //Same thing as previous b-tag loop
	  //with a different b-tagging algo: 
	  
	  //In this case the default t-channel selection
	  //requires a b-veto with a second algorythm,
	  //Therefore a vector of anti b-jets is produced
	  antibjets.push_back(jets.back());
	  //treat the case where the jet is anti-b tagged due to out-of-acceptance
	  if(fabs(eta) > 2.5 && valueAlgo2 < -20) b_weight_antitag_algo2.push_back(1.);
	  else{
	    //For the rest, it is a repeat the previous case
	    if(abs(flavour)==4) {
	      BTagWeight*=TCHE_AntiCTag ;
	      b_weight_antitag_algo2.push_back(TCHE_AntiCTag);
	    }
	    if(abs(flavour)==5){
	      BTagWeight*=TCHE_AntiBTag ;
	    b_weight_antitag_algo2.push_back(TCHE_AntiBTag);
	    }
	    if((abs(flavour)<4 && abs(flavour)!=0) || abs(flavour) > 5 ){
	      
	      double etaMin =  min(fabs(eta),(float)2.3999);
	      double ptMin =  min(jets.back().pt(),998.0);
	      
	      measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
	      measurePoint.insert(BinningVariables::JetEt,ptMin);
	      
	      double eff =(perfHE->getResult(PerformanceResult::BTAGLEFF,measurePoint));
	      double SF = (perfHE->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
	      double SFErr = (perfHE->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	      
	      double mistagsf =AntiMisTagScaleFactor("TCHE_L",syst_name,SF,eff,SFErr);
	      BTagWeight*=  mistagsf;
	      b_weight_antitag_algo2.push_back(mistagsf);
	      
	      measurePoint.reset();
	      //	  cout <<" jet "<< i <<" passes anti-btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << " eff "<<  eff<<" SF "<< SF << " sf unc "<< SFErr <<endl;
	    }
	    if(flavour == 0) b_weight_antitag_algo2.push_back(1.); 
	    //	    cout <<" jet "<< i <<" passes anti btag, flavour "<< abs(flavour)<< " b weight " << b_weight_antitag_algo2.back() << endl;
	  }
	}
	else{
	  //antibjets.push_back(jets.back());
	  if(abs(flavour)==4) {
	    BTagWeight*=TCHE_CTag ;
	    b_weight_tag_algo2.push_back(TCHE_CTag);
	  }
	  if(abs(flavour)==5){
	    BTagWeight*=TCHE_BTag ;
	    b_weight_tag_algo2.push_back(TCHE_BTag);
	  }
	  if((abs(flavour)<4 && abs(flavour)!=0) || abs(flavour) > 5 ){
	    
	    double etaMin =  min(fabs(eta),(float)2.3999);
	    double ptMin =  min(jets.back().pt(),998.0);
	    
	    measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
	    measurePoint.insert(BinningVariables::JetEt,ptMin);
	    
	    double eff =(perfHE->getResult(PerformanceResult::BTAGLEFF,measurePoint));
	    double SF = (perfHE->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
	    double SFErr = (perfHE->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	  
	    double mistagsf =AntiMisTagScaleFactor("TCHE_L",syst_name,SF,eff,SFErr);
	    BTagWeight*=  mistagsf;
	    b_weight_tag_algo2.push_back(mistagsf);
	    
	  measurePoint.reset();
	  //	  cout <<" jet "<< i <<" passes anti-btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << " eff "<<  eff<<" SF "<< SF << " sf unc "<< SFErr <<endl;
	  
	  }
	  if(flavour == 0) b_weight_tag_algo2.push_back(1.); 
	  //	cout <<" jet "<< i <<" passes anti btag, flavour "<< abs(flavour)<< " b weight " << BTagWeight << endl;
	}
      }
        
      //Condition to find the highest/lowest b-tag 
	//according to algo 1 (tchp) 
      //      cout << " test highLow " << endl;
      //cout << " i "<< i <<" jets size "<< jets.size()<< " btag  "<< 
      if(jetsBTagAlgo->at(i) > highBTagTree){
	  highBTagTree=jetsBTagAlgo->at(i);
	  highBTagTreePosition=jets.size()-1;
	  bJetFlavourTree = jetsFlavour->at(i);
	} 
	if(jetsBTagAlgo->at(i) < lowBTagTree){
	  lowBTagTree=jetsBTagAlgo->at(i);
	  lowBTagTreePosition=jets.size()-1;
	}
    }
    /////////
    ///End of the standard lepton-jet loop 
    /////////
    
    if( jets.size()<2 )continue;
    if( maxPtTree< maxPtCut )continue;
    
    //Loop for the qcd leptons
     if(doQCD_){
       for(size_t i = 0;i<nLeptons;++i){
	 
	 float leptonRelIso = leptonsRelIso->at(i);
	 
	 lepRelIso = leptonRelIso;
	 //Use an anti-isolation requirement
	 if(leptonRelIso < RelIsoQCDCut )continue;
	 
	 if(leptonsFlavour_ == "electron"  ) {
	   float leptonID = leptonsID->at(i);
	   electronID = leptonID;
	 } 

	 float leptonPt = leptonsPt->at(i);
	 float leptonPhi = leptonsPhi->at(i);
	 float leptonEta = leptonsEta->at(i);
	 float leptonE = leptonsEnergy->at(i);
	 //Create the lepton
	 leptonsQCD.push_back(math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE));
      }
     }
          
     //Part of the effective selection and filling
     
     //QCD Samples
     if (doQCD_){
       //      cout << " qcd lep size " <<leptonsQCD.size()<< " non qcd lep size" <<looseMuonsRelIso->size()+ looseElectronsRelIso->size()<< endl;
       if(leptonsQCD.size()==1 && jets.size()==2){
	 //MTW definition
	 metPt = sqrt(metPx*metPx+metPy*metPy);
	 MTWValueQCD =  sqrt((leptonsQCD.at(0).pt()+metPt)*(leptonsQCD.at(0).pt()+metPt)  -(leptonsQCD.at(0).px()+metPx)*(leptonsQCD.at(0).px()+metPx) -(leptonsQCD.at(0).py()+metPy)*(leptonsQCD.at(0).py()+metPy));
	 
	 //Signal QCD sample
	 if( bjets.size()==1 && antibjets.size()==1){
	   
	   //cout << " Signal Sample QCD; antib  algo 2 weight size "<< b_weight_antitag_algo2.size()<< " b algo 1 weight size "<< b_weight_tag_algo1.size()<< endl;
	   
	   math::PtEtaPhiELorentzVector top = top4Momentum(leptonsQCD.at(0),bjets.at(0),metPx,metPy);
	   float fCosThetaLJ =  cosThetaLJ(leptonsQCD.at(0), antibjets.at(0), top);
	   
	   runTree = iEvent.eventAuxiliary().run();
	   lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	   eventTree = iEvent.eventAuxiliary().event();
	   weightTree = Weight*b_weight_tag_algo1.at(0)*b_weight_antitag_algo2.at(0) ;
	   
	   etaTree = fabs(antibjets.at(0).eta());
	   cosTree = fCosThetaLJ;
	   topMassTree = top.mass();
	   mtwMassTree = MTWValueQCD;
	   chargeTree = leptonsCharge->at(0) ; 
	   
	   lepPt = leptonsQCD.at(0).pt();
	   lepEta = leptonsQCD.at(0).eta();
	   lepPhi = leptonsQCD.at(0).phi();
	   
	   bJetPt = bjets.at(0).pt();
	   bJetE = bjets.at(0).energy();
	   bJetEta = bjets.at(0).eta();
	   bJetPhi = bjets.at(0).phi();
	   
	   fJetPt = antibjets.at(0).pt();
	   fJetE = antibjets.at(0).energy();
	   fJetEta = antibjets.at(0).eta();
	   fJetPhi = antibjets.at(0).phi();
	   
	   topPt = top.pt();
	   topE = top.energy();
	   topEta = top.eta();
	   topPhi = top.phi();
	   
	   totalEnergy = (top+antibjets.at(0)).energy();
	   totalMomentum = (top+antibjets.at(0)).P();
	   
	   metPt = METPt->at(0);
	   metPhi = METPhi->at(0);
	   
	   
	   treesQCD[syst_name]->Fill();
	   
	   cout << " passes cuts pre-mtw qcd sample, syst " << syst_name << " top mass "<< top.mass() << " cosTheta* "<< fCosThetaLJ << " fjetEta " << fabs(antibjets.at(0).eta()) << " Weight "  << Weight << " B Weight "<<BTagWeight << " b weight 2 test"<< b_weight_tag_algo1.at(0)*b_weight_antitag_algo2.at(0)  <<endl;
	 }
	 
	 if( lowBTagTreePosition > -1 && highBTagTreePosition > -1) {
	   if(highBTagTreePosition != lowBTagTreePosition){
	     
	     	     
	     
	     //Sample B QCD:
	     if(doBScan_ && antibjets.size()==1 && bjets.size()==0){
	       for( size_t step = 1; step <= bScanSteps; ++step){
		
		 math::PtEtaPhiELorentzVector top = top4Momentum(leptonsQCD.at(0),jets.at(highBTagTreePosition),metPx,metPy);
		 float fCosThetaLJ =  cosThetaLJ(leptonsQCD.at(0),jets.at(lowBTagTreePosition),top);

		 //      cout << " passes cuts pre-mtw, syst " << syst_name << " b tag weight " <<  BTagWeight<< " Weight " << Weight  <<endl;
		 
		 //cout << " Sample B QCD; antib weight size algo 2 "<< b_weight_antitag_algo2.size()<< " b weight size algo 2 "<< b_weight_tag_algo2.size()<<  " antib weight size algo 1" << b_weight_antitag_algo1.size()<<endl;	
	 
		 
		 runTree = iEvent.eventAuxiliary().run();
		 lumiTree = iEvent.eventAuxiliary().luminosityBlock();
		 eventTree = iEvent.eventAuxiliary().event();
		 weightTree = Weight*b_weight_antitag_algo2.at(0)*b_weight_tag_algo2.at(0)*b_weight_antitag_algo1.at(0);
		 
		 etaTree = fabs(jets.at(lowBTagTreePosition).eta());
		 etaTree2 = fabs(jets.at(highBTagTreePosition).eta());
		 cosTree = fCosThetaLJ;
		 topMassTree = top.mass();
		 mtwMassTree = MTWValueQCD;
		 chargeTree = leptonsCharge->at(0);
		 treesScanQCD[step-1][syst_name]->Fill();            
	       }
	     }
	     
	     //WSample QCD
	     if (antibjets.size()==2 ){ 

	       int positionHigh = highBTagTreePosition;
	       int positionLow = lowBTagTreePosition;
	       
	       if(mode_=="pt"){
		 positionHigh = maxPtTreePosition;
		 positionLow = minPtTreePosition;
	       }
	       
	       math::PtEtaPhiELorentzVector top = top4Momentum(leptonsQCD.at(0),jets.at(positionHigh),metPx,metPy);
	       float fCosThetaLJ =  cosThetaLJ(leptonsQCD.at(0),jets.at(positionLow),top);
	       
	       runTree = iEvent.eventAuxiliary().run();
	       lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	       eventTree = iEvent.eventAuxiliary().event();
	       //weightTree = Weight * b_weight_antitag_algo2.at(0) * b_weight_antitag_algo2.at(1);
	       weightTree = Weight * b_weight_antitag_algo2.at(0) * b_weight_antitag_algo2.at(1);
       
	       cosTree = fCosThetaLJ;
	       topMassTree = top.mass();
	       mtwMassTree = MTWValue;
	       chargeTree = leptonsCharge->at(0);
	       
	       lepPt = leptonsQCD.at(0).pt();
	       lepEta = leptonsQCD.at(0).eta();
	       lepPhi = leptonsQCD.at(0).phi();

	       topPt = top.pt();
	       topE = top.energy();
	       topEta = top.eta();
	       topPhi = top.phi();
	       
	       
	       metPt = METPt->at(0);
	       metPhi = METPhi->at(0);
	       
	       topMassTree = top.mass();
	       mtwMassTree = MTWValueQCD;
	       
	       //Mode - dependent part


	       etaTree = fabs(jets.at(positionLow).eta());
	       etaTree2 = fabs(jets.at(positionHigh).eta());

	       bJetPt = jets.at(positionHigh).pt();
	       bJetE = jets.at(positionHigh).energy();
	       bJetEta = jets.at(positionHigh).eta();
	       bJetPhi = jets.at(positionHigh).phi();
	       
	       fJetPt = jets.at(positionLow).pt();
	       fJetE = jets.at(positionLow).energy();
	       fJetEta = jets.at(positionLow).eta();
	       fJetPhi = jets.at(positionLow).phi();
	       
	       totalEnergy = (top+jets.at(positionLow)).energy();
	       totalMomentum = (top+jets.at(positionLow)).P();
		 
	       //
	       
	       
	       
	       treesWSampleQCD[syst_name]->Fill();
	     }
	   }
	 }
       }
     }
     

     //Non-qcd lepton requirement:
     if(leptons.size()!=1)continue;
     
     //MTW Definition
     metPt = sqrt(metPx*metPx+metPy*metPy);
     MTWValue =  sqrt((leptons.at(0).pt()+metPt)*(leptons.at(0).pt()+metPt)  -(leptons.at(0).px()+metPx)*(leptons.at(0).px()+metPx) -(leptons.at(0).py()+metPy)*(leptons.at(0).py()+metPy));
     
     if( (leptons.size() == 1 && (looseMuonsRelIso->size( )+ looseElectronsRelIso->size())>1))   continue;//Loose muons size always >=1 due to presence of tight muon 
    
    //Signal sample
    if( jets.size()!=2)continue;
    
    if( bjets.size()==1 && antibjets.size()==1){
      
      math::PtEtaPhiELorentzVector top = top4Momentum(leptons.at(0),bjets.at(0),metPx,metPy);
      float fCosThetaLJ =  cosThetaLJ(leptons.at(0), antibjets.at(0), top);
      
      //cout << " Signal Sample QCD; antib  algo 2 weight size "<< b_weight_antitag_algo2.size()<< " b algo 1 weight size "<< b_weight_tag_algo1.size()<< endl;
      runTree = iEvent.eventAuxiliary().run();
      lumiTree = iEvent.eventAuxiliary().luminosityBlock();
      eventTree = iEvent.eventAuxiliary().event();
      weightTree = Weight*b_weight_tag_algo1.at(0)*b_weight_antitag_algo2.at(0) ;
      
      etaTree = fabs(antibjets.at(0).eta());
      cosTree = fCosThetaLJ;
      topMassTree = top.mass();
      mtwMassTree = MTWValue;
      chargeTree = leptonsCharge->at(0) ; 
      
      lepPt = leptons.at(0).pt();
      lepEta = leptons.at(0).eta();
      lepPhi = leptons.at(0).phi();
      
      bJetPt = bjets.at(0).pt();
      bJetE = bjets.at(0).energy();
      bJetEta = bjets.at(0).eta();
      bJetPhi = bjets.at(0).phi();

      fJetPt = antibjets.at(0).pt();
      fJetE = antibjets.at(0).energy();
      fJetEta = antibjets.at(0).eta();
      fJetPhi = antibjets.at(0).phi();

      topPt = top.pt();
      topE = top.energy();
      topEta = top.eta();
      topPhi = top.phi();
      
      totalEnergy = (top+antibjets.at(0)).energy();
      totalMomentum = (top+antibjets.at(0)).P();
      
      metPt = METPt->at(0);
      metPhi = METPhi->at(0);
      
      trees[syst_name]->Fill();

      //      cout << " b weight size " << b_weight_tag_algo1.size()<< " anti b weight size " <<  b_weight_antitag_algo2.size()<< endl;
      
      cout << " passes cuts pre-mtw, syst " << syst_name << " top mass "<< top.mass() << " cosTheta* "<< fCosThetaLJ << " fjetEta " << fabs(antibjets.at(0).eta()) << " Weight "  << Weight << " B Weight "<<BTagWeight << " b weight 2 test"<< b_weight_tag_algo1.at(0)*b_weight_antitag_algo2.at(0)  <<endl;
      
    }
     if( lowBTagTreePosition > -1 && highBTagTreePosition > -1 ){
      if(highBTagTreePosition != lowBTagTreePosition){
	
	//Sample B
	if(doBScan_ && antibjets.size()==1 && bjets.size()==0){
	  for( size_t step = 1; step <= bScanSteps; ++step){

	    math::PtEtaPhiELorentzVector top = top4Momentum(leptons.at(0),jets.at(highBTagTreePosition),metPx,metPy);
	    float fCosThetaLJ =  cosThetaLJ(leptons.at(0),jets.at(lowBTagTreePosition),top);
	
	    
	    //cout << " Sample B Iso; antib weight size algo 2 "<< b_weight_antitag_algo2.size()<< " b weight size algo 2 "<< b_weight_tag_algo2.size()<<  " antib weight size algo 1" << b_weight_antitag_algo1.size()<<endl;	

	    runTree = iEvent.eventAuxiliary().run();
	    lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	    eventTree = iEvent.eventAuxiliary().event();
	    weightTree = Weight*b_weight_antitag_algo2.at(0)*b_weight_tag_algo2.at(0)*b_weight_antitag_algo1.at(0);
	    
	    etaTree = fabs(jets.at(lowBTagTreePosition).eta());
	    etaTree2 = fabs(jets.at(highBTagTreePosition).eta());
	    cosTree = fCosThetaLJ;
	    topMassTree = top.mass();
	    mtwMassTree = MTWValue;

	    fJetPt = jets.at(lowBTagTreePosition).pt();
	    bJetPt = jets.at(highBTagTreePosition).pt();

	    chargeTree = leptonsCharge->at(0);
	    treesScan[step-1][syst_name]->Fill();            
	  }
	}
	//W Sample
	if (antibjets.size()==2 ){ 
	  //if(bjets.size()==0 ){
	  
	  //	  cout << "W Sample Iso; antib weight size algo 1 "<< b_weight_antitag_algo1.size() <<" number 0 "<< b_weight_antitag_algo1.at(0) <<" number 1 "<< b_weight_antitag_algo1.at(1) <<endl;
	  
	  int positionHigh = highBTagTreePosition;
	  int positionLow = lowBTagTreePosition;
	  
	  if(mode_=="pt"){
	    positionHigh = maxPtTreePosition;
	    positionLow = minPtTreePosition;
	  }
	  
	  math::PtEtaPhiELorentzVector top = top4Momentum(leptons.at(0),jets.at(positionHigh),metPx,metPy);
	  float fCosThetaLJ =  cosThetaLJ(leptons.at(0),jets.at(positionLow),top);
	  
	  runTree = iEvent.eventAuxiliary().run();
	  lumiTree = iEvent.eventAuxiliary().luminosityBlock();
	  eventTree = iEvent.eventAuxiliary().event();
	  weightTree = Weight*b_weight_antitag_algo2.at(0)*b_weight_antitag_algo2.at(1);
	  
	  cosTree = fCosThetaLJ;
	  topMassTree = top.mass();
	  mtwMassTree = MTWValue;
	  chargeTree = leptonsCharge->at(0);
	  
	  lepPt = leptons.at(0).pt();
	  lepEta = leptons.at(0).eta();
	  lepPhi = leptons.at(0).phi();


	  //Mode - dependent part

	  etaTree = fabs(jets.at(positionLow).eta());
	  etaTree2 = fabs(jets.at(positionHigh).eta());
	  
	  bJetPt = jets.at(positionHigh).pt();
	  bJetE = jets.at(positionHigh).energy();
	  bJetEta = jets.at(positionHigh).eta();
	  bJetPhi = jets.at(positionHigh).phi();
	  
	  fJetPt = jets.at(positionLow).pt();
	  fJetE = jets.at(positionLow).energy();
	  fJetEta = jets.at(positionLow).eta();
	  fJetPhi = jets.at(positionLow).phi();
	  
	  totalEnergy = (top+jets.at(positionLow)).energy();
	  totalMomentum = (top+jets.at(positionLow)).P();
	  //
	  
	  topPt = top.pt();
	  topE = top.energy();
	  topEta = top.eta();
	  topPhi = top.phi();
	  
	  
	  metPt = METPt->at(0);
	  metPhi = METPhi->at(0);
	  
	  treesWSample[syst_name]->Fill();
	}
      }
    }
  }
}
																					
//CosThetalj given top quark, lepton and light jet
float SingleTopSystematicsTreesDumper::cosThetaLJ(math::PtEtaPhiELorentzVector lepton, math::PtEtaPhiELorentzVector jet, math::PtEtaPhiELorentzVector top){
  
  math::PtEtaPhiELorentzVector boostedLepton = ROOT::Math::VectorUtil::boost(lepton,top.BoostToCM());
  math::PtEtaPhiELorentzVector boostedJet = ROOT::Math::VectorUtil::boost(jet,top.BoostToCM());

  return  ROOT::Math::VectorUtil::CosTheta(boostedJet.Vect(),boostedLepton.Vect());
  
}

//top quark 4-momentum given lepton, met and b-jet
math::PtEtaPhiELorentzVector SingleTopSystematicsTreesDumper::top4Momentum(math::PtEtaPhiELorentzVector lepton, math::PtEtaPhiELorentzVector jet, float metPx, float metPy){
  return top4Momentum(lepton.px(),lepton.py(),lepton.pz(),lepton.energy(),jet.px(),jet.py(),jet.pz(),jet.energy(),metPx,metPy);
}

//top quark 4-momentum original function given the necessary parameters 
math::PtEtaPhiELorentzVector SingleTopSystematicsTreesDumper::top4Momentum(float leptonPx, float leptonPy, float leptonPz, float leptonE, float jetPx, float jetPy, float jetPz,float jetE, float metPx, float metPy){
  float lepton_Pt = sqrt( (leptonPx*leptonPx)+  (leptonPy*leptonPy) );
  
  math::XYZTLorentzVector neutrino = NuMomentum(leptonPx,leptonPy,leptonPz,lepton_Pt,leptonE,metPx,metPy);//.at(0);;
    
  math::XYZTLorentzVector lep(leptonPx,leptonPy,leptonPz,leptonE);
  math::XYZTLorentzVector jet(jetPx,jetPy,jetPz,jetE);
  
  math::XYZTLorentzVector top = lep + jet + neutrino;
  return math::PtEtaPhiELorentzVector(top.pt(),top.eta(),top.phi(),top.E());  
}

//top neutrino 4-momentum function given the parameters
//In brief: 
//Works for top->1l+1neutrino+1bjet
//Assuming all met comes from neutrino
/////What it does:
//w boson mass put to pdg value
//obtained neutrino pz from kinematics
//We get a second order equation 
/////In case of two positive Delta solutions:
//we choose solution with minimum |pz|
/////In case of two negative Delta solutions:
//in such case: mtw > mw
//To solve this: put mtw = mw 
//Solve the equations
//In this way we must
//drop the constraints px_Nu = MET_x and py_Nu = MET_y
//Solve this by chosing the px_Nu and py_Nu that 
//minimize the distance from the MET in the px-py plane
//Such minimization can be done analytically with derivatives
//and much patience. Here we exploit such analytical minimization
/////
//More detailed inline description: work in progress! 
math::XYZTLorentzVector SingleTopSystematicsTreesDumper::NuMomentum(float leptonPx, float leptonPy, float leptonPz, float leptonPt, float leptonE, float metPx, float metPy ){

  double  mW = 80.399;
  
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

//JES uncertainty as a function of pt, eta and jet flavour 
double SingleTopSystematicsTreesDumper::jetUncertainty(double eta, double ptCorr, int flavour){
  jecUnc->setJetEta(eta); 
  jecUnc->setJetPt(ptCorr);
  double JetCorrection = jecUnc->getUncertainty(true); // In principle, boolean controls if uncertainty on +ve or -ve side is returned (asymmetric errors) but not yet implemented.
  bool cut = ptCorr> 50 && ptCorr < 200 && fabs(eta) < 2.0;
  // JES_SW = 0.015;                                                                                                                                 
  //  double JES_PU=0.75*0.8*2.2/ptCorr;
  double JES_PU=0.; //We are using pfNoPU must understand what value to put there
  double JES_b=0;
  if(abs(flavour)==5){
    if(cut) JES_b = JES_b_cut;
    else JES_b = JES_b_overCut;
  }
  //    float JESUncertaintyTmp = sqrt(JESUncertainty*JESUncertainty + JetCorrection*JetCorrection);                                                 
  return sqrt(JES_b*JES_b + JES_PU*JES_PU +JES_SW*JES_SW + JetCorrection*JetCorrection);
}

//EndJob filling rate systematics trees
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
  
//B-C weight as function of jet flavour, systematics and scale factors: 
//WILL BE CHANGED VERY SOON ACCORDING TO NEW PRESCRIPTIONS 
double SingleTopSystematicsTreesDumper::BScaleFactor(string algo,string syst_name){
  
  double bcentral =0.9;  
  double berr = 0.15*bcentral;
  double cerr =0.3*bcentral;
  double tcheeff =0.7;
  
  if(syst_name == "BTagUp"){
    if(algo == "TCHP_B"){
      return bcentral+berr;
    }
    if(algo == "TCHP_C"){
      return bcentral+cerr;
    }
    
    if(algo == "TCHE_B"){
      return bcentral+berr;
    }
    
    if(algo == "TCHE_C"){
      return bcentral+cerr;
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
      return bcentral-berr;
    }
    if(algo == "TCHE_C"){
      return bcentral-berr;
    }
  }

  if(algo == "TCHP_B"){
    return bcentral;
  }
  if(algo == "TCHP_C"){
    return bcentral;
  }
  if(algo == "TCHE_B"){
    return bcentral;
  }
  if(algo == "TCHE_C"){
    return bcentral;
  }
    
  return 0.9;
}

//Mistag weight as function of jet flavour, systematics and scale factors: 
//WILL BE CHANGED VERY SOON ACCORDING TO NEW PRESCRIPTIONS 
double SingleTopSystematicsTreesDumper::MisTagScaleFactor(string algo,string syst_name,double sf, double eff, double sferr){
  double mistagcentral = sf;  
  double mistagerr = sferr;
  double tcheeff = eff;

  
  if(syst_name == "MisTagUp"){
    if(algo == "TCHP_L"){
      return mistagcentral+mistagerr;
    }
    if(algo == "TCHE_L"){
      return mistagcentral+mistagerr;
    }
    
  }
  
  if(syst_name == "MisTagDown"){
    if(algo == "TCHP_L"){
      return mistagcentral-mistagerr;
    }
    if(algo == "TCHE_L"){
      return mistagcentral-mistagerr;
    }
  }

  if(algo == "TCHP_L"){
    return mistagcentral;
  }
  if(algo == "TCHE_L"){
    return mistagcentral;
  }
  
  return 0.9;


}



//B-C veto weight as function of jet flavour, systematics and scale factors: 
//WILL BE CHANGED VERY SOON ACCORDING TO NEW PRESCRIPTIONS 
double SingleTopSystematicsTreesDumper::AntiBScaleFactor(string algo,string syst_name){
  
  double bcentral =0.9;  
  double berr = 0.15*bcentral;
  double cerr =0.3*bcentral;
  double tcheeff =0.7;
  double tchpeff =0.26;
  
  if(syst_name == "BTagUp"){
    if(algo == "TCHP_B"){
      return (1-tchpeff*(bcentral+berr))/(1-tchpeff);
    }
    if(algo == "TCHP_C"){
      return (1-tchpeff*(bcentral+cerr))/(1-tchpeff);
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
      return (1-tchpeff*(bcentral-berr))/(1-tchpeff);
    }
    if(algo == "TCHP_C"){
      return (1-tchpeff*(bcentral-cerr))/(1-tchpeff);
    }
  
    if(algo == "TCHE_B"){
      return (1-tcheeff*(bcentral-berr))/(1-tcheeff);
    }
    if(algo == "TCHE_C"){
      return (1-tcheeff*(bcentral-cerr))/(1-tcheeff);
    }
  }

  if(algo == "TCHP_B"){
    return (1-tchpeff*(bcentral))/(1-tchpeff);
  }
  if(algo == "TCHP_C"){
    return (1-tchpeff*(bcentral))/(1-tchpeff);
  }
  if(algo == "TCHE_B"){
    return (1-tcheeff*(bcentral))/(1-tcheeff);
  }
  if(algo == "TCHE_C"){
    return (1-tcheeff*(bcentral))/(1-tcheeff);
  }
    
  return 0.9;
}

//MisTag veto weight as function of jet flavour, systematics and scale factors: 
//WILL BE CHANGED VERY SOON ACCORDING TO NEW PRESCRIPTIONS 
double SingleTopSystematicsTreesDumper::AntiMisTagScaleFactor(string algo,string syst_name,double sf, double eff, double sferr){
  double mistagcentral = sf;  
  double mistagerr = sferr;
  double tcheeff = eff;
  double tchpeff =eff;
  
  if(syst_name == "MisTagUp"){
    if(algo == "TCHP_L"){
      return (1-tchpeff)/(1-tchpeff/(mistagcentral+mistagerr));
      }
    if(algo == "TCHE_L"){
      return (1-tcheeff)/(1-tcheeff/(mistagcentral+mistagerr));
    }
    
  }
  
  if(syst_name == "MisTagDown"){
    if(algo == "TCHP_L"){
      return (1-tchpeff)/(1-tchpeff/(mistagcentral-mistagerr));
    }
    if(algo == "TCHE_L"){
      return (1-tcheeff)/(1-tcheeff/(mistagcentral-mistagerr));
    }
  }

  if(algo == "TCHP_L"){
    return (1-tchpeff)/(1-tchpeff/(mistagcentral));
  }
  if(algo == "TCHE_L"){
    return (1-tcheeff)/(1-tcheeff/(mistagcentral));
  }
  
  return 0.9;


}





//define this as a plug-in
DEFINE_FWK_MODULE(SingleTopSystematicsTreesDumper);
