#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "RecoBTag/MCTools/interface/MCParticleInfo.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include <Math/GenVector/VectorUtil.h>
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/LorentzVector.h"

using namespace edm;
using namespace std;
using namespace HepMC;
using namespace reco;
using namespace math;

JetFlavourIdentifier::JetFlavourIdentifier(const edm::ParameterSet& iConfig)
{
  fillPartons =  iConfig.getParameter<bool>("fillPartons");
  fillHeavyHadrons =  iConfig.getParameter<bool>("fillHeavyHadrons");
  fillLeptons =  iConfig.getParameter<bool>("fillLeptons");
  coneSizeToAssociate =  iConfig.getParameter<double>("coneSizeToAssociate");
  physDefinition =  iConfig.getParameter<bool>("physicsDefinition");
  rejectBCSplitting =  iConfig.getParameter<bool>("rejectBCSplitting");
  vector<string> veto = iConfig.getParameter< vector<string> >("vetoFlavour");
  vetoB = false;vetoC = false;vetoL = false;vetoG = false;
  for (vector<string>::iterator iVeto = veto.begin(); iVeto!=veto.end(); ++iVeto)
  {
    if (*iVeto=="B") vetoB = true;
    if (*iVeto=="C") vetoC = true;
    if (*iVeto=="UDS") vetoL = true;
    if (*iVeto=="G") vetoG = true;
  }
}

void JetFlavourIdentifier::readEvent(const edm::Event& iEvent, std::string label_)
{
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);
  
  HepMC::GenEvent * generated_event = new HepMC::GenEvent(*(evt->GetEvent()));
  fillInfo(generated_event);
}


void JetFlavourIdentifier::fillInfo ( const HepMC::GenEvent * event ) {

//   HeavyHadrons.erase( HeavyHadrons.begin() , HeavyHadrons.end() ) ;
  m_partons.clear();
//   Leptons.erase( Leptons.begin() , Leptons.end() ) ;

  // print RawHepEvent in Debug mode
//    { event->print(); }

  // one loop to find out the index of the last parton
  const HepMC::GenParticle* lastParton = event->particle(event->particles_size()-1) ;
  HepMC::GenEvent::particle_const_iterator p;
  for (p = event->particles_begin(); p != event->particles_end(); ++p) {
    // decode particle info from Lund Code
    MCParticleInfo testCode( (*p)->pdg_id() ); 
//     cout << "pid: "<<(*p)->pdg_id()<<" barcode "<<(*p)->barcode()<<endl;
    if ( testCode.isParton() ) {
      lastParton = *p;
    }
  }
//   cout << "====>>>>> JetFlavourIdentifier::fillInfo : index of last parton : " << lastParton->barcode() << endl ;

  
  
  // loop over all generated particles and retrieve information
  //

  for (p = event->particles_begin(); p != event->particles_end(); ++p) {

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
	 MCParton aParton ( *p, event , lastParton ) ;
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

JetFlavour  JetFlavourIdentifier::identifyBasedOnPartons (const Jet & theJet) const
{
  JetFlavour jetFlavour = basicIdentityBasedOnPartons(theJet.p4(), coneSizeToAssociate);//Hep3Vector(theJet.p4().x(),theJet.p4().y(),theJet.p4().z())
  if (physDefinition) fillPhysicsDefinition(jetFlavour, theJet.p4());
    else fillAlgorithmicDefinition(jetFlavour);
  if ( jetFlavour.flavour()==5   && vetoB ) jetFlavour.flavour(0);
  if ( jetFlavour.flavour()==4   && vetoC ) jetFlavour.flavour(0);
  if ( jetFlavour.flavour()==1   && vetoL ) jetFlavour.flavour(0);
  if ( jetFlavour.flavour()==2   && vetoL ) jetFlavour.flavour(0);
  if ( jetFlavour.flavour()==3   && vetoL ) jetFlavour.flavour(0);
  if ( jetFlavour.flavour()==21  && vetoG ) jetFlavour.flavour(0);
  return jetFlavour;
}


JetFlavour  JetFlavourIdentifier::basicIdentityBasedOnPartons
	(const XYZTLorentzVector & jet4Vec, const double coneSize) const
{
  JetFlavour jetFlavour;
  // to count partons from how many sources
  jetFlavour.numberOfSources(0);
  vector<const GenParticle *> indicesSources;
  int nDown = 0, nUp = 0, nStrange = 0, nCharm = 0, nBottom = 0, nGluon = 0;
  // kinematics of underlying parton by summing them up
  
  // don't assign initial parton if more than one in cone
  int nInitialPartons = 0;
  
  for ( vector<MCParton>::const_iterator itP  = m_partons.begin();
	                           itP != m_partons.end(); itP++ ) {

    // get Lundcode
    int theFlavour = abs ( itP->flavour() );
    // get mother status
    // int motherStatus = itP->motherStatusPythia();
    // get kinemat. info
    XYZTLorentzVector partonMomentum = itP->fourVector();

    // for initial partons: use the resummed daughters
    if ( itP->isInitialParton() ) partonMomentum = itP->summedDaughterMomentum();
    //
    double deltaR     = ROOT::Math::VectorUtil::DeltaR(jet4Vec, partonMomentum);
    double pAbsParton = partonMomentum.P();
    
    //CW match also to the initial partons for the 'physics definition'
    if ( itP->isInitialParton() ) {
      if ( deltaR < coneSize ) {   // is in cone
	jetFlavour.initialFlavour(theFlavour);
	nInitialPartons++;
	jetFlavour.vec4OriginParton(jetFlavour.vec4OriginParton() + itP->summedDaughterMomentum());
	// reset if it has other HF nearby (from other sources!!)
	if ( itP->initialPartonHasCloseHF() ) jetFlavour.initialFlavour(0);
	// check if it has split to heavy flavours
	jetFlavour.initialPartonSplitsToC(itP->splitsToC());
	jetFlavour.initialPartonSplitsToB(itP->splitsToB());
      }
    }
    
    // final parton? (asking both final parton and from primary process removes remnant/underlying event)
    //CW    if ( itP->isFinalParton() && itP->fromPrimaryProcess() ) {
    if ( itP->isFinalParton() ) {
      
      if ( deltaR < coneSize ) {   // is in cone
	//
	// set members
	//
// cout <<"Got "<< itP->fourVector().px()<< " , "  
//     << itP->fourVector().py() << " , " << itP->fourVector().pz() << " , "<<itP->fourVector().eta()<<" , "<< itP->fourVector().phi()<<" , "<< itP->flavour()<<endl;
	// sum up parton 4-momenta
	jetFlavour.vec4SummedPartons(jetFlavour.vec4SummedPartons()+itP->fourVector());

	// origin source of this parton
	int origSourceCode  = abs ( itP->motherLundCode() );
	// check if this source is already there
	bool sourceAlreadyThere = false;
	for ( vector<const GenParticle *>::const_iterator itS = indicesSources.begin(); itS != indicesSources.end(); itS++ ) {
	  if ( *itS == itP->mother() ) sourceAlreadyThere = true;
	}
	// add the new source if not there
	if ( !sourceAlreadyThere ) indicesSources.push_back(itP->mother());
// 	cout << jetFlavour.heaviestFlavour()<<" ";
	
	
	// main flavour is given to Parton with highest |p|
	if ( pAbsParton     > jetFlavour.pMainParton()          ) {
	  jetFlavour.mainFlavour     (theFlavour);//cout <<"P "<<jetFlavour.pMainParton()<<endl;
	  jetFlavour.pMainParton     (pAbsParton);
	  jetFlavour.vec4MainParton  (itP->fourVector());	  
	  jetFlavour.deltaRMainParton(deltaR);
	  jetFlavour.mainOrigFlavour (origSourceCode);
	}
	
	// Def.: gluon (21) is lighter than quarks!!
	if ( theFlavour > jetFlavour.heaviestFlavour()  &&  theFlavour != 21  ) {
	  jetFlavour.pHeaviestParton   (pAbsParton);	  
	  jetFlavour.vec4HeaviestParton(itP->fourVector());	  
	  jetFlavour.heaviestFlavour   (theFlavour);
	}
// 	cout << jetFlavour.heaviestFlavour()<<" ";
	
	if ( deltaR     < jetFlavour.deltaRClosestParton() ) {
	  jetFlavour.minimumDeltaRFlavour(theFlavour);
	  jetFlavour.pClosestParton      (pAbsParton);	  
	  jetFlavour.vec4ClosestParton   (itP->fourVector());	  
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

// 	cout << jetFlavour.heaviestFlavour()<<" ";

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
  if ( jetFlavour.mainFlavour() == 21 && jetFlavour.heaviestFlavour() == 0 ) {
    jetFlavour.heaviestFlavour(jetFlavour.mainFlavour());
    jetFlavour.pHeaviestParton(jetFlavour.pMainParton());
  }

  // determine the origin flavour: the source of the hardest one
  jetFlavour.originFlavour(jetFlavour.mainOrigFlavour());

//  cout << "CW JetIdentifier : " << jetFlavour.originFlavour() << " , " << jetFlavour.mainFlavour() << " , "  << jetFlavour.numberOfSources() << " , " << endl;
  if ( nInitialPartons > 1 ) {
    //    cout << "CW JetIdentifier : reset initialFlavour because > 1 initial partons : " << nInitialPartons << endl;
    jetFlavour.initialFlavour(0);
  }
  
//  cout << "CW JetIdentifier initial flavour  = "
//       << jetFlavour.initialFlavour() << " , "<< jetFlavour.heaviestFlavour()<<"\n ";

  return jetFlavour;
}

void JetFlavourIdentifier::fillAlgorithmicDefinition(JetFlavour & jetFlavour) const
{
  // if the heaviest flavour is a b or c, give that one
  if ( jetFlavour.heaviestFlavour() == 5 || jetFlavour.heaviestFlavour() == 4 ) {
    jetFlavour.flavour(jetFlavour.heaviestFlavour() );
    jetFlavour.underlyingParton4Vec(jetFlavour.vec4SummedPartons() );
  }
  else {
    // take the main parton
    jetFlavour.flavour(jetFlavour.mainFlavour() );
    jetFlavour.underlyingParton4Vec(jetFlavour.vec4SummedPartons() );
  }
}

void JetFlavourIdentifier::fillPhysicsDefinition(JetFlavour & jetFlavour,
	const math::XYZTLorentzVector & jet4Vec) const
{
  int flavour = jetFlavour.initialFlavour() ;
  // clean: do not accept if final state heavy partons from many sources -> take bigger cone here
  JetFlavour phIdentifierPartons = basicIdentityBasedOnPartons(jet4Vec, 0.7);

  // if there is heavy flavour content from another source we reject it 
  bool resetJetFlavour = false ;
  int maxHF = 0 ;
  if ( phIdentifierPartons.numberOfSources() > 1 ) {
    vector<int> sources = jetFlavour.flavourSources() ;
    for ( vector<int>::const_iterator itS = sources.begin() ; itS != sources.end() ; itS++ ) {
      if ( *itS == 4  ||  *itS == 5 ) {
	resetJetFlavour = true ;
	if ( *itS > maxHF ) maxHF = *itS ; 
      }
    }
  }
  if ( resetJetFlavour ) {
    cout << "DefaultJEtIdentifier::mcJetInfo : will reset jet flavour = "
	 << flavour
	 << " because of heavy flavour content from other sources: "
	 << maxHF << endl ;
    flavour = 0 ;
  }

  // if wanted, we reject the ones that have splitted into heavy flavours
  if ( rejectBCSplitting ) {
    if ( jetFlavour.initialPartonSplitsToC() || jetFlavour.initialPartonSplitsToB() ) {
      cout << "CW: reject initial parton because of b/c splitting" << endl ;
      flavour = 0 ;
    }
  }

  jetFlavour.flavour(flavour);
  jetFlavour.underlyingParton4Vec(jetFlavour.vec4OriginParton() );
}
