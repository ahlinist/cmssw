/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
/// Written by:                          ///
/// Andrew W. Rose                       ///
/// 2008                                 ///
///                                      ///
/// Changed by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, June, August, October          ///
///                                      ///
/// Added features:                      ///
/// Tracklets must be made of Stubs in   ///
/// consecutive Stacks with inner one    ///
/// which is odd in order and outer one  ///
/// which is even (LB Hermetic Design).  ///
/// Other options (e.g. 2nd-3rd) are     ///
/// FORBIDDEN!                           ///
/// Possibility to have a flag telling   ///
/// if the Tracklet is ok with the       ///
/// constraints imposed by hermetic      ///
/// design. Just work with iPhi()        ///
/// Possibility to have Fake Tracklet    ///
/// flag in Simulations 'isFake()' and   ///
/// Trk ID too 'trackID()'. A Tracklet   ///
/// is flagged as Fake in two cases:     ///
/// 1) at least one Stub is Fake         ///
/// 2) both Stubs are genuine but coming ///
/// from different SimTracks             ///
/// Just get Stub fakeness and/or use    ///
/// the same procedure with the same     ///
/// CAVEATs.                             ///
/// Possibility to have a Tracklet VTX   ///
/// which is different from the default  ///
/// one (0,0,z). Just try to build 2     ///
/// Tracklets from each pair of Stubs,   ///
/// one with (0,0,z), one with the other ///
/// option which is supposed to be the   ///
/// "true" one.                          ///
/// Find Axis and VTX by TRUE helix fit  ///
///                                      ///
/// Changed by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, November                       ///
/// Get rid of Cramer, keeping helix     ///
/// ////////////////////////////////////////

#ifndef TRACKLET_BUILDER_H
#define TRACKLET_BUILDER_H

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
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetUnit.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetId.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

template< typename T >
class TrackletBuilder : public edm::EDProducer {

  typedef cmsUpgrades::GlobalStub< T >                  GlobalStubType;
  typedef std::vector  < GlobalStubType >               GlobalStubCollectionType;
  //typedef edm::Ref< GlobalStubCollectionType, GlobalStubType >  GlobalStubRefType;
  typedef edm::Ptr< GlobalStubType >  GlobalStubPtrType;
  typedef std::vector<  cmsUpgrades::Tracklet< T > >    TrackletCollectionType;
  
  //just for internal use
  //typedef std::map< unsigned int, std::vector< GlobalStubRefType > >   GlobalStubMapType;
  typedef std::map< unsigned int, std::vector< GlobalStubPtrType > >   GlobalStubMapType;

  public:
    explicit TrackletBuilder(const edm::ParameterSet& iConfig): mClassInfo( new cmsUpgrades::classInfo(__PRETTY_FUNCTION__) )
    {
      produces<TrackletCollectionType>("ShortTracklets");

      mPtThreshold = iConfig.getParameter<double>("minPtThreshold");
      mIPWidth = iConfig.getParameter<double>("ipWidth");
      mFastPhiCut = iConfig.getParameter<double>("fastPhiCut");
      GlobalStubsInputTag  = iConfig.getParameter<edm::InputTag>("GlobalStubs");
      
      theBeamSpotLabel = iConfig.getParameter<edm::InputTag>("BeamSpotLabel");
      customLabel = iConfig.getParameter<edm::InputTag>("customLabel");
      
      xbeam = iConfig.getParameter<double>("BeamSpotSetX");
      ybeam = iConfig.getParameter<double>("BeamSpotSetY");
      zbeam = iConfig.getParameter<double>("BeamSpotSetZ");
    }

    ~TrackletBuilder(){}

  private:

    edm::InputTag theBeamSpotLabel;
    edm::InputTag customLabel;
    double xbeam, ybeam, zbeam;

    virtual void beginJob(const edm::EventSetup& iSetup)
    {
      iSetup.get<cmsUpgrades::StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
      theStackedTracker = StackedTrackerGeomHandle.product();

      iSetup.get<IdealMagneticFieldRecord>().get(magnet);
      magnet_ = magnet.product();
      mMagneticFieldStrength = magnet_->inTesla(GlobalPoint(0,0,0)).z();
      // Compute the scaling factor (conversion cm->m, Gev-c factor, magnetic field)
      mCompatibilityScalingFactor = (100.0 * 2.0e+9 * mPtThreshold) / (cmsUpgrades::KGMS_C * mMagneticFieldStrength);
      // Invert so we use multiplication instead of division in the comparison
      mCompatibilityScalingFactor = 1.0 / mCompatibilityScalingFactor;
    }

    virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
    {
      unsigned int max_layer=0;

      edm::Handle< GlobalStubCollectionType > GlobalStubHandle;
      iEvent.getByLabel( GlobalStubsInputTag , GlobalStubHandle);

      edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink;
      iEvent.getByLabel("simSiPixelDigis", thePixelDigiSimLink);
      
      // The beam spot position
      edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
      iEvent.getByLabel(theBeamSpotLabel,recoBeamSpotHandle); 
      math::XYZPoint BSPosition_;
      if (theBeamSpotLabel==customLabel)  {
          BSPosition_.SetX(xbeam);
          BSPosition_.SetY(ybeam);
          BSPosition_.SetZ(zbeam);
      }
      else BSPosition_ = recoBeamSpotHandle->position();
  
      GlobalStubMapType GlobalStubs;
      //GlobalStubMapType::const_iterator GlobalStubsIter;

      for (  unsigned int i = 0; i != GlobalStubHandle->size() ; ++i ) {
        unsigned int layer = GlobalStubHandle->at(i).Id().layer();
        //GlobalStubs[ layer ].push_back( GlobalStubRefType( GlobalStubHandle , i ) );
        GlobalStubs[ layer ].push_back( GlobalStubPtrType( GlobalStubHandle , i ) );
        if( layer > max_layer ) max_layer = layer;
      }

      ////////////////////////////////////////////////////////////////////////////////////////////
      //First get the global stubs and form short tracklets...
      ////////////////////////////////////////////////////////////////////////////////////////////

      std::auto_ptr< TrackletCollectionType > ShortTrackletOutput(new TrackletCollectionType );

      for  ( unsigned int innerLayerNum = 0 ; innerLayerNum != max_layer ; ++innerLayerNum ){

        ///*** NEW for 3_3_6
        ///*** forbid all Tracklets made across different
        ///*** Double Stacks (eg Outer L6 Inner L7)
        ///*** If inner Layer is ODD, skip it
        if ( innerLayerNum % 2 == 1 ) continue;
        
        //std::vector< GlobalStubRefType > innerHits, outerHits;
        //typedef typename std::vector< GlobalStubRefType >::iterator VRT_IT;
        std::vector< GlobalStubPtrType > innerHits, outerHits;
        typedef typename std::vector< GlobalStubPtrType >::iterator VRT_IT;

        if ( GlobalStubs.find( innerLayerNum )  !=  GlobalStubs.end() )  innerHits = GlobalStubs.find( innerLayerNum )->second;
        if ( GlobalStubs.find( innerLayerNum+1 )!=  GlobalStubs.end() )  outerHits = GlobalStubs.find( innerLayerNum+1 )->second;

        if( innerHits.size() && outerHits.size() ){
          for( VRT_IT innerHitIter = innerHits.begin() ; innerHitIter != innerHits.end() ; ++innerHitIter ){
            for( VRT_IT outerHitIter = outerHits.begin() ; outerHitIter != outerHits.end() ; ++outerHitIter ){

              ///*** NEW for 3_3_6
              ///*** we want both stubs to be in the same phi-sector
              ///*** (see Hermetic RPhi Section)
              ///*** and therefore a cross check is needed to
              ///*** forbid Tracklets across different ladders
              cmsUpgrades::StackedTrackerDetId IdetID = (**innerHitIter).Id();
              cmsUpgrades::StackedTrackerDetId OdetID = (**outerHitIter).Id();

              GlobalPoint inner = (**innerHitIter).position();
              GlobalPoint outer = (**outerHitIter).position();
              double deltaPhi = inner.phi()-outer.phi();
              if (deltaPhi<0) deltaPhi = -deltaPhi;
              if (deltaPhi > cmsUpgrades::KGMS_PI) deltaPhi = 2 * cmsUpgrades::KGMS_PI - deltaPhi;

              GlobalPoint innerBeamSpot(inner.x()-BSPosition_.x(), inner.y()-BSPosition_.y(), inner.z()-BSPosition_.z());
              GlobalPoint outerBeamSpot(outer.x()-BSPosition_.x(), outer.y()-BSPosition_.y(), outer.z()-BSPosition_.z());
              double deltaPhiBeamSpot = innerBeamSpot.phi()-outerBeamSpot.phi();
              if (deltaPhiBeamSpot<0) deltaPhiBeamSpot = -deltaPhiBeamSpot;
              if (deltaPhiBeamSpot > cmsUpgrades::KGMS_PI) deltaPhiBeamSpot = 2 * cmsUpgrades::KGMS_PI - deltaPhiBeamSpot;

              ///*** NEW for 3_3_6
              ///*** fakeness confirmation in MC events
              std::pair<int,int> zxy;
              if (iEvent.isRealData() == false) {                
                zxy = CheckFakeness(*innerHitIter,
                              *outerHitIter//,
                              //theStackedTracker,
                              //thePixelDigiSimLink
                            );
              }

              if( deltaPhi<mFastPhiCut ){ // rough search in phi!
                if( (inner.z()>0&&outer.z()>-mIPWidth) || (inner.z()<0&&outer.z()<+mIPWidth)  ){  //rough search by z sector         

                  double outerPointRadius = outer.perp(); 
                  double innerPointRadius = inner.perp();
                  double deltaRadius = outerPointRadius - innerPointRadius;

                  double deltaPhiThreshold = deltaRadius * mCompatibilityScalingFactor;  
                  if ( deltaPhi < deltaPhiThreshold ) { // detailer search in phi!
                    double positiveZBoundary = (mIPWidth - outer.z()) * deltaRadius;
                    double negativeZBoundary = -(mIPWidth + outer.z()) * deltaRadius;
                    double multipliedLocation = (inner.z() - outer.z()) * outerPointRadius;

                    if ( ( multipliedLocation < positiveZBoundary ) &&   ( multipliedLocation > negativeZBoundary ) ){ // detailed search in z!
                      // all agree so make a tracklet!!!
                      cmsUpgrades::Tracklet<T> tempShortTracklet;
                      tempShortTracklet.addHit( 0 , *innerHitIter );
                      tempShortTracklet.addHit( 1 , *outerHitIter );

                      /// OLD Fit still available
                      double xv = 0.0;
                      double yv = 0.0;

                      double projected_z22X = outer.z() - ( outerPointRadius * (outer.z()-inner.z()) / deltaRadius );
                      tempShortTracklet.addVertex22X( GlobalPoint(xv,yv,projected_z22X ));
                      
                      /// Here the VERY NEW Fit begins
                      double phidiff = outer.phi() - inner.phi();
                      double r1 = inner.perp();
                      double r2 = outer.perp();
                      double x2 = r1*r1 + r2*r2 - 2*r1*r2*cos(phidiff);
                      double radius = 0.5*sqrt(x2)/sin(fabs(phidiff));
                      double phioi = acos(1 - x2/(2*radius*radius));
                      double phiiv = acos(1 - r1*r1/(2*radius*radius));

                      /// Find advancement!
                      double pigreco = 4.0*atan(1.0);
                      if ( fabs(phioi) >= pigreco) {
                        if ( phioi>0 ) phioi = phioi - 2*pigreco;
                        else phioi = 2*pigreco - fabs(phioi);
                      }
                      if ( fabs(phiiv) >= pigreco) {
                        if ( phiiv>0 ) phiiv = phiiv - 2*pigreco;
                        else phiiv = 2*pigreco - fabs(phiiv);
                      }
                      if (phioi==0) continue;
                      double projected_z = inner.z() - (outer.z()-inner.z())*phiiv/phioi;

                      /// Set Results of the New Fit
                      tempShortTracklet.addVertex( GlobalPoint(xv,yv,projected_z ));
                      //tempShortTracklet.addAxis( GlobalPoint(xc,yc,0 ));

                      if ( IdetID.iPhi() != OdetID.iPhi() )    tempShortTracklet.setHermetic(false);
                      else  tempShortTracklet.setHermetic(true);

                      if (iEvent.isRealData() == false) {
                        tempShortTracklet.setFakeness( zxy.second );
                        tempShortTracklet.setTrackID( zxy.first );
                      }
                      tempShortTracklet.setBeamSpot00(true);
                      
                      // add tracket into event
                      ShortTrackletOutput->push_back( tempShortTracklet );
                      
                    } // end if(detailed search in z)
                  } //end if(detailed search in phi)

                } //end if(rough search by z-sector)
              } //end if(rough search in phi)

              /// Check tracklet made with true beam spot
              /// Use everything in the beamspot reference frame for a while
              if( deltaPhiBeamSpot<mFastPhiCut ){ // rough search in phi!
                if( (innerBeamSpot.z()>0&&outerBeamSpot.z()>-mIPWidth) || (innerBeamSpot.z()<0&&outerBeamSpot.z()<+mIPWidth)  ){  //rough search by z sector        

                  double outerPointRadius = outerBeamSpot.perp(); 
                  double innerPointRadius = innerBeamSpot.perp();
                  double deltaRadius = outerPointRadius - innerPointRadius;

                  double deltaPhiThreshold = deltaRadius * mCompatibilityScalingFactor;  
                  if ( deltaPhiBeamSpot < deltaPhiThreshold ) { // detailer search in phi!
                    double positiveZBoundary = (mIPWidth - outerBeamSpot.z()) * deltaRadius;
                    double negativeZBoundary = -(mIPWidth + outerBeamSpot.z()) * deltaRadius;
                    double multipliedLocation = (innerBeamSpot.z() - outerBeamSpot.z()) * outerPointRadius;

                    if ( ( multipliedLocation < positiveZBoundary ) &&  ( multipliedLocation > negativeZBoundary ) ){ // detailed search in z!
                      // all agree so make a tracklet!!!
                      cmsUpgrades::Tracklet<T> tempShortTracklet;
                      tempShortTracklet.addHit( 0 , *innerHitIter );
                      tempShortTracklet.addHit( 1 , *outerHitIter );

                      /// OLD Fit still available
                      double xv = BSPosition_.x();
                      double yv = BSPosition_.y();

                      double projectedBS_z22X = outerBeamSpot.z() - ( outerPointRadius * (outerBeamSpot.z()-innerBeamSpot.z()) / deltaRadius );
                      tempShortTracklet.addVertex22X( GlobalPoint(xv,yv,projectedBS_z22X ));

                      /// Here the VERY NEW Fit begins
                      double phidiff = outerBeamSpot.phi() - innerBeamSpot.phi();
                      double r1 = innerBeamSpot.perp();
                      double r2 = outerBeamSpot.perp();
                      double x2 = r1*r1 + r2*r2 - 2*r1*r2*cos(phidiff);
                      double radius = 0.5*sqrt(x2)/sin(fabs(phidiff));
                      double phioi = acos(1 - x2/(2*radius*radius));
                      double phiiv = acos(1 - r1*r1/(2*radius*radius));

                      /// Find advancement!
                      double pigreco = 4.0*atan(1.0);
                      if ( fabs(phioi) >= pigreco) {
                        if ( phioi>0 ) phioi = phioi - 2*pigreco;
                        else phioi = 2*pigreco - fabs(phioi);
                      }
                      if ( fabs(phiiv) >= pigreco) {
                        if ( phiiv>0 ) phiiv = phiiv - 2*pigreco;
                        else phiiv = 2*pigreco - fabs(phiiv);
                      }
                      if (phioi==0) continue;
                      double projectedBS_z = inner.z() - (outer.z()-inner.z())*phiiv/phioi; /// This is fine enough
                                                                                            /// as using innerBeamSpot
                                                                                            /// or outerBeamSpot and then
                                                                                            /// recorrecting back to the
                                                                                            /// CMS coordinate frame
                                                                                            /// would bring to the same
                                                                                            /// result

                      /// Set Results of the New Fit
                      tempShortTracklet.addVertex( GlobalPoint(xv,yv,projectedBS_z ));
                      //tempShortTracklet.addAxis( GlobalPoint(xc,yc,0 ));

                      if ( IdetID.iPhi() != OdetID.iPhi() )    tempShortTracklet.setHermetic(false);
                      else  tempShortTracklet.setHermetic(true);

                      if (iEvent.isRealData() == false) {
                        tempShortTracklet.setFakeness( zxy.second );
                        tempShortTracklet.setTrackID( zxy.first );
                      }
                      tempShortTracklet.setBeamSpot00(false);
                      
                      // add tracket into event
                      ShortTrackletOutput->push_back( tempShortTracklet );

                    } // end if(detailed search in z)
                  } //end if(detailed search in phi)

                } //end if(rough search by z-sector)
              } //end if(rough search in phi)

            }
          }
        }
      }

      //std::cout  <<"Made " << ShortTrackletOutput->size() << " short (two stub) tracklets of type " << (mClassInfo->TemplateTypes().begin()->second) << "." << std::endl;
      iEvent.put(ShortTrackletOutput, "ShortTracklets");

      GlobalStubs.clear();
    } //end of produce

    virtual void endJob(){
    }
      

//
// The class members
//
    edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> StackedTrackerGeomHandle;
    const cmsUpgrades::StackedTrackerGeometry *theStackedTracker;
    //cmsUpgrades::StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;

    edm::InputTag GlobalStubsInputTag;

    edm::ESHandle<MagneticField> magnet;
    const MagneticField *magnet_;
    double mMagneticFieldStrength;
    double mCompatibilityScalingFactor;

    double mPtThreshold;
    double mIPWidth;

    double mFastPhiCut;

    const cmsUpgrades::classInfo *mClassInfo;

    
    std::pair<int,int> CheckFakeness(       GlobalStubPtrType innerStub,
                                            GlobalStubPtrType outerStub//,
                                            //const cmsUpgrades::StackedTrackerGeometry *theStackedTracker,
                                            //edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink 
                      );
}; /// End of Class Implementation


/// Specify Templates
/// Pixel Digis
template<>
  std::pair<int,int> TrackletBuilder<cmsUpgrades::Ref_PixelDigi_>::CheckFakeness( 
                                          edm::Ptr< cmsUpgrades::GlobalStub<cmsUpgrades::Ref_PixelDigi_> > innerStub,
                                          edm::Ptr< cmsUpgrades::GlobalStub<cmsUpgrades::Ref_PixelDigi_> > outerStub//,
                                          //const cmsUpgrades::StackedTrackerGeometry *theStackedTracker,
                                          //edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink 
                                        ) 
  {
    std::pair<int,int> abc;
    if ((*innerStub).isFake() || (*outerStub).isFake() ) {
      abc.second = 1;
      abc.first = -9999;
      return abc;
    }
    else {
          int innerSimTrackId = innerStub->trackID(); // = -1;
          int outerSimTrackId = outerStub->trackID(); // = -1;

          if (!(innerSimTrackId!=-1 && innerSimTrackId!=-1 && innerSimTrackId==outerSimTrackId)) {
            abc.second = 2;
            abc.first = -9999;
            return abc;
          }
          else {
            abc.second = 0;
            abc.first = innerSimTrackId;
            return abc;
          }
    }
  }

/// Not Pixel Digis
template<typename T>
  std::pair<int,int>TrackletBuilder<T>::CheckFakeness( 
                                          edm::Ptr< cmsUpgrades::GlobalStub<T> > innerStub,
                                          edm::Ptr< cmsUpgrades::GlobalStub<T> > outerStub//,
                                          //const cmsUpgrades::StackedTrackerGeometry *theStackedTracker,
                                          //edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  thePixelDigiSimLink
                                        ) 
  {
    std::pair<int,int> abc;
    abc.second = 1;
    abc.first = -9999;
    return abc;
  }



//}
//
// constants, enums and typedefs
//


//
// static data member definitions
//

#endif


