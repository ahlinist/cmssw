#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include <vector>

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"

using namespace std;
using namespace reco;
using namespace pat;

string overlapName = "electrons";               //the overlap name
//string overlapName = "tkIsoElectrons";        //switch the overlap name and see the effect

//Each histogram in this collection, will be written to the output file at the end
std::vector<TH1*> histoContainer;

//Definition of the method, body is implemented at the end of the file
void AnalyzeNotCleanJets( const pat::Jet* );

//The MAIN method
int main(int argc, char *argv[]) {

  string patTupleFileName = "patTuple.root" ;   //default input file name, if no parameter is passed to the executable, it assumes that the input file is in the same directory and its name is patTuple.root
  //Read the input file name from the arguments
  if(argc == 2) //if any argument has been given
    patTupleFileName = std::string( *(++argv) );

  //HISTOGRAMS
  TH1F hEMFAllJets("hEMFAllJets" , "EMF For All Jets" , 21 , 0 , 1.05);
  //add the histograms to the container to be written at the end
  histoContainer.push_back( &hEMFAllJets   );

  //TH1F hEMFCleanJets("hEMFCleanJets" , "EMF For Clean Jets" , 21 , 0 , 1.05);
  //histoContainer.push_back( &hEMFCleanJets );

  //initialize FWLite
  AutoLibraryLoader::enable();
  TFile file(patTupleFileName.c_str());

  fwlite::Event ev(&file);

  //FWLite loop over the events in the file
  for(ev.toBegin(); !ev.atEnd(); ++ev){
    //make a handle and get the clean Jets from the event
    fwlite::Handle<std::vector<pat::Jet> > jets;
    jets.getByLabel(ev,"cleanPatJets");

    //Loop over the jets in the event
    for( vector<pat::Jet>::const_iterator jet = jets->begin(); jet != jets->end(); jet++ ){
      //accept the jets with pt > 20
      if(jet->pt() > 20){
	//Fill the histogram with all of the jets
	hEMFAllJets.Fill( jet->emEnergyFraction() );
	//seprate the jets into two clean and notClean categories
	if(! jet->hasOverlaps(overlapName)){
	  //Draw the EMF of clean jets
	  //hEMFCleanJets.Fill( jet->emEnergyFraction() );
	}
	else{
	  //Analyze the notClean jets
	  //AnalyzeNotCleanJets( & (*jet) );
	}
      }//JET PT CONDITION
    } // JET LOOP
    }//EVENT LOOP


  //open the output file
  TFile* fOut = TFile::Open("out.root" , "RECREATE");
  fOut->cd();

  //make a loop over all histograms and write them into the file
  for(vector<TH1*>::const_iterator histo = histoContainer.begin(); histo != histoContainer.end(); histo ++){
    (*histo)->Write();
  }
  
  //close the file
  fOut->Close();
  
  return 0;
}





//include to have access to deltaR function :
#include "DataFormats/Math/interface/deltaR.h"


//Histograms for notClean jets
TH1F hEleJetDistance("hEleJetDistance" , "Distance between jets and the closest electron" , 5 , 0 , 0.5);
TH1F hEleJetEnergyRatio("hEleJetEnergyRatio" , "Energy ratio of jet and closest electron" , 100 , 1. , 6.);
TH1F hNumberOfOverlaps("hNumberOfOverlaps" , "Number of overlaps" , 5 , 0 , 5);
TH2F hEnergyRatiovsDistance("hEnergyRatiovsDistance" , "Energy ratio vs. distance" , 5 , 0 , 0.5 , 100 , 1. , 6. );


bool firstTime(true);

//the body of AnalyzeNotCleanJets method
void AnalyzeNotCleanJets( const pat::Jet* jet ){

  //if it's the first time that the method is called, the histograms should be added into the container
  if(firstTime){
    firstTime = false;

    histoContainer.push_back( &hEleJetDistance );
    histoContainer.push_back( &hEleJetEnergyRatio );
    histoContainer.push_back( &hNumberOfOverlaps );
    histoContainer.push_back( &hEnergyRatiovsDistance );
  }


  //read the properties of the jet
  float jetEnergy = jet->energy();
  float jetEta    = jet->eta();
  float jetPhi    = jet->phi();


  //get all of the electrons that overlape with the jet
  const reco::CandidatePtrVector overlappingElectrons =  jet->overlaps(overlapName);

  //in some cases, more that one electron are in the cone of the jet
  hNumberOfOverlaps.Fill(overlappingElectrons.size());

  //loop over the overlapping electrons
  for( reco::CandidatePtrVector::const_iterator electron = overlappingElectrons.begin() ;
       electron != overlappingElectrons.end() ;
       electron ++){

    //read the properties of the electron
    float elecEnergy = (*electron)->energy() ;
    float elecEta    = (*electron)->eta();
    float elecPhi    = (*electron)->phi();
    
    float energyRatio = jetEnergy / elecEnergy ;
    float distance    = reco::deltaR( elecEta , elecPhi , jetEta , jetPhi );

    //filll the histograms
    hEleJetDistance.Fill( distance );
    hEleJetEnergyRatio.Fill( energyRatio );
    hEnergyRatiovsDistance.Fill( distance , energyRatio );

  }//ELECRON LOOP
}//END OF THE AnalyzeNotCleanJets METHOD
