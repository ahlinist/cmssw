#include "PhysicsTools/JetRejectorTool/interface/JetRejComLR.h"
#include "PhysicsTools/JetRejectorTool/interface/JetRejObsProducer.h"
//#include "PhysicsTools/PatUtils/interface/RefHelper.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/ValueMap.h"


//
// constructor
//

JetRejComLR::JetRejComLR(const edm::ParameterSet& iConfig){
  obssrc_ = iConfig.getParameter<edm::InputTag>( "obssrc" );  
  selcalojetsrc_ = iConfig.getParameter<edm::InputTag>( "selcalojetsrc" );  
  DeltaRcut_   = iConfig.getParameter< double > ("DeltaRcut");
  JetSelObs_ = iConfig.getUntrackedParameter< vector<int> > ( "JetSelObs"); 
  lrJetCombFile_ = iConfig.getParameter< std::string > ("lrJetCombFile");
 
  //likelihood histogram variables
  nrJetCombLRtotBins   		= 50;
  JetCombLRtotMin   		= -7;
  JetCombLRtotMax      		= 11;
  JetCombLRtotFitFunction      	= "[0]/(1 + 1/exp([1]*([2] - x)))+[3]";
    //   "[0]/(1 + 1/exp([1]*([2] - x)))";
  
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
   using namespace edm;
   using namespace std;
   using namespace reco;
 
   JetRejObs obsValue;
   double myJet= -1;
   double logLR;
   vector<double> obsVals;
   
   //read in jets, use jets to access JetRejObsMap:
   Handle<View<Jet> > jets; 
   iEvent.getByLabel( selcalojetsrc_, jets); 

   Handle<JetRejObsProducer::JetRejObsMap> observables;
   iEvent.getByLabel(obssrc_, observables); 

   
   // create map passing the handle to the matched collection
   std::auto_ptr<JetLRRejectorMap> jetlrs(new JetLRRejectorMap);
   JetLRRejectorMap::Filler filler(*jetlrs);
   {
     size_t n = jets->size();
     std::vector<double> lrs(n);
     //vector<reco::CaloJet>::const_iterator thisjet = calojet.begin();
     View<Jet>::const_iterator thisjet = jets->begin();
     for(size_t ll = 0; ll != n; ++ll, ++thisjet) {
       // compute the match for i-th jet, store the index in genParticles collection
       RefToBase<Jet> thisjetref = jets->refAt(ll);
       obsValue = (*observables)[thisjetref]; // vector of observables for each jet!
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
       // ---------------------------------------------------------------------
       lrs[ll] = logLR;
     }
     filler.insert(jets, lrs.begin(), lrs.end());
   }
   
   
   // really fill the association map
   filler.fill();
   // put into the event 
   iEvent.put(jetlrs);
}

 //define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
 DEFINE_FWK_MODULE(JetRejComLR);
