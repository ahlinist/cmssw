/** class 
 *
 * \author Stephen Mrenna, FNAL
 *
 * \version $Id: VstParticleCandidateSelector.cc,v 1.0
 *
 */
#include "FWCore/Framework/interface/EDProducer.h"
#include "SimGeneral/HepPDTRecord/interface/PdtEntry.h"
#include <string>
#include <vector>
#include <set>

namespace { const char electronCollection[]="electronVst"; 
const char muonCollection[]="muonVst"; 
const char tauCollection[]="tauVst"; 
const char photonCollection[]="photonVst"; 
const char partonShowerCollection[]="partonShowerVst"; 
const char otherStableCollection[]="otherStableVst"; 
}

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

class VstParticleCandidateSelector : public edm::EDProducer {
 public:
  /// constructor
  VstParticleCandidateSelector( const edm::ParameterSet & );
  /// destructor
  ~VstParticleCandidateSelector();

 private:
  /// vector of strings
  typedef std::vector<PdtEntry> vpdt;
  /// module init at begin of job
  void beginJob( const edm::EventSetup & );
  /// process one event
  void produce( edm::Event& e, const edm::EventSetup& );
  /// source collection name  
  edm::InputTag src_;
  //  std::string src_;
  // selects partons (HEPEVT status = 2) and daughter is a string/cluster 
  //bool partons_;
  /// name of particles in include or exclude list
  vpdt pdtList_;
  /// using include list?
  bool bInclude_;
  // selects only stable particles (HEPEVT status = 1)
  bool stableOnly_;
  /// output string for debug
  std::string caseString_;
      /// 

  /// set of excluded particle id's
  std::set<int> pIds_;
  double ptMinParticle_;
  double ptMinShower_;
  double etaMaxParticle_;
  double etaMaxShower_;
  /// verbose flag
  bool verbose_;
  bool unDoLeptonRadiation;
};


#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <fstream>
using namespace edm;
using namespace reco;
using namespace std;

VstParticleCandidateSelector::VstParticleCandidateSelector( const ParameterSet & p ) :
  src_( p.getParameter<InputTag>( "src" ) ),
  bInclude_(0),
  stableOnly_( p.getParameter<bool>( "stableOnly" ) ),
  ptMinParticle_( p.getUntrackedParameter<double>( "ptMinParticle" ,0.5 )),  
  ptMinShower_( p.getUntrackedParameter<double>( "ptMinShower" ,1.0 )),  
  etaMaxParticle_( p.getUntrackedParameter<double>( "etaMaxParticle" ,5.0 )),  
  etaMaxShower_( p.getUntrackedParameter<double>( "etaMaxShower" ,5.0 )),  
  verbose_( p.getUntrackedParameter<bool>( "verbose" ) ),
  unDoLeptonRadiation(false)
{

  produces<CandidateCollection>(electronCollection);
  produces<CandidateCollection>(muonCollection);
  produces<CandidateCollection>(tauCollection);
  produces<CandidateCollection>(photonCollection);
  produces<CandidateCollection>(partonShowerCollection);
  produces<CandidateCollection>(otherStableCollection);

  //check optional parameters includeList and excludeList
  const std::string excludeString("excludeList");
  const std::string includeString("includeList");
  vpdt includeList, excludeList;

  vector<string> vPdtParams = p.getParameterNamesForType<vpdt>();
  // check for include list
  bool found = std::find( vPdtParams.begin(), vPdtParams.end(), includeString) != vPdtParams.end();
  if ( found ) includeList = p.getParameter<vpdt>( includeString );
  // check for exclude list
  found = std::find( vPdtParams.begin(), vPdtParams.end(), excludeString) != vPdtParams.end();
  if ( found ) excludeList = p.getParameter<vpdt>( excludeString );

  vector<string> boolParams = p.getParameterNamesForType<bool>();
  const std::string leptonRadiation("unDoLeptonRadiation");
  
  found = std::find( boolParams.begin(), boolParams.end(), leptonRadiation ) != boolParams.end();

  if ( found ) unDoLeptonRadiation = p.getParameter<bool>( leptonRadiation );


  // checking configuration cases
  bool bExclude(0);
  if ( includeList.size() > 0 ) bInclude_ = 1;
  if ( excludeList.size() > 0 ) bExclude = 1;

  if ( bInclude_ && bExclude ) {
    throw cms::Exception( "ConfigError", "not allowed to use both includeList and excludeList at the same time\n");
  }
  else if ( bInclude_ ) {
    caseString_ = "Including";
    pdtList_ = includeList;
  }
  else {
    caseString_ = "Excluding";
    pdtList_ = excludeList;
  }

}

VstParticleCandidateSelector::~VstParticleCandidateSelector() { 
}

void VstParticleCandidateSelector::beginJob( const EventSetup & es ) {
  //  const PDTRecord & rec = es.get<PDTRecord>();
  ESHandle<ParticleDataTable> pdt;
  es.getData( pdt );
  if ( verbose_ && stableOnly_ )
    LogInfo ( "Vista INFO" ) << "Excluding unstable particles";
  for( vpdt::iterator p = pdtList_.begin(); 
       p != pdtList_.end(); ++ p ) {
    p->setup( es );
    LogInfo ( "Vista INFO" ) << caseString_ <<" particle " << p->name() << ", id: " << p->pdgId();
    pIds_.insert( abs( p->pdgId() ) );
  }
}

void VstParticleCandidateSelector::produce( Event& evt, const EventSetup& ) {
  //  Handle<CandidateCollection> particles;
  // Change 1.
  Handle<GenParticleCollection> particles;
  evt.getByLabel( src_, particles );
  //  auto_ptr<CandidateCollection> cands( new CandidateCollection );
  //  cands->reserve( particles->size() );
  size_t idx = 0;

  auto_ptr<CandidateCollection> muonVst( new CandidateCollection );
  auto_ptr<CandidateCollection> electronVst( new CandidateCollection );
  auto_ptr<CandidateCollection> tauVst( new CandidateCollection );
  auto_ptr<CandidateCollection> photonVst( new CandidateCollection );
  auto_ptr<CandidateCollection> partonShowerVst( new CandidateCollection );
  auto_ptr<CandidateCollection> otherStableVst( new CandidateCollection );


// map the pointers
  std::vector<const reco::Candidate *> cands;
  std::vector<const Candidate *>::const_iterator found = cands.begin();
  for(GenParticleCollection::const_iterator p = particles->begin();
      p != particles->end(); ++ p) {
     cands.push_back(&*p);
  }



  //  for( CandidateCollection::const_iterator p = particles->begin();
  // Change 2.
  for( GenParticleCollection::const_iterator p = particles->begin(); 
       p != particles->end(); ++ p, ++ idx ) {
    int idabs = abs( p->pdgId() );
    int nMo = p->numberOfMothers();
    int nDa = p->numberOfDaughters();

    //	CandidateBaseRef ref( CandidateRef( particles, idx ) );
    //	cands->push_back( new ShallowCloneCandidate( ref ) );

    if( p->status()==1 && p->pt()>ptMinParticle_ && fabs(p->eta())<etaMaxParticle_ ) {
      // Change 3.
      // CandidateBaseRef ref( CandidateRef( particles, idx ) );
      CandidateBaseRef ref( GenParticleRef( particles, idx ) );
      if( idabs == 11 || idabs == 13) {

         if(unDoLeptonRadiation && nMo>0) {

            size_t c=idx;
            found = find(cands.begin(), cands.end(), p->mother(0));
            if(found != cands.end()) c = found - cands.begin() ;
            CandidateBaseRef tref( GenParticleRef( particles, c ) );
            
//            if( tref->pdgId() == p->pdgId() && tref->status()== 3 ) ref = tref;
            if( tref->pdgId() == p->pdgId() && tref->status()>= 3 ) ref = tref;

         }



//         if( nMo>0 && ( p->mother(0)->status()==3 || abs(p->mother(0)->pdgId())==idabs ) ) {
         if( nMo>0 && ( p->mother(0)->status()>=3 || abs(p->mother(0)->pdgId())==idabs ) ) {
            if( idabs == 11 ) {
               electronVst->push_back( new ShallowCloneCandidate( ref ) );
            } else {
               muonVst->push_back( new ShallowCloneCandidate( ref ) );
            }
         } else if ( nMo>0 && abs(p->mother(0)->pdgId())==15 ) {
            const reco::Candidate *ptau = p->mother(0);
            if( ptau->numberOfMothers() > 0 ) {
               const reco::Candidate *pmom = ptau->mother(0);
               int amomId = abs(pmom->pdgId());
               if( (amomId>400 && amomId<600) || (amomId>4000 && amomId<6000) ) {
                  otherStableVst->push_back( new ShallowCloneCandidate( ref ) );
               } else {
                  if( idabs == 11 ) {
                     electronVst->push_back( new ShallowCloneCandidate( ref ) );
                  } else {
                     muonVst->push_back( new ShallowCloneCandidate( ref ) );
                  }
               }
               
            } else {
               otherStableVst->push_back( new ShallowCloneCandidate( ref ) );
            }
         } else {
            otherStableVst->push_back( new ShallowCloneCandidate( ref ) );
         }
	// I am not satisfied with this 
	// probably won't work for tauola
	// probably won't work if rho->pi pi in tau decay products
	//      } else if ( (nMo>0 && abs(p->mother(0)->pdgId() )==15) && idabs!=12 && idabs!=14 && idabs!=16) {
	//	tauVst->push_back( new ShallowCloneCandidate( ref ) );
      } else if ( idabs==22 ) {

//         if( p->mother(0)->status()==3 || abs(p->mother(0)->pdgId())==11  || abs(p->mother(0)->pdgId())==13 || 
         if( p->mother(0)->status()>=3 || abs(p->mother(0)->pdgId())==11  || abs(p->mother(0)->pdgId())==13 || 
             abs(p->mother(0)->pdgId())==15 || abs(p->mother(0)->pdgId())<7) {
            if( !unDoLeptonRadiation ) photonVst->push_back( new ShallowCloneCandidate( ref ) );
         } else {
            otherStableVst->push_back( new ShallowCloneCandidate( ref ) );
         }
      } else if( idabs!=12 && idabs!=14 && idabs!=16) {
         otherStableVst->push_back( new ShallowCloneCandidate( ref ) );
      }
//    } else if ( p->status() == 2 ) {
    } else if ( p->status() == 2 || ( p->status()>70 && p->status()<80) ) {
       if ( nDa > 0 && ( p->daughter(0)->pdgId() == 91 || p->daughter(0)->pdgId() == 92 ||
                          p->daughter(0)->pdgId() == 93 || p->status()>70 ) ) {
          if(p->pt() > ptMinShower_ && fabs(p->eta())<etaMaxShower_) {
	  //          CandidateBaseRef ref( CandidateRef( particles, idx ) );
             CandidateBaseRef ref( GenParticleRef( particles, idx ) );
             partonShowerVst->push_back( new ShallowCloneCandidate( ref ) );
          }
       }
    }
    
  }

  evt.put( electronVst,electronCollection);
  evt.put( muonVst,muonCollection);
  evt.put( tauVst,tauCollection );
  evt.put( photonVst,photonCollection );
  evt.put( partonShowerVst,partonShowerCollection );
  evt.put( otherStableVst,otherStableCollection );
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( VstParticleCandidateSelector );
