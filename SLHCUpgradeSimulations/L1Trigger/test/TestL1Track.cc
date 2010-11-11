//////////////////////////
//  Analyzer by Nicola  //
//   sept 2010 @ CERN   //
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

#include "SimDataFormats/SLHC/interface/L1TrackFit.h"

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
class TestL1Track : public edm::EDAnalyzer
{
  /// Public methods
  public:
    /// Constructor/destructor
    explicit TestL1Track(const edm::ParameterSet& conf);
    virtual ~TestL1Track();
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
    typedef std::pair< int, std::vector< GlobalStub<Ref_PixelDigi_> > > L1Trk_base;


    
  /// Protected methods only internally used
  protected:
    /// Get Pt from a Stub with the 2 points + tangent constraints
    double getPtFromStub( double xx, double yy, float ddxx, float ddyy );
    /// EtaPhi space
    double deltaPhi( double phi1, double phi2 );
    double deltaPhiNP( double phi1, double phi2 );
    double deltaEta( double eta1, double eta2 );
    double deltaR( double phi1, double eta1, double phi2, double eta2 );

    std::pair<double,PSimHit> makeHit( const GeomDetUnit* dU, BaseParticlePropagator* tP,
                                       FreeTrajectoryState hV, int pID, int tkID );

                                       /// Make L1Track with 1 Tracklet and N Stubs
    //std::vector< std::pair< int, std::vector< GlobalStub<Ref_PixelDigi_> > > >
    std::vector< L1Track<Ref_PixelDigi_> >
        makeL1Track( const edm::EventSetup& es,
                     const MagneticField *magnetF,
                     Tracklet_PixelDigi_Collection::const_iterator seed,
                     std::vector< GlobalStub_PixelDigi_Collection::const_iterator > bricks,
                     bool erase_nulls );

    struct predicate_unique {
      bool operator() ( std::pair< int, std::vector< GlobalStub<Ref_PixelDigi_> > > a, std::pair< int, std::vector< GlobalStub<Ref_PixelDigi_> > > b ) {
        if (a.second.size() != b.second.size()) return false;
        else {
          for (unsigned int q=0; q<a.second.size(); q++) {
            if (a.second.at(q).localStub() != b.second.at(q).localStub()) return false;
          }
        }
        return true;
      }
    };

    struct predicate_sort {
      bool operator() ( std::pair< int, std::vector< GlobalStub<Ref_PixelDigi_> > > a, std::pair< int, std::vector< GlobalStub<Ref_PixelDigi_> > > b ) {
        if (a.second.size() != b.second.size())
          return (a.second.size()<b.second.size());
        else {
          for (unsigned int q=0; q<a.second.size(); q++) {
            if (a.second.at(q).localStub() != b.second.at(q).localStub())
              return (a.second.at(q).localStub() < b.second.at(q).localStub());
          }
        }
        return false;
      }
    };


                     
  /// Private methods and variables
  private:

    int PDGCode;
    double lumiZ;
    bool testedGeometry;
    bool beamspot00;
    int seedsuperlayer;
    int numberstubs;
    int howmanytight;

    double probwindow;

    /// TO CHECK ALL EVENTS ARE PROCESSED
    TH1D* h_EvtCnt;

    TH2D* hDET_LAYER_r;
    TH2D* hDET_IPHI_p;
    TH2D* hDET_IZ_z;

    TH2D* hL1Trk_e_ST_e;
    TH2D* hL1Trk_p_ST_p;
    TH2D* hL1Trk_Pt_ST_Pt;
    TH2D* hL1Trk_xvtx_ST_xvtx;
    TH2D* hL1Trk_yvtx_ST_yvtx;
    TH2D* hL1Trk_zvtx_ST_zvtx;
    TH2D* hL1Trk_q_ST_q;
    TH2D* hL1Trk_chi2rphi_n;
    TH2D* hL1Trk_chi2rz_n;
    
    TH1D* hL1Trk_FakeRate_n;
    TH1D* hL1Trk_Number_n;
    TH1D* hST_Pt_matched;
    TH1D* hST_Pt;
    TH1D* hST_Pt_matched_any;    

    TH1D* hST_e_Pt10_matched;
    TH1D* hST_e_Pt10;
    TH1D* hST_e_Pt10_matched_any;    

    /// Containers of parameters passed by python
    /// configuration file
    edm::ParameterSet config;

    /// Geometry handles etc
    edm::ESHandle<TrackerGeometry> geometryESH;
    const TrackerGeometry*  theGeometry;
    edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> stackedgeometryESH;
    const cmsUpgrades::StackedTrackerGeometry*  theStackedGeometry;


    struct store_trks {
      int trkid;
      int doublestack;
      int phisector;    
    };
    typedef store_trks StoreTrks;

    struct store_hits {
      int origindoublestack;
      int targetlayer;
      int targetiphisector;
      int targetizsector;
      PSimHit hit;
    };
    typedef store_hits StoreHits;

    struct store_PtEta {
      double pt;
      double eta;
    };
    typedef store_PtEta PtEta;

};

//////////////////////////////////
//                              //
//     CLASS IMPLEMENTATION     //
//                              //
//////////////////////////////////

//////////////
// CONSTRUCTOR
TestL1Track::TestL1Track(edm::ParameterSet const& conf) : 
  config(conf)
{
  /// Insert here what you need to initialize
}

/////////////
// DESTRUCTOR
TestL1Track::~TestL1Track()
{
  /// Insert here what you need to delete
  /// when you close the class instance
}  

//////////
// END JOB
void TestL1Track::endJob() 
{
  /// Things to be done at the exit of the event Loop
  std::cout << " TestL1Track::endJob" << std::endl;
  /// End of things to be done at the exit from the event Loop
}

////////////
// BEGIN JOB
void TestL1Track::beginJob(const edm::EventSetup& es)
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
  
  probwindow = -99.9;
  
  testedGeometry = false;
  
  seedsuperlayer = 0;
  numberstubs = -9;
  howmanytight = 0;
  
  /// Select the Superlayer you want to check
  if ( config.getParameter<int>("seedSuperLayer") >= 0  ) seedsuperlayer = config.getParameter<int>("seedSuperLayer");
  /// Select the number of Stubs of the L1Track
  if ( config.getParameter<int>("numberStubs") >= 0  ) numberstubs = config.getParameter<int>("numberStubs");
  /// Select the kind of Tracklets you want to handle
  if ( config.getParameter<string>("trackletVTX") == "offcenter" ) beamspot00 = false;
  /// Select the windows corresponding to chosen match probabilities
  if ( config.getParameter<double>("windowSize") >= 0 ) probwindow = config.getParameter<double>("windowSize");

  /// Select how many tight Stubs you want in the L1Track
  /// 0 means any
  /// 1 means at least 1
  /// 2 means all the Stubs in the L1Track must be tight
  if ( config.getParameter<int>("tightStubsL1Trk") >= 0 && config.getParameter<int>("tightStubsL1Trk") <= 2 ) howmanytight = config.getParameter<int>("tightStubsL1Trk");

  /// Things to be done before entering the event Loop
  std::cout << " TestL1Track::beginJob" << std::endl;

  /// Book histograms etc
  edm::Service<TFileService> fs;
  h_EvtCnt = fs->make<TH1D>( "h_EvtCnt", "MC Tau Decay", 1000, 0, 1000 );
  h_EvtCnt->Sumw2();

  hDET_LAYER_r = fs->make<TH2D>( "hDET_LAYER_r",  "Layer Number vs radius", 220, 0, 110, 12, 0, 12 );
  hDET_IPHI_p = fs->make<TH2D>( "hDET_IPHI_p",  "iPhi Number vs #phi", 660, -3.3, 3.3, 70, 0, 70 );
  hDET_IZ_z = fs->make<TH2D>( "hDET_IZ_z",  "iZ Number vs z", 580, -290, 290, 80, 0, 80 );
  hDET_LAYER_r->Sumw2();
  hDET_IPHI_p->Sumw2();
  hDET_IZ_z->Sumw2();

  hL1Trk_e_ST_e = fs->make<TH2D>( "hL1Trk_e_ST_e",  "L1Trk #eta vs SimTrack #eta", 90, -3, 3, 90, -3, 3 );
  hL1Trk_p_ST_p = fs->make<TH2D>( "hL1Trk_p_ST_p",  "L1Trk #phi vs SimTrack #phi", 99, -3.3, 3.3, 99, -3.3, 3.3 );
  hL1Trk_Pt_ST_Pt = fs->make<TH2D>( "hL1Trk_Pt_ST_Pt",  "L1Trk p_{T} vs SimTrack p_{T}", 440, 0, 110, 440, 0, 110 );
  hL1Trk_xvtx_ST_xvtx = fs->make<TH2D>( "hL1Trk_xvtx_ST_xvtx",  "L1Trk x_{vtx} vs SimTrack x_{vtx}", 99, -3.3, 3.3, 99, -3.3, 3.3 );
  hL1Trk_yvtx_ST_yvtx = fs->make<TH2D>( "hL1Trk_yvtx_ST_yvtx",  "L1Trk y_{vtx} vs SimTrack y_{vtx}", 99, -3.3, 3.3, 99, -3.3, 3.3 );
  hL1Trk_zvtx_ST_zvtx = fs->make<TH2D>( "hL1Trk_zvtx_ST_zvtx",  "L1Trk z_{vtx} vs SimTrack z_{vtx}", 300, -20, 20, 300, -20, 20 );
  hL1Trk_q_ST_q = fs->make<TH2D>( "hL1Trk_q_ST_q",  "L1Trk charge vs SimTrack charge", 9, -4.5, 4.5, 9, -4.5, 4.5 );
  hL1Trk_chi2rphi_n = fs->make<TH2D>( "hL1Trk_chi2rphi_n",  "L1Trk #chi^{2}_{r#phi} vs num of Stubs", 10, -0.5, 9.5, 150, 0, 0.03 );
  hL1Trk_chi2rz_n = fs->make<TH2D>( "hL1Trk_chi2rz_n",  "L1Trk #chi^{2}_{rz} vs num of Stubs", 10, -0.5, 9.5, 150, 0, 0.03 );
  hL1Trk_e_ST_e->Sumw2();
  hL1Trk_p_ST_p->Sumw2();
  hL1Trk_Pt_ST_Pt->Sumw2();
  hL1Trk_xvtx_ST_xvtx->Sumw2();
  hL1Trk_yvtx_ST_yvtx->Sumw2();
  hL1Trk_zvtx_ST_zvtx->Sumw2();
  hL1Trk_q_ST_q->Sumw2();
  hL1Trk_chi2rphi_n->Sumw2();
  hL1Trk_chi2rz_n->Sumw2();

  hL1Trk_FakeRate_n = fs->make<TH1D>( "hL1Trk_FakeRate_n", "Fake Rate vs num of Stubs", 10, -0.5, 9.5 );
  hL1Trk_Number_n = fs->make<TH1D>( "hL1Trk_Number_n", "Num of L1Tracks vs num of Stubs", 10, -0.5, 9.5 );
  hST_Pt_matched = fs->make<TH1D>( "hST_Pt_matched", "L1Tracks p_{T} matched", 440, 0, 110 );
  hST_Pt_matched_any = fs->make<TH1D>( "hST_Pt_matched_any", "L1Tracks p_{T} matched", 440, 0, 110 );
  hST_Pt = fs->make<TH1D>( "hST_Pt", "L1Tracks p_{T}", 440, 0, 110 );
  hL1Trk_FakeRate_n->Sumw2();
  hL1Trk_Number_n->Sumw2();
  hST_Pt_matched->Sumw2();
  hST_Pt_matched_any->Sumw2();
  hST_Pt->Sumw2();


  hST_e_Pt10_matched = fs->make<TH1D>( "hST_e_Pt10_matched", "L1Tracks #eta matched, p_{T}>10 GeV/c", 90, -3, 3 );
  hST_e_Pt10_matched_any = fs->make<TH1D>( "hST_e_Pt10_matched_any", "L1Tracks p_{T} #eta matched, p_{T}>10 GeV/c", 90, -3, 3 );
  hST_e_Pt10 = fs->make<TH1D>( "hST_e_Pt10", "L1Tracks #eta, p_{T}>10 GeV/c", 90, -3, 3 );
  hST_e_Pt10_matched->Sumw2();
  hST_e_Pt10_matched_any->Sumw2();
  hST_e_Pt10->Sumw2();
  
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
void TestL1Track::analyze(const edm::Event& e, const edm::EventSetup& es)
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

  /////////////////////////////////////////
  // First of all, we need to look       //
  // at each vertex: this is to check    //
  // that we have that each single track //
  // event does not provide us with      //
  // (0,0,0) tracks but has really       //
  // a wide luminous region...           //
  // Moreover, here we put some counting //
  // of multiplicity and leading track   //
  /////////////////////////////////////////
  /// Get SimTracks and their Vertices
  /// NOTE: this is good also for later on,
  /// when checking efficiencies etc ...
  edm::Handle<edm::SimTrackContainer> theSimTracks;
  edm::Handle<edm::SimVertexContainer> theSimVtx;
  edm::InputTag simtrackHitsTag = config.getParameter<edm::InputTag>("simtrackHits");
  e.getByLabel( simtrackHitsTag, theSimTracks ); //e.getByLabel( "famosSimHits", theSimTracks );
  e.getByLabel( simtrackHitsTag, theSimVtx );    //e.getByLabel( "famosSimHits", theSimVtx );
  /// Get the L1 Tracks
  edm::Handle<cmsUpgrades::L1Track_PixelDigi_Collection> l1trackHandlePD;
  edm::InputTag l1tracksPixelDigisTag = config.getParameter<edm::InputTag>("l1tracksPixelDigis");
  e.getByLabel(l1tracksPixelDigisTag, l1trackHandlePD);
  /*
  /// SimTrack Multiplicity
  ///*** Fill histogram
  hST_N->Fill( theSimTracks->size() );
  /// Slave variables for determination of leading SimTrack
  double leadingPt = -999.9;
  int leadingPID = 12345678;
  /// This is useful in determination of Good SimTracks according to Mark
  /// WARNING maybe this kind of approcach is not the best one, however
  /// as I took inspiration from stuff written for hybrid geometry and
  /// I worked 5/6 months with hybrid geometry, I kept the same approach
  /// but to deal with 5 Double Stacks, I moved to arrays and, to avoid
  /// confusion in accessing elements with operator [], these arrays are
  /// arrays of pairs where the first element is the same integer as the
  /// index of the array element so that it is just a trick to win against
  /// C++ evils. Therefore only [i].second will be truly important in the
  /// following code, ok?
  /// NOTE: modification is that Pt map, maps a (Pt, eta) pair instead of
  /// Pt only...
  std::pair< int, std::map< int, int > > simTrackPdgMap;
  std::pair< int, std::map< int, PtEta > > simTrackPtMap;
  std::pair< int, std::map< int, PtEta > > simTrackPtMap_Pdg;
  std::map< int, PtEta > tempPtMap, tempPtMap_Pdg;

  simTrackPdgMap.first = 0;
  simTrackPtMap.first = 0;
  simTrackPtMap_Pdg.first = 0;
  */
  
  /// Define Tracker acceptance
  double eta[5];
  eta[0] = -log( tan( 0.5*atan2(32.0,210.0) ) );
  eta[1] = -log( tan( 0.5*atan2(48.0,270.0) ) );
  eta[2] = -log( tan( 0.5*atan2(64.3,270.0) ) );
  eta[3] = -log( tan( 0.5*atan2(80.3,270.0) ) );
  eta[4] = -log( tan( 0.5*atan2(98.5,270.0) ) );
  
  double ceta[5];
  ceta[0] = 0;
  ceta[1] = 0;
  ceta[2] = -log( tan( 0.5*atan2(68.3,210.0) ) );
  ceta[3] = -log( tan( 0.5*atan2(84.3,210.0) ) );
  ceta[4] = 0;
  
  /// Go on only if there are SimTracks
  if ( theSimTracks->size() != 0 ) {
    /// Loop over SimTracks
    SimTrackContainer::const_iterator iterSimTracks;
    for ( iterSimTracks = theSimTracks->begin();  iterSimTracks != theSimTracks->end();  ++iterSimTracks ) {

      /// Tracker Acceptance
      if (fabs(iterSimTracks->momentum().eta()) > eta[seedsuperlayer]) continue;
      if (fabs(iterSimTracks->momentum().eta()) < ceta[seedsuperlayer]) continue;
      
      /// Primary VTX      
      int vertexIndex = iterSimTracks->vertIndex();
      const SimVertex& theSimVertex = (*theSimVtx)[vertexIndex];
     
      hST_Pt->Fill(iterSimTracks->momentum().pt());
      if (iterSimTracks->momentum().pt()>10.0)
        hST_e_Pt10->Fill(iterSimTracks->momentum().eta());
      bool anymatched = false;
      bool exactnumber = false;
      /// Go on only if there are L1Tracks from Pixel Digis
      if ( l1trackHandlePD->size() > 0 ) {
        /// Loop over L1Tracks
        L1Track_PixelDigi_Collection::const_iterator iterL1Track;
        for ( iterL1Track = l1trackHandlePD->begin();  iterL1Track != l1trackHandlePD->end();  ++iterL1Track ) {
          /// Select only chosen window
          if ( iterL1Track->probWindow() != probwindow ) continue;
          /// Select only beamspot-wise L1Tracks
          if ( iterL1Track->isBeamSpot00() != beamspot00 ) continue;
          /// Select only L1Tracks with chosen seed
          if ( iterL1Track->whichSeed() != seedsuperlayer ) continue;
          /// Select only good L1Tracks
          if ( iterL1Track->isFake() != 0 ) continue;

          /// Select only Tracks matching VTX
          ///if ( fabs(iterL1Track->fitVertex().z()) >= 20.0 ) continue;
          // comment because of new L1Track class version, but you can keep the option
          // just by putting the fit here above
          if ( iterL1Track->simTrkId() == iterSimTracks->trackId() )
            anymatched = true;
      
          /// Select only tight
          if (howmanytight==1) {
            bool tight = false;
            std::vector< GlobalStub<Ref_PixelDigi_> > theStubs = iterL1Track->getStubs();
            for (unsigned int p=0; p<theStubs.size(); p++ ) {
              if ( theStubs.at(p).localStub()->isTight() == true ) tight = true;
            }
            if (tight == false) continue;
          }
          else if (howmanytight==2) {
            bool tight = true;
            std::vector< GlobalStub<Ref_PixelDigi_> > theStubs = iterL1Track->getStubs();
            for (unsigned int p=0; p<theStubs.size(); p++ ) {
              if ( theStubs.at(p).localStub()->isTight() == false ) tight = false;
            }
            if (tight == false) continue;
          }

          /// Select only L1Tracks with chosen size
          if ( iterL1Track->numberStubs() != numberstubs ) continue;
          /// Already checked trk ID
          exactnumber = true;

        }
      }

      if (anymatched) {
        hST_Pt_matched_any->Fill(iterSimTracks->momentum().pt());
        if (iterSimTracks->momentum().pt()>10.0)
          hST_e_Pt10_matched_any->Fill(iterSimTracks->momentum().eta());
      }
      if (exactnumber) {
        hST_Pt_matched->Fill(iterSimTracks->momentum().pt());
        if (iterSimTracks->momentum().pt()>10.0)
          hST_e_Pt10_matched->Fill(iterSimTracks->momentum().eta());
      }
    } /// End of Loop over SimTracks
  } /// End of if ( theSimTracks->size() != 0 )

  ///////////////////////
  // Test the Geometry //
  // of The Tracker    //
  ///////////////////////
  /// Do this only once, please
  std::vector<StackedTrackerDetUnit*> stackContainer = theStackedGeometry->stacks();
  if (testedGeometry==false) {
    /// Loop over Detector Pieces
    for ( unsigned int k=0; k<stackContainer.size(); k++ ) {
      StackedTrackerDetUnit* detUnitIt = stackContainer[k];
      StackedTrackerDetId stackedMemberId = detUnitIt->Id();
      int layer = stackedMemberId.layer();
      int iphi = stackedMemberId.iPhi();
      int iz = stackedMemberId.iZ();
      int doublestack;
      if (layer%2==0) doublestack = layer/2;
      else doublestack = (layer-1)/2;
      /// Choose Double Stack
      if (doublestack==seedsuperlayer) {
        const GeomDetUnit* detUnit = theStackedGeometry->idToDetUnit(  stackedMemberId, layer%2 );
        GlobalPoint zerozeroGP = detUnit->toGlobal( detUnit->topology().localPosition( MeasurementPoint( 0, 0) ) );
        //GlobalPoint zerozeroGP = detUnit->toGlobal( detUnit->surface().position(  ) );
        hDET_LAYER_r->Fill( zerozeroGP.perp()  , layer );
        hDET_IPHI_p->Fill( zerozeroGP.phi(), iphi );
        hDET_IZ_z->Fill( zerozeroGP.z(), iz );
      }
    } /// End of Loop over Detector Pieces
    /// End of Test the Geometry of The Tracker
    testedGeometry = true;
  }

  ///////////////////////////////////
  // Get all PixelDigis and look   //
  // at correlations in row-column //
  // between inner and outer layer //
  // of a stacked module           //
  ///////////////////////////////////
  /// Get the Digis  
  edm::Handle<edm::DetSetVector<PixelDigi> > theDigis;
  e.getByLabel("simSiPixelDigis", theDigis);
  /// Get the PixelDigis SimLink
  /// This is useful to find references to SimTracks
  edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink;
  e.getByLabel("simSiPixelDigis", thePixelDigiSimLink);

  edm::Handle<edm::PSimHitContainer> theSimHitsPP;
  e.getByLabel( "famosSimHits", "TrackerHits", theSimHitsPP );

  /////////////////////////////////////////
  // Get Stubs from PixelDigis and start //
  // counting them to check the goodness //
  // of the clustering and hit matching  //
  // algorithm. We aim at duplicate and  //
  // fake Stubs. We also want to find    //
  // how much the efficiency in Stub     //
  // production is and changes ...       //
  /////////////////////////////////////////
  /// Get the Stubs
  edm::Handle<cmsUpgrades::GlobalStub_PixelDigi_Collection> globStubHandlePD;
  edm::InputTag globStubsPixelDigisTag = config.getParameter<edm::InputTag>("globStubsPixelDigis");
  e.getByLabel(globStubsPixelDigisTag, globStubHandlePD);
  /// Go on only if there are Global Stubs from Pixel Digis
  /// /////////////////////
  /// THIS IS FOR L1 TRACKS
  /// /////////////////////
  std::vector< GlobalStub_PixelDigi_Collection::const_iterator > brickStubs;
  brickStubs.clear();
  if ( globStubHandlePD->size() > 0 ) { 
    /// Loop over GlobalStubs
    GlobalStub_PixelDigi_Collection::const_iterator iterGlobStub;
    for ( iterGlobStub = globStubHandlePD->begin();  iterGlobStub != globStubHandlePD->end();  ++iterGlobStub ) {
      /// /////////////////////
      /// THIS IS FOR L1 TRACKS
      /// /////////////////////
      brickStubs.push_back( iterGlobStub );
      /// Some variables needed to check if the
      /// Stub is fake or duplicate and which must
      /// be initialized here

    }
  }

/*

  //////////////
  // BEAMSPOT //
  //////////////
  
  double xBeam = 0;
  double yBeam = 0;

  ////////////////////////////////////////////////
  // Get Tracklets from Pixel Digis and         //
  // start looking at relevant spectra and      //
  // correlations. Look also for Fake Tracklets //
  // which have a Fake Stub of Stubs coming     //
  // from different SimTracks. Check also       //
  // Tracklets from the Luminous Region and try //
  // to clean from those coming from outside    //
  ////////////////////////////////////////////////
  /// Get the tracklets
  edm::Handle<cmsUpgrades::Tracklet_PixelDigi_Collection> trackletHandlePD;
  edm::InputTag trackletsPixelDigisTag = config.getParameter<edm::InputTag>("trackletsPixelDigis");
  e.getByLabel(trackletsPixelDigisTag, trackletHandlePD);
  /// Storage elements to handle Tracklets by Superlayer
  /// and use them as seeds later on...
  std::vector< Tracklet_PixelDigi_Collection::const_iterator > trackletSeeds[5];
  for (int ujh=0; ujh<5; ujh++) trackletSeeds[ujh].clear();
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
 
    }
  }
*/


  ////////////////////////////////////////////////
  // Get L1Tracks from Pixel Digis and          //
  // start looking at relevant spectra and      //
  // correlations. Look also for Fake L1Tracks  //
  // which have a Fake Stub of Stubs coming     //
  // from different SimTracks. Check also       //
  // L1Tracks from the Luminous Region and try  //
  // to clean from those coming from outside    //
  ////////////////////////////////////////////////
  /// Go on only if there are L1Tracks from Pixel Digis
  if ( l1trackHandlePD->size() > 0 ) {
  
    /// Loop over L1Tracks
    L1Track_PixelDigi_Collection::const_iterator iterL1Track;
    for ( iterL1Track = l1trackHandlePD->begin();  iterL1Track != l1trackHandlePD->end();  ++iterL1Track ) {
      /// Select only chosen window according to match probability
      if ( iterL1Track->probWindow() != probwindow ) continue;
      /// Select only beamspot-wise L1Tracks
      if ( iterL1Track->isBeamSpot00() != beamspot00 ) continue;

      /// Select only L1Tracks with chosen seed
      if ( iterL1Track->whichSeed() != seedsuperlayer ) continue;

      L1TrackFit iterL1TrackFit = iterL1Track->fitL1Track(true);

      /// Select only Tracks matching VTX
      if ( fabs(iterL1TrackFit.getVertex().z()) >= 20.0 ) continue;

      /// Select only good L1Tracks
      bool isFakeL1Track = false;
      if ( iterL1Track->isFake() != 0 ) {
        if (iterL1Track->numberStubs() == 2) {
          /// Check if the Tracklet itself is fake
          std::vector< GlobalStub<Ref_PixelDigi_> > theStubs = iterL1Track->getStubs();
          if ( theStubs.at(0).isFake() || theStubs.at(1).isFake() ||
               theStubs.at(0).trackID() != theStubs.at(1).trackID() ) {
            isFakeL1Track = true;
          }
       
        }
        else isFakeL1Track = true;
      }

      hL1Trk_Number_n->Fill(iterL1Track->numberStubs());

      if (isFakeL1Track) {
        hL1Trk_FakeRate_n->Fill(iterL1Track->numberStubs());
        continue;
      }
      
      /// Select only L1Tracks with chosen size
      if ( iterL1Track->numberStubs() != numberstubs ) continue;      

      /// Select only tight
      if (howmanytight==1) {
        bool tight = false;
        std::vector< GlobalStub<Ref_PixelDigi_> > theStubs = iterL1Track->getStubs();
        for (unsigned int p=0; p<theStubs.size(); p++ ) {
          if ( theStubs.at(p).localStub()->isTight() == true ) tight = true;
        }
        if (tight == false) continue;
      }
      else if (howmanytight==2) {
        bool tight = true;
        std::vector< GlobalStub<Ref_PixelDigi_> > theStubs = iterL1Track->getStubs();
        for (unsigned int p=0; p<theStubs.size(); p++ ) {
          if ( theStubs.at(p).localStub()->isTight() == false ) tight = false;
        }
        if (tight == false) continue;
      }
      /// Fit the Track already done in builder
      /// Compare with SimTracks (correlation)
      /// Go on only if there are SimTracks
      if ( theSimTracks->size() > 0 ) {
        /// Loop over SimTracks
        SimTrackContainer::const_iterator iterSimTracks;
        for ( iterSimTracks = theSimTracks->begin();  iterSimTracks != theSimTracks->end();  ++iterSimTracks ) {
          int vertexIndex = iterSimTracks->vertIndex();
          const SimVertex& theSimVertex = (*theSimVtx)[vertexIndex];

          //if (iterL1Track->trkCharge() > 0 ) continue;
          
          if ( iterL1Track->simTrkId() == iterSimTracks->trackId() ) {


            hL1Trk_e_ST_e->Fill( iterSimTracks->momentum().eta(), iterL1TrackFit.getMomentum().eta() );
            hL1Trk_p_ST_p->Fill( iterSimTracks->momentum().phi(), iterL1TrackFit.getMomentum().phi() );
            hL1Trk_Pt_ST_Pt->Fill( iterSimTracks->momentum().pt(), iterL1TrackFit.getMomentum().perp() );
            hL1Trk_xvtx_ST_xvtx->Fill( theSimVertex.position().x(), iterL1TrackFit.getVertex().x() );
            hL1Trk_yvtx_ST_yvtx->Fill( theSimVertex.position().y(), iterL1TrackFit.getVertex().y() );
            hL1Trk_zvtx_ST_zvtx->Fill( theSimVertex.position().z(), iterL1TrackFit.getVertex().z() );
            hL1Trk_q_ST_q->Fill( iterSimTracks->charge(), iterL1TrackFit.getCharge() );
            //hL1Trk_chi2rphi_n->Fill( abcde->numberStubs(), abcde->fitChi2RPhi() );
            //hL1Trk_chi2rz_n->Fill( abcde->numberStubs(), abcde->fitChi2ZPhi() );

            continue;
          } /// End of association to SimTrack
        } /// End of loop on SimTracks
      } /// End of theSimTracks->size() > 0
    } /// End of loop over L1Tracks
    
  } /// End of l1trackHandlePD->size() > 0




















  

} /// End of analyze()

/////////////////////////////////
//                             //
// SOME OTHER CUSTOM FUNCTIONS //
//                             //
/////////////////////////////////


/////////////////////
// Calculate DeltaPhi
double TestL1Track::deltaPhi( double phi1, double phi2 ){
  double dp = phi1 - phi2;
  double pigreco = 4.0*atan(1.0);
  if (fabs(dp) < pigreco) return fabs(dp);
  else return (2*pigreco - fabs(dp));
}
double TestL1Track::deltaPhiNP( double phi1, double phi2 ){
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
double TestL1Track::deltaEta( double eta1, double eta2 ){
  return fabs(eta1 - eta2);
}

///////////////////
// Calculate DeltaR
double TestL1Track::deltaR( double phi1, double eta1, double phi2, double eta2 ){
  double dp2 = deltaPhi(phi1, phi2)*deltaPhi(phi1, phi2);
  double de2 = deltaEta(eta1, eta2)*deltaEta(eta1, eta2);
  return sqrt(dp2 + de2);
}


//////////////
// L1TRACKS //
//////////////

///////////////////////////
// DEFINE THIS AS A PLUG-IN
DEFINE_ANOTHER_FWK_MODULE(TestL1Track);





