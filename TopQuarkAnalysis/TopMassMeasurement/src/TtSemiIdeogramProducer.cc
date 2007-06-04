#include "TopQuarkAnalysis/TopMassMeasurement/interface/TtSemiIdeogramProducer.h"

//
// constructors and destructor
//
TtSemiIdeogramProducer::TtSemiIdeogramProducer(const edm::ParameterSet& iConfig)
{
   solChoice_ 	  	= iConfig.getParameter< string > ("solChoice");
   hadWProbChi2Cut_ 	= iConfig.getParameter< double > ("hadWProbChi2Cut");
   corrJetCombProb_ 	= iConfig.getParameter< double > ("corrJetCombProb");
   signalEvtProb_    	= iConfig.getParameter< double > ("signalEvtProb");
   mtvals_ 	  	= iConfig.getParameter< vector<double> > ("mtValues");
   myDMtopCalc    	= new MtopUncertaintyCalc();
   myIdeogramScan 	= new TtSemiIdeogramScan(mtvals_);
   produces<TtSemiMassSolution>();
}


TtSemiIdeogramProducer::~TtSemiIdeogramProducer()
{
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TtSemiIdeogramProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{     
  
   // Get the vector of solutions from the event
   edm::Handle<vector<TtSemiEvtSolution> > sols;
   iEvent.getByLabel( "solutions", sols );
   
   // define the best solution
   TtSemiMassSolution massSol;
   int bestSol = -999;
   if(solChoice_ == "MCtrue"){
     //get MCtrue bestSol
     bestSol = (*sols)[0].getMCCorrJetComb();
     if(bestSol < -1) return;
     massSol = TtSemiMassSolution((*sols)[bestSol]);
   }
   else if(solChoice_ == "simple"){
     bestSol = (*sols)[0].getSimpleCorrJetComb();
     if(bestSol < -1) return;
     massSol = TtSemiMassSolution((*sols)[bestSol]);
   }
   else if(solChoice_ == "LR"){
     bestSol = (*sols)[0].getLRCorrJetComb();
     if(bestSol < -1) return;
     massSol = TtSemiMassSolution((*sols)[bestSol]);
   }
   
   // event selection cuts
   if(massSol.getProbChi2() <= hadWProbChi2Cut_) return;
   if(solChoice_ == "LR" && massSol.getLRCorrJetCombProb() > -1. && massSol.getLRCorrJetCombProb() <= corrJetCombProb_) return;
   if(massSol.getLRSignalEvtProb() > -1. && massSol.getLRSignalEvtProb() <= signalEvtProb_) return;
   
   // Calculate the top mass uncertainty by error propagation (to be used in the construction of the gausian ideogram)
   (*myDMtopCalc)(massSol);
   
   // scan this solution
   vector<pair<double,double> > scanVals = myIdeogramScan->getScanValues(massSol);
   massSol.setScanValues(scanVals);
   
   // put genEvt object in Event
   TtSemiMassSolution *thesol = new TtSemiMassSolution(massSol);
   auto_ptr<TtSemiMassSolution> myIdeogramSol(thesol);
   iEvent.put(myIdeogramSol);   
}
