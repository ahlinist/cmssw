#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "RecoBTag/MCTools/interface/MCParticleInfo.h"

using namespace edm;
using namespace std;
using namespace HepMC;

JetFlavourIdentifier::JetFlavourIdentifier(const edm::ParameterSet& iConfig)
{
  fillPartons =  iConfig.getParameter<bool>("fillPartons");
  fillHeavyHadrons =  iConfig.getParameter<bool>("fillHeavyHadrons");
  fillLeptons =  iConfig.getParameter<bool>("fillLeptons");
  coneSizeToAssociate =  iConfig.getParameter<double>("coneSizeToAssociate");
}

void JetFlavourIdentifier::readEvent(const edm::Event& iEvent, std::string label_)
{
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);
  
  HepMC::GenEvent * generated_event = new HepMC::GenEvent(*(evt->GetEvent()));
  fillInfo(generated_event);
}

void JetFlavourIdentifier::getAssociatedLepton(const reco::Jet & theJet) const
{
}

void JetFlavourIdentifier::fillInfo ( const HepMC::GenEvent * event ) {

//   // reset HeavyHadrons vector (erase wants iterators as arguments)
//   HeavyHadrons.erase( HeavyHadrons.begin() , HeavyHadrons.end() ) ;
//   cout << "===> Size of HeavyHadrons after erasing : " << HeavyHadrons.size() << endl ;
  m_partons.clear();
  cout << "===> Size of Partons after erasing : " << m_partons.size() << endl ;
//   Leptons.erase( Leptons.begin() , Leptons.end() ) ;
//   cout << "===> Size of Leptons after erasing : " << Leptons.size() << endl ;
// 
  // print RawHepEvent in Debug mode
   { event->print(); }
// 
  // one loop to find out the index of the last parton
  int indexLastParton = -1 , counter=0;
  HepMC::GenEvent::particle_const_iterator p;
  for (p = event->particles_begin(); p != event->particles_end(); p++) {
  ++counter;
//  for ( unsigned int i = 1; i<= event->n(); i++ ){
//     const RawHepEventParticle* particle = event->getParticle(i); 
    // decode particle info from Lund Code
    MCParticleInfo testCode( (*p)->pdg_id() ); 
    cout << "pid: "<<(*p)->pdg_id()<<" barcode "<<(*p)->barcode()<<endl;
    if ( testCode.isParton() ) {
      indexLastParton = counter;
    }
  }
  cout << "====>>>>> MCInfoFiller::fillInfo : index of last parton : " << indexLastParton << endl ;

  
  
  // loop over all generated particles and retrieve information
  //

  for (p = event->particles_begin(); p != event->particles_end(); p++) {

//    for ( unsigned int i = 1; i<= event->n(); i++ ){
//      const RawHepEventParticle* particle = event->getParticle(i); 
// 
     // decode particle info from Lund Code
     MCParticleInfo testCode( (*p)->pdg_id() ); 
// 
// 
//      if ( testCode.isBottomHadron() || testCode.isCharmHadron() ) {
//        // only if wanted
//        if ( fillHeavyHadrons ) {
// 	 MCHeavyHadron HH( particle, event ) ;
// 	 // Warning: when the STL vector is resized, the destructor for all objects in the vector is invoked
// 	 //          and all objects are copied to the new vector! ==> provide a copy constructor!!
// 	 HeavyHadrons.push_back( HH ) ;
//        }
//      } // end 'heavy-hadron' if
// 
     // **********************************************************************
     // check, if it's a parton. If yes, add to corresponding container
     // **********************************************************************
     if ( testCode.isParton() ) {
       if ( fillPartons ) {
	 MCParton aParton ( *p, event , indexLastParton ) ;
	 m_partons.push_back( aParton ) ;
       }
     } // end parton-if
// 
//      // **********************************************************************
//      // check, if it's a lepton. If yes, add to corresponding container
//      // **********************************************************************
//      if ( testCode.isLepton() ) {
//        if ( fillLeptons ) {
//          MCLepton aLepton ( particle, event ) ;
//          Leptons.push_back( aLepton ) ;
//        }
//      } // end lepton-if
// 
//    } // end genparticles loop
// 
// 
//    
// 
//    // test printout of info from HeavyHadron Container
//    if ( uvMCTools.testOut ) {
//      cout << "####### check HeavyHadrons #######" << endl;
//      cout << "size of vector : " << HeavyHadrons.size() << endl;
//      for ( vector<MCHeavyHadron>::iterator HHit = HeavyHadrons.begin(); HHit != HeavyHadrons.end(); ++HHit ) {
//        HHit->print() ; 
//      }
//    }
//    // test printout of info from Parton Container
//    if ( uvMCTools.testOut ) {
//      cout << "####### check Partons #######" << endl;
//      cout << "size of vector : " << Partons.size() << endl;
//      for ( vector<MCParton>::iterator Pit = Partons.begin(); Pit != Partons.end(); ++Pit ) {
//        Pit->print() ; 
//      }
//    }
//    // test printout of info from Lepton Container
//    if ( uvMCTools.testOut ) {
//      cout << "####### check Leptons #######" << endl;
//      cout << "size of vector : " << Leptons.size() << endl;
//      for ( vector<MCLepton>::iterator Lit = Leptons.begin(); Lit != Leptons.end(); ++Lit ) {
//        Lit->print() ;
//      }
   }
  
}

template <class JetCollection>
JetFlavour  JetFlavourIdentifier::identifyBasedOnPartons ( const edm::Ref<JetCollection> & theJet ) {
  JetFlavour jetFlavour;
  // to count partons from how many sources
  jetFlavour.numberOfSources = 0;
  vector<int> indicesSources;
  int nDown, nUp, nStrange, nCharm, nBottom, nGluon;
  // kinematics of underlying parton by summing them up
  
  // don't assign initial parton if more than one in cone
  int nInitialPartons = 0;
  
  for ( vector<MCParton>::iterator itP  = m_partons.begin();
	                           itP != m_partons.end(); itP++ ) {

    // get Lundcode
    int theFlavour = abs ( itP->getFlavour() );
    // get mother status
    // int motherStatus = itP->getMotherStatusPythia();
    // get kinemat. info
    Hep3Vector Jet3Vec ( theJet.getLorentzVector() );
    Hep3Vector P3Vec   ( itP->getFourVector() );
    // for initial partons: use the resummed daughters
    if ( itP->getIsInitialParton() ) P3Vec = itP->getSummedDaughters();
    //
    double deltaR     = Jet3Vec.deltaR( P3Vec );
    double pAbsParton = P3Vec.mag();
    
    //CW match also to the initial partons for the 'physics definition'
    if ( itP->getIsInitialParton() ) {
      if ( deltaR < coneSizeToAssociate ) {   // is in cone
	jetFlavour.initialFlavour(theFlavour);
	nInitialPartons++;
	jetFlavour.vec4OriginParton += itP->getSummedDaughters();
	// reset if it has other HF nearby (from other sources!!)
	if ( itP->getInitialPartonHasCloseHF() ) jetFlavour.initialFlavour(0);
	// check if it has split to heavy flavours
	jetFlavour.initialPartonSplitsToC(itP->getSplitsToC());
	jetFlavour.initialPartonSplitsToB(itP->getSplitsToB());
      }
    }
    
    // final parton? (asking both final parton and from primary process removes remnant/underlying event)
    //CW    if ( itP->getIsFinalParton() && itP->getFromPrimaryProcess() ) {
    if ( itP->getIsFinalParton() ) {
      
      if ( deltaR < ConeSizeToAssociate ) {   // is in cone
	//
	// set members
	//

	// sum up parton 4-momenta
	jetFlavour.vec4SummedPartons(jetFlavour.vec4SummedPartons()+itP->getFourVector());

	// origin source of this parton
	int origSourceCode  = abs ( itP->getMotherLundCode() );
	int origSourceIndex = itP->getMotherIndex();
	// check if this source is already there
	bool sourceAlreadyThere = false;
	for ( vector<int>::const_iterator itS = indicesSources.begin(); itS != indicesSources.end(); itS++ ) {
	  if ( *itS == origSourceIndex ) sourceAlreadyThere = true;
	}
	// add the new source if not there
	if ( !sourceAlreadyThere ) indicesSources.push_back(origSourceIndex);
	
	
	// main flavour is given to Parton with highest |p|
	if ( pAbsParton     > jetFlavour.pMainParton          ) {
	  jetFlavour.mainFlavour     (theFlavour);
	  jetFlavour.pMainParton     (pAbsParton);
	  jetFlavour.vec4MainParton  (itP->getFourVector());	  
	  jetFlavour.deltaRMainParton(deltaR);
	  jetFlavour.mainOrigFlavour (origSourceCode);
	}
	
	// Def.: gluon (21) is lighter than quarks!!
	if ( theFlavour > jetFlavour.heaviestFlavour  &&  theFlavour != 21  ) {
	  jetFlavour.pHeaviestParton   (pAbsParton);	  
	  jetFlavour.vec4HeaviestParton(itP->getFourVector());	  
	  jetFlavour.heaviestFlavour   (theFlavour);
	}
	
	if ( deltaR     < jetFlavour.deltaRClosestParton ) {
	  jetFlavour.minimumDeltaRFlavour(theFlavour);
	  jetFlavour.pClosestParton      (pAbsParton);	  
	  jetFlavour.vec4ClosestParton   (itP->getFourVector());	  
	  jetFlavour.deltaRClosestParton (deltaR);
	}

	if ( theFlavour == 1 ) nDown++; 
	if ( theFlavour == 2 ) nUp++; 
	if ( theFlavour == 3 ) nStrange++; 
	if ( theFlavour == 4 ) nCharm++; 
	if ( theFlavour == 5 ) nBottom++; 
	if ( theFlavour == 21) nGluon++; 
      
      } // end deltaR-if 
    } // end primary-if 
  } // end HH for-loop


  jetFlavour.nDown(nDown); 
  jetFlavour.nUp(nUp);
  jetFlavour.nStrange(nStrange); 
  jetFlavour.nCharm(nCharm); 
  jetFlavour.nBottom(nBottom); 
  jetFlavour.nGluon(nGluon); 
  if ( nDown  >= 1 )	jetFlavour.hasDown (true);
  if ( nUp  >= 1 )	jetFlavour.hasUp (true);
  if ( nStrange  >= 1 ) jetFlavour.hasStrange (true);
  if ( nBottom >= 1 )	jetFlavour.hasBottom(true);
  if ( nCharm  >= 1 )	jetFlavour.hasCharm (true);
  if ( nGluon  >= 1 )	jetFlavour.hasGluon (true);

  jetFlavour.numberOfSources(indicesSources.size());
  
  // temporary fudge for gluon as heaviest flavour
  // (future: get info from ordered list):
  // if there is a gluon and the heaviest flavour == 0 -> set it to 21 for a gluon
  if ( jetFlavour.mainFlavour == 21 && jetFlavour.heaviestFlavour == 0 ) {
    jetFlavour.heaviestFlavour(jetFlavour.mainFlavour);
    jetFlavour.pHeaviestParton(jetFlavour.pMainParton);
  }

  // determine the origin flavour: the source of the hardest one
  jetFlavour.originFlavour(jetFlavour.mainOrigFlavour);

//  cout << "CW JetIdentifier : " << jetFlavour.originFlavour << " , " << jetFlavour.mainFlavour << " , "  << jetFlavour.numberOfSources << " , " << endl;
  if ( nInitialPartons > 1 ) {
    //    cout << "CW JetIdentifier : reset initialFlavour because > 1 initial partons : " << nInitialPartons << endl;
    jetFlavour.initialFlavour(0);
  }
  
//CW  cout << "CW JetIdentifier initial flavour  = "
//CW       << jetFlavour.initialFlavour << " , "
//CW       << theJet.getLorentzVector() << endl;
  

}
