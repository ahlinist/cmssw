//////////////////////////
//  Analyzer by Nicola  //
//  oct 2010 @ Padova   //
//////////////////////////

/////////////////////////
//       HEADERS       //
/////////////////////////

////////////////
// CLASS HEADER
// No more necessary in the current "no *.h file" implementation

////////////////////
// FRAMEWORK HEADERS
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
//
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

///////////////////////
// DATA FORMATS HEADERS
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h" 
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h" 
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSMatchedRecHit2DCollection.h"
//
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
//
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h" 
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h" 
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/Ref.h"
//
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
//
#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"
#include "SimDataFormats/SLHC/interface/LocalStub.h"
//
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
//
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
//
#include "SimDataFormats/SLHC/interface/L1CaloCluster.h"
//
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
//
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
//
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"
////////////////////////
// FAST SIMULATION STUFF
#include "FastSimulation/Particle/interface/RawParticle.h"
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "SimGeneral/HepPDTRecord/interface/PDTRecord.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimGeneral/HepPDTRecord/interface/PdtEntry.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"

////////////////////////////
// DETECTOR GEOMETRY HEADERS
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelTopologyBuilder.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
//
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetUnit.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetId.h"

////////////////
// PHYSICS TOOLS
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "RecoTracker/TkSeedGenerator/interface/FastHelix.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "RecoTauTag/TauTagTools/interface/GeneratorTau.h"
//
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementVector.h"
#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "SLHCUpgradeSimulations/Utilities/interface/constants.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/CircleFit.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/LineFit.h"
#include "SimDataFormats/SLHC/interface/L1Track.h"

///////////////
// ROOT HEADERS
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TH2.h>
#include <TH1.h>

//////////////
// STD HEADERS
#include <memory>
#include <string>
#include <iostream>

//////////////
// NAMESPACES
// I hate them, but there is something
// magic behind and beyond ...
using namespace std;
using namespace edm;
using namespace reco;
using namespace cmsUpgrades;
using namespace l1slhc;
using namespace l1extra;

//////////////////////////////
//                          //
//     CLASS DEFINITION     //
//                          //
//////////////////////////////

class TTree;
class TFile;
class TH1D;
class TH2D;
class TGraph;
class RectangularPixelTopology;
class TransientInitialStateEstimator;
class MagneticField;
class TrackerGeometry;
class TrajectoryStateOnSurface;
class PTrajectoryStateOnDet;
//
class TestL1PrimComp : public edm::EDAnalyzer
{
  /// Public methods
  public:
    /// Constructor/destructor
    explicit TestL1PrimComp(const edm::ParameterSet& conf);
    virtual ~TestL1PrimComp();
    // Typical methods used on Loops over events
    virtual void beginJob(const edm::EventSetup& es);
    virtual void endJob();
    virtual void analyze(const edm::Event& e, const edm::EventSetup& es);

    /// Some Type definitions
    typedef edm::Ptr< GlobalStub<Ref_PixelDigi_> > GlobalStubPtrType;
    typedef std::set< std::pair<unsigned int , GlobalStubPtrType> > TrackletMap;

    typedef edm::Ptr< Tracklet<Ref_PixelDigi_> > TrackletPtrType;
    typedef edm::Ptr< L1Track<Ref_PixelDigi_> > L1TrackPtrType;

    /// Define the type
    
  /// Protected methods only internally used
  protected:
    /// EtaPhi space
    double deltaPhi( double phi1, double phi2 );
    double deltaPhiNP( double phi1, double phi2 );
    double deltaEta( double eta1, double eta2 );
    double deltaR( double phi1, double eta1, double phi2, double eta2 );

    std::pair<double,PSimHit> makeHit( const GeomDetUnit* dU, BaseParticlePropagator* tP,
                                       FreeTrajectoryState hV, int pID, int tkID );

                     
  /// Private methods and variables
  private:

    int PDGCode;
    double lumiZ;
    bool testedGeometry;
    bool beamspot00;
    int seedsuperlayer;
    int numberstubs;

    /// TO CHECK ALL EVENTS ARE PROCESSED
    TH1D* h_EvtCnt;
    TH1D* hTrk_ST_Pt_d;
    TH1D* hTrk_ST_Pz_d;
    TH1D* hTrk_ST_Pt_rd;
    TH1D* hTrk_ST_Pt_rd_1;
    TH1D* hTrk_ST_Pz_rd_1;
    TH1D* hTrk_ST_e_d;
    TH1D* hTrk_ST_p_d;
    TH1D* hTrk_ST_zvtx_d;
    TH1D* hTrk_ST_q_d;
    TH2D* hTrk_ST_Pt_d_ST_e;
    TH2D* hTrk_ST_Pz_d_ST_e;
    TH2D* hTrk_ST_Pt_rd_ST_e;
    TH2D* hTrk_ST_Pt_rd_1_ST_e;
    TH2D* hTrk_ST_Pz_rd_1_ST_e;

    TH2D* hTrk_ST_e_d_ST_e;
    TH2D* hTrk_ST_p_d_ST_e;
    TH2D* hTrk_ST_zvtx_d_ST_e;
    TH2D* hTrk_ST_q_d_ST_e;

    /// Containers of parameters passed by python
    /// configuration file
    edm::ParameterSet config;

    /// Geometry handles etc
    edm::ESHandle<TrackerGeometry> geometryESH;
    const TrackerGeometry*  theGeometry;
    edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> stackedgeometryESH;
    const cmsUpgrades::StackedTrackerGeometry*  theStackedGeometry;

    struct store_hits {
      int origindoublestack;
      int targetlayer;
      int targetiphisector;
      int targetizsector;
      PSimHit hit;
    };
    typedef store_hits StoreHits;

};

//////////////////////////////////
//                              //
//     CLASS IMPLEMENTATION     //
//                              //
//////////////////////////////////

//////////////
// CONSTRUCTOR
TestL1PrimComp::TestL1PrimComp(edm::ParameterSet const& conf) : 
  config(conf)
{
  /// Insert here what you need to initialize
}

/////////////
// DESTRUCTOR
TestL1PrimComp::~TestL1PrimComp()
{
  /// Insert here what you need to delete
  /// when you close the class instance
}  

//////////
// END JOB
void TestL1PrimComp::endJob() 
{
  /// Things to be done at the exit of the event Loop
  std::cout << " TestL1PrimComp::endJob" << std::endl;
  /// End of things to be done at the exit from the event Loop
}

////////////
// BEGIN JOB
void TestL1PrimComp::beginJob(const edm::EventSetup& es)
{
  /// Initialize all slave variables
  /// mainly histogram ranges and resolution
  double trkMPt = 99999.9;
  int trkBn = 1;
  double trkltMPt = 99999.9;
  int trkltBn = 1;

  double rBnW = 0.087/2.0;
  int rBn = 40;

  PDGCode = 0;
  lumiZ = 10.0;
  beamspot00 = true;
  
  testedGeometry = false;
  
  seedsuperlayer = 0;
  numberstubs = -9;
  
  /// Select the Superlayer you want to check
  if ( config.getParameter<int>("seedSuperLayer") >= 0  ) seedsuperlayer = config.getParameter<int>("seedSuperLayer");
  /// Select the number of Stubs of the L1Track
  //if ( config.getParameter<int>("numberStubs") >= 0  ) numberstubs = config.getParameter<int>("numberStubs");
  /// Select the kind of Tracklets you want to handle
  if ( config.getParameter<string>("trackletVTX") == "offcenter" ) beamspot00 = false;

  /// Things to be done before entering the event Loop
  std::cout << " TestL1PrimComp::beginJob" << std::endl;

  /// Book histograms etc
  edm::Service<TFileService> fs;
  h_EvtCnt = fs->make<TH1D>( "h_EvtCnt", "MC Tau Decay", 1000, 0, 1000 );
  h_EvtCnt->Sumw2();

  hTrk_ST_Pt_d = fs->make<TH1D>( "hTrk_ST_Pt_d", "#Delta p_{T}", 201, -5.025, 5.025 );
  hTrk_ST_Pz_d = fs->make<TH1D>( "hTrk_ST_Pz_d", "#Delta p_{z}", 201, -5.025, 5.025 );
  hTrk_ST_Pt_rd = fs->make<TH1D>( "hTrk_ST_Pt_rd", "#Delta p_{T}/p_{T}", 201, -5.025, 5.025 );
  hTrk_ST_Pt_rd_1 = fs->make<TH1D>( "hTrk_ST_Pt_rd_1", "#Delta p_{T}/p_{T}, #eta>1", 201, -5.025, 5.025 );
  hTrk_ST_Pz_rd_1 = fs->make<TH1D>( "hTrk_ST_Pz_rd_1", "#Delta p_{z}/p_{z}, #eta>1", 201, -5.025, 5.025 );

  hTrk_ST_e_d = fs->make<TH1D>( "hTrk_ST_e_d", "#Delta #eta", 201, -0.1005, 0.1005 );
  hTrk_ST_p_d = fs->make<TH1D>( "hTrk_ST_p_d", "#Delta #phi", 201, -0.1005, 0.1005 );
  hTrk_ST_zvtx_d = fs->make<TH1D>( "hTrk_ST_zvtx_d", "#Delta z_{vtx}", 201, -10.05, 10.05 );
  hTrk_ST_q_d = fs->make<TH1D>( "hTrk_ST_q_d", "#Delta q", 21, -10.5, 10.5 );

  hTrk_ST_Pt_d_ST_e = fs->make<TH2D>( "hTrk_ST_Pt_d_ST_e", "#Delta p_{T} vs #eta", 300, -3, 3, 201, -5.025, 5.025 );
  hTrk_ST_Pz_d_ST_e = fs->make<TH2D>( "hTrk_ST_Pz_d_ST_e", "#Delta p_{z} vs #eta", 300, -3, 3, 201, -5.025, 5.025 );
  hTrk_ST_Pt_rd_ST_e = fs->make<TH2D>( "hTrk_ST_Pt_rd_ST_e", "#Delta p_{T}/p_{T} vs #eta", 300, -3, 3, 201, -5.025, 5.025 );
  hTrk_ST_Pt_rd_1_ST_e = fs->make<TH2D>( "hTrk_ST_Pt_rd_1_ST_e", "#Delta p_{T}/p_{T} vs #eta, #eta>1", 300, -3, 3, 201, -5.025, 5.025 );
  hTrk_ST_Pz_rd_1_ST_e = fs->make<TH2D>( "hTrk_ST_Pz_rd_1_ST_e", "#Delta p_{z}/p_{z} vs #eta, #eta>1", 300, -3, 3, 201, -5.025, 5.025 );
  
  hTrk_ST_e_d_ST_e = fs->make<TH2D>( "hTrk_ST_e_d_ST_e", "#Delta #eta vs #eta", 300, -3, 3, 201, -0.1005, 0.1005 );
  hTrk_ST_p_d_ST_e = fs->make<TH2D>( "hTrk_ST_p_d_ST_e", "#Delta #phi vs #eta", 300, -3, 3, 201, -0.1005, 0.1005 );
  hTrk_ST_zvtx_d_ST_e = fs->make<TH2D>( "hTrk_ST_zvtx_d_ST_e", "#Delta z_{vtx} vs #eta", 300, -3, 3, 201, -10.05, 10.05 );
  hTrk_ST_q_d_ST_e = fs->make<TH2D>( "hTrk_ST_q_d_ST_e", "#Delta q vs #eta", 300, -3, 3, 21, -10.5, 10.5 );

  hTrk_ST_Pt_d->Sumw2();
  hTrk_ST_Pz_d->Sumw2();
  hTrk_ST_Pt_rd->Sumw2();  
  hTrk_ST_Pt_rd_1->Sumw2();
  hTrk_ST_Pz_rd_1->Sumw2();

  hTrk_ST_e_d->Sumw2();
  hTrk_ST_p_d->Sumw2();
  hTrk_ST_zvtx_d->Sumw2();
  hTrk_ST_q_d->Sumw2();

  hTrk_ST_Pt_d_ST_e->Sumw2();
  hTrk_ST_Pz_d_ST_e->Sumw2();
  hTrk_ST_Pt_rd_ST_e->Sumw2();
  hTrk_ST_Pt_rd_1_ST_e->Sumw2();
  hTrk_ST_Pz_rd_1_ST_e->Sumw2();
  
  hTrk_ST_e_d_ST_e->Sumw2();
  hTrk_ST_p_d_ST_e->Sumw2();
  hTrk_ST_zvtx_d_ST_e->Sumw2();
  hTrk_ST_q_d_ST_e->Sumw2();

  /// Geometry setup
  /// Set pointers to Geometry
  es.get<TrackerDigiGeometryRecord>().get(geometryESH);
  theGeometry = &(*geometryESH);
  /// Set pointers to Stacked Modules
  es.get<StackedTrackerGeometryRecord>().get(stackedgeometryESH);
  theStackedGeometry = stackedgeometryESH.product(); /// Note this is different 
                                                     /// from the "global" geometry

  /// End of things to be done before entering the event Loop
}

//////////
// ANALYZE
void TestL1PrimComp::analyze(const edm::Event& e, const edm::EventSetup& es)
{
  /// Functions that gets called by framework every event
  ///*** Fill Event NUmber Histogram
  h_EvtCnt->Fill(e.id().event()+0.2); /// The +0.2 is to be sure of being in the correct bin

  ////////////////////////
  // GET MAGNETIC FIELD //
  ////////////////////////
  edm::ESHandle<MagneticField> magnet;
  const MagneticField *magnetF;
  
  es.get<IdealMagneticFieldRecord>().get(magnet);
  magnetF = magnet.product();
  double mMagneticFieldStrength = magnetF->inTesla(GlobalPoint(0,0,0)).z();

  /// Get SimTracks and their Vertices
  /// NOTE: this is good also for later on,
  /// when checking efficiencies etc ...
  edm::Handle<edm::SimTrackContainer> theSimTracks;
  edm::Handle<edm::SimVertexContainer> theSimVtx;
  edm::InputTag simtrackHitsTag = config.getParameter<edm::InputTag>("simtrackHits");
  e.getByLabel( simtrackHitsTag, theSimTracks ); //e.getByLabel( "famosSimHits", theSimTracks );
  e.getByLabel( simtrackHitsTag, theSimVtx );    //e.getByLabel( "famosSimHits", theSimVtx );
  /// Get the L1 Tracks
  //edm::Handle<cmsUpgrades::L1Track_PixelDigi_Collection> l1trackHandlePD;
  //edm::InputTag l1tracksPixelDigisTag = config.getParameter<edm::InputTag>("l1tracksPixelDigis");
  //e.getByLabel(l1tracksPixelDigisTag, l1trackHandlePD);
  
  /// Define Tracker acceptance
  double eta[5];
  eta[0] = -log( tan( 0.5*atan2(32.0,210.0) ) );
  eta[1] = -log( tan( 0.5*atan2(48.0,240.0) ) );
  eta[2] = -log( tan( 0.5*atan2(64.3,240.0) ) );
  eta[3] = -log( tan( 0.5*atan2(80.3,240.0) ) );
  eta[4] = -log( tan( 0.5*atan2(98.5,240.0) ) );
  
  double ceta[5];
  ceta[0] = 0;
  ceta[1] = 0;
  ceta[2] = -log( tan( 0.5*atan2(68.3,210.0) ) );
  ceta[3] = -log( tan( 0.5*atan2(84.3,210.0) ) );
  ceta[4] = 0;

  /// Get the Digis  
  edm::Handle<edm::DetSetVector<PixelDigi> > theDigis;
  e.getByLabel("simSiPixelDigis", theDigis);
  /// Get the PixelDigis SimLink
  /// This is useful to find references to SimTracks
  edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink;
  e.getByLabel("simSiPixelDigis", thePixelDigiSimLink);
  edm::Handle<edm::PSimHitContainer> theSimHitsPP;
  e.getByLabel( "famosSimHits", "TrackerHits", theSimHitsPP );
  /// Get the Stubs
  edm::Handle<cmsUpgrades::GlobalStub_PixelDigi_Collection> globStubHandlePD;
  edm::InputTag globStubsPixelDigisTag = config.getParameter<edm::InputTag>("globStubsPixelDigis");
  e.getByLabel(globStubsPixelDigisTag, globStubHandlePD);
  /// Get the tracklets
  edm::Handle<cmsUpgrades::Tracklet_PixelDigi_Collection> trackletHandlePD;
  edm::InputTag trackletsPixelDigisTag = config.getParameter<edm::InputTag>("trackletsPixelDigis");
  e.getByLabel(trackletsPixelDigisTag, trackletHandlePD);

  /// Go on only if there are Tracklets from Pixel Digis
  if ( trackletHandlePD->size() > 0 ) {
    /// Loop over Tracklets
    Tracklet_PixelDigi_Collection::const_iterator iterTracklet;
    for ( iterTracklet = trackletHandlePD->begin();  iterTracklet != trackletHandlePD->end();  ++iterTracklet ) {
      if ( iterTracklet->isBeamSpot00() != beamspot00 ) continue;

      /// Accept only Hermetic Design
      bool isHermetic = iterTracklet->isHermetic();
      if (isHermetic==false) continue;
      bool isFakeS = false;
      bool isFakeT = false;

      /// Get the Stubs of this Tracklet, and Local ones
      TrackletMap theStubs = iterTracklet->stubs();
      GlobalStubPtrType innerStub = theStubs.begin()->second;
      GlobalStubPtrType outerStub = theStubs.rbegin()->second;
      FreeTrajectoryState helVtx = iterTracklet->VertexTrajectoryState( es );
      const LocalStub<cmsUpgrades::Ref_PixelDigi_> *innerLoc = innerStub->localStub();
      const LocalStub<cmsUpgrades::Ref_PixelDigi_> *outerLoc = outerStub->localStub();
      /// Tracklets with fake Stubs have more fakeness than those
      /// with good Stubs from different SimTracks ...
      /// Therefore, isFakeS and isFakeT can't be both TRUE
      /// These ID's are useful to match Tracklets to SimTracks so
      /// I decide to put them here ...
      int innerSimTrackId = -1;
      int outerSimTrackId = -1;

      /// NOTE SAME COMMENTS AS IN STUBS LOOP ABOVE:
      /// The above selection is reproduced in a simplified version:
      /// in fact, as both the Stubs here are good (non-fake), the
      /// SimTrack ID matched to the Digis is always the same
      /// and then only a fast check is needed: only one hit for just
      /// one layer within the Stub (0th hit, 0-module chosen)
      /// Already declared needed handles
      StackedTrackerDetId innerId = innerStub->Id();
      StackedTrackerDetId outerId = outerStub->Id();
      const DetId innerId0 = theStackedGeometry->idToDet(innerId,0)->geographicalId();
      const DetId outerId0 = theStackedGeometry->idToDet(outerId,0)->geographicalId();

      /// Renormalize layer number from 5-14 to 0-9
      int normLayerInner = innerId.layer();
      int normLayerOuter = outerId.layer();
      /// Get the Double Stack index
      int dSidx = normLayerInner/2;
      if (dSidx != (normLayerOuter-1)/2 ) std::cerr << "HOUSTON WE GOT A PROBLEM!" << std::endl;
      /// Accept only right superlayer
      if (seedsuperlayer != dSidx) continue;
      int fakeness = iterTracklet->isFake();
      if ( fakeness == 1 ) 
        isFakeS = true;
      else {
        innerSimTrackId = innerStub->trackID();
        outerSimTrackId = outerStub->trackID();
        if ( fakeness == 2 ) isFakeT = true;
      }
      /// Use only good Tracklets
      if (isFakeS==false && isFakeT==false) {
        if ( theSimTracks->size() > 0 ) {
          /// Loop over SimTracks
          SimTrackContainer::const_iterator iterSimTracks;
          for ( iterSimTracks = theSimTracks->begin();  iterSimTracks != theSimTracks->end();  ++iterSimTracks ) {
            int trkID = iterSimTracks->trackId();
            /// Fill only with thr Tracklet corresponding to the right SimTrack!!!
            if (innerSimTrackId==trkID && outerSimTrackId==trkID) { /// Some redundancy
              int vertexIndex = iterSimTracks->vertIndex();
              const SimVertex& theSimVertex = (*theSimVtx)[vertexIndex];

              double stmometa = iterSimTracks->momentum().eta();
              double stmomphi = iterSimTracks->momentum().phi();
              double stmompt  = iterSimTracks->momentum().pt();
              double stmompz  = iterSimTracks->momentum().pz();
              double stvtxz   = theSimVertex.position().z();
              double stvtxq   = iterSimTracks->charge();

              double trkvtxq   = -iterTracklet->deltaPhiNorm()/fabs(iterTracklet->deltaPhiNorm());
              double trkmompt  = iterTracklet->twoPointPt();

              double trkmompz ;              
              double trkmometa ; 
              double trkmomphi ;//= helVtx.momentum().phi();
              double trkvtxz   ;//= helVtx.position().z();


              /*double DELTAR = (outerStub->position() - iterTracklet->vertex()).perp() - (innerStub->position() - iterTracklet->vertex()).perp();
              double DELTAZ = outerStub->position().z() - innerStub->position().z();
              double fitmompz = trkmompt * DELTAZ / DELTAR;
              trkmompz = fitmompz;
              trkmometa = -log(tan(0.5*atan2(trkmompt,fitmompz)));*/

              trkmompz = iterTracklet->twoPointPz();
              trkmometa = iterTracklet->twoPointEta();
              trkmomphi = iterTracklet->twoPointPhi();
              trkvtxz = iterTracklet->vertex().z();

              hTrk_ST_Pt_d->Fill(trkmompt-stmompt);
              hTrk_ST_Pt_d_ST_e->Fill(stmometa, trkmompt-stmompt);

              hTrk_ST_Pz_d->Fill(trkmompz-stmompz);
              hTrk_ST_Pz_d_ST_e->Fill(stmometa, trkmompz-stmompz);

              hTrk_ST_Pt_rd->Fill((trkmompt-stmompt)/stmompt);
              hTrk_ST_Pt_rd_ST_e->Fill(stmometa, (trkmompt-stmompt)/stmompt);

              if (fabs(stmometa) > 1) {
                hTrk_ST_Pt_rd_1->Fill((trkmompt-stmompt)/stmompt);
                hTrk_ST_Pz_rd_1->Fill((trkmompz-stmompz)/stmompz);
                hTrk_ST_Pt_rd_1_ST_e->Fill(stmometa, (trkmompt-stmompt)/stmompt);
                hTrk_ST_Pz_rd_1_ST_e->Fill(stmometa, (trkmompz-stmompz)/stmompz);
              }              

              hTrk_ST_e_d->Fill(trkmometa-stmometa);
              hTrk_ST_e_d_ST_e->Fill(stmometa, trkmometa-stmometa);
              
              hTrk_ST_p_d->Fill(trkmomphi-stmomphi);
              hTrk_ST_p_d_ST_e->Fill(stmometa, trkmomphi-stmomphi);
              
              hTrk_ST_zvtx_d->Fill(trkvtxz-stvtxz);
              hTrk_ST_zvtx_d_ST_e->Fill(stmometa, trkvtxz-stvtxz);

              hTrk_ST_q_d->Fill(trkvtxq-stvtxq);
              hTrk_ST_q_d_ST_e->Fill(stmometa, trkvtxq-stvtxq);
              
            } /// End of same track
          } /// End of loop over SimTracks
        } /// End of if ( theSimTracks->size() > 0 )
      } /// End of only good Tracklets
    } /// End of loop over Tracklets
  } /// End if ( trackletHandlePD->size() > 0 )












  

} /// End of analyze()

/////////////////////////////////
//                             //
// SOME OTHER CUSTOM FUNCTIONS //
//                             //
/////////////////////////////////


/////////////////////
// Calculate DeltaPhi
double TestL1PrimComp::deltaPhi( double phi1, double phi2 ){
  double dp = phi1 - phi2;
  double pigreco = 4.0*atan(1.0);
  if (fabs(dp) < pigreco) return fabs(dp);
  else return (2*pigreco - fabs(dp));
}
double TestL1PrimComp::deltaPhiNP( double phi1, double phi2 ){
  double deltaPhiNP = phi1 - phi2;
  double pigreco = 4.0*atan(1.0);
  if ( fabs(deltaPhiNP) >= pigreco) {
    if ( deltaPhiNP>0 ) deltaPhiNP = deltaPhiNP - 2*pigreco;
    else deltaPhiNP = 2*pigreco - fabs(deltaPhiNP);
  }
  return deltaPhiNP;
}

/////////////////////
// Calculate DeltaEta
double TestL1PrimComp::deltaEta( double eta1, double eta2 ){
  return fabs(eta1 - eta2);
}

///////////////////
// Calculate DeltaR
double TestL1PrimComp::deltaR( double phi1, double eta1, double phi2, double eta2 ){
  double dp2 = deltaPhi(phi1, phi2)*deltaPhi(phi1, phi2);
  double de2 = deltaEta(eta1, eta2)*deltaEta(eta1, eta2);
  return sqrt(dp2 + de2);
}

//////////////////////////////////////
// Find "Hit" from Propagated Tracklet
std::pair<double,PSimHit> TestL1PrimComp::makeHit( const GeomDetUnit* dU, BaseParticlePropagator* tP,
                                                              FreeTrajectoryState hV, int pID, int tkID ) {
  const float onSurfaceTolarance = 0.01; /// 10 microns
  LorentzVector globVtx = tP->vertex();
  LorentzVector globMom = tP->momentum();
  LocalPoint lpos;
  LocalVector lmom;
  GlobalPoint gpos = GlobalPoint(  globVtx.x(), globVtx.y(), globVtx.z() );
  GlobalVector gmom = GlobalVector(  globMom.x(), globMom.y(), globMom.z() );
  GlobalPoint pCentDet = dU->toGlobal( dU->topology().localPosition( MeasurementPoint( 0.5*dU->surface().bounds().width(), 0.5*dU->surface().bounds().length()   ) ) );
  
  /// Apply constraint on "phi-wedge"
  if ( deltaPhi(gpos.phi(), pCentDet.phi()) > 4.0*atan(1.0)/12.0 ) return std::pair<double,PSimHit>(-999.9,PSimHit());
  PropagationDirection dirP = anyDirection;
//std::cerr<<"checkhit"<<std::endl;
  /// If the module is insidewards, change the direction
  /// if the module is outsidewards the propagation, keep
  /// the direction
  //if ( gpos.perp() <  pCentDet.perp() ) dirP = alongMomentum;
  //else dirP = oppositeToMomentum;
  if ( fabs( dU->toLocal( gpos ).z() ) < onSurfaceTolarance ) {
    lpos = dU->toLocal( gpos );
    lmom = dU->toLocal( gmom );
  }
  else {
    HelixArbitraryPlaneCrossing crossing( gpos.basicVector(),
                                          gmom.basicVector(),
                                          hV.transverseCurvature(),
                                          dirP );
    std::pair<bool,double> path = crossing.pathLength(dU->surface());
    if (!path.first) {
      return  std::pair<double,PSimHit>(-999.9,PSimHit());
    }
    GlobalPoint gcrosspos = GlobalPoint( crossing.position(path.second));
    lpos = dU->toLocal( gcrosspos );
    lmom = dU->toLocal( GlobalVector( crossing.direction(path.second)));
    lmom = lmom.unit() * (dU->toLocal( gmom )).mag();
  }
  // The module (half) thickness 
  const BoundPlane& theDetPlane = dU->surface();
  float halfThick = 0.5*theDetPlane.bounds().thickness();
  // The entry and exit points, and the time of flight
  float pZ = lmom.z();
//std::cerr<<"momentum "<<globMom.x()<<" "<<globMom.y()<<" "<<globMom.z()<<" "<<std::endl;
  LocalPoint entry = lpos + (-halfThick/pZ) * lmom;
  LocalPoint exit = lpos + halfThick/pZ * lmom;
  float tof = gpos.mag() / 30. ; // in nanoseconds, FIXME: very approximate
  // FIXME: fix the track ID and the particle ID
  PSimHit hit( entry, exit, lmom.mag(), tof, 0, pID,
                dU->geographicalId().rawId(), tkID,
                lmom.theta(), lmom.phi() );
//std::cerr<<"entryexit "<<entry.x()<<" "<<entry.y()<<" "<<exit.x()<<" "<<exit.y()<<std::endl;
  // Check that the PSimHit is physically on the module!
  //unsigned subdet = DetId(hit.detUnitId()).subdetId();
  double boundX = theDetPlane.bounds().width()/2.;
  double boundY = theDetPlane.bounds().length()/2.;
  // Check if the hit is on the physical volume of the module
  // (It happens that it is not, in the case of double sided modules,
  //  because the envelope of the gluedDet is larger than each of 
  //  the mono and the stereo modules)
  double dist = 0.;
  GlobalPoint IP (hV.position().x(),
                  hV.position().y(),
                  hV.position().z());
  dist = ( fabs(hit.localPosition().x()) > boundX  || 
           fabs(hit.localPosition().y()) > boundY ) ?  
    // Will be used later as a flag to reject the PSimHit!
    -( dU->surface().toGlobal(hit.localPosition()) - IP ).mag2() 
    : 
    // These hits are kept!
    ( dU->surface().toGlobal(hit.localPosition()) - IP ).mag2();
//std::cerr<<"  makePSimHit  "<< dist <<" "<<IP.x()<<" "<<IP.y()<<" "<<IP.z()<< " "<<dU->surface().toGlobal(hit.localPosition()).x()<< " "<<dU->surface().toGlobal(hit.localPosition()).y()<< " "<<dU->surface().toGlobal(hit.localPosition()).z()<< std::endl;
  return std::pair<double,PSimHit>(dist,hit);
}


 

///////////////////////////
// DEFINE THIS AS A PLUG-IN
DEFINE_ANOTHER_FWK_MODULE(TestL1PrimComp);





