/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: SingleTopElectronProducer.cc,v 1.5 2010/11/08 08:26:42 oiorio Exp $ 
 */

// Single Top producer: produces a top candidate made out of a Lepton, a B jet and a MET

#include "PhysicsTools/PatAlgos/plugins/PATJetProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"

#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"


#include "FWCore/Framework/interface/Selector.h"

#include "TopQuarkAnalysis/SingleTop/interface/SingleTopElectronProducer.h"


#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "PhysicsTools/SelectorUtils/interface/SimpleCutBasedElectronIDSelectionFunctor.h"


#include <vector>
#include <memory>

#include "DataFormats/Math/interface/LorentzVector.h"


//using namespace pat;


SingleTopElectronProducer::SingleTopElectronProducer(const edm::ParameterSet& iConfig)
{
  src_                 = iConfig.getParameter<edm::InputTag>( "src" );
  cut_ = iConfig.getParameter< std::string >("cut"); 
  isData_ = iConfig.getUntrackedParameter<bool>("isData",false); 
  id_ = iConfig.getParameter<std::string> ("id"); 
  useConversionVeto_ = iConfig.getUntrackedParameter<bool>("useConversionVeto",true);
  useVertexVeto_ = iConfig.getUntrackedParameter<bool>("useVertexVeto",true);
  
  produces<std::vector<pat::Electron> >();
}

void SingleTopElectronProducer::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){


  ////std::cout << " mark 0 " << std::endl;

  ////std::cout << " mark 1 " << std::endl;
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(src_,electrons);

  Selector cut(cut_);
  std::vector< pat::Electron > * finalElectrons = new std::vector<pat::Electron>;

  ////std::cout << " mark 2 " << std::endl;

  edm::Handle<reco::TrackCollection> ctfTracks;
  iEvent.getByLabel("generalTracks",ctfTracks);  

  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByLabel("offlinePrimaryVertices",vertices);
  //iEvent.getByLabel("PVFilterProducer",vertices);

  ////std::cout << " mark 3 " << std::endl;
  double bfield = 3.8;

  if(isData_){
    edm::Handle<DcsStatusCollection> dcsHandle;
    //    iEvent.getByLabel(dcsTag_,dcsHandle);
    iEvent.getByLabel("scalersRawToDigi",dcsHandle);
    float currentScaleFactor =2.09237036221512717e-04;
    float current = (*dcsHandle)[0].magnetCurrent();
    bfield = current*currentScaleFactor;
  }
  else{
  edm::ESHandle<MagneticField> magneticField;
  iEventSetup.get<IdealMagneticFieldRecord>().get(magneticField);
  bfield = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
  }


  bool bit0 = true;

  for(size_t i = 0; i < electrons->size(); ++i){
    
    if(id_==std::string("cIso70")){
      //      SimpleCutBasedElectronIDSelectionFunctor patSele(SimpleCutBasedElectronIDSelectionFunctor::cIso70,bfield,ctfTracks);
      //      patSele.set("cIso_EB",100000.);
      // patSele.set("cIso_EE",100000.);
      // patSele.set("conversionRejection",0);
      // patSele.set("maxNumberOfExpectedMissingHits",11);
      //bit0 = bool(patSele(electrons->at(i)));
      int eid = (int) electrons->at(i).electronID("simpleEleId70cIso");
      bool hadId(eid & 0x1) ;
      bit0 = hadId;
    }
    else if(id_ == std::string("cIso95")){
      //SimpleCutBasedElectronIDSelectionFunctor patSele(SimpleCutBasedElectronIDSelectionFunctor::cIso95,bfield,ctfTracks);
      //patSele.set("cIso_EB",100000.);
      //patSele.set("cIso_EE",100000.);
      //patSele.set("conversionRejection",0);
      //patSele.set("maxNumberOfExpectedMissingHits",11);
      //bit0 = bool(patSele(electrons->at(i)));
      int eid = (int) electrons->at(i).electronID("simpleEleId95cIso");
      bool hadId(eid & 0x1) ;
      bit0 = hadId;
    }else if(id_ == std::string("antiIso")){
    SimpleCutBasedElectronIDSelectionFunctor patSele(SimpleCutBasedElectronIDSelectionFunctor::cIso95,bfield,ctfTracks);
    //    std::cout << "is anti iso"  <<std::endl;
    patSele.set("cIso_EB",100000.);
    patSele.set("cIso_EE",100000.);
    patSele.set("conversionRejection",0);
    bit0 = bool(patSele(electrons->at(i)));

  }
    ConversionFinder conv;
    ConversionInfo convInfo= conv.getConversionInfo(electrons->at(i),ctfTracks,bfield);
    
    double dist = convInfo.dist();
    double dcot = convInfo.dcot();
    //std::cout << " bit 0 producer "<< bit0 << " use conversion "<< useConversionVeto_ << " conversion " << (fabs(dist)<0.02&&fabs(dcot<0.02)) << std::endl;

    if(!cut(electrons->at(i)))continue; 
    
    //std::cout << " passes cut " << cut_ <<  std::endl;
    
    if(!bit0) continue;
    if(useConversionVeto_ && (fabs(dist) < 0.02 && fabs(dcot) < 0.02))continue;

    bool goodVertexDist = true;
    for(size_t v = 0; v < vertices->size(); ++v){
      if(electrons->at(i).vertex().z()-vertices->at(0).z()>=1){goodVertexDist = false;break;}
    }
    if (useVertexVeto_ && !goodVertexDist)continue;
    //std::cout << " pass all "<< std::endl;
    finalElectrons->push_back(electrons->at(i));
   
    
  } 
 
  ////std::cout << " mark 7 " << std::endl;

std::auto_ptr< std::vector< pat::Electron > > finalElectronsPtr(finalElectrons);
 

iEvent.put(finalElectronsPtr);

}

SingleTopElectronProducer::~SingleTopElectronProducer(){;}
DEFINE_FWK_MODULE(SingleTopElectronProducer);
