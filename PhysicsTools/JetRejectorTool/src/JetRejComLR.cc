#include "PhysicsTools/JetRejectorTool/interface/JetRejComLR.h"


//
// constructor
//

JetRejComLR::JetRejComLR(const edm::ParameterSet& iConfig){
  obssrc_ = iConfig.getParameter<edm::InputTag>( "obssrc" );  
  DeltaRcut_   = iConfig.getParameter< double > ("DeltaRcut");
  JetSelObs_ = iConfig.getUntrackedParameter< vector<int> > ( "JetSelObs"); 
  lrJetCombFile_ = iConfig.getParameter< std::string > ("lrJetCombFile");
 
  //likelihood histogram variables
  nrJetCombLRtotBins   		= 50;
  JetCombLRtotMin   		= -5;
  JetCombLRtotMax      		= 11;
  JetCombLRtotFitFunction      	= "[0]/(1 + 1/exp([1]*([2] - x)))";
  
  //output files ps/root
  JetCombOutFileName   		= "../data/JetRejLRJetCombSelObsAndPurity.root";
  JetCombPSfile     		= "../data/JetRejLRJetCombSelObsAndPurity.ps";


  for(unsigned int j = 0; j < JetSelObs_.size(); j++){
    obsNrs.push_back(JetSelObs_[j]);
  }

  //methods of LRHelpFunction--------- 
  myLRhelper = new LRHelpFunctions();

  // read in S/S+N fits of observables to use
  myLRhelper -> readObsHistsAndFits(lrJetCombFile_, obsNrs, false);
  
  // initialize the LR hists & fits
  myLRhelper -> initLRHistsAndFits(nrJetCombLRtotBins, JetCombLRtotMin, JetCombLRtotMax , JetCombLRtotFitFunction);

  ///-----------------------------------------------------------------

produces<  vector< double > >(); 
}

// destructor
JetRejComLR::~JetRejComLR()
{

  // make Purity vs logLR and Purity vs. Efficiency plot
  myLRhelper -> makeAndFitPurityHists();       

  // store histograms and fits in root-file
  myLRhelper -> storeToROOTfile(JetCombOutFileName);

  // make some control plots and put them in a .ps file
  myLRhelper -> storeControlPlots(JetCombPSfile);

  // delete myJetRejLRObs;
}

void JetRejComLR::produce( edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::vector <double >  *myObs = new std::vector <double> ;

  Handle<vector<JetRejObs> > observables;
  iEvent.getByLabel(obssrc_, observables); 

  JetRejObs obsValue;
  double myJet= -1;
  double logLR;
  // get observable values
  vector<double> obsVals;
  for(unsigned int ll =0; ll<observables->size(); ll++){ // loop on the jets in the event!
    obsValue = (*observables)[ll]; // vector of observables for each jet!
    obsVals.clear();
    for(unsigned int j=0; j<obsValue.getSize(); j++){//loop on all the observables
      if(obsValue.getPair(j).first == 0 ) myJet = obsValue.getPair(j).second;
      
      for(unsigned int os = 0; os < JetSelObs_.size(); os++){ // loop on the selected observables
	if(obsValue.getPair(j).first == JetSelObs_[os]){
	  obsVals.push_back(obsValue.getPair(j).second);
	}
      }
    }
    
    logLR =  myLRhelper -> calcLRval(obsVals);
    
    if(myJet == 1) {
      myLRhelper -> fillLRSignalHist(logLR);
    }
    else
      {
	myLRhelper -> fillLRBackgroundHist(logLR);
      }
    myObs->push_back( logLR );//store the logLR in the AOD 
  }//close loop on the jets in the events!
  
  //---------------------------------
  
  std::auto_ptr<std::vector< double > > pOut(myObs);
  iEvent.put( pOut );
  ///-----------------------
  
}
