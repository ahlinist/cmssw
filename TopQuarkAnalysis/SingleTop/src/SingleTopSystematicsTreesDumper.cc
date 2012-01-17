/*
*\Author:  O.Iorio
*
*
*
*\version  $Id: SingleTopSystematicsTreesDumper.cc,v 1.13 2011/07/04 18:24:25 oiorio Exp $ 
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

  //tight leptons 
  leptonsFlavour_ =  iConfig.getUntrackedParameter< std::string >("leptonsFlavour");



  //  dataPUFile_ =  iConfig.getUntrackedParameter< std::string >("dataPUFile","pileUpDistr.root");
  //  mcPUFile_ =  iConfig.getUntrackedParameter< std::string >("mcPUFile","pileupdistr_TChannel.root");
  
  leptonsPt_ =  iConfig.getParameter< edm::InputTag >("leptonsPt");
  leptonsPhi_ =  iConfig.getParameter< edm::InputTag >("leptonsPhi");
  leptonsEta_ =  iConfig.getParameter< edm::InputTag >("leptonsEta");
  leptonsEnergy_ =  iConfig.getParameter< edm::InputTag >("leptonsEnergy");
  leptonsCharge_ =  iConfig.getParameter< edm::InputTag >("leptonsCharge");
  leptonsRelIso_ =  iConfig.getParameter< edm::InputTag >("leptonsRelIso");
  leptonsDB_ =  iConfig.getParameter< edm::InputTag >("leptonsDB");
  leptonsID_ =  iConfig.getParameter< edm::InputTag >("leptonsID");

  //qcd leptons

  qcdLeptonsPt_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsPt");
  qcdLeptonsPhi_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsPhi");
  qcdLeptonsEta_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsEta");
  qcdLeptonsEnergy_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsEnergy");
  qcdLeptonsCharge_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsCharge");
  qcdLeptonsRelIso_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsRelIso");
  qcdLeptonsDB_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsDB");
  qcdLeptonsID_ =  iConfig.getParameter< edm::InputTag >("qcdLeptonsID");



  leptonsFlavour_ =  iConfig.getUntrackedParameter< std::string >("leptonsFlavour");
  
  looseMuonsRelIso_ =  iConfig.getParameter< edm::InputTag >("looseMuonsRelIso");
  looseElectronsRelIso_ =  iConfig.getParameter< edm::InputTag >("looseElectronsRelIso");

  //Jets
  
  jetsEta_ =  iConfig.getParameter< edm::InputTag >("jetsEta");
  jetsPt_ =  iConfig.getParameter< edm::InputTag >("jetsPt");
  jetsPhi_ =  iConfig.getParameter< edm::InputTag >("jetsPhi");
  jetsEnergy_ =  iConfig.getParameter< edm::InputTag >("jetsEnergy");
  
  jetsBTagAlgo_ =  iConfig.getParameter< edm::InputTag >("jetsBTagAlgo");
  jetsAntiBTagAlgo_ =  iConfig.getParameter< edm::InputTag >("jetsAntiBTagAlgo");
  jetsFlavour_ =  iConfig.getParameter< edm::InputTag >("jetsFlavour");

  genJetsPt_  = iConfig.getParameter< edm::InputTag >("genJetsPt");
  genJetsEta_  = iConfig.getParameter< edm::InputTag >("genJetsEta");


  METPhi_ =  iConfig.getParameter< edm::InputTag >("METPhi");
  METPt_ =  iConfig.getParameter< edm::InputTag >("METPt");
  
  //  UnclMETPx_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPx");
  //  UnclMETPy_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPy");
  UnclMETPx_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPx");
  UnclMETPy_ =  iConfig.getParameter< edm::InputTag >("UnclusteredMETPy");
  
  
  jetsCorrTotal_ =  iConfig.getParameter< edm::InputTag >("jetsCorrTotal");

  doBScan_  =  iConfig.getUntrackedParameter< bool >("doBScan",false); 
  doQCD_  =  iConfig.getUntrackedParameter< bool >("doQCD",true); 
  
  //Q2 part
  x1_ = iConfig.getParameter<edm::InputTag>("x1") ;
  x2_ = iConfig.getParameter<edm::InputTag>("x2") ;
  
  //Pile Up Part
  np1_ = iConfig.getParameter< edm::InputTag >("nVerticesPlus");//,"PileUpSync"); 
  nm1_ = iConfig.getParameter< edm::InputTag >("nVerticesMinus");//,"PileUpSync"); 
  n0_ = iConfig.getParameter< edm::InputTag >("nVertices");//,"PileUpSync"); 
  
  doPU_ = iConfig.getUntrackedParameter< bool >("doPU",false);
  doResol_ = iConfig.getUntrackedParameter< bool >("doResol",false);
  doTurnOn_ = iConfig.getUntrackedParameter< bool >("doTurnOn",true);

  doReCorrection_ = iConfig.getUntrackedParameter< bool >("doReCorrection",false);
  dataPUFile_ =  iConfig.getUntrackedParameter< std::string >("dataPUFile","pileUpDistr.root");
  
  if(doPU_){
    //    cout << " before lumIweightse "<<endl;
    LumiWeights_ = edm::Lumi3DReWeighting(mcPUFile_,
					dataPUFile_,
					puHistoName_,
					std::string("pileup") );
    //    cout << " built lumiWeights "<<endl;
  }
  
  //  preWeights_ =  iConfig.getParameter< edm::InputTag >("preWeights");
  
  systematics.push_back("noSyst");
  
  Service<TFileService> fs;
  

  bTagThreshold =3.41;

  TFileDirectory SingleTopSystematics = fs->mkdir( "systematics_histograms" );
  TFileDirectory SingleTopTrees = fs->mkdir( "systematics_trees" );
  
  std::vector<std::string> all_syst = systematics;
   
  for(size_t i = 0; i < rate_systematics.size();++i){
    all_syst.push_back(rate_systematics.at(i));  
  }

  all_syst.push_back("noSyst");
  

  for(size_t i = 0; i < all_syst.size();++i){
    
    string syst = all_syst[i];
    
    for(int bj = 0; bj<=5;++bj){

      stringstream tags;
      int ntagss = bj;
      if(ntagss >2 ){
	
	ntagss= ntagss-3;
	tags<<ntagss;
	tags <<  "T_QCD";
	  }
      else{
	tags<< ntagss<<"T";
      }
      //2J1T
      
      string treename = (channel+"_2J_"+tags.str()+"_"+syst);
      
      trees2J[bj][syst] = new TTree(treename.c_str(),treename.c_str()); 
      
      //basic quantities
      
      trees2J[bj][syst]->Branch("eta",&etaTree);
      trees2J[bj][syst]->Branch("costhetalj",&cosTree);
      trees2J[bj][syst]->Branch("topMass",&topMassTree);
      trees2J[bj][syst]->Branch("mtwMass",&mtwMassTree);
      
      trees2J[bj][syst]->Branch("charge",&chargeTree);
      trees2J[bj][syst]->Branch("runid",&runTree);
      trees2J[bj][syst]->Branch("lumiid",&lumiTree);
      trees2J[bj][syst]->Branch("eventid",&eventTree);
      trees2J[bj][syst]->Branch("weight",&weightTree);
      
      trees2J[bj][syst]->Branch("bWeight",&bWeightTree);
      trees2J[bj][syst]->Branch("PUWeight",&PUWeightTree);
      trees2J[bj][syst]->Branch("turnOnWeight",&turnOnWeightTree);
      
      //Extra info
      
      trees2J[bj][syst]->Branch("leptonPt",&lepPt);
      trees2J[bj][syst]->Branch("leptonEta",&lepEta);
      trees2J[bj][syst]->Branch("leptonPhi",&lepPhi);
      trees2J[bj][syst]->Branch("leptonRelIso",&lepRelIso);
      
      trees2J[bj][syst]->Branch("fJetPt",&fJetPt);
      trees2J[bj][syst]->Branch("fJetE",&fJetE);
      trees2J[bj][syst]->Branch("fJetEta",&fJetEta);
      trees2J[bj][syst]->Branch("fJetPhi",&fJetPhi);
      trees2J[bj][syst]->Branch("fJetBtag",&fJetBTag);
      
      trees2J[bj][syst]->Branch("bJetPt",&bJetPt);
      trees2J[bj][syst]->Branch("bJetE",&bJetE);
      trees2J[bj][syst]->Branch("bJetEta",&bJetEta);
      trees2J[bj][syst]->Branch("bJetPhi",&bJetPhi);
      trees2J[bj][syst]->Branch("fJetBtag",&bJetBTag);
      trees2J[bj][syst]->Branch("bJetFlavour",&bJetFlavourTree);
      
      trees2J[bj][syst]->Branch("metPt",&metPt);
      trees2J[bj][syst]->Branch("metPhi",&metPhi);
      
      trees2J[bj][syst]->Branch("topPt",&topPt);
      trees2J[bj][syst]->Branch("topPhi",&topPhi);
      trees2J[bj][syst]->Branch("topEta",&topEta);
      trees2J[bj][syst]->Branch("topE",&topE);
      
      trees2J[bj][syst]->Branch("ID",&electronID);
      trees2J[bj][syst]->Branch("nVertices",&nVertices);
      
      trees2J[bj][syst]->Branch("totalEnergy",&totalEnergy);
      trees2J[bj][syst]->Branch("totalMomentum",&totalMomentum);
      
      trees2J[bj][syst]->Branch("lowBTag",&lowBTagTree);
      trees2J[bj][syst]->Branch("highBTag",&highBTagTree);


      treename = (channel+"_3J_"+tags.str()+"_"+syst);
      
      trees3J[bj][syst] = new TTree(treename.c_str(),treename.c_str()); 
      
      //basic quantities
      
      trees3J[bj][syst]->Branch("eta",&etaTree);
      trees3J[bj][syst]->Branch("costhetalj",&cosTree);
      trees3J[bj][syst]->Branch("topMass",&topMassTree);
      trees3J[bj][syst]->Branch("mtwMass",&mtwMassTree);
      
      trees3J[bj][syst]->Branch("charge",&chargeTree);
      trees3J[bj][syst]->Branch("runid",&runTree);
      trees3J[bj][syst]->Branch("lumiid",&lumiTree);
      trees3J[bj][syst]->Branch("eventid",&eventTree);
      trees3J[bj][syst]->Branch("weight",&weightTree);
      
      trees3J[bj][syst]->Branch("bWeight",&bWeightTree);
      trees3J[bj][syst]->Branch("PUWeight",&PUWeightTree);
      trees3J[bj][syst]->Branch("turnOnWeight",&turnOnWeightTree);
      
      //Extra info
      
      trees3J[bj][syst]->Branch("leptonPt",&lepPt);
      trees3J[bj][syst]->Branch("leptonEta",&lepEta);
      trees3J[bj][syst]->Branch("leptonPhi",&lepPhi);
      trees3J[bj][syst]->Branch("leptonRelIso",&lepRelIso);
      
      trees3J[bj][syst]->Branch("fJetPt",&fJetPt);
      trees3J[bj][syst]->Branch("fJetE",&fJetE);
      trees3J[bj][syst]->Branch("fJetEta",&fJetEta);
      trees3J[bj][syst]->Branch("fJetPhi",&fJetPhi);
      trees3J[bj][syst]->Branch("fJetBtag",&fJetBTag);
      
      trees3J[bj][syst]->Branch("bJetPt",&bJetPt);
      trees3J[bj][syst]->Branch("bJetE",&bJetE);
      trees3J[bj][syst]->Branch("bJetEta",&bJetEta);
      trees3J[bj][syst]->Branch("bJetPhi",&bJetPhi);
      trees3J[bj][syst]->Branch("fJetBtag",&bJetBTag);
      trees3J[bj][syst]->Branch("bJetFlavour",&bJetFlavourTree);
      
      trees3J[bj][syst]->Branch("metPt",&metPt);
      trees3J[bj][syst]->Branch("metPhi",&metPhi);
      
      trees3J[bj][syst]->Branch("topPt",&topPt);
      trees3J[bj][syst]->Branch("topPhi",&topPhi);
      trees3J[bj][syst]->Branch("topEta",&topEta);
      trees3J[bj][syst]->Branch("topE",&topE);
      
      trees3J[bj][syst]->Branch("ID",&electronID);
      trees3J[bj][syst]->Branch("nVertices",&nVertices);
      
      trees3J[bj][syst]->Branch("totalEnergy",&totalEnergy);
      trees3J[bj][syst]->Branch("totalMomentum",&totalMomentum);
      
      trees3J[bj][syst]->Branch("lowBTag",&lowBTagTree);
      trees3J[bj][syst]->Branch("highBTag",&highBTagTree);


    }
  
  }  

  passingLepton=0;
  passingJets=0;
  passingBJets=0;
  passingMET=0;

  b_tchpt_signal_region = BTagWeight(1,1);
  b_tchel_sample_A = BTagWeight(0,0);
  b_tchel_sample_B = BTagWeight(1,1);
  b_tchpt_sample_B = BTagWeight(0,0);

  b_tchpm_sample_ESB = BTagWeight(1,2);
  b_tchpt_sample_ESB = BTagWeight(0,0);

  //  JEC_PATH = "CondFormats/JetMETObjects/data/";
  //  JEC_PATH = "./JECs/";
  JEC_PATH = "./";
  //  fip = edm::FileInPath(JEC_PATH+"Spring10_Uncertainty_AK5PF.txt");
  //fip = edm::FileInPath(JEC_PATH+"GR_R_42_V19_AK5PF_Uncertainty.txt");
  //jecUnc = new JetCorrectionUncertainty(fip.fullPath());
  jecUnc  = new JetCorrectionUncertainty(JEC_PATH+"GR_R_42_V19_AK5PF_Uncertainty.txt");
  JES_SW = 0.015;
  JES_b_cut = 0.02;
  JES_b_overCut = 0.03;
  

  //JetResolution part
  string fileResolName = "Spring10_PtResolution_AK5PF.txt";
  bool  doGaussianResol = false;
  //  ptResol = new JetResolution(fileResolName, doGaussianResol);  
  
  leptonRelIsoQCDCutUpper = 0.5,leptonRelIsoQCDCutLower=0.3;  


  topMassMeas = 172.9;
  doReCorrection_= false;  
  /*  if(doReCorrection_){//FIXME CURRENTLY NOT WORKING!!!
  cout << "jec 1" << endl;
  JetCorrectorParameters *ResJetParData = new JetCorrectorParameters(JEC_PATH+"GR_R_42_V19_AK5PF_L2L3Residual.txt"); 
  cout << "jec 1A" << endl;
  JetCorrectorParameters *L3JetParData  = new JetCorrectorParameters(JEC_PATH+"GR_R_42_V19_AK5PF_L3Absolute.txt");
  JetCorrectorParameters *L2JetParData  = new JetCorrectorParameters(JEC_PATH+"GR_R_42_V19_AK5PF_L2Relative.txt");
  JetCorrectorParameters *L1JetParData  = new JetCorrectorParameters(JEC_PATH+"GR_R_42_V19_AK5PF_L1FastJet.txt");
  cout << "jec 1B" << endl;
  vParData->push_back(*L1JetParData);
  cout << "jec 1C" << endl;
  vParData->push_back(*L2JetParData);
  vParData->push_back(*L3JetParData);
  vParData->push_back(*ResJetParData);

  cout << "jec 2" << endl;
  JetCorrectorParameters *L3JetParMC = new JetCorrectorParameters(JEC_PATH+"START42_V13::All_AK5PF_L3Absolute.txt");
  JetCorrectorParameters *L2JetParMC  = new JetCorrectorParameters(JEC_PATH+"START42_V13::All_AK5PF_L2Relative.txt");
  JetCorrectorParameters *L1JetParMC  = new JetCorrectorParameters(JEC_PATH+"START42_V13::All_AK5PF_L1FastJet.txt");
   vParMC->push_back(*L1JetParMC);
   vParMC->push_back(*L2JetParMC);
   vParMC->push_back(*L3JetParMC);
   
  cout << "jec 3" << endl;
   JetCorrectorData = new FactorizedJetCorrector(*vParData);
   JetCorrectorMC = new FactorizedJetCorrector(*vParMC);
   }*/
   InitializeEventScaleFactorMap();
  //  cout<< "I work for now but I do nothing. But again, if you gotta do nothing, you better do it right. To prove my good will I will provide you with somse numbers later."<<endl;
   isFirstEvent = true;
}

void SingleTopSystematicsTreesDumper::analyze(const Event& iEvent, const EventSetup& iSetup)
{

  cout <<" test 1 "<<endl;
  iEvent.getByLabel(jetsEta_,jetsEta);
  iEvent.getByLabel(jetsPt_,jetsPt);
  //  if(jetsPt->size() < 2)return; 
  if(jetsPt->size() > 25 && channel != "Data")return;  //Crazy events with huge jet multiplicity in mc
  iEvent.getByLabel(jetsPhi_,jetsPhi);

  if(isFirstEvent){
    cout <<  "isfirst " << endl;
    iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHPT",perfMHP);
    iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHPM",perfMHPM);
    iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHEL",perfMHE);
    
    iSetup.get<BTagPerformanceRecord>().get("BTAGTCHPM",perfBHPM);
    iSetup.get<BTagPerformanceRecord>().get("BTAGTCHPT",perfBHP);
    iSetup.get<BTagPerformanceRecord>().get("BTAGTCHEL",perfBHE);
    isFirstEvent = false;
  }
  //  iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHPT",perfHP);
  // iSetup.get<BTagPerformanceRecord>().get("MISTAGTCHEL",perfHE);

  //  cout << "test 0 "<<endl;

  gotLeptons=0;
  gotQCDLeptons=0;
  gotLooseLeptons=0;
  gotJets=0;
  gotMets=0;
  gotPU=0;

  cout <<" test 2 "<<endl;

  bool passesSignal=false,passesQCDSignal=false,
    passesSampleA=false,passesSampleB=false,
    passesQCDSampleA=false,passesQCDSampleB= false,
    passesESBQCD = false, passesESB = false;    

  //bool isControlSample_ = false; 

  //  iEvent.getByLabel(leptonsQCDRelIso_,leptonsQCDRelIso);
  /*=======
  iEvent.getByLabel(jetsEnergy_,jetsEnergy);
  iEvent.getByLabel(jetsBTagAlgo_,jetsBTagAlgo);
  iEvent.getByLabel(jetsAntiBTagAlgo_,jetsAntiBTagAlgo);
  iEvent.getByLabel(jetsFlavour_,jetsFlavour);
  iEvent.getByLabel(jetsCorrTotal_,jetsCorrTotal);
  
  iEvent.getByLabel(leptonsEta_,leptonsEta);
  iEvent.getByLabel(leptonsPt_,leptonsPt);
  iEvent.getByLabel(leptonsPhi_,leptonsPhi);
  iEvent.getByLabel(leptonsEnergy_,leptonsEnergy);
  iEvent.getByLabel(leptonsCharge_,leptonsCharge);
  >>>>>>> 1.13*/
  
  iEvent.getByLabel(METPhi_,METPhi);
  iEvent.getByLabel(METPt_,METPt);
  iEvent.getByLabel(leptonsRelIso_,leptonsRelIso);
  //  iEvent.getByLabel(leptonsID_,leptonsID);
  
  //  iEvent.getByLabel(looseElectronsRelIso_,looseElectronsRelIso);
  //  iEvent.getByLabel(looseMuonsRelIso_,looseMuonsRelIso);     
  
  

  double PUWeight =1;

  BinningPointByMap measurePoint;
  
  float metPx = 0; 
  float metPy = 0;
  
  metPx = METPt->at(0)*cos(METPhi->at(0));
  metPy = METPt->at(0)*sin(METPhi->at(0));

  float metPxTmp = metPx; 
  float metPyTmp = metPy;

  size_t nLeptons = 0;//leptonsPt->size();
  size_t nQCDLeptons = 0;//leptonsPt->size();
  size_t nJets = 0;
  size_t nBJets = 0;
  size_t nAntiBJets = 0;

  
  double WeightLumi = finalLumi*crossSection/originalEvents;
  double Weight = 1;
  double MTWValue =0;
  double MTWValueQCD =0;
  double RelIsoQCDCut = 0.1;
  
  float ptCut = 30;  
  //  float maxPtCut = maxPtCut_;

  double myWeight = 1.;

  /*  if(channel != "Data"){
    iEvent.getByLabel(npv_,npv);
    myWeight = LumiWeights_.weight(*npv);
  }*/

  //edm::EventBase* const iEventB = dynamic_cast<edm::EventBase*>(&iEvent);
  //double MyWeight = LumiWeights_.weight( (*iEventB) );
  
  //double w = LumiWeights_.weight(iEvent);
  
  if(channel=="Data")WeightLumi=1;
  
  cout <<" test 3 "<<endl;

  for(size_t s = 0; s < systematics.size();++s){
    string syst_name =  systematics.at(s);
    string syst = syst_name;  
    //    cout <<" syst "<<  syst_name<< " nlept " << nLeptons<<endl;
    nLeptons =0;
    nQCDLeptons =0;
    nJets =0;
    nBJets =0;
    nAntiBJets =0;


    //Here the weight of the event is the weight
    //to normalize the sample to the luminosity 
    //required in the cfg
    Weight = WeightLumi;
    //    Weight *= PUWeight;
    

    bool is_btag_relevant = ((syst_name=="noSyst" || syst_name == "BTagUp" || syst_name == "BTagDown" 
			           || syst_name == "MisTagUp" || syst_name == "MisTagDown"
			      || syst_name == "JESUp" || syst_name == "JESDown" ) && channel != "Data"
			     );


    //    cout << "test 02 systs "<< syst_name <<endl;
    //Setup for systematics

    //This is done according to old b-tagging prescriptions
    //Where the b(c) jet Scale Factors were taken 0.9 +-15%(30%)
    //Will have to be updated
    

    //Here we have vectors of weights 
    //to be associated with the 
    //b-jets selection in the sample according to algorythm X: 
    //a b-tag requirement implies a b_weight_tag_algoX,
    //a b-veto requirement implies a b_weight_antitag_algoX
 
    
    b_weight_signal_region =1; b_weight_sample_A=1; b_weight_sample_B=1; 
   
    nb =0;
    nc =0;
    nudsg =0;

    //Clear the vector of objects to be used in the selection
    
    //Define - initialize some variables
    MTWValue =0;
    
    
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
    //     leptonsRelIso->size()
  cout <<" test 4 "<<endl;

    for(size_t i = 0;i < leptonsRelIso->size();++i){
      //      cout << "test 03 systs "<< syst_name <<" leps "<< nLeptons << " loop pos" <<  i<<endl;
      float leptonRelIso = leptonsRelIso->at(i);
      lepRelIso = leptonRelIso;
      
      //Apply isolation cut
      if(leptonRelIso>RelIsoCut)continue;
      if(!gotLeptons){
	iEvent.getByLabel(leptonsEta_,leptonsEta);
	iEvent.getByLabel(leptonsPt_,leptonsPt);
	iEvent.getByLabel(leptonsPhi_,leptonsPhi);
	iEvent.getByLabel(leptonsEnergy_,leptonsEnergy);
	iEvent.getByLabel(leptonsCharge_,leptonsCharge);
	iEvent.getByLabel(leptonsID_,leptonsID);
	iEvent.getByLabel(leptonsDB_,leptonsDB);
	gotLeptons=true;
      }
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
      float leptonDB = leptonsDB->at(i);
      if ( fabs(leptonDB) >0.02) continue;
      
     lepRelIso = leptonRelIso;
      
      float leptonPt = leptonsPt->at(i);
      float leptonPhi = leptonsPhi->at(i);
      float leptonEta = leptonsEta->at(i);
      float leptonE = leptonsEnergy->at(i);
      //Build the lepton 4-momentum
      ++nLeptons;

      leptons[nLeptons-1]=math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE);
      //  leptons.push_back(math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE));
      if(nLeptons >= 3) break;      
    }
        
    bool passesLeptons = (nLeptons ==1);
    if(passesLeptons){
      iEvent.getByLabel(looseMuonsRelIso_,looseMuonsRelIso);
      iEvent.getByLabel(looseElectronsRelIso_,looseElectronsRelIso);
      bool passesLooseLeptons = (looseMuonsRelIso->size()+looseElectronsRelIso->size())==1;
      passesLeptons = passesLeptons && passesLooseLeptons;
    }
    if(passesLeptons && syst=="noSyst")++passingLepton;

    bool isQCD = (!passesLeptons);

    //   cout <<" syst "<<  syst_name<< " nlept " << nLeptons << " passesLept "<< passesLeptons<< " passes QCD1 "<<isQCD<< endl;
    //  cout << "test 03 "<<endl;
    //Loop for the qcd leptons
    if(doQCD_ && isQCD){
      iEvent.getByLabel(qcdLeptonsRelIso_,qcdLeptonsRelIso);
      for(size_t i = 0;i<qcdLeptonsRelIso->size();++i){
	
	float leptonRelIso = qcdLeptonsRelIso->at(i);
	float leptonQCDRelIso = leptonRelIso;
	//Use an anti-isolation requirement
	
	if(leptonsFlavour_ == "muon"){
	  if( leptonQCDRelIso > leptonRelIsoQCDCutUpper )continue;
	  if( leptonQCDRelIso < leptonRelIsoQCDCutLower )continue;
	  
	  if(!gotQCDLeptons){
	    iEvent.getByLabel(qcdLeptonsEta_,qcdLeptonsEta);
	    iEvent.getByLabel(qcdLeptonsPt_,qcdLeptonsPt);
	    iEvent.getByLabel(qcdLeptonsPhi_,qcdLeptonsPhi);
	    iEvent.getByLabel(qcdLeptonsEnergy_,qcdLeptonsEnergy);
	    iEvent.getByLabel(qcdLeptonsCharge_,qcdLeptonsCharge);
	    iEvent.getByLabel(qcdLeptonsID_,qcdLeptonsID);
	    

	    iEvent.getByLabel(qcdLeptonsDB_,qcdLeptonsDB);
	    gotQCDLeptons=true;
	  }
	 
	}
	
	if(leptonsFlavour_ == "electron"  ) {
	  bool QCDCondition = false;
	  iEvent.getByLabel(qcdLeptonsID_,qcdLeptonsID);
	  iEvent.getByLabel(qcdLeptonsDB_,qcdLeptonsDB);
	  float leptonID = qcdLeptonsID->at(i);
	  float beamspot  = abs(qcdLeptonsDB->at(i));
	  bool isid =	(leptonID ==  1 || leptonID == 3 || leptonID == 5 || leptonID == 7);
	  //Legenda for eleId : 0 fail, 1 ID only, 2 iso Only, 3 ID iso only, 4 conv rej, 5 conv rej and ID, 6 conv rej and iso, 7 all 
	  QCDCondition = (!(leptonRelIso < 0.1) && !(beamspot<0.02))  || (!(leptonRelIso<0.1) && !isid) ||(!isid && !(beamspot<0.02));
	  electronID = leptonID;
	  
	  if(!QCDCondition) continue;
	  if(!gotQCDLeptons){
	    iEvent.getByLabel(qcdLeptonsEta_,qcdLeptonsEta);
	    iEvent.getByLabel(qcdLeptonsPt_,qcdLeptonsPt);
	    iEvent.getByLabel(qcdLeptonsPhi_,qcdLeptonsPhi);
	    iEvent.getByLabel(qcdLeptonsEnergy_,qcdLeptonsEnergy);
	    iEvent.getByLabel(qcdLeptonsCharge_,qcdLeptonsCharge);
	    gotQCDLeptons=true;
	  }
	} 
	
	lepRelIso = leptonRelIso;
	
	float qcdLeptonPt = qcdLeptonsPt->at(i);
	float qcdLeptonPhi = qcdLeptonsPhi->at(i);
	float qcdLeptonEta = qcdLeptonsEta->at(i);
	float qcdLeptonE = qcdLeptonsEnergy->at(i);
	//Create the lepton
	++nQCDLeptons;
	
	qcdLeptons[nQCDLeptons-1]=math::PtEtaPhiELorentzVector(qcdLeptonPt,qcdLeptonEta,qcdLeptonPhi,qcdLeptonE);
	//	 leptonsQCD.push_back(math::PtEtaPhiELorentzVector(leptonPt,leptonEta,leptonPhi,leptonE));
	if(nQCDLeptons == 3) break;
	
      }
    }
    
    isQCD = (nQCDLeptons == 1 && !passesLeptons);

    //    cout <<" syst "<<  syst_name<< " nlept " << nLeptons << " passesLept "<< passesLeptons<< " passes QCD2 "<<isQCD<< " passes OR "<< (bool)(passesLeptons || isQCD) <<" n jets " << nJets<< endl;


    if(!passesLeptons && !isQCD)continue;


    //    cout <<" test syst 1 "<<  syst_name<< " nlept " << nLeptons << " passesLept "<< passesLeptons<< " passes QCD1 "<<isQCD<< endl;

    //Clear the vector of btags //NOT USED NOW
    //    b_weight_tag_algo1.clear();
    //    b_weight_tag_algo2.clear();
    //    b_weight_antitag_algo1.clear();
    //    b_weight_antitag_algo2.clear();
    //        b_discriminator_value_tag_algo1.clear();
    //    b_discriminator_value_antitag_algo2.clear();
    
    ntchpt_tags=0;
    ntchpm_tags=0;
    ntche_tags=0;
    ntchpt_antitags=0;
    ntche_antitags=0;


    //Clear the vectors of non-leptons
    //    jets.clear();
    //    bjets.clear();
    //    antibjets.clear();


    
  if(doPU_){
    if(!gotPU ){
      //    cout << " before npv "<<endl;
    iEvent.getByLabel(nm1_,nm1);
    iEvent.getByLabel(n0_,n0);
    iEvent.getByLabel(np1_,np1);
    nVertices = *npv;
    //    cout << "after npv val"<< *npv << endl;
    //int temppv= *npv; 
    puZero =    LumiWeights_.weight3D( *nm1,*n0,*np1);
    //    cout << "got weight val "<<PUWeight << endl;
      //    PUWeight = *preWeights;
    gotPU = true;
    }
  
    /*    if(syst_name=="PUUp"){
	  if(*npv<=49){
	  int temppv= *npv +1 ;
	  //	PUWeight =1;
	  PUWeight =  LumiWeights_.weight3D( ,temppv);
	  }
	  }
	  if(syst_name=="PUDown"){
	  if(*npv>=1){
	  //	PUWeight =1;
	  int temppv= *npv -1 ;
	  PUWeight =  LumiWeights_.weight( temppv);
	  }
	  //    PUWeight = *preWeights;
	  }*/
    if((syst_name != "PUUp" && syst_name!= "PUDown")){
      PUWeight =  puZero;
    }
  }
  else(nVertices = -1);
  //  cout << " test 1 "<<endl;

  jsfshpt.clear();//  bjs.clear();cjs.clear();ljs.clear(); 
  jsfshpm.clear();//  bjs.clear();cjs.clear();ljs.clear(); 
  jsfshel.clear();//  bjs.clear();cjs.clear();ljs.clear(); 
  jetprobs.clear();

  bool hasTurnOnWeight = false;
  double turnOnWeightValue =1;


    if(!gotMets){
      iEvent.getByLabel(METPhi_,METPhi);
      iEvent.getByLabel(METPt_,METPt);
      
      metPx = METPt->at(0)*cos(METPhi->at(0));
      metPy = METPt->at(0)*sin(METPhi->at(0));
      
      metPxTmp = metPx; 
      metPyTmp = metPy;

      metPhi = METPhi->at(0);

      
      gotMets = true;
    }

    metPx = metPxTmp; 
    metPy = metPyTmp;

     
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
    
  cout <<" test 5 "<<endl;

    
    if(!gotJets){
      iEvent.getByLabel(jetsEta_,jetsEta);
      iEvent.getByLabel(jetsPhi_,jetsPhi);
      
      iEvent.getByLabel(jetsEnergy_,jetsEnergy);
      iEvent.getByLabel(jetsBTagAlgo_,jetsBTagAlgo);
      //      iEvent.getByLabel(jetsAntiBTagAlgo_,jetsAntiBTagAlgo);
      iEvent.getByLabel(jetsFlavour_,jetsFlavour);
      iEvent.getByLabel(jetsCorrTotal_,jetsCorrTotal);
      if(doResol_)iEvent.getByLabel(genJetsPt_,genJetsPt);
      

      //     nJets = jetsPt->size();

      if(channel != "Data"){
      iEvent.getByLabel(x1_,x1h);
      iEvent.getByLabel(x2_,x2h);
      
      x1 = *x1h;
      x2 = *x2h;
      }
      Q2 = x1 * x2 * 7000*7000;
    
      gotJets= true;
    }
  cout <<" test 6 "<<endl;
    
    if(leptonsFlavour_ == "electron" && doTurnOn_){
      if (!hasTurnOnWeight){
	for(size_t i = 0;i<nJets;++i){
	  // 	  cout << "test 05 systs "<< syst_name <<" jets qcd "<< nJets << " loop pos" <<  i<<endl;
	  if (fabs(jetsEta->at(i))>2.6) jetprobs.push_back(0.);
	  else jetprobs.push_back(jetprob(jetsPt->at(i),jetsBTagAlgo->at(i)));
	}
	turnOnWeightValue = turnOnWeight(jetprobs,1);
	hasTurnOnWeight=true;
      }
    }

    turnOnWeightTree = turnOnWeightValue;
    PUWeightTree = PUWeight;
    
    //    Weight * turnOnWeightValue;
    Weight *= PUWeight;

    //    cout <<" test syst 2 "<<  syst_name<< " nJets " << nJets << " passesLept "<< passesLeptons<< " passes QCD1 "<<isQCD<< endl;
    
  cout <<" test 7 "<<endl;

    for(size_t i = 0;i<jetsPt->size();++i){
      
      //      cout << "test 06 systs "<< syst_name <<" jets qcd "<< nJets << " loop pos" <<  i<<endl;



      eta = jetsEta->at(i);
      if (fabs(eta )>4.5)continue;
      ptCorr = jetsPt->at(i);
      flavour = jetsFlavour->at(i);
      double energyCorr = jetsEnergy->at(i); 

      if(doReCorrection_){
	ptCorr = ptCorr/jetsCorrTotal->at(i);
	
      }
      
      //      float geneta =genJetsEta->at(i);
      float genpt = -1.;
      if(doResol_)genpt = genJetsPt->at(i);
      float rndm = 0.1;
      
      
      //If systematics JES up/down we need to change the pt of the jet
      //consider if it passes the threshold or not

      if(syst_name == "JESUp"){
	unc = jetUncertainty( eta,  ptCorr, flavour);
	ptCorr = ptCorr * (1+unc);
	energyCorr = energyCorr *(1+unc);
	metPx-=(jetsPt->at(i)*cos(jetsPhi->at(i)))*unc;
	metPy-=(jetsPt->at(i)*sin(jetsPhi->at(i)))*unc;
      }
      if(syst_name == "JESDown"){
	unc = jetUncertainty( eta,  ptCorr, flavour);
	ptCorr = ptCorr * (1-unc);
	energyCorr = energyCorr *(1-unc);
	metPx-=-(jetsPt->at(i)*cos(jetsPhi->at(i)))*unc;
	metPy-=-(jetsPt->at(i)*sin(jetsPhi->at(i)))*unc;
      }
      
      if(doResol_ && genpt > 0.0){
	//	TF1* fPtResol = ptResol->resolutionEtaPt(eta,ptCorr);
	//cout<< " function parameters ";
	//	  fPtResol -> Print();
	//cout<<endl;
	//double sigmaFunc = fPtResol->GetParameter("\\sigma");
	//fPtResol->SetParameter("\\sigma",sigmaFunc*1.1);
	//rndm = fPtResol->GetRandom();
	resolScale = resolSF(fabs(eta),syst_name);
	double smear = std::max((double)(0.0),(double)(ptCorr+(ptCorr-genpt)*resolScale)/ptCorr);
	energyCorr = energyCorr * smear;
	ptCorr = ptCorr*smear;
      }
      
	//Pt cut
	bool passesPtCut = ptCorr>ptCut;
	if(passesPtCut) {
	  ++nJets;
	  jets[nJets-1]=math::PtEtaPhiELorentzVector(ptCorr,jetsEta->at(i), jetsPhi->at(i), energyCorr);
	  //  jets.push_back(math::PtEtaPhiELorentzVector(ptCorr,jetsEta->at(i), jetsPhi->at(i), energyCorr) ); 
	}
    
  cout <<" test 7.5 "<<endl;
      //b tag thresholds 
      
      double valueAlgo1 = jetsBTagAlgo->at(i);
      //      double valueAlgo2 = jetsAntiBTagAlgo->at(i);
      
      bool passesMediumBTag = valueAlgo1  > 1.93;

      bool passesBTag = valueAlgo1  >bTagThreshold;
      //      bool passesAntiBTag = valueAlgo2 <1.7;

      if(!passesPtCut) continue;
      
      //max pt position:
      int pos =nJets-1;
      if(ptCorr > maxPtTree){ 
	maxPtTreePosition = nJets-1;
	maxPtTree = ptCorr;
      }
      //min pt position:
      if(ptCorr < minPtTree){ 
	minPtTreePosition = nJets-1;
	minPtTree = ptCorr;
      }
      
      //Passes firs algorythm (b tag requirement in the case of t-channel standard selection)
      //      cout << " test 2 "<<endl;
      
  double etaMin =  min(fabs(eta),(float)2.3999);
  double ptMin =  min(ptCorr,(float)239.9);//min(jets.back().pt(),998.0);
  //  if(is_btag_relevant ){
  measurePoint.insert(BinningVariables::JetAbsEta,etaMin);
  measurePoint.insert(BinningVariables::JetEt,ptMin);
    //  }
  //Apply different SFs if it is b,c or light jet
  if(abs(flavour)==4){ 
  cout <<" test 7.75 "<<endl;
    ++nc;
    if(is_btag_relevant ){
      //double hpteff =(perfBHP->getResult(PerformanceResult::BTAGBEFF,measurePoint));
      double hpteff = EFFMap("TCHPT_C");
      double hptSF = (perfBHP->getResult(PerformanceResult::BTAGBEFFCORR,measurePoint));
      double hptSFErr =0.;
      //double hpteff =(perfBHP->getResult(PerformanceResult::BTAGBEFF,measurePoint));
      double hptmeff = EFFMap("TCHPM_C");
      double hptmSF = (perfBHPM->getResult(PerformanceResult::BTAGBEFFCORR,measurePoint));
      double hptmSFErr =0.;
      //double heleff =(perfBHE->getResult(PerformanceResult::BTAGBEFF,measurePoint));
      double heleff = EFFMap("TCHEL_C");
      double helSF = (perfBHE->getResult(PerformanceResult::BTAGBEFFCORR,measurePoint));
      double helSFErr =0.;
      if(syst_name == "BTagUp"){                             
	hptSFErr = fabs(perfBHP->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	hptmSFErr = fabs(perfBHPM->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	helSFErr = fabs(perfBHE->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
      }
      if(syst_name == "BTagDown"){
	hptSFErr = -fabs(perfBHP->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	hptmSFErr = -fabs(perfBHPM->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	helSFErr = -fabs(perfBHE->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
      }
      //      cout <<"cjet hpt "<<hpteff<< " hpt sf "<< hptSF <<endl;
      
      jsfshpm.push_back(BTagWeight::JetInfo(hptmeff,hptmSF+hptmSFErr));
      jsfshpt.push_back(BTagWeight::JetInfo(hpteff,hptSF+hptSFErr));
      jsfshel.push_back(BTagWeight::JetInfo(heleff,helSF+helSFErr));
    }
    //  jsfshpt.push_back(BTagWeight::JetInfo(1.,1.));
    //    jsfshel.push_back(BTagWeight::JetInfo(1.,1.));

  }
  else if(abs(flavour)==5){
  cout <<" test 7.75 2"<<endl;
    ++nb;
    if(is_btag_relevant ){
      //double hpteff =(perfBHP->getResult(PerformanceResult::BTAGBEFF,measurePoint));
      double hpteff = EFFMap("TCHPT_B");
      double hptSF = (perfBHP->getResult(PerformanceResult::BTAGBEFFCORR,measurePoint));
      double hptSFErr =0.;
      //double hpteff =(perfBHP->getResult(PerformanceResult::BTAGBEFF,measurePoint));
      double hptmeff = EFFMap("TCHPM_B");
      double hptmSF = (perfBHP->getResult(PerformanceResult::BTAGBEFFCORR,measurePoint));
      double hptmSFErr =0.;
      //    double heleff =(perfBHE->getResult(PerformanceResult::BTAGBEFF,measurePoint));
      double heleff = EFFMap("TCHEL_B");
      double helSF = (perfBHE->getResult(PerformanceResult::BTAGBEFFCORR,measurePoint));
      double helSFErr =0.;
      if(syst_name == "BTagUp"){
	hptSFErr = fabs(perfBHP->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	hptmSFErr = fabs(perfBHPM->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	helSFErr = fabs(perfBHE->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
      }
      if(syst_name == "BTagDown"){
	hptSFErr = -fabs(perfBHP->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	hptmSFErr = -fabs(perfBHPM->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	helSFErr = -fabs(perfBHE->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
      }
      //    cout <<"bjet hpt "<<hpteff<< " hpt sf "<< hptSF << " jet pt "<< ptCorr<<"eta"<<etaMin <<endl;
      jsfshpm.push_back(BTagWeight::JetInfo(hptmeff,hptmSF+hptmSFErr));
      jsfshpt.push_back(BTagWeight::JetInfo(hpteff,hptSF+hptSFErr));
      jsfshel.push_back(BTagWeight::JetInfo(heleff,helSF+helSFErr));
    }

    //  jsfshpt.push_back(BTagWeight::JetInfo(1.,1.));
    //  jsfshel.push_back(BTagWeight::JetInfo(1.,1.));
  }
  else{
    cout <<" test 7.75 3"<<endl;
    if(is_btag_relevant ){
      double hpteff =(perfMHP->getResult(PerformanceResult::BTAGLEFF,measurePoint));
      cout <<" test 7.75 4"<<endl;
      double hptSF = (perfMHP->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
      cout <<" test 7.75 5"<<endl;
      double hptSFErr =0.;
      cout <<" test 7.75 6"<<endl;
      double hptmeff =(perfMHPM->getResult(PerformanceResult::BTAGLEFF,measurePoint));
      cout <<" test 7.75 7"<<endl;
      double hptmSF = (perfMHPM->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
      double hptmSFErr =0.;
      double heleff =(perfMHE->getResult(PerformanceResult::BTAGLEFF,measurePoint));
      double helSF = (perfMHE->getResult(PerformanceResult::BTAGLEFFCORR,measurePoint));
      double helSFErr =0.;
      cout <<" test 7.75 3.5"<<endl;
      if(syst_name == "MisTagUp"){
	hptSFErr = fabs(perfMHP->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	hptmSFErr = fabs(perfMHPM->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	helSFErr = fabs(perfMHE->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
      }
      if(syst_name == "MisTagDown"){
	hptSFErr = -fabs(perfMHP->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
	hptmSFErr = -fabs(perfMHPM->getResult(PerformanceResult::BTAGBERRCORR,measurePoint));
	helSFErr = -fabs(perfMHE->getResult(PerformanceResult::BTAGLERRCORR,measurePoint));
      }
      //    cout <<"light jet hpt "<<hpteff<< " hpt sf "<< hptSF <<endl;
      jsfshpm.push_back(BTagWeight::JetInfo(hptmeff,hptmSF+hptmSFErr));
      jsfshpt.push_back(BTagWeight::JetInfo(hpteff,hptSF+hptSFErr));
      jsfshel.push_back(BTagWeight::JetInfo(heleff,helSF+helSFErr));
    }
    ++nudsg;
  }
  cout <<" test 7.8"<<endl;

  if(is_btag_relevant ) measurePoint.reset();
  //	cout << " test 3 "<<endl;
    
  if(passesMediumBTag){
    ++ntchpm_tags;
  }
  if(passesBTag) {
    //Add to b-jet collection
    ++nBJets;
    bjets[nBJets-1]=jets[nJets-1];
    //math::PtEtaPhiELorentzVector(ptCorr,jetsEta->at(i), jetsPhi->at(i), energyCorr);
    //    bjets.push_back(jets.back()); 
    ++ntchpt_tags;
  }
  else {
    ++ntchpt_antitags;
    ++nAntiBJets;
  }
  cout <<" test 7.9"<<endl;
  
  /*  if(passesAntiBTag){
    antibjets.push_back(jets.back());
    ++ntche_antitags;
  }else ++ntche_tags;
  */
  
  //Condition to find the highest/lowest b-tag 
  //according to algo 1 (tchp) 
  //      cout << " test highLow " << endl;
  //cout << " i "<< i <<" jets size "<< jets.size()<< " btag  "<< 
  if(jetsBTagAlgo->at(i) > highBTagTree){
    highBTagTree=jetsBTagAlgo->at(i);
    highBTagTreePosition=nJets-1;
    bJetFlavourTree = jetsFlavour->at(i);
  } 
  if(jetsBTagAlgo->at(i) < lowBTagTree){
    lowBTagTree=jetsBTagAlgo->at(i);
    lowBTagTreePosition=nJets-1;
  }
  if(nJets>=10 )break;
    }
  cout <<" test 8 "<<endl;
    
    
    //cout << " test 3 "<<endl;
    //    cout <<" test "<<channel<<"nb" << nb << "nc"  << nc << " nudsg " <<nudsg<< " passes ? "<<  flavourFilter(channel,nb,nc,nudsg)<<endl;
    if( !flavourFilter(channel,nb,nc,nudsg) ) continue;

    //    cout <<" test syst 3 "<<  syst_name<< " nJets " << nJets << " passesLept "<< passesLeptons<< " passes QCD1 "<<isQCD<< endl;
    

    /////////
    ///End of the standard lepton-jet loop 
    /////////
    
    if( nJets <2 )continue;
    if(nJets >3)continue;

    //    if( maxPtTree< maxPtCut )continue;

    //LEGENDA
    //    0T = 0;
    //    1T = 1;
    //    2T = 2;
    //    0T_QCD=3;
    //    1T_QCD=4;
    //    2T_QCD=5;

    //    cout <<" syst "<<  syst_name<< " nlept " << nLeptons << " passesLept "<< passesLeptons<<" n jets " << nJets<< endl;
    

    
    if(nJets == 2 || nJets == 3){
      //      cout << " njets "<< nJets <<" nBJets "<< nBJets <<endl;


      int B;
      if(nBJets==0) B = 0 ;
      else if(nBJets==1){
	B=1;
      }
      else if(nBJets==2)B=2; 
     
      if(isQCD) {
	B +=3;
	leptonPFour = qcdLeptons[0];
	chargeTree = qcdLeptonsCharge->at(0) ; 
      }
      else {
	leptonPFour = leptons[0];
	chargeTree = leptonsCharge->at(0) ; 
      }

    metPt = sqrt(metPx*metPx+metPy*metPy);
    MTWValue =  sqrt((leptonPFour.pt()+metPt)*(leptonPFour.pt()+metPt)  -(leptonPFour.px()+metPx)*(leptonPFour.px()+metPx) -(leptonPFour.py()+metPy)*(leptonPFour.py()+metPy));
    bool passesMet= false;
    
    if( syst=="noSyst" && nJets ==2){
      ++passingJets;

      if(leptonsFlavour_ == "muon" && MTWValue>40 ) {++passingMET;passesMet= true;}
      if(leptonsFlavour_ == "electron" && metPt>35) {++passingMET;passesMet= true;}
    
    
      if( B==1 && passesMet)   
      ++passingBJets;
    }
    
      //      cout << " test b: low "<< lowBTagTreePosition<<" high "<< highBTagTreePosition << endl;
      math::PtEtaPhiELorentzVector top = top4Momentum(leptonPFour,jets[highBTagTreePosition],metPx,metPy);
      float fCosThetaLJ =  cosThetaLJ(leptonPFour, jets[lowBTagTreePosition], top);
      
      runTree = iEvent.eventAuxiliary().run();
      lumiTree = iEvent.eventAuxiliary().luminosityBlock();
      eventTree = iEvent.eventAuxiliary().event();
      

      //cout << " test c"<<endl;
      etaTree = fabs(jets[lowBTagTreePosition].eta());
      cosTree = fCosThetaLJ;
      topMassTree = top.mass();
      mtwMassTree = MTWValue;

      //cout << " test d"<<endl;
      
      lepPt = leptonPFour.pt();
      lepEta = leptonPFour.eta();
      lepPhi = leptonPFour.phi();

      //cout << " test e"<<endl;
      
      bJetPt = jets[highBTagTreePosition].pt();
      bJetE = jets[highBTagTreePosition].energy();
      bJetEta = jets[highBTagTreePosition].eta();
      bJetPhi = jets[highBTagTreePosition].phi();
      
      //cout << " test f"<<endl;

      fJetPt = jets[lowBTagTreePosition].pt();
      fJetE = jets[lowBTagTreePosition].energy();
      fJetEta = jets[lowBTagTreePosition].eta();
      fJetPhi = jets[lowBTagTreePosition].phi();
      

      //cout << " test g"<<endl;
      weightTree = bWeightTree*turnOnWeightValue*Weight;
      bWeightTree = b_weight_sample_B;
      // weightTree = bWeightTree*Weight;
      
      etaTree = fabs(jets[lowBTagTreePosition].eta());
      etaTree2 = fabs(jets[highBTagTreePosition].eta());
      cosTree = fCosThetaLJ;
      topMassTree = top.mass();
      mtwMassTree = MTWValue;
      
      //	    metPt = METPt->at(0);
      
      //cout << " test h"<<endl;

      fJetPt = jets[lowBTagTreePosition].pt();
      bJetPt = jets[highBTagTreePosition].pt();
      
      //cout << " test i"<<endl;
      //      chargeTree = leptonsCharge->at(0);

      if (nJets ==2){
	cout << " B is "<< B<<  " tree name "<< trees2J[B][syst_name]->GetName() <<endl;
	trees2J[B][syst_name]->Fill();            
      }
      if (nJets ==3){
	cout << " B is "<< B<<  " tree name "<< trees3J[B][syst_name]->GetName() <<endl;
	trees3J[B][syst_name]->Fill();            
      }
    }
    //W Sample
    
      
	
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

  cout <<endl<< passingLepton<< " | "<< passingJets <<" | "<< passingMET <<" | "<< passingBJets << endl<<endl;

  for(size_t i = 0; i < rate_systematics.size();++i){
    string syst = rate_systematics[i];
    string treename = (channel+"_"+syst);

    cout<< " endjob"  << syst<< " 0 "<<endl;
    int bj =0;
    trees2J[bj][syst]->CopyAddresses(trees2J[bj]["noSyst"]);
    

        cout<< " endjob"  << syst<< " 1 "<<endl;

    /*  for(size_t step = 0; step < bScanSteps;++step){
      treesScan[step][syst]->CopyAddresses(treesScan[step]["noSyst"]); 
      treesScan[step][syst]->CopyEntries(treesScan[step]["noSyst"]); 
    }
    */
    
    //modify the weight by a constant factor    
    double tmpWeight = 0;
    double weightSF = 1.;
    
    TBranch * b = trees2J[bj]["noSyst"]->GetBranch("weight");
    int entries = b->GetEntries();
    b->SetAddress(&tmpWeight);    


    cout<< " endjob"  << syst<< " 2 "<<endl;
    
    trees2J[bj][syst]->GetBranch("weight")->Reset();
    trees2J[bj][syst]->GetBranch("weight")->SetAddress(&tmpWeight);
    

    cout<< " endjob"  << syst<< " 3 "<<endl;
    
    for(int t =0; t < entries ; ++t){
      b->GetEntry(t);
      tmpWeight*=weightSF;
      trees2J[bj][syst]->GetBranch("weight")->Fill();
      
    }
    

    
    b->SetAddress(&weightTree);
    trees2J[bj][syst]->GetBranch("weight")->SetAddress(&weightTree);
    

    
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

void SingleTopSystematicsTreesDumper::InitializeEventScaleFactorMap(){

  TCHPT_B = EventScaleFactor("TCHPT_B","noSyst");
  TCHPT_C = EventScaleFactor("TCHPT_C","noSyst");
  TCHPT_L = EventScaleFactor("TCHPT_L","noSyst");
    
  
  TCHPT_BBTagUp = EventScaleFactor("TCHPT_B","BTagUp");
  TCHPT_BBTagDown = EventScaleFactor("TCHPT_B","BTagDown");
  TCHPT_CBTagUp = EventScaleFactor("TCHPT_C","BTagUp");
  TCHPT_CBTagDown = EventScaleFactor("TCHPT_C","BTagDown");
  TCHPT_LMisTagUp = EventScaleFactor("TCHPT_L","MisTagUp");
  TCHPT_LMisTagDown = EventScaleFactor("TCHPT_L","MisTagDown");
    

  TCHPT_BAnti = EventAntiScaleFactor("TCHPT_B","noSyst");
  TCHPT_CAnti = EventAntiScaleFactor("TCHPT_C","noSyst");
  TCHPT_LAnti = EventAntiScaleFactor("TCHPT_L","noSyst");

  TCHPT_BAntiBTagUp = EventAntiScaleFactor("TCHPT_B","BTagUp");
  TCHPT_BAntiBTagDown = EventAntiScaleFactor("TCHPT_B","BTagDown");
  TCHPT_CAntiBTagUp = EventAntiScaleFactor("TCHPT_C","BTagUp");
  TCHPT_CAntiBTagDown = EventAntiScaleFactor("TCHPT_C","BTagDown");
  TCHPT_LAntiMisTagUp = EventAntiScaleFactor("TCHPT_L","MisTagUp");
  TCHPT_LAntiMisTagDown = EventAntiScaleFactor("TCHPT_L","MisTagDown");


  //  TCHP_LAntiMisTagDown = EventAntiScaleFactor("TCHP_L","MisTagDown");

  TCHPM_B = EventScaleFactor("TCHPM_B","noSyst");
  TCHPM_C = EventScaleFactor("TCHPM_C","noSyst");
  TCHPM_L = EventScaleFactor("TCHPM_L","noSyst");
    
  
  TCHPM_BBTagUp = EventScaleFactor("TCHPM_B","BTagUp");
  TCHPM_BBTagDown = EventScaleFactor("TCHPM_B","BTagDown");
  TCHPM_CBTagUp = EventScaleFactor("TCHPM_C","BTagUp");
  TCHPM_CBTagDown = EventScaleFactor("TCHPM_C","BTagDown");
  TCHPM_LMisTagUp = EventScaleFactor("TCHPM_L","MisTagUp");
  TCHPM_LMisTagDown = EventScaleFactor("TCHPM_L","MisTagDown");
    

  TCHPM_BAnti = EventAntiScaleFactor("TCHPM_B","noSyst");
  TCHPM_CAnti = EventAntiScaleFactor("TCHPM_C","noSyst");
  TCHPM_LAnti = EventAntiScaleFactor("TCHPM_L","noSyst");

  TCHPM_BAntiBTagUp = EventAntiScaleFactor("TCHPM_B","BTagUp");
  TCHPM_BAntiBTagDown = EventAntiScaleFactor("TCHPM_B","BTagDown");
  TCHPM_CAntiBTagUp = EventAntiScaleFactor("TCHPM_C","BTagUp");
  TCHPM_CAntiBTagDown = EventAntiScaleFactor("TCHPM_C","BTagDown");
  TCHPM_LAntiMisTagUp = EventAntiScaleFactor("TCHPM_L","MisTagUp");
  TCHPM_LAntiMisTagDown = EventAntiScaleFactor("TCHPM_L","MisTagDown");

  /////

  TCHEL_B = EventScaleFactor("TCHEL_B","noSyst");
  TCHEL_C = EventScaleFactor("TCHEL_C","noSyst");
  TCHEL_L = EventScaleFactor("TCHEL_L","noSyst");
    
  TCHEL_BBTagUp = EventScaleFactor("TCHEL_B","BTagUp");
  TCHEL_BBTagDown = EventScaleFactor("TCHEL_B","BTagDown");
  TCHEL_CBTagUp = EventScaleFactor("TCHEL_C","BTagUp");
  TCHEL_CBTagDown = EventScaleFactor("TCHEL_C","BTagDown");
  TCHEL_LMisTagUp = EventScaleFactor("TCHEL_L","MisTagUp");
  TCHEL_LMisTagDown = EventScaleFactor("TCHEL_L","MisTagDown");

  TCHEL_BAnti = EventAntiScaleFactor("TCHEL_B","noSyst");
  TCHEL_CAnti = EventAntiScaleFactor("TCHEL_C","noSyst");
  TCHEL_LAnti = EventAntiScaleFactor("TCHEL_L","noSyst");
   
  TCHEL_BAntiBTagUp = EventAntiScaleFactor("TCHEL_B","BTagUp");
  TCHEL_BAntiBTagDown = EventAntiScaleFactor("TCHEL_B","BTagDown");
  TCHEL_CAntiBTagUp = EventAntiScaleFactor("TCHEL_C","BTagUp");
  TCHEL_CAntiBTagDown = EventAntiScaleFactor("TCHEL_C","BTagDown");
  TCHEL_LAntiMisTagUp = EventAntiScaleFactor("TCHEL_L","MisTagUp");
  TCHEL_LAntiMisTagDown = EventAntiScaleFactor("TCHEL_L","MisTagDown");
}

double SingleTopSystematicsTreesDumper::SFMap(string algo ){
  if(algo == "TCHPT_B")return 0.89;
  if(algo == "TCHPT_C")return 0.89;
  if(algo == "TCHPT_L")return 1.17;

  if(algo == "TCHPM_B")return 0.91;
  if(algo == "TCHPM_C")return 0.91;
  if(algo == "TCHPM_L")return 0.91;

  if(algo == "TCHEL_B")return 0.95;
  if(algo == "TCHEL_C")return 0.95;
  if(algo == "TCHEL_L")return 1.11;


  return 0.9;
}

double SingleTopSystematicsTreesDumper::SFErrMap(string algo ){
  if(algo == "TCHPT_B")return 0.092;
  if(algo == "TCHPT_C")return 0.092;
  if(algo == "TCHPT_L")return 0.18;

  if(algo == "TCHPM_B")return 0.10;
  if(algo == "TCHPM_C")return 0.10;
  if(algo == "TCHPM_L")return 0.11;

  if(algo == "TCHEL_B")return 0.10;
  if(algo == "TCHEL_C")return 0.10;
  if(algo == "TCHEL_L")return 0.11;

  return 0.1;
}

double SingleTopSystematicsTreesDumper::EFFMap(string algo ){
  if(algo == "TCHPT_B")return 0.365;
  if(algo == "TCHPT_C")return 0.365;
  if(algo == "TCHPT_L")return 0.0017;

  if(algo == "TCHEL_B")return 0.765;
  if(algo == "TCHEL_C")return 0.765;
  if(algo == "TCHEL_L")return 0.13;

  if(algo == "TCHPM_B")return 0.48;
  if(algo == "TCHPM_C")return 0.48;
  if(algo == "TCHPM_L")return 0.0177;

  return 0.36;
}



double SingleTopSystematicsTreesDumper::EFFErrMap(string algo ){
  if(algo == "TCHPT_B")return 0.05;
  if(algo == "TCHPT_C")return 0.05;
  if(algo == "TCHPT_L")return 0.0004;

  if(algo == "TCHEL_B")return 0.05;
  if(algo == "TCHEL_C")return 0.05;
  if(algo == "TCHEL_L")return 0.03;

  if(algo == "TCHEL_B")return 0.05;
  if(algo == "TCHEL_C")return 0.05;
  if(algo == "TCHEL_L")return 0.004;

  return 0.05;
}
double SingleTopSystematicsTreesDumper::EventScaleFactor(string algo,string syst_name){//,double sf, double eff, double sferr){

  //  double mistagcentral = sf;  
  //double mistagerr = sferr;
  //double tcheeff = eff;

  double mistagcentral = SFMap(algo);  
  double mistagerr = SFErrMap(algo);
  double tcheeff = EFFMap(algo);

  
  if(syst_name == "MisTagUp" || syst_name == "BTagUp"){
    return mistagcentral+mistagerr;
  }

  if(syst_name == "MisTagDown" || syst_name == "BTagDown"){
    return mistagcentral-mistagerr;
  }

  return mistagcentral;
}

//EventAntiScaleFactor

double SingleTopSystematicsTreesDumper::EventAntiScaleFactor(string algo,string syst_name ){
  //,double sf, double eff, double sferr){

  
  //double mistagcentral = sf;  
  //double mistagerr = sferr;
  //double tcheeff = eff;

  double mistagcentral = SFMap(algo);  
  double mistagerr = SFErrMap(algo);
  double tcheeff = EFFMap(algo);

  
  if(syst_name == "MisTagUp" || syst_name == "BTagUp"){
    return (1-tcheeff)/(1-tcheeff/(mistagcentral+mistagerr));
  }
  
  if(syst_name == "MisTagDown" || syst_name == "BTagDown"){
    return (1-tcheeff)/(1-tcheeff/(mistagcentral-mistagerr));
    
  }

  return (1-tcheeff)/(1-tcheeff/(mistagcentral));
  
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


double SingleTopSystematicsTreesDumper::turnOnWeight (std::vector<double> probabilities, int njets_req =1){
  double prob =0;
  for(unsigned int i=0; i<pow(2,probabilities.size());++i){
    //at least njets_req objects for trigger required
    int ntrigobj=0;
    for(unsigned int j=0; j<probabilities.size();++j){
      if((int)(i/pow(2,j))%2) ntrigobj++;
    }
    if(ntrigobj<njets_req) continue;  
    double newprob=1;
    for(unsigned int j=0; j<probabilities.size();++j){
      if((int)(i/pow(2,j))%2) newprob*=probabilities[j];
      else newprob*=1-probabilities[j];
    }
    prob+=newprob;
  }
  return prob;
}


bool SingleTopSystematicsTreesDumper::flavourFilter(string ch, int nb, int nc, int nl){
  
  if(ch == "WJets_wbb" || ch == "ZJets_wbb") return (nb>0 );
  if(ch == "WJets_wcc" || ch == "ZJets_wcc") return (nb==0 && nc>0);
  if(ch == "WJets_wlight" || ch == "ZJets_wlight") return (nb==0 && nc==0);
   
  return true;
}

/*double SingleTopSystematicsTreesDumper::jetprob(double pt, double btag){
  double prob=0.993*(exp(-51.0*exp(-0.160*pt)));
  prob*=0.902*exp((-5.995*exp(-0.604*btag)));
  return prob;
  }*/

double SingleTopSystematicsTreesDumper::jetprob(double pt, double btag){
  double prob=0.982*exp(-30.6*exp(-0.151*pt));//PT turnOn
  prob*=0.844*exp((-6.72*exp(-0.720*btag)));//BTag turnOn
  return prob;
}

double SingleTopSystematicsTreesDumper::resolSF(double eta,string syst){
  double fac = 0.;
  if(syst== "JERUp")fac=1.;
  if(syst== "JERDown")fac=-1.;
  if(eta<=0.5) return 1.05+0.06*fac;
  else if( eta>0.5 && eta<=1.1 ) return 1.06+0.06*fac;
  else if( eta>1.1 && eta<=1.7 ) return 1.1+0.06*fac;
  else if( eta>1.7 && eta<=2.3 ) return 1.13+0.1*fac;
  else if( eta>2.3 && eta<=5. ) return 1.29+0.2*fac;
  return 1.1;
}


//BTag weighter
bool SingleTopSystematicsTreesDumper::BTagWeight::filter(int t)
{
  return (t >= minTags && t <= maxTags);
}

float SingleTopSystematicsTreesDumper::BTagWeight::weight(vector<JetInfo> jets, int tags)
{
  if(!filter(tags))
    {
      //   std::cout << "This event should not pass the selection, what is it doing here?" << std::endl;
      return 0;
    }
  int njets=jets.size();
  int comb= 1 << njets;
  float pMC=0;
  float pData=0;
  for(int i=0;i < comb; i++)
    {
      float mc=1.;
      float data=1.;
      int ntagged=0;
      for(int j=0;j<njets;j++)
	{
	  bool tagged = ((i >> j) & 0x1) == 1;
	  if(tagged) 
	    {
	      ntagged++;
	      mc*=jets[j].eff;
	      data*=jets[j].eff*jets[j].sf;
	    }
	  else
	    {
	      mc*=(1.-jets[j].eff);
	      data*=(1.-jets[j].eff*jets[j].sf);
	    }
	}       
   
      if(filter(ntagged))
	{
	  //  std::cout << mc << " " << data << endl;
	  pMC+=mc;
	  pData+=data;
	}
    }

  if(pMC==0) return 0; 
  return pData/pMC;
}


//define this as a plug-in
DEFINE_FWK_MODULE(SingleTopSystematicsTreesDumper);
