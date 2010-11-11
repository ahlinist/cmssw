
/// ////////////////////////////////////////
/// Written by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, November                       ///
/// Modified fr the new L1Track class    ///
/// structure                            ///
/// ////////////////////////////////////////

#ifndef L1TRACK_BUILDER_LB_H
#define L1TRACK_BUILDER_LB_H

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"

#include "SLHCUpgradeSimulations/Utilities/interface/constants.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "RecoTracker/TkSeedGenerator/interface/FastHelix.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"

#include "SLHCUpgradeSimulations/Utilities/interface/classInfo.h"
//
////////////////////////////
// DETECTOR GEOMETRY HEADERS
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

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"

//
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"

template< typename T >
class L1TrackBuilderLB : public edm::EDProducer {

  typedef cmsUpgrades::GlobalStub< T >       GlobalStubType;
  typedef std::vector< GlobalStubType >      GlobalStubCollectionType;
  typedef edm::Ptr< GlobalStubType >         GlobalStubPtrType;

  typedef cmsUpgrades::Tracklet< T >       TrackletType;
  typedef std::vector< TrackletType >      TrackletCollectionType;
  typedef edm::Ptr< TrackletType >         TrackletPtrType;
  
  typedef cmsUpgrades::L1Track< T >       L1TrackType;
  typedef std::vector< L1TrackType >      L1TrackCollectionType;

  //just for internal use
  //typedef std::map< unsigned int, std::vector< GlobalStubRefType > >  GlobalStubMapType;
  typedef std::map< unsigned int, std::vector< GlobalStubPtrType > >  GlobalStubMapType;
  typedef std::map< unsigned int, std::vector< TrackletPtrType > >    TrackletMapType;
  typedef std::set< std::pair<unsigned int , GlobalStubPtrType> > TrackletMap;

  public:
    explicit L1TrackBuilderLB(const edm::ParameterSet& iConfig): mClassInfo( new cmsUpgrades::classInfo(__PRETTY_FUNCTION__) )
    {
      produces<L1TrackCollectionType>("L1Tracks");

      GlobalStubsInputTag  = iConfig.getParameter<edm::InputTag>("GlobalStubs");
      TrackletsInputTag  = iConfig.getParameter<edm::InputTag>("Tracklets");

      aPtRef = iConfig.getParameter< std::vector<double> >("aptref");

      aConfig = iConfig;
    }

    ~L1TrackBuilderLB(){}

  private:
    
    edm::InputTag theBeamSpotLabel;
    edm::InputTag customLabel;
    double xbeam, ybeam, zbeam;

    double deltaPhi( double phi1, double phi2 ){
      double dp = phi1 - phi2;
      double pigreco = 4.0*atan(1.0);
      if (fabs(dp) < pigreco) return fabs(dp);
      else return (2*pigreco - fabs(dp));
    };

    double deltaPhiNP( double phi1, double phi2 ){
      double deltaPhiNP = phi1 - phi2;
      double pigreco = 4.0*atan(1.0);
      if ( fabs(deltaPhiNP) >= pigreco) {
      if ( deltaPhiNP>0 ) deltaPhiNP = deltaPhiNP - 2*pigreco;
      else deltaPhiNP = 2*pigreco - fabs(deltaPhiNP);
      }
      return deltaPhiNP;
    };
   
    virtual void beginJob(const edm::EventSetup& iSetup)
    {
      iSetup.get<cmsUpgrades::StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
      theStackedGeometry = StackedTrackerGeomHandle.product();

      iSetup.get<IdealMagneticFieldRecord>().get(magnet);
      magnet_ = magnet.product();
      mMagneticFieldStrength = magnet_->inTesla(GlobalPoint(0,0,0)).z();
    }

    virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
    {
      /// /////////////////////
      /// GET MAGNETIC FIELD //
      /// /////////////////////
      edm::ESHandle<MagneticField> magnet;
      iSetup.get<IdealMagneticFieldRecord>().get(magnet);
      const MagneticField *magnetF;
      magnetF = magnet.product();
      double mMagneticFieldStrength = magnetF->inTesla(GlobalPoint(0,0,0)).z();

      /// /////////////////////
      /// Get the Tracklets ///
      /// /////////////////////
      edm::Handle< TrackletCollectionType > TrackletHandle;
      iEvent.getByLabel( TrackletsInputTag , TrackletHandle);
      unsigned int max_superlayer=0;

      /// ////////////////////////////////////
      /// Map the Tracklets per SuperLayer ///
      /// 0-01 1-23 2-45 3-67 4-89         ///
      /// ////////////////////////////////////
      TrackletMapType Tracklets;
      for (  unsigned int i = 0; i != TrackletHandle->size() ; ++i ) {
        /// Select only Hermetic Tracklets
        if ( !(TrackletHandle->at(i).isHermetic()) ) continue;
        unsigned int layer  = TrackletHandle->at(i).stubs().begin()->second->Id().layer();
        unsigned int layer0 = TrackletHandle->at(i).stubs().rbegin()->second->Id().layer();
        unsigned int superlayer = layer/2;
        if (superlayer != (layer0-1)/2) continue; /// Reject bad formed Tracklets
        Tracklets[ superlayer ].push_back( TrackletPtrType( TrackletHandle , i ) );
        if( superlayer > max_superlayer ) max_superlayer = superlayer;
      }

      /// /////////////////////////////////////
      /// Get the Stubs and the DigiSimLink ///
      /// /////////////////////////////////////
      edm::Handle< GlobalStubCollectionType > GlobalStubHandle;
      iEvent.getByLabel( GlobalStubsInputTag , GlobalStubHandle);
      edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink;
      iEvent.getByLabel("simSiPixelDigis", thePixelDigiSimLink);
      unsigned int max_layer=0;

      /// ///////////////////////////
      /// Map the stubs per Layer ///
      /// 01 23 45 67 89          ///
      /// ///////////////////////////
      GlobalStubMapType GlobalStubs;
      for (  unsigned int i = 0; i != GlobalStubHandle->size() ; ++i ) {
        unsigned int layer = GlobalStubHandle->at(i).Id().layer();
        GlobalStubs[ layer ].push_back( GlobalStubPtrType( GlobalStubHandle , i ) );
        if( layer > max_layer ) max_layer = layer;
      }

      /// ///////////////////////////////////////////
      /// Map the stubs per SuperLayer            ///
      /// The rule is that if superlayer is "j"   ///
      /// the corresponding map is built with     ///
      /// Stubs in superlayers different from "j" ///
      /// ///////////////////////////////////////////
      GlobalStubMapType BrickStubs;
      for (  unsigned int i = 0; i != GlobalStubHandle->size() ; ++i ) {
        unsigned int layer = GlobalStubHandle->at(i).Id().layer();
        unsigned int superlayer;
        if (layer%2==0) superlayer = layer/2;
        else superlayer = (layer-1)/2;
        for (unsigned int q=0; q< max_superlayer+1; q++) {
          if (superlayer != q) BrickStubs[ q ].push_back( GlobalStubPtrType( GlobalStubHandle , i ) );
        }
      }

      std::auto_ptr< L1TrackCollectionType > L1TracksOutput(new L1TrackCollectionType );

      /// /////////////////
      /// BUILD TRACKS!! //
      /// /////////////////
      /// Define the output
      std::vector< cmsUpgrades::L1Track< T > > tracksToClean;
      /// Loop on SupeLayers
      for (int j=0; j<max_superlayer+1; j++) {

        /// Loop on Seeds
        for ( unsigned int k=0; k<Tracklets[j].size(); k++ ) {

          /// Get all the candidates from this seed
          /// including combinatorics
          std::vector< cmsUpgrades::L1Track< T > > candidates;
          double prob;
          
    /// 99% windows
          candidates.clear();
          prob = 99.0;
          candidates = makeL1Track( iSetup,
                                    Tracklets[j].at(k), prob,
                                    BrickStubs[j], true );
          /// Loop on candidates
          /// just push back in the global storage of L1Tracks
          for (unsigned int h=0; h<candidates.size(); h++) {
            L1TracksOutput->push_back( candidates.at(h) );
          } /// End of loop on candidates
    /// 98% windows
          candidates.clear();
          prob = 98.0;
          candidates = makeL1Track( iSetup,
                                    Tracklets[j].at(k), prob,
                                    BrickStubs[j], true );
          /// Loop on candidates
          /// just push back in the global storage of L1Tracks
          for (unsigned int h=0; h<candidates.size(); h++) {
            L1TracksOutput->push_back( candidates.at(h) );
          } /// End of loop on candidates
    /// 95% windows
          candidates.clear();
          prob = 95.0;
          candidates = makeL1Track( iSetup,
                                    Tracklets[j].at(k), prob,
                                    BrickStubs[j], true );
          /// Loop on candidates
          /// just push back in the global storage of L1Tracks
          for (unsigned int h=0; h<candidates.size(); h++) {
            L1TracksOutput->push_back( candidates.at(h) );
          } /// End of loop on candidates
    /// 90% windows
          candidates.clear();
          prob = 90.0;
          candidates = makeL1Track( iSetup,
                                    Tracklets[j].at(k), prob,
                                    BrickStubs[j], true );
          /// Loop on candidates
          /// just push back in the global storage of L1Tracks
          for (unsigned int h=0; h<candidates.size(); h++) {
            L1TracksOutput->push_back( candidates.at(h) );
          } /// End of loop on candidates

        } /// End of loop on seeds
      } /// End of loop on Superlayers

      /// Store
      iEvent.put(L1TracksOutput, "L1Tracks");

      /// Free some memory
      GlobalStubs.clear();
    }

    virtual void endJob(){
    }
      
//
// The class members
//
    edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> StackedTrackerGeomHandle;
    const cmsUpgrades::StackedTrackerGeometry *theStackedGeometry;

    edm::InputTag GlobalStubsInputTag;
    edm::InputTag TrackletsInputTag;
    edm::ParameterSet aConfig;
    std::vector<double> aPtRef;
    
    edm::ESHandle<MagneticField> magnet;
    const MagneticField *magnet_;
    double mMagneticFieldStrength;
    const cmsUpgrades::classInfo *mClassInfo;

    std::vector< cmsUpgrades::L1Track< T > >
                makeL1Track( const edm::EventSetup& iSetup,
                             TrackletPtrType seed, double prob,
                             std::vector< GlobalStubPtrType > bricks,
                             bool erase_nulls );
                             
    std::pair<double,PSimHit> makeHit( const GeomDetUnit* dU, BaseParticlePropagator* tP,
                                       FreeTrajectoryState hV, int pID, int tkID );

};

//////////////
// L1TRACKS //
//////////////
template <typename T>
std::vector< cmsUpgrades::L1Track< T > >
L1TrackBuilderLB< T >::makeL1Track( const edm::EventSetup& iSetup,
                            TrackletPtrType seed, double prob,
                            std::vector< GlobalStubPtrType > bricks,
                            bool erase_nulls ) {
  std::vector< cmsUpgrades::L1Track< T > > output;
  output.clear();

  /// /////////////////////
  /// GET MAGNETIC FIELD //
  /// /////////////////////
  edm::ESHandle<MagneticField> magnet;
  iSetup.get<IdealMagneticFieldRecord>().get(magnet);
  const MagneticField *magnetF;
  magnetF = magnet.product();
  double mMagneticFieldStrength = magnetF->inTesla(GlobalPoint(0,0,0)).z();

  /// Step00: linear interpolation of Pt!
  /// NOTE assumes size > 1 !!!
  double ptSeed = seed->twoPointPt();
  int whichOne = -1;
  for (unsigned int i=0; i<aPtRef.size(); i++) {
    if (ptSeed > aPtRef.at(i) ) whichOne = (int)i;
  }
  /// Now whichOne contains the index of the largest
  /// aPtRef lower than ptSeed. i.e. if ptSeed = 25 and
  /// aPtRef = {3,10,20,30} whichOne = 2. ok?

  /// qui bisogna prendere e fare i casi diversi... che pigna...
  double scaleFactor = 0;
  if (whichOne == aPtRef.size()-1) {
    whichOne -= 1;
    scaleFactor = 1;
  }
  else if (whichOne < 0) {
    whichOne = 0;
    //scaleFactor = 0;
    scaleFactor = (ptSeed - aPtRef.at(whichOne)) / (aPtRef.at(whichOne+1) - aPtRef.at(whichOne));
    /// Here we keep linear interpolation also below 3 GeV/c Pt, using the same slope as
    /// in the first interval between 3 and 5 GeV/c
  }
  else {
    scaleFactor = (ptSeed - aPtRef.at(whichOne)) / (aPtRef.at(whichOne+1) - aPtRef.at(whichOne));
  }
  
  /// Build names.
  std::ostringstream phiBiasName;
  std::ostringstream phiWinName;
  std::ostringstream zWinName;
  int probLabel = (int)prob;
  phiBiasName << "a" << "phibias";
  phiWinName << "a" << probLabel << "phiwin";
  zWinName << "a" << probLabel << "zwin";

  /// Step 0; constraints
  /// from superlayer [5] to layer [10]
  double biasPhi[5][10];
  double windowsPhi[5][10];
  double windowsZ[5][10];

  /// Initialize
  for (int v=0; v<10; v++) {
    for (int m=0; m<5; m++) {
      biasPhi[m][v] = 0;
      windowsPhi[m][v] = -9999.9;
      windowsZ[m][v] = -9999.9;
    }
  }

  std::vector<double> aBiasPhiRef;
  std::vector<double> aWinPhiRef;
  std::vector<double> aWinZRef;

  /// Get the charge of the Particle
  /// NOTE the minus sign is after 22nd May debugging
  double trkQ = -seed->deltaPhiNorm() / fabs( seed->deltaPhiNorm() );

  /// Load Tables with constraints
  for (int v=0; v<10; v++) {
    for (int m=0; m<5; m++) {
      /// Reject bad Seed SL/Target L combinations and keep default
      if (m==0 && v<2) continue;
      if (m==1 && (v==2 || v==3)) continue;
      if (m>1 && v>3) continue;
      /// Here we have only good pairs.
      std::ostringstream phiBiasName1;
      std::ostringstream phiWinName1;
      std::ostringstream zWinName1;
      phiBiasName1 << phiBiasName.str().c_str() << m << v;
      phiWinName1 << phiWinName.str().c_str() << m << v;
      zWinName1 << zWinName.str().c_str() << m << v;

      aBiasPhiRef = aConfig.getParameter< std::vector<double> >(phiBiasName1.str().c_str());
      aWinPhiRef = aConfig.getParameter< std::vector<double> >(phiWinName1.str().c_str());
      aWinZRef = aConfig.getParameter< std::vector<double> >(zWinName1.str().c_str());
      
      biasPhi[m][v] = trkQ*aBiasPhiRef.at(whichOne)+scaleFactor*(aBiasPhiRef.at(whichOne+1)-aBiasPhiRef.at(whichOne));
      windowsPhi[m][v] = aWinPhiRef.at(whichOne)+scaleFactor*(aWinPhiRef.at(whichOne+1)-aWinPhiRef.at(whichOne));
      windowsZ[m][v] = aWinZRef.at(whichOne)+scaleFactor*(aWinZRef.at(whichOne+1)-aWinZRef.at(whichOne));
    }
  }

  /// Get Seed properties
  /// Stubs of this Tracklet, and Local ones
  TrackletMap theStubs = seed->stubs();
  GlobalStubPtrType innerStub = theStubs.begin()->second;
  GlobalStubPtrType outerStub = theStubs.rbegin()->second;
  FreeTrajectoryState helVtx = seed->VertexTrajectoryState( iSetup ); /// This is needed only because provides curvature information
                                                                      /// which is unaffected by longitudinal plane fit
  /// Renormalize layer number from 5-14 to 0-9
  cmsUpgrades::StackedTrackerDetId innerId = innerStub->Id();
  cmsUpgrades::StackedTrackerDetId outerId = outerStub->Id();
  int normLayerInner = innerId.layer();
  int normLayerOuter = outerId.layer();
  /// Get the Double Stack index
  int dSidx = normLayerInner/2;
  if (dSidx != (normLayerOuter-1)/2 ) std::cerr << "HOUSTON WE GOT A PROBLEM!" << std::endl;

  /// Find Tracklet Direction to work with Pt x and y
  /// compoments to put in propagators to enable
  /// propagation from tracklet and not from vertexIndex
  GlobalVector trkDirection = outerStub->position() - innerStub->position();
  /// Renormalize to 1
  trkDirection = trkDirection / trkDirection.mag();
  /// Propagate the seed Tracklet
  /// First step is to construct the propagator
  double mom[4] = {0,0,0,0};
  /// Pz = Pt / tan theta
  double DELTAR = (outerStub->position() - seed->vertex()).perp() - (innerStub->position() - seed->vertex()).perp();
  double DELTAZ = outerStub->position().z() - innerStub->position().z();
  mom[3] = seed->twoPointPz();//t() * DELTAZ / DELTAR; // this comes from old fit
  mom[2] = seed->twoPointPt() * trkDirection.y()/trkDirection.perp();
  mom[1] = seed->twoPointPt() * trkDirection.x()/trkDirection.perp();
  mom[0] = sqrt( 0.1*0.1 + mom[1]*mom[1] + mom[2]*mom[2] + mom[3]*mom[3] );
  math::XYZTLorentzVector trkMom( mom[1], mom[2], mom[3], mom[0] );
  /// Energy component is due to the fact that both pions and muons have ~100 MeV mass
  double pos[4] = {0,0,0,0};
  pos[0] = 0.0;
  pos[1] = innerStub->position().x() + outerStub->position().x();
  pos[2] = innerStub->position().y() + outerStub->position().y();
  pos[3] = innerStub->position().z() + outerStub->position().z();
  math::XYZTLorentzVector trkPos( 0.5*pos[1], 0.5*pos[2], 0.5*pos[3], pos[0] );

  /// We can declare a BaseParticlePropagator without time component of VTX
  RawParticle trkRaw( trkMom , trkPos );
  trkRaw.setCharge( trkQ );

  /// Build one propagator for each Stack
  /// Remember that short barrels count twice, however, as BaseParticlePropagator
  /// thinks only in terms of symmetric z bounds, we will work with a double
  /// propagation with an accept-forward/exclude-central anticoincidence
  double StackR[14] = { 32.0, 36.0,
                        48.0, 52.0,
                        64.3, 68.3, /// Short Barrel
                        80.3, 84.3, /// Short Barrel
                        98.5, 102.5,
                        64.3, 68.3,   /// Veto
                        80.3, 84.3 }; /// Veto
  double StackZ[14] = { 420.0, 420.0,
                        540.0, 540.0,
                        540.0, 540.0, /// Short Barrel
                        540.0, 540.0, /// Short Barrel
                        540.0, 540.0,
                        420.0, 420.0,    /// Veto
                        420.0, 420.0 };  /// Veto
  /// Build Propagators and Propagate
  /// NOTE do that only for the right layers
  BaseParticlePropagator* trkProp[14];
  for(int k=0; k<10; k++) {
    /// Do not propagate to the same layers as
    /// the seed ones
    if ( k==2*dSidx || k==2*dSidx+1 ) continue;
    if ( k > 2*dSidx ) {
      trkProp[k] = new BaseParticlePropagator( trkRaw, StackR[k], StackZ[k]/2.0, mMagneticFieldStrength );
      trkProp[k]->propagate();
    }
    else {
      /// WARNING: if the destination layer is inside
      /// the current one, change sign to the momentum!
      trkProp[k] = new BaseParticlePropagator( trkRaw, StackR[k], StackZ[k]/2.0, mMagneticFieldStrength );
      trkProp[k]->backPropagate();
    }
    if (k>3 && k<8) { /// Propagate for vetoes
      if ( k > 2*dSidx ) {
        trkProp[k+6] = new BaseParticlePropagator( trkRaw, StackR[k+6], StackZ[k+6]/2.0, mMagneticFieldStrength );
        trkProp[k+6]->propagate();
      }
      else {
        /// WARNING: if the destination layer is inside
        /// the current one, change sign to the momentum!
        trkProp[k+6] = new BaseParticlePropagator( trkRaw, StackR[k+6], StackZ[k+6]/2.0, mMagneticFieldStrength );
        trkProp[k+6]->backPropagate();
      }
    }
  } /// End of Propagator construction

  /// Ok, here we have the up-dated status of the
  /// propagated Tracklet at the nominal radius
  /// Store matched Stubs on a layer-wise way
  std::vector< GlobalStubType > stub0[10];
  stub0[normLayerInner].push_back( *innerStub );
  stub0[normLayerOuter].push_back( *outerStub );
  GlobalPoint vertexBeamSpot;
  /// Loop on Brick Stubs
  for (unsigned int j=0; j<bricks.size(); j++) {

    GlobalStubPtrType tobematched = bricks.at(j);

    /// Skip same SL bricks
    if ( tobematched->Id().layer()==2*dSidx || tobematched->Id().layer()==2*dSidx+1 ) continue;
    /// Get success
    bool accepted = false;

    int success = trkProp[tobematched->Id().layer()]->getSuccess();
    if (tobematched->Id().layer()>=4 && tobematched->Id().layer()<=7) {
      int complementarysuccess = trkProp[tobematched->Id().layer()+6]->getSuccess();
      /// 10 is complementary to 4
      /// 11 is complementary to 5
      /// 12 is complementary to 6
      /// 13 is complementary to 7
      if (success==1 && complementarysuccess!=1) accepted = true;
      /// Go on only if SUCCESS is 1 for full barrel
      /// but is not 1 for the central gap
    }
    else { /// We are propagating to a long barrel
      if (success==1) accepted = true;; /// Go on only if SUCCESS is 1
    }

    if (accepted==false) continue;

    /// Get DetID
    cmsUpgrades::StackedTrackerDetId IdTBM = tobematched->Id();
    /// Check also consecutive modules
    cmsUpgrades::StackedTrackerDetId IdTBMp1(IdTBM.subdet(), IdTBM.layer(), IdTBM.iPhi(), IdTBM.iZ()+1);
    cmsUpgrades::StackedTrackerDetId IdTBMm1(IdTBM.subdet(), IdTBM.layer(), IdTBM.iPhi(), IdTBM.iZ()-1);

    for (int cntmodz=0; cntmodz<1; cntmodz++) {
      /// First iteration is current module, second iteration is plus 1 in z,
      /// third iterazion is minus 1 in z
      if (cntmodz==1) {
        IdTBM = IdTBMp1;
      }
      if (cntmodz==2) {
        IdTBM = IdTBMm1;
      }
      /// Get GeomDetUnits to be used later on
      const GeomDetUnit* Id0detUnitTBM = theStackedGeometry->idToDetUnit(IdTBM,0);
      const GeomDetUnit* Id1detUnitTBM = theStackedGeometry->idToDetUnit(IdTBM,1);

      /// Set boolean variables to say there is the match or not
      bool isMatched = false;
      int k = IdTBM.layer();

      /// If we are here, it means that the Tracklet could be
      /// propagated to the n-th Stack Layer!
      /// Let's make the "mimic-PSimHit"
      GeomDetUnit* detUnitHit;
      if ( k > 2*dSidx ) /// Forward
        detUnitHit = (GeomDetUnit*)Id0detUnitTBM;
      else /// Back
        detUnitHit = (GeomDetUnit*)Id1detUnitTBM;
      std::pair<double,PSimHit>  hitProp = makeHit( detUnitHit, trkProp[k], helVtx, -99999, -99999 );

      /// makeHit acceptance
      if (hitProp.first > 0) { /// Go on only if the first member of the pair
                               /// is strictly positive
        /// Get the BRICK corresponding stub position
        GlobalPoint stubTBMglobalPosition_hits;
        if ( k > 2*dSidx ) /// Forward
          stubTBMglobalPosition_hits = tobematched->localStub()->averagePosition( &(*theStackedGeometry) , 0);
        else /// Back
          stubTBMglobalPosition_hits = tobematched->localStub()->averagePosition( &(*theStackedGeometry) , 1);
        /// WORK FOR THE MATCH
        GlobalPoint curHitglobalPosition = detUnitHit->surface().toGlobal( hitProp.second.localPosition() );
        /// Get coordinates of corresponding Pixel, just as in
        /// SimTracker/SiPixelDigitizer/interface/SiPixelDigitizerAlgorithm.h
        /// NOTE this is a very simplified version
        const PixelGeomDetUnit* pixelDet = dynamic_cast<const PixelGeomDetUnit*>( detUnitHit );
        const PixelTopology* pixelTopol = dynamic_cast<const PixelTopology*>( &(pixelDet->specificTopology()));
        int numColumns = pixelTopol->ncolumns();  // det module number of cols&rows
        int numRows = pixelTopol->nrows();
        MeasurementPoint mpDigi = pixelTopol->measurementPosition( hitProp.second.localPosition() );
        int IPixRightUpX = int( floor( mpDigi.x()));
        int IPixRightUpY = int( floor( mpDigi.y()));
        int IPixLeftDownX = int( floor( mpDigi.x()));
        int IPixLeftDownY = int( floor( mpDigi.y()));
        IPixRightUpX = numRows>IPixRightUpX ? IPixRightUpX : numRows-1 ;
        IPixRightUpY = numColumns>IPixRightUpY ? IPixRightUpY : numColumns-1 ;
        IPixLeftDownX = 0<IPixLeftDownX ? IPixLeftDownX : 0 ;
        IPixLeftDownY = 0<IPixLeftDownY ? IPixLeftDownY : 0 ;
        float ix = 0.5*(IPixLeftDownX + IPixRightUpX);
        float iy = 0.5*(IPixLeftDownY + IPixRightUpY);
        mpDigi = MeasurementPoint (ix + 0.5, iy + 0.5);
        GlobalPoint curHitglobalPositionDigi = detUnitHit->surface().toGlobal( pixelTopol->localPosition(mpDigi) );
        /// Correct for beamspot position
        vertexBeamSpot = GlobalPoint(seed->vertex().x(), seed->vertex().y(), (double)0.0 );          
        curHitglobalPosition = GlobalPoint(curHitglobalPosition.x() - vertexBeamSpot.x(),
                                           curHitglobalPosition.y() - vertexBeamSpot.y(),
                                           curHitglobalPosition.z() - vertexBeamSpot.z());
        stubTBMglobalPosition_hits = GlobalPoint(stubTBMglobalPosition_hits.x() - vertexBeamSpot.x(),
                                                 stubTBMglobalPosition_hits.y() - vertexBeamSpot.y(),
                                                 stubTBMglobalPosition_hits.z() - vertexBeamSpot.z());
        curHitglobalPositionDigi = GlobalPoint(curHitglobalPositionDigi.x() - vertexBeamSpot.x(),
                                               curHitglobalPositionDigi.y() - vertexBeamSpot.y(),
                                               curHitglobalPositionDigi.z() - vertexBeamSpot.z());   
        /// LOAD CONSTRAINTS FROM A TABLE TO BE WRITTEN
        double dZ = fabs( curHitglobalPositionDigi.z() - stubTBMglobalPosition_hits.z() );
        double dPhi = deltaPhiNP( stubTBMglobalPosition_hits.phi() , curHitglobalPositionDigi.phi() );
        /// NOTE: DO NOT CHECK Z consistency of seed and brick tracklets
        /// information not available for Stubs
        /// CHECK MATCH
        if ( dZ <= windowsZ[dSidx][k] &&
             fabs(dPhi - biasPhi[dSidx][k]) <= windowsPhi[dSidx][k] ) isMatched = true;

        /// SAVE THE PAIR LATER ON, IF BOTH STUBS ARE WITHIN LIMITS
      } /// End of makeHit acceptance
      /// 
      if ( isMatched ) stub0[k].push_back( *(tobematched) );
    } /// End of loop over adjacent modules
  } /// End of loop over Bricks

  /// Fill with NULL those empty vectors
  for (int y=0; y<10; y++) {
    if (stub0[y].size() == 0) stub0[y].push_back( GlobalStubType() );
  }
  cmsUpgrades::StackedTrackerDetId nullDet = GlobalStubType().Id();
  /// Create all possible multiplets
  for (unsigned int r0 = 0; r0 < stub0[0].size(); r0++) {
    for (unsigned int r1 = 0; r1 < stub0[1].size(); r1++) {
      for (unsigned int r2 = 0; r2 < stub0[2].size(); r2++) {
        for (unsigned int r3 = 0; r3 < stub0[3].size(); r3++) {
          for (unsigned int r4 = 0; r4 < stub0[4].size(); r4++) {
            for (unsigned int r5 = 0; r5 < stub0[5].size(); r5++) {
              for (unsigned int r6 = 0; r6 < stub0[6].size(); r6++) {
                for (unsigned int r7 = 0; r7 < stub0[7].size(); r7++) {
                  for (unsigned int r8 = 0; r8 < stub0[8].size(); r8++) {
                    for (unsigned int r9 = 0; r9 < stub0[9].size(); r9++) {
                      std::vector<  GlobalStubType > fghij;
                      fghij.clear();
                      if (erase_nulls==false) {
                        fghij.push_back( stub0[0].at(r0) );
                        fghij.push_back( stub0[1].at(r1) );
                        fghij.push_back( stub0[2].at(r2) );
                        fghij.push_back( stub0[3].at(r3) );
                        fghij.push_back( stub0[4].at(r4) );
                        fghij.push_back( stub0[5].at(r5) );
                        fghij.push_back( stub0[6].at(r6) );
                        fghij.push_back( stub0[7].at(r7) );
                        fghij.push_back( stub0[8].at(r8) );
                        fghij.push_back( stub0[9].at(r9) );
                      }
                      else {
                        if (stub0[0].at(r0).Id() !=nullDet) fghij.push_back( stub0[0].at(r0) );
                        if (stub0[1].at(r1).Id() !=nullDet) fghij.push_back( stub0[1].at(r1) );
                        if (stub0[2].at(r2).Id() !=nullDet) fghij.push_back( stub0[2].at(r2) );
                        if (stub0[3].at(r3).Id() !=nullDet) fghij.push_back( stub0[3].at(r3) );
                        if (stub0[4].at(r4).Id() !=nullDet) fghij.push_back( stub0[4].at(r4) );
                        if (stub0[5].at(r5).Id() !=nullDet) fghij.push_back( stub0[5].at(r5) );
                        if (stub0[6].at(r6).Id() !=nullDet) fghij.push_back( stub0[6].at(r6) );
                        if (stub0[7].at(r7).Id() !=nullDet) fghij.push_back( stub0[7].at(r7) );
                        if (stub0[8].at(r8).Id() !=nullDet) fghij.push_back( stub0[8].at(r8) );
                        if (stub0[9].at(r9).Id() !=nullDet) fghij.push_back( stub0[9].at(r9) );                      
                      }
                      /// Here the Track constructor
                      cmsUpgrades::L1Track< T > abcde( fghij, *seed, mMagneticFieldStrength, prob );
                      output.push_back(abcde);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return output;
}





//////////////////////////////////////
// Find "Hit" from Propagated Tracklet
template <typename T>
std::pair<double,PSimHit> L1TrackBuilderLB< T >::makeHit( const GeomDetUnit* dU, BaseParticlePropagator* tP,
                                                      FreeTrajectoryState hV, int pID, int tkID ) {
  const float onSurfaceTolarance = 0.01; /// 10 microns
  math::XYZTLorentzVector globVtx = tP->vertex();
  math::XYZTLorentzVector globMom = tP->momentum();
  LocalPoint lpos;
  LocalVector lmom;
  GlobalPoint gpos = GlobalPoint(  globVtx.x(), globVtx.y(), globVtx.z() );
  GlobalVector gmom = GlobalVector(  globMom.x(), globMom.y(), globMom.z() );
  GlobalPoint pCentDet = dU->toGlobal( dU->topology().localPosition( MeasurementPoint( 0.5*dU->surface().bounds().width(), 0.5*dU->surface().bounds().length()   ) ) );
  
  /// Apply constraint on "phi-wedge"
  if ( deltaPhi(gpos.phi(), pCentDet.phi()) > 4.0*atan(1.0)/12.0 ) return std::pair<double,PSimHit>(-999.9,PSimHit());
  PropagationDirection dirP = anyDirection;

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

  LocalPoint entry = lpos + (-halfThick/pZ) * lmom;
  LocalPoint exit = lpos + halfThick/pZ * lmom;
  float tof = gpos.mag() / 30. ; // in nanoseconds, FIXME: very approximate
  // FIXME: fix the track ID and the particle ID
  PSimHit hit( entry, exit, lmom.mag(), tof, 0, pID,
                dU->geographicalId().rawId(), tkID,
                lmom.theta(), lmom.phi() );

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
    /// NOTE THIS IS NOT IMPORTANT AS ONLY THE SIGN MATTERS
    /// IN FACT hV IS OBTAINED FROM A WRONG KIND OF FIT ON LONGITUDINAL PLANE
    /// HOWEVER THE CURVATURE ESTIMATE IS FINE AND THIS IS THE ONLY
    /// NUMBER COMING FROM hV WHICH IS OF REAL INTEREST HERE!
    
  return std::pair<double,PSimHit>(dist,hit);
}

//
// constants, enums and typedefs
//


//
// static data member definitions
//


#endif


