#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_BeamHalo.h"
#include "FWCore/Framework/interface/Event.h"

using namespace edm;
using namespace std;
using namespace reco;

int Phi_To_iPhi(float phi) 
{
  phi = phi < 0 ? phi + 2.*TMath::Pi() : phi ;
  float phi_degrees = phi * (360.) / ( 2. * TMath::Pi() ) ;
  int iPhi = (int) ( ( phi_degrees/5. ) + 1.);
   
  return iPhi < 73 ? iPhi : 73 ;
}
PromptAna_BeamHalo::PromptAna_BeamHalo(const edm::ParameterSet& iConfig) 
{
  //Get Input Tags
  prefix   = iConfig.getParameter<std::string>  ("Prefix"  );
  suffix   = iConfig.getParameter<std::string>  ("Suffix"  );
  //Digi Level 
  IT_L1MuGMTReadout = iConfig.getParameter<edm::InputTag>("L1MuGMTReadoutLabel");
  
  //RecHit Level
  IT_CSCRecHit   = iConfig.getParameter<edm::InputTag>("CSCRecHitLabel");
  IT_EBRecHit    = iConfig.getParameter<edm::InputTag>("EBRecHitLabel");
  IT_EERecHit    = iConfig.getParameter<edm::InputTag>("EERecHitLabel");
  IT_ESRecHit    = iConfig.getParameter<edm::InputTag>("ESRecHitLabel");
  IT_HBHERecHit  = iConfig.getParameter<edm::InputTag>("HBHERecHitLabel");
  IT_HFRecHit    = iConfig.getParameter<edm::InputTag>("HFRecHitLabel");
  IT_HORecHit    = iConfig.getParameter<edm::InputTag>("HORecHitLabel");

  //Higher Level Reco 
  IT_CSCSegment = iConfig.getParameter<edm::InputTag>("CSCSegmentLabel");  
  IT_CosmicStandAloneMuon = iConfig.getParameter<edm::InputTag>("CosmicStandAloneMuonLabel"); 
  IT_BeamHaloMuon = iConfig.getParameter<edm::InputTag>("BeamHaloMuonLabel");
  IT_CollisionMuon = iConfig.getParameter<edm::InputTag>("CollisionMuonLabel");
  IT_CollisionStandAloneMuon  = iConfig.getParameter<edm::InputTag>("CollisionStandAloneMuonLabel"); 
  IT_met = iConfig.getParameter<edm::InputTag>("metLabel");
  IT_CaloTower = iConfig.getParameter<edm::InputTag>("CaloTowerLabel");
  IT_SuperCluster = iConfig.getParameter<edm::InputTag>("SuperClusterLabel");
  IT_Photon = iConfig.getParameter<edm::InputTag>("PhotonLabel") ;
  
  //Halo Data
  IT_CSCHaloData = iConfig.getParameter<edm::InputTag> ("CSCHaloDataLabel");
  IT_EcalHaloData = iConfig.getParameter<edm::InputTag>("EcalHaloDataLabel");
  IT_HcalHaloData = iConfig.getParameter<edm::InputTag>("HcalHaloDataLabel");
  IT_GlobalHaloData = iConfig.getParameter<edm::InputTag>("GlobalHaloDataLabel");
  IT_BeamHaloSummary = iConfig.getParameter<edm::InputTag>("BeamHaloSummaryLabel");

  //CSCHaloData quantities
  // Global X-position of the central most constituent rechit of any CSC standalone cosmic tracks in the event
  produces <std::vector<double> > ( prefix + "CSCHaloDataInnerMostTrackHitX"  + suffix );
  // Global Y-position of the central most constituent rechit of any CSC standalone cosmic tracks in the event
  produces <std::vector<double> > ( prefix + "CSCHaloDataInnerMostTrackHitY"  + suffix );
  // Global R-position of the central most constituent rechit of any CSC standalone cosmic tracks in the event
  produces <std::vector<double> > ( prefix + "CSCHaloDataInnerMostTrackHitR"  + suffix );
  // Global Hcal iPhi position of the central most constituent rechit of any CSC standalone cosmic tracks in the event (converted from global Phi)
  produces <std::vector<double> > ( prefix + "CSCHaloDataInnerMostTrackHitiPhi"  + suffix );

  //produces <std::vector<bool> >   ( prefix + "CSCTrackPlus"  + suffix );
  //produces <std::vector<bool> >   ( prefix + "CSCTrackMinus"  + suffix );
  //produces <std::vector<bool> >   ( prefix + "CSCSegmentPlus"  + suffix );
  //produces <std::vector<bool> >   ( prefix + "CSCSegmentMinus"  + suffix );
  //produces <std::vector<bool> >   ( prefix + "CSCRecHitPlus"  + suffix );
  //produces <std::vector<bool> >   ( prefix + "CSCRecHitMinus"  + suffix );

  //produces <std::vector<int> >    ( prefix + "CSCHaloDataCaloPointingHaloTrackMultiplicity"  + suffix );
  //produces <std::vector<int> >    ( prefix + "TheHaloOrigin"  + suffix );

  
  // Number of L1 CSC Halo triggers in +/- endcap
  produces <std::vector<int> >    ( prefix + "CSCHaloDataHaloTriggersMEPlus"  + suffix );
  produces <std::vector<int> >    ( prefix + "CSCHaloDataHaloTriggersMEMinus"  + suffix );
  //produces <std::vector<int> >    ( prefix + "CSCHaloDataHaloTriggers"  + suffix );  // redundant

  // Number of CSC stand alone tracks built by cosmic reconstruction in +/- endcaps
  produces <std::vector<int> >    ( prefix + "CSCHaloDataTrackMultiplicityMEPlus"  + suffix );
  produces <std::vector<int> >    ( prefix + "CSCHaloDataTrackMultiplicityMEMinus"  + suffix );
 
  //produces <std::vector<int> >    ( prefix + "CSCHaloDataTrackMultiplicity"  + suffix ); // redundant

  //EcalHaloData quantities

  // Number of constituent Rechits in the reconstructed Ecal PhiWedge 
  produces <std::vector<int> >    ( prefix + "EcalHaloDataPhiWedgeConstituents"  + suffix );
  // Logical phi index of the Ecal PhiWedge (goes from 1-360) 
  produces <std::vector<int> >    ( prefix + "EcalHaloDataPhiWedgeiPhi"  + suffix );
  // Number of Ecal PhiWedges reconstructed in the event, probably redundant... 
  produces <std::vector<int> >    ( prefix + "EcalHaloDataPhiWedgeMultiplicity"  + suffix );
  // Sum Energy in the Ecal PhiWedge
  produces <std::vector<double> > ( prefix + "EcalHaloDataPhiWedgeEnergy"  + suffix );
  // Maximum time recorded by a constituent rechit in the Ecal PhiWedge
  produces <std::vector<double> > ( prefix + "EcalHaloDataPhiWedgeMinTime"  + suffix );
  // Minimum time recorded by a constituent rechit in the Ecal PhiWedge 
  produces <std::vector<double> > ( prefix + "EcalHaloDataPhiWedgeMaxTime"  + suffix );
  // Confidence of halo trajectory based on the simultaneous time-ordering and z-ordering of the rechits.  Quantity is bounded by -1 and +1.  For halos from beam 1, the quantity should be closer to  -1. For halos from beam 2, this quantity should be closer to +1.  For non-halos, this quantity should be closer to zero. 
  produces <std::vector<double> > ( prefix + "EcalHaloDataPhiWedgeZDirectionConfidence"  + suffix );
  // Angle between the principal axis and z-axis of the EB Supercluster (see:  RecoEcal/EgammaCoreTools/src/EcalClusterTools.cc)
  produces <std::vector<double> > ( prefix + "EcalHaloDataSuperClusterShowerShapesAngle"  + suffix );
  // Roundness of the EB supercluster
  produces <std::vector<double> > ( prefix + "EcalHaloDataSuperClusterShowerShapesRoundness"  + suffix );

  // HcalHaloData quantities

  // Number of constituent Rechits in the reconstructed Hcal PhiWedge
  produces <std::vector<int> >    ( prefix + "HcalHaloDataPhiWedgeConstituents"  + suffix );
  // Logical phi index of the Hcal PhiWedges (goes from 1-72) 
  produces <std::vector<int> >    ( prefix + "HcalHaloDataPhiWedgeiPhi"  + suffix );
  // Number of Hcal PhiWedges reconstructed in the event, probably redundant
  produces <std::vector<int> >    ( prefix + "HcalHaloDataPhiWedgeMultiplicity"  + suffix );
  // Sum Energy of the Hcal PhiWedge
  produces <std::vector<double> > ( prefix + "HcalHaloDataPhiWedgeEnergy"  + suffix );
  // Minimum time recorded by the rechits in the Hcal PhiWedge 
  produces <std::vector<double> > ( prefix + "HcalHaloDataPhiWedgeMinTime"  + suffix );
  // Maximum time recorded by the rechits in the Hcal PhiWedge 
  produces <std::vector<double> > ( prefix + "HcalHaloDataPhiWedgeMaxTime"  + suffix );
  //Confidence of halo trajectory based on the simultaneous time-ordering and z-ordering of the rechits.  Quantity is bounded by -1 and +1.  For halos from beam 1, the quantity should be closer to  -1. For halos from beam 2, this quantity should be closer to +1.  For non-halos, this quantity should be closer to zero. 
  produces <std::vector<double> > ( prefix + "HcalHaloDataPhiWedgeZDirectionConfidence"  + suffix );


  // GlobalHaloData quantities
  // Number of Hcal PhiWedges matched to a CSC Cosmic track ,  probably redundant
  produces <std::vector<int> >    ( prefix + "GlobalHaloDataMatchedHcalPhiWedgeMultiplicity"  + suffix );
  // Logical phi index of the matched Hcal PhiWedges
  produces <std::vector<int> >    ( prefix + "GlobalHaloDataMatchedHcalPhiWedgeiPhi"  + suffix );
  // Number of constituent rechits of the matched Hcal PhiWedges
  produces <std::vector<int> >    ( prefix + "GlobalHaloDataMatchedHcalPhiWedgeConstituents"  + suffix );
  // SumEnergy of matched Hcal PhiWedge
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMatchedHcalPhiWedgeEnergy"  + suffix );
  // Minimum time recorded by the rechits in the matched Hcal PhiWedge 
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMatchedHcalPhiWedgeMinTime"  + suffix );
  // Maximum time recorded by the rechits in the matched Hcal PhiWedge 
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMatchedHcalPhiWedgeMaxTime"  + suffix );
  // Number of Ecal PhiWedges matched to a CSC cosmic track, probably redundant
  produces <std::vector<int> >    ( prefix + "GlobalHaloDataMatchedEcalPhiWedgeMultiplicity"  + suffix );
  // Logical phi index of the matched Ecal PhiWedge
  produces <std::vector<int> >    ( prefix + "GlobalHaloDataMatchedEcalPhiWedgeiPhi"  + suffix );
  // Number of constituent rechits in the matched Ecal PhiWedge
  produces <std::vector<int> >    ( prefix + "GlobalHaloDataMatchedEcalPhiWedgeConstituents"  + suffix );
  // SumEnergy of matched Ecal PhiWedge
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMatchedEcalPhiWedgeEnergy"  + suffix );
  // Minimum time recorded by the rechits in the matched Ecal PhiWedge 
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMatchedEcalPhiWedgeMinTime"  + suffix );
  // Maximum time recorded by the rechits in the matched Ecal PhiWedge 
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMatchedEcalPhiWedgeMaxTime"  + suffix );                          
  // Correction to the CaloMET, done by removing barrel calotowers that were within CSC-matched PhiWedges
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMExCorrection"  + suffix );
  produces <std::vector<double> > ( prefix + "GlobalHaloDataMEyCorrection"  + suffix );
  produces <std::vector<double> > ( prefix + "GlobalHaloDataHaloCorrectedMET"  + suffix );
  // Delta (Raw CaloMET - Halo Corrected CaloMET)
  produces <std::vector<double> > ( prefix + "GlobalHaloDataRawMETMinusHaloCorrectedMET"  + suffix );
  // CaloMET/SumET
  produces <std::vector<double> > ( prefix + "GlobalHaloDataRawMETOverSumEt"  + suffix );

  
  // Beam Halo Summary Id    
  // 1: CSC Loose Id 
  // 2: CSC Tight Id
  // 3: Ecal Loose Id
  // 4: Ecal Tight Id
  // 5: Hcal Loose Id
  // 6: Hcal Tight Id
  // 7: Global Loose Id
  // 8: Global Tight Id
  // 9: Event Loose Id ( the "or" of all 4 loose criteria)
  // 10: Event Tight Id ( the "or" of all 4 tight criteria)
  // 11: Nothing
  produces <std::vector<int> >    ( prefix + "BeamHaloSummaryId"  + suffix );
  
  //Extra quantities  

  //Ecal rechit timing
  produces <std::vector<double> > ( prefix + "EcalToFtime"  + suffix );
  //Hcal rechit timing
  produces <std::vector<double> > ( prefix + "HcalToFtime"  + suffix );
  //Ecal rechit ieta
  produces <std::vector<int> >    ( prefix + "EcalToFieta"  + suffix );
  //Hcal rechit ieta
  produces <std::vector<int> >    ( prefix + "HcalToFieta"  + suffix );
  
  //Hcal rechit timing for rechits which belong to matched Hcal PhiWedges 
  produces <std::vector<double> > ( prefix + "HcalToFHaloIdtime"  + suffix );
  //EHcal rechit timing for rechits which belong to matched Ecal PhiWedges 
  produces <std::vector<double> > ( prefix + "EcalToFHaloIdtime"  + suffix );
  //Hcal rechit ieta for rechits which belong to matched Hcal PhiWedges 
  produces <std::vector<int> >    ( prefix + "HcalToFHaloIdieta"  + suffix );
  //Ecal rechit ieta for rechits which belong to matched Ecal PhiWedges 
  produces <std::vector<int> >    ( prefix + "EcalToFHaloIdieta"  + suffix );

}

void PromptAna_BeamHalo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  //  std::auto_ptr<std::vector<bool> >   f_CSCTrackPlus                                   ( new std::vector<bool>()  ) ;
  //std::auto_ptr<std::vector<bool> >   f_CSCTrackMinus                                  ( new std::vector<bool>()  ) ;
  //std::auto_ptr<std::vector<bool> >   f_CSCSegmentPlus                                 ( new std::vector<bool>()  ) ;
  //std::auto_ptr<std::vector<bool> >   f_CSCSegmentMinus                                ( new std::vector<bool>()  ) ;
  //std::auto_ptr<std::vector<bool> >   f_CSCRecHitPlus                                  ( new std::vector<bool>()  ) ;
  //std::auto_ptr<std::vector<bool> >   f_CSCRecHitMinus                                 ( new std::vector<bool>()  ) ;
  std::auto_ptr<std::vector<int> >    f_Ecal_ToF_ieta                                  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_Hcal_ToF_ieta                                  ( new std::vector<int>()  ) ;
  //std::auto_ptr<std::vector<int> >    f_CSCHaloData_CaloPointingHaloTrackMultiplicity  ( new std::vector<int>()  ) ;
  //std::auto_ptr<std::vector<int> >    f_TheHaloOrigin                                  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_CSCHaloData_HaloTriggersMEPlus                 ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_CSCHaloData_HaloTriggersMEMinus                ( new std::vector<int>()  ) ;
  //std::auto_ptr<std::vector<int> >    f_CSCHaloData_HaloTriggers                       ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_CSCHaloData_TrackMultiplicityMEPlus            ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_CSCHaloData_TrackMultiplicityMEMinus           ( new std::vector<int>()  ) ;
  //std::auto_ptr<std::vector<int> >    f_CSCHaloData_TrackMultiplicity                  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_EcalHaloData_PhiWedgeConstituents              ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_EcalHaloData_PhiWedgeiPhi                      ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_EcalHaloData_PhiWedgeMultiplicity              ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_HcalHaloData_PhiWedgeConstituents              ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_HcalHaloData_PhiWedgeiPhi                      ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_HcalHaloData_PhiWedgeMultiplicity              ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_GlobalHaloData_MatchedHcalPhiWedgeMultiplicity ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_GlobalHaloData_MatchedHcalPhiWedgeConstituents ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_GlobalHaloData_MatchedHcalPhiWedgeiPhi         ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_GlobalHaloData_MatchedEcalPhiWedgeMultiplicity ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_GlobalHaloData_MatchedEcalPhiWedgeConstituents ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_GlobalHaloData_MatchedEcalPhiWedgeiPhi         ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_HcalToF_HaloId_ieta                      ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_EcalToF_HaloId_ieta                      ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >    f_BeamHaloSummary_Id                             ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> > f_Ecal_ToF_time                                  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_Hcal_ToF_time                                  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalToF_HaloId_time                      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_HcalToF_HaloId_time                      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_CSCHaloData_InnerMostTrackHitX                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_CSCHaloData_InnerMostTrackHitY                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_CSCHaloData_InnerMostTrackHitR                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_CSCHaloData_InnerMostTrackHitiPhi              ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalHaloData_PhiWedgeEnergy                    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalHaloData_PhiWedgeMinTime                   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalHaloData_PhiWedgeMaxTime                   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalHaloData_PhiWedgeZDirectionConfidence  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalHaloData_SuperClusterShowerShapes_angle    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_EcalHaloData_SuperClusterShowerShapes_roundness( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_HcalHaloData_PhiWedgeEnergy                    ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_HcalHaloData_PhiWedgeMinTime                   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_HcalHaloData_PhiWedgeMaxTime                   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_HcalHaloData_PhiWedgeZDirectionConfidence  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MExCorrection                   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MEyCorrection                   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_HaloCorrectedMET                ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_RawMETMinusHaloCorrectedMET     ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_RawMETOverSumEt                 ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MatchedHcalPhiWedgeEnergy       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MatchedHcalPhiWedgeMinTime      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MatchedHcalPhiWedgeMaxTime      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MatchedEcalPhiWedgeEnergy       ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MatchedEcalPhiWedgeMinTime      ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> > f_GlobalHaloData_MatchedEcalPhiWedgeMaxTime      ( new std::vector<double>()  ) ;


  //Get CSC Geometry
  edm::ESHandle<CSCGeometry> TheCSCGeometry;
  iSetup.get<MuonGeometryRecord>().get(TheCSCGeometry);

  //Get CaloGeometry
  edm::ESHandle<CaloGeometry> TheCaloGeometry;
  iSetup.get<CaloGeometryRecord>().get(TheCaloGeometry);
  
  /*
  //Get Stand-alone Muons from Cosmic Muon Reconstruction
  edm::Handle<reco::TrackCollection > TheCosmics;
  iEvent.getByLabel(IT_CosmicStandAloneMuon, TheCosmics);

  //Get CSC Segments
  edm::Handle<CSCSegmentCollection> TheCSCSegments;
  iEvent.getByLabel(IT_CSCSegment, TheCSCSegments);
  */

  //Get  EB RecHits
  edm::Handle<EBRecHitCollection> TheEBRecHits;
  iEvent.getByLabel(IT_EBRecHit, TheEBRecHits);
  if( TheEBRecHits.isValid() )
    {
      for( EBRecHitCollection::const_iterator iEBRecHit = TheEBRecHits->begin() ; iEBRecHit != TheEBRecHits->end(); iEBRecHit++)
	{
	  if( iEBRecHit->energy() < 0.5 ) continue;
	  DetId id = DetId( iEBRecHit->id() );
	  EBDetId EcalId ( id.rawId() );

	  f_Ecal_ToF_ieta->push_back(EcalId.ieta());
	  f_Ecal_ToF_time->push_back(iEBRecHit->time());
	}
    }

  //Get HB/HE RecHits
  edm::Handle<HBHERecHitCollection> TheHBHERecHits;
  iEvent.getByLabel(IT_HBHERecHit, TheHBHERecHits);
  if( TheHBHERecHits.isValid() )
    {
      for( HBHERecHitCollection::const_iterator iHBHERecHit = TheHBHERecHits->begin(); iHBHERecHit != TheHBHERecHits->end(); iHBHERecHit++)  
	{
	  if( iHBHERecHit->energy() < 1.) continue;
	  HcalDetId id = HcalDetId( iHBHERecHit->id() );
	  
	  f_Hcal_ToF_ieta->push_back(id.ieta());
	  f_Hcal_ToF_time->push_back(iHBHERecHit->time());
	}
    }

  //Get MET
  edm::Handle< reco::CaloMETCollection > TheCaloMET;
  iEvent.getByLabel(IT_met, TheCaloMET);

  //Get CSCHaloData
  edm::Handle<reco::CSCHaloData> TheCSCDataHandle;
  iEvent.getByLabel(IT_CSCHaloData,TheCSCDataHandle);
  if (TheCSCDataHandle.isValid())
    {
      const CSCHaloData CSCData = (*TheCSCDataHandle.product());
      for( std::vector<GlobalPoint>::const_iterator i=CSCData.GetCSCTrackImpactPositions().begin();  i != CSCData.GetCSCTrackImpactPositions().end() ; i++ )   
	{    
	  f_CSCHaloData_InnerMostTrackHitX    -> push_back(i->x());
	  f_CSCHaloData_InnerMostTrackHitY    -> push_back(i->y());
	  f_CSCHaloData_InnerMostTrackHitR    -> push_back(TMath::Sqrt( i->x()*i->x() + i->y()*i->y() ));
	  f_CSCHaloData_InnerMostTrackHitiPhi -> push_back(Phi_To_iPhi( i->phi()));
	}                   
      f_CSCHaloData_HaloTriggersMEPlus  -> push_back(CSCData.NumberOfHaloTriggers(1));
      f_CSCHaloData_HaloTriggersMEMinus -> push_back( CSCData.NumberOfHaloTriggers(-1));

      f_CSCHaloData_TrackMultiplicityMEPlus -> push_back( CSCData.NumberOfHaloTracks(1) );
      f_CSCHaloData_TrackMultiplicityMEMinus-> push_back( CSCData.NumberOfHaloTracks(-1) );
    }

  //Get EcalHaloData 
  edm::Handle<reco::EcalHaloData> TheEcalHaloData;
  iEvent.getByLabel(IT_EcalHaloData, TheEcalHaloData );
  if( TheEcalHaloData.isValid() ) 
    {
      const EcalHaloData EcalData = (*TheEcalHaloData.product()); 
      std::vector<PhiWedge> EcalWedges = EcalData.GetPhiWedges();                                                                                              
      for(std::vector<PhiWedge>::const_iterator iWedge = EcalWedges.begin() ; iWedge != EcalWedges.end(); iWedge ++ )                                  
	{               
	  f_EcalHaloData_PhiWedgeEnergy->push_back( iWedge->Energy() );
	  f_EcalHaloData_PhiWedgeConstituents->push_back( iWedge->NumberOfConstituents() );
	  f_EcalHaloData_PhiWedgeMinTime->push_back( iWedge->MinTime() );
	  f_EcalHaloData_PhiWedgeMaxTime->push_back( iWedge->MaxTime() );
	  f_EcalHaloData_PhiWedgeZDirectionConfidence->push_back( iWedge->ZDirectionConfidence() );
	  f_EcalHaloData_PhiWedgeiPhi->push_back(iWedge->iPhi() ) ;
	}

      f_EcalHaloData_PhiWedgeMultiplicity->push_back( EcalWedges.size() );

      edm::ValueMap<float> vm_Angle = EcalData.GetShowerShapesAngle();
      edm::ValueMap<float> vm_Roundness = EcalData.GetShowerShapesRoundness();

      //Access selected SuperClusters
      for(unsigned int n = 0 ; n < EcalData.GetSuperClusters().size() ; n++ )
	{
	  edm::Ref<SuperClusterCollection> cluster(EcalData.GetSuperClusters(), n );
	  float angle = vm_Angle[cluster];
	  float roundness = vm_Roundness[cluster];
	  f_EcalHaloData_SuperClusterShowerShapes_angle->push_back(angle);
	  f_EcalHaloData_SuperClusterShowerShapes_roundness->push_back(roundness);
	}
    }

  //Get HcalHaloData
  edm::Handle<reco::HcalHaloData> TheHcalHaloData;
  iEvent.getByLabel(IT_HcalHaloData ,TheHcalHaloData );
  if( TheHcalHaloData.isValid( ) )
    {
      const HcalHaloData HcalData = (*TheHcalHaloData.product());                                                                
      std::vector<PhiWedge> HcalWedges = HcalData.GetPhiWedges();                                                                                   
      f_HcalHaloData_PhiWedgeMultiplicity->push_back( HcalWedges.size() );
      for(std::vector<PhiWedge>::const_iterator iWedge = HcalWedges.begin() ; iWedge != HcalWedges.end(); iWedge ++ )                               
	{                                                                                                                                        
	  f_HcalHaloData_PhiWedgeEnergy->push_back( iWedge->Energy() );
	  f_HcalHaloData_PhiWedgeConstituents->push_back( iWedge->NumberOfConstituents() );
	  f_HcalHaloData_PhiWedgeiPhi->push_back( iWedge->iPhi() );
	  f_HcalHaloData_PhiWedgeMinTime->push_back( iWedge->MinTime() );
	  f_HcalHaloData_PhiWedgeMaxTime->push_back( iWedge->MaxTime() );
	  f_HcalHaloData_PhiWedgeZDirectionConfidence->push_back( iWedge->ZDirectionConfidence() );
	}
    }
  
  //Get GlobalHaloData
  edm::Handle<reco::GlobalHaloData> TheGlobalHaloData;
  iEvent.getByLabel(IT_GlobalHaloData, TheGlobalHaloData );
  if( TheGlobalHaloData.isValid() ) 
    {
      const GlobalHaloData GlobalData =(*TheGlobalHaloData.product());                                                           
      if( TheCaloMET.isValid() ) 
	{
	  // Get Raw Uncorrected CaloMET
	  const CaloMETCollection *calometcol = TheCaloMET.product();
	  const CaloMET *RawMET = &(calometcol->front());

	  // Get BeamHalo Corrected CaloMET 
	  const CaloMET CorrectedMET = GlobalData.GetCorrectedCaloMET(*RawMET);
	  f_GlobalHaloData_MExCorrection->push_back( GlobalData.DeltaMEx() );
	  f_GlobalHaloData_MEyCorrection->push_back( GlobalData.DeltaMEy() );
	  f_GlobalHaloData_HaloCorrectedMET->push_back(CorrectedMET.pt() );
	  f_GlobalHaloData_RawMETMinusHaloCorrectedMET->push_back( RawMET->pt() - CorrectedMET.pt() );
	  if( RawMET->sumEt() )
	    f_GlobalHaloData_RawMETOverSumEt->push_back( RawMET->pt() / RawMET->sumEt() ); 
	    }

      // Get Matched Hcal Phi Wedges
      std::vector<PhiWedge> HcalWedges = GlobalData.GetMatchedHcalPhiWedges();
      f_GlobalHaloData_MatchedHcalPhiWedgeMultiplicity->push_back(HcalWedges.size());
      // Loop over Matched Hcal Phi Wedges
      for( std::vector<PhiWedge>::const_iterator iWedge = HcalWedges.begin() ; iWedge != HcalWedges.end() ; iWedge ++ )
	{
	  f_GlobalHaloData_MatchedHcalPhiWedgeEnergy->push_back( iWedge->Energy() );
	  f_GlobalHaloData_MatchedHcalPhiWedgeConstituents->push_back( iWedge->NumberOfConstituents());
	  f_GlobalHaloData_MatchedHcalPhiWedgeiPhi->push_back( iWedge->iPhi() );
	  f_GlobalHaloData_MatchedHcalPhiWedgeMinTime->push_back( iWedge->MinTime() );
	  f_GlobalHaloData_MatchedHcalPhiWedgeMaxTime->push_back( iWedge->MaxTime() );
	  if( TheHBHERecHits.isValid() )
	    {
	      for( HBHERecHitCollection::const_iterator iHBHERecHit = TheHBHERecHits->begin(); iHBHERecHit != TheHBHERecHits->end(); iHBHERecHit++)  
		{
		  HcalDetId id = HcalDetId( iHBHERecHit->id() ) ;
		  int iphi = id.iphi() ;
		  if( iphi != iWedge->iPhi() ) continue;
		  if( iHBHERecHit->energy() < 1.0) continue;  // Otherwise there are thousands of hits per event (even with negative energies)
		  
		  float time = iHBHERecHit->time();
		  int ieta = id.ieta();
		  f_HcalToF_HaloId_ieta->push_back( ieta );
		  f_HcalToF_HaloId_time->push_back( time );
		}
	    }
	}

      // Get Matched Hcal Phi Wedges
      std::vector<PhiWedge> EcalWedges = GlobalData.GetMatchedEcalPhiWedges();
      f_GlobalHaloData_MatchedEcalPhiWedgeMultiplicity->push_back(EcalWedges.size());
      for( std::vector<PhiWedge>::const_iterator iWedge = EcalWedges.begin() ; iWedge != EcalWedges.end() ; iWedge ++ )
	{
	  f_GlobalHaloData_MatchedEcalPhiWedgeEnergy->push_back(iWedge->Energy());
	  f_GlobalHaloData_MatchedEcalPhiWedgeConstituents->push_back(iWedge->NumberOfConstituents());
	  f_GlobalHaloData_MatchedEcalPhiWedgeiPhi->push_back(iWedge->iPhi());
	  f_GlobalHaloData_MatchedEcalPhiWedgeMinTime->push_back(iWedge->MinTime());
	  f_GlobalHaloData_MatchedEcalPhiWedgeMaxTime->push_back(iWedge->MaxTime());

	  if( TheEBRecHits.isValid() ) 
	    {
	      for( EBRecHitCollection::const_iterator iEBRecHit = TheEBRecHits->begin() ; iEBRecHit != TheEBRecHits->end(); iEBRecHit++ )
		{
		  if( iEBRecHit->energy() < 0.5 ) continue;
		  DetId id = DetId( iEBRecHit->id() ) ;
		  EBDetId EcalId ( id.rawId() );
		  int iPhi = EcalId.iphi() ;
		  if( iPhi != iWedge->iPhi() ) continue;
		  f_EcalToF_HaloId_ieta->push_back( EcalId.ieta() );
		  f_EcalToF_HaloId_time->push_back( iEBRecHit->time() );
		}
	    }
	}
    }


  // Get BeamHaloSummary 
  edm::Handle<BeamHaloSummary> TheBeamHaloSummary ;
  iEvent.getByLabel(IT_BeamHaloSummary, TheBeamHaloSummary) ;

  if( TheBeamHaloSummary.isValid() ) 
    {
      const BeamHaloSummary TheSummary = (*TheBeamHaloSummary.product() );
      
      if( TheSummary.CSCLooseHaloId() ) 
	f_BeamHaloSummary_Id->push_back(1);
      if( TheSummary.CSCTightHaloId() ) 
	f_BeamHaloSummary_Id->push_back(2);
      if( TheSummary.EcalLooseHaloId() )
	f_BeamHaloSummary_Id->push_back(3);
      if( TheSummary.EcalTightHaloId() ) 
	f_BeamHaloSummary_Id->push_back(4);
      if( TheSummary.HcalLooseHaloId() ) 
	f_BeamHaloSummary_Id->push_back(5);
      if( TheSummary.HcalTightHaloId() ) 
	f_BeamHaloSummary_Id->push_back(6);
      if( TheSummary.GlobalLooseHaloId()) 
	f_BeamHaloSummary_Id->push_back(7);
      if( TheSummary.GlobalTightHaloId() )
	f_BeamHaloSummary_Id->push_back(8);
      if( TheSummary.LooseId() ) 
	f_BeamHaloSummary_Id->push_back(9);
      if( TheSummary.TightId() )
	f_BeamHaloSummary_Id->push_back(10);
      if( !TheSummary.EcalLooseHaloId()  && !TheSummary.HcalLooseHaloId() && !TheSummary.CSCLooseHaloId() && !TheSummary.GlobalLooseHaloId() )
	f_BeamHaloSummary_Id->push_back(11);
    }  

  //iEvent.put(  f_CSCTrackPlus                                            ,      prefix + "CSCTrackPlus"  + suffix );
  //iEvent.put(  f_CSCTrackMinus                                  	 ,      prefix + "CSCTrackMinus"  + suffix );						       
  //iEvent.put(  f_CSCSegmentPlus                                 	 ,      prefix + "CSCSegmentPlus"  + suffix );						       
  //iEvent.put(  f_CSCSegmentMinus                                	 ,      prefix + "CSCSegmentMinus"  + suffix );						       
  //iEvent.put(  f_CSCRecHitPlus                                  	 ,      prefix + "CSCRecHitPlus"  + suffix );						       
  //iEvent.put(  f_CSCRecHitMinus                                 	 ,      prefix + "CSCRecHitMinus"  + suffix );						       
  iEvent.put(  f_Ecal_ToF_ieta                                   	 ,      prefix + "EcalToFieta"  + suffix );						       
  iEvent.put(  f_Hcal_ToF_ieta                                   	 ,      prefix + "HcalToFieta"  + suffix );						       
  //iEvent.put(  f_CSCHaloData_CaloPointingHaloTrackMultiplicity  	 ,      prefix + "CSCHaloDataCaloPointingHaloTrackMultiplicity"  + suffix );		     
  //iEvent.put(  f_TheHaloOrigin                                  	 ,      prefix + "TheHaloOrigin"  + suffix );						     
  iEvent.put(  f_CSCHaloData_HaloTriggersMEPlus                 	 ,      prefix + "CSCHaloDataHaloTriggersMEPlus"  + suffix );				     
  iEvent.put(  f_CSCHaloData_HaloTriggersMEMinus                	 ,      prefix + "CSCHaloDataHaloTriggersMEMinus"  + suffix );				     
  //iEvent.put(  f_CSCHaloData_HaloTriggers                       	 ,      prefix + "CSCHaloDataHaloTriggers"  + suffix );					       
  iEvent.put(  f_CSCHaloData_TrackMultiplicityMEPlus            	 ,      prefix + "CSCHaloDataTrackMultiplicityMEPlus"  + suffix );			     
  iEvent.put(  f_CSCHaloData_TrackMultiplicityMEMinus           	 ,      prefix + "CSCHaloDataTrackMultiplicityMEMinus"  + suffix );			     
  //iEvent.put(  f_CSCHaloData_TrackMultiplicity                  	 ,      prefix + "CSCHaloDataTrackMultiplicity"  + suffix );				        
  iEvent.put(  f_EcalHaloData_PhiWedgeConstituents              	 ,      prefix + "EcalHaloDataPhiWedgeConstituents"  + suffix );			        
  iEvent.put(  f_EcalHaloData_PhiWedgeiPhi                      	 ,      prefix + "EcalHaloDataPhiWedgeiPhi"  + suffix );				        
  iEvent.put(  f_EcalHaloData_PhiWedgeMultiplicity              	 ,      prefix + "EcalHaloDataPhiWedgeMultiplicity"  + suffix );			        
  iEvent.put(  f_HcalHaloData_PhiWedgeConstituents              	 ,      prefix + "HcalHaloDataPhiWedgeConstituents"  + suffix );			        
  iEvent.put(  f_HcalHaloData_PhiWedgeiPhi                      	 ,      prefix + "HcalHaloDataPhiWedgeiPhi"  + suffix );				        
  iEvent.put(  f_HcalHaloData_PhiWedgeMultiplicity              	 ,      prefix + "HcalHaloDataPhiWedgeMultiplicity"  + suffix );			     
  iEvent.put(  f_GlobalHaloData_MatchedHcalPhiWedgeMultiplicity 	 ,      prefix + "GlobalHaloDataMatchedHcalPhiWedgeMultiplicity"  + suffix );		     
  iEvent.put(  f_GlobalHaloData_MatchedHcalPhiWedgeiPhi 	         ,      prefix + "GlobalHaloDataMatchedHcalPhiWedgeiPhi"  + suffix );			     
  iEvent.put(  f_GlobalHaloData_MatchedHcalPhiWedgeConstituents          ,      prefix + "GlobalHaloDataMatchedHcalPhiWedgeConstituents"  + suffix );		     
  iEvent.put(  f_GlobalHaloData_MatchedEcalPhiWedgeMultiplicity 	 ,      prefix + "GlobalHaloDataMatchedEcalPhiWedgeMultiplicity"  + suffix );		     
  iEvent.put(  f_GlobalHaloData_MatchedEcalPhiWedgeiPhi 	         ,      prefix + "GlobalHaloDataMatchedEcalPhiWedgeiPhi"  + suffix );			     
  iEvent.put(  f_GlobalHaloData_MatchedEcalPhiWedgeConstituents          ,      prefix + "GlobalHaloDataMatchedEcalPhiWedgeConstituents"  + suffix );		        
  iEvent.put(  f_HcalToF_HaloId_ieta       	                 ,      prefix + "HcalToFHaloIdieta"  + suffix );				        
  iEvent.put(  f_EcalToF_HaloId_ieta	       	                 ,      prefix + "EcalToFHaloIdieta"  + suffix );				        
  iEvent.put(  f_BeamHaloSummary_Id                             	 ,      prefix + "BeamHaloSummaryId"  + suffix );					     
  iEvent.put(  f_Ecal_ToF_time                                   	 ,      prefix + "EcalToFtime"  + suffix );						     
  iEvent.put(  f_Hcal_ToF_time                                   	 ,      prefix + "HcalToFtime"  + suffix );						        
  iEvent.put(  f_HcalToF_HaloId_time                      	 ,      prefix + "HcalToFHaloIdtime"  + suffix );				        
  iEvent.put(  f_EcalToF_HaloId_time	                         ,      prefix + "EcalToFHaloIdtime"  + suffix );				     
  iEvent.put(  f_CSCHaloData_InnerMostTrackHitX                  	 ,      prefix + "CSCHaloDataInnerMostTrackHitX"  + suffix );				     
  iEvent.put(  f_CSCHaloData_InnerMostTrackHitY                  	 ,      prefix + "CSCHaloDataInnerMostTrackHitY"  + suffix );				     
  iEvent.put(  f_CSCHaloData_InnerMostTrackHitR                  	 ,      prefix + "CSCHaloDataInnerMostTrackHitR"  + suffix );				        
  iEvent.put(  f_CSCHaloData_InnerMostTrackHitiPhi               	 ,      prefix + "CSCHaloDataInnerMostTrackHitiPhi"  + suffix );			        
  iEvent.put(  f_EcalHaloData_PhiWedgeEnergy                     	 ,      prefix + "EcalHaloDataPhiWedgeEnergy"  + suffix );				     
  iEvent.put(  f_EcalHaloData_PhiWedgeMinTime                    	 ,      prefix + "EcalHaloDataPhiWedgeMinTime"  + suffix );				     
  iEvent.put(  f_EcalHaloData_PhiWedgeMaxTime                    	 ,      prefix + "EcalHaloDataPhiWedgeMaxTime"  + suffix );				     
  iEvent.put(  f_EcalHaloData_PhiWedgeZDirectionConfidence   	         ,      prefix + "EcalHaloDataPhiWedgeZDirectionConfidence"  + suffix );		        
  iEvent.put(  f_EcalHaloData_SuperClusterShowerShapes_angle     	 ,      prefix + "EcalHaloDataSuperClusterShowerShapesAngle"  + suffix );		     
  iEvent.put(  f_EcalHaloData_SuperClusterShowerShapes_roundness 	 ,      prefix + "EcalHaloDataSuperClusterShowerShapesRoundness"  + suffix );		        
  iEvent.put(  f_HcalHaloData_PhiWedgeEnergy                     	 ,      prefix + "HcalHaloDataPhiWedgeEnergy"  + suffix );				     
  iEvent.put(  f_HcalHaloData_PhiWedgeMinTime                    	 ,      prefix + "HcalHaloDataPhiWedgeMinTime"  + suffix );				     
  iEvent.put(  f_HcalHaloData_PhiWedgeMaxTime                    	 ,      prefix + "HcalHaloDataPhiWedgeMaxTime"  + suffix );				     
  iEvent.put(  f_HcalHaloData_PhiWedgeZDirectionConfidence   	         ,      prefix + "HcalHaloDataPhiWedgeZDirectionConfidence"  + suffix );		     
  iEvent.put(  f_GlobalHaloData_MExCorrection                    	 ,      prefix + "GlobalHaloDataMExCorrection"  + suffix );				     
  iEvent.put(  f_GlobalHaloData_MEyCorrection                    	 ,      prefix + "GlobalHaloDataMEyCorrection"  + suffix );				     
  iEvent.put(  f_GlobalHaloData_HaloCorrectedMET                 	 ,      prefix + "GlobalHaloDataHaloCorrectedMET"  + suffix );				        
  iEvent.put(  f_GlobalHaloData_RawMETMinusHaloCorrectedMET      	 ,      prefix + "GlobalHaloDataRawMETMinusHaloCorrectedMET"  + suffix );		     
  iEvent.put(  f_GlobalHaloData_RawMETOverSumEt                  	 ,      prefix + "GlobalHaloDataRawMETOverSumEt"  + suffix );				     
  iEvent.put(  f_GlobalHaloData_MatchedHcalPhiWedgeEnergy        	 ,      prefix + "GlobalHaloDataMatchedHcalPhiWedgeEnergy"  + suffix );
  iEvent.put(  f_GlobalHaloData_MatchedHcalPhiWedgeMinTime       	 ,      prefix + "GlobalHaloDataMatchedHcalPhiWedgeMinTime"  + suffix );		        
  iEvent.put(  f_GlobalHaloData_MatchedHcalPhiWedgeMaxTime       	 ,      prefix + "GlobalHaloDataMatchedHcalPhiWedgeMaxTime"  + suffix );		     
  iEvent.put(  f_GlobalHaloData_MatchedEcalPhiWedgeEnergy        	 ,      prefix + "GlobalHaloDataMatchedEcalPhiWedgeEnergy"  + suffix );			       
  iEvent.put(  f_GlobalHaloData_MatchedEcalPhiWedgeMinTime       	 ,      prefix + "GlobalHaloDataMatchedEcalPhiWedgeMinTime"  + suffix );		        
  iEvent.put(  f_GlobalHaloData_MatchedEcalPhiWedgeMaxTime      	 ,      prefix + "GlobalHaloDataMatchedEcalPhiWedgeMaxTime"  + suffix );                  
}
