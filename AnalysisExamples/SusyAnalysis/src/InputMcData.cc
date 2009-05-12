
#include "AnalysisExamples/SusyAnalysis/interface/InputMcData.h"


using namespace std;
using namespace reco;
using namespace edm;

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
// InputMcData::InputMcData(Config_t * aConfig):
// myConfig(aConfig)
InputMcData::InputMcData(Config_t * aConfig, 
                         const edm::Event& aEvent):
myConfig(aConfig), iEvent(aEvent)
{

  // get names of modules, producing object collections
  InputMC_params =
    (*myConfig).InputMC_params;
//  m_jetsgenSrc = iConfig.getParameter<string>("jetsgen");
  m_genParticles = InputMC_params.getParameter<string>("genParticles");
  m_jetsgenSrc = InputMC_params.getParameter<string>("jetsgen");
 
  // get parameters, save them in a structure Config
  
  // get debug level
//  DEBUGLVL = .getUntrackedParameter<int>("debuglvl", 0);  
  

}


InputMcData::~InputMcData()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)


}


//
// member functions
//

// ------------ method called to produce the MC data  ------------

//void InputMcData::InputMcDriver(const edm::Event& iEvent, MrEvent* EventData)
void InputMcData::InputMcDriver(MrEvent* EventData)
{
  // Variables and counters valid per event
  
  

  // ******************************************************** 
  // Input the MC data and store them as MrParticle
  // ******************************************************** 

/*
  // get MC info from HepMC
  Handle<HepMCProduct> EvtHandle ;
  iEvent.getByLabel( "VtxSmeared", EvtHandle ) ;
  const HepMC::GenEvent* evt = EvtHandle->GetEvent() ;
  // evt->print();
  vector<int> barPart, barMoth;
  
  // fill MC info into MCData vector  
  for ( HepMC::GenEvent::particle_const_iterator p = evt->particles_begin();
	p != evt->particles_end(); ++p ) {

    MrParticle* mcpart = new MrParticle((*p)->momentum().px(),
           (*p)->momentum().py(),(*p)->momentum().pz(),(*p)->momentum().e());
           
    mcpart->setPID((*p)->pdg_id());
    mcpart->setStatus((*p)->status());
    mcpart->setMotherIndex(0);         // to be fixed, see below
  
    MCData.push_back(mcpart);  
  
    // Temporary fiddle to set up the mother index from barcodes
    //cout << " pointer of particle: " << (*p) << endl;
    //cout << " barcode of particle: " << (*p)->barcode() << endl;
    barPart.push_back((*p)->barcode()); 
    //cout << " pointer to mother: " << (*p)->mother() << endl;
    //cout << " barcode of mother: " << (*p)->mother()->barcode() << endl;
    if ((*p)->mother() != NULL) {
      barMoth.push_back((*p)->mother()->barcode()); 
    } else {
      barMoth.push_back(-1);
    }
  }
  
  for (int i= (int) barPart.size()-1; i>=0; i--){
    if (barMoth[i] < 0){
      MCData[i]->setMotherIndex(-1);
    } else {
      for (int j=i-1; j>=0; j--){
        if (barMoth[i] == barPart[j]){
          MCData[i]->setMotherIndex(j);
          break;
        }
      }
    }
  }

  barPart.clear();
  barMoth.clear();
  
  */
  
   // get MC info from GenParticleCandidates 
    Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel( m_genParticles, genParticles );
  
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
    
     const Candidate& p = (*genParticles)[ i ];
     
     MrParticle* mcpart = new MrParticle(p.px(),p.py(),p.pz(),p.energy());
  
     mcpart->setPID(p.pdgId());
     mcpart->setStatus(p.status());
     mcpart->setMotherIndex(0);
     
     MCData.push_back(mcpart);  
 
     if (p.numberOfMothers() > 0 ) { 
       const Candidate * mom = p.mother();
//       for( size_t j = 0; j < genParticles->size(); ++ j ) {
       for( size_t j = 0; j < i; ++ j ) {
 	const Candidate * ref = &((*genParticles)[j]);
 	if (ref == mom)
 	  {
 	    MCData[i]->setMotherIndex(j);
 	  }
 	//if (ref == mom) MCData[i]->setMotherIndex(j);
       }  
     } else {
       MCData[i]->setMotherIndex(-1);
    }

  }
  
  
  
  // Save the MCData in the Event data
  EventData->setMCData(&MCData);

/*
  //Save the cross section and the event weight (CSA07 only!)
  Handle<int> genProcessID;
  iEvent.getByLabel( "genEventProcID", genProcessID );
  double processID = *genProcessID;
  double cs = -1;
  if (processID != 4) { 
    Handle<double> genCrossSect;
    iEvent.getByLabel( "genEventRunInfo", "PreCalculatedCrossSection", genCrossSect); 
    cs = *genCrossSect;
  }
  Handle< double> weightHandle;
  iEvent.getByLabel ("csaweightproducer","weight", weightHandle);
  double weight = * weightHandle;
  EventData->setCrossSection(cs);
  EventData->setEventWeight(weight);
*/

  // make the GenData
  // first loop over the electrons and muons from the MCData 
  // and copy them in the GenData
  for (int i = 0; i < (int) MCData.size(); i++) { 
    if((abs(MCData[i]->pid()) == 11 || abs(MCData[i]->pid()) == 13) 
        && MCData[i]->status() == 1) { 
      MrParticle* part = new MrParticle(*MCData[i]);
      part->setPartonIndex(i);
      GenData.push_back(part);
    }
  }   
  // then add the gen jet collection
  Handle<GenJetCollection> jetsgen;
  iEvent.getByLabel(m_jetsgenSrc, jetsgen);
//  iEvent.getByLabel("iterativeCone5GenJetsNoNuBSM", jetsgen);
  for (unsigned int j = 0; j < jetsgen->size(); j++) {

      MrJet* part = new MrJet((*jetsgen)[j].px(),
           (*jetsgen)[j].py(),(*jetsgen)[j].pz(),(*jetsgen)[j].energy(), &(*jetsgen)[j],0);

      part->setPartonIndex(-1);
      GenData.push_back(part);
   
  }

  // Save the GenData in the Event data
  EventData->setGenData(&GenData);


   

  // ******************************************************** 
  // End of the MC data input
  // ******************************************************** 

  return;
}



