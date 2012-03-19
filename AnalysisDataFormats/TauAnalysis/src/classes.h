
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"
#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3Fwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisSummary.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToDaughtersHypothesisBaseT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitStableSingleParticleHypothesisBaseT.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleNeutrinoHypothesisBaseT.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2Fwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergyFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/GenPhaseSpaceEventInfo.h"

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"
#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopologyFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/HtRatio.h"
#include "AnalysisDataFormats/TauAnalysis/interface/HtRatioFwd.h"

#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"

namespace {
  struct AnalysisDataFormats_TauAnalysis_dictionary {
    /// create dictionaries for DiCandidatePair objects
    DiCandidatePair dummyDiCandidatePair;
    edm::Wrapper<DiCandidatePair> dummyDiCandidatePairWrapper;
    DiCandidatePairCollection dummyDiCandidatePairCollection;
    edm::Wrapper<DiCandidatePairCollection> dummyDiCandidatePairCollectionWrapper;
    edm::Ref<DiCandidatePairCollection> dummyDiCandidatePairRef;
    edm::RefProd<DiCandidatePairCollection> dummyDiCandidatePairRefProd;
    edm::RefVector<DiCandidatePairCollection> dummyDiCandidatePairRefVector;

    DiCandidatePairVBFEvent dummyDiCandidatePairVBFEvent;
    edm::Wrapper<DiCandidatePairVBFEvent> dummyDiCandidatePairVBFEventWrapper;
    DiCandidatePairVBFEventCollection dummyDiCandidatePairVBFEventCollection;
    edm::Wrapper<DiCandidatePairVBFEventCollection> dummyDiCandidatePairVBFEventCollectionWrapper;
    edm::Ref<DiCandidatePairVBFEventCollection> dummyDiCandidatePairVBFEventRef;
    edm::RefProd<DiCandidatePairVBFEventCollection> dummyDiCandidatePairVBFEventRefProd;
    edm::RefVector<DiCandidatePairVBFEventCollection> dummyDiCandidatePairVBFEventRefVector;

    /// create dictionaries for 
    ///  o std::vector<edm::Ptr<pat::Jet> >
    /// as these dictionaries are not yet created in DataFormats/PatCandidates/src/classes.h
    std::vector<edm::Ptr<pat::Jet> > dummyPATJetPtrCollection;

    /// create dictionaries for 
    ///  o edm::Ptr<reco::CaloJet>
    ///  o edm::Ptr<reco::PFJet>
    /// as these dictionaries are not yet created in DataFormats/JetReco/src/classes.h
    edm::Ptr<reco::CaloJet> dummyCaloJetPtr;
    edm::Ptr<reco::PFJet> dummyPFJetPtr;
    std::vector<edm::Ptr<reco::Jet> > dummyJetPtrCollection;

    /// create dictionaries for
    ///  o edm::Ptr<reco::GenParticle>
    /// as these dictionaries are not yet created in DataFormats/HepMCCandidate/src/classes.h
    edm::Ptr<reco::GenParticle> dummyGenParticlePtr;

    /// create dictionaries for
    ///  o edm::Ptr<reco::Track>
    ///  o edm::Ptr<reco::GsfTrack>
    /// as these dictionaries are not yet created in DataFormats/TrackReco/src/classes.h (DataFormats/GsfTrackReco/src/classes.h)
    edm::Ptr<reco::Track> dummyTrackPtr;
    edm::Ptr<reco::GsfTrack> dummyGsfTrackPtr;

    /// create disctionaries for MET covariance matrix 
    /// computed by (PF)MET significance algorithm
    PFMEtSignCovMatrix dummyPFMEtSignCovMatrix;
    edm::Wrapper<PFMEtSignCovMatrix> dummyPFMEtSignCovMatrixWrapper;

    /// create dictionaries for NSVfit solution data-formats
    NSVfitEventHypothesisBase dummyNSVfitEventHypothesisBase;
    NSVfitEventHypothesis dummyNSVfitEventHypothesis;
    NSVfitEventHypothesisByIntegration dummyNSVfitEventHypothesisByIntegration;
    NSVfitResonanceHypothesisBase dummyNSVfitResonanceHypothesisBase;
    NSVfitResonanceHypothesis dummyNSVfitResonanceHypothesis;
    NSVfitResonanceHypothesisByIntegration dummyNSVfitResonanceHypothesisByIntegration;
    NSVfitResonanceHypothesisSummary dummyNSVfitResonanceHypothesisSummary;
    NSVfitTauToElecHypothesisBase dummyNSVfitTauToElecHypothesisBase;
    NSVfitTauToElecHypothesis dummyNSVfitTauToElecHypothesis;
    NSVfitTauToMuHypothesisBase dummyNSVfitTauToMuHypothesisBase;
    NSVfitTauToMuHypothesis dummyNSVfitTauToMuHypothesis;
    NSVfitTauToHadHypothesisBase dummyNSVfitTauToHadHypothesisBase;
    NSVfitTauToHadHypothesis dummyNSVfitTauToHadHypothesis;
    NSVfitStableSingleParticleHypothesisBase dummyNSVfitStableSingleParticleHypothesisBase;
    NSVfitSingleNeutrinoHypothesisBase dummyNSVfitSingleNeutrinoHypothesisBase;
    std::vector<NSVfitEventHypothesisBase> dummyNSVfitEventHypothesisBaseCollection;
    std::vector<NSVfitEventHypothesis> dummyNSVfitEventHypothesisCollection;
    std::vector<NSVfitEventHypothesisByIntegration> dummyNSVfitEventHypothesisByIntegrationCollection;
    edm::Wrapper<std::vector<NSVfitEventHypothesisBase> > dummyNSVfitEventHypothesisBaseCollectionWrapper;
    edm::Wrapper<std::vector<NSVfitEventHypothesis> > dummyNSVfitEventHypothesisCollectionWrapper;
    edm::Wrapper<std::vector<NSVfitEventHypothesisByIntegration> > dummyNSVfitEventHypothesisByIntegrationCollectionWrapper;
    std::vector<NSVfitEventHypothesisBase*> dummyNSVfitEventHypothesisBasePtrCollection;
    edm::OwnVector<NSVfitEventHypothesisBase, edm::ClonePolicy<NSVfitEventHypothesisBase> > dummyNSVfitEventHypothesisBaseOwnVector;
    std::vector<NSVfitResonanceHypothesisBase*> dummyNSVfitResonanceHypothesisBasePtrCollection;
    edm::OwnVector<NSVfitResonanceHypothesisBase, edm::ClonePolicy<NSVfitResonanceHypothesisBase> > dummyNSVfitResonanceHypothesisBaseOwnVector;
    std::vector<NSVfitResonanceHypothesisSummary> dummyNSVfitResonanceHypothesisSummaryCollection;
    std::auto_ptr<TH1> dummyTH1Ptr;
    std::vector<NSVfitSingleParticleHypothesisBase*> dummyNSVfitSingleParticleHypothesisBasePtrCollection;
    edm::OwnVector<NSVfitSingleParticleHypothesisBase, edm::ClonePolicy<NSVfitSingleParticleHypothesisBase> >  dummyNSVfitSingleParticleHypothesisBaseOwnVector;
    
    /// create dictionaries for PATDiElecPair objects
    PATDiElecPair dummyPATDiElecPair;
    edm::Wrapper<PATDiElecPair> dummyPATDiElecPairWrapper;
    PATDiElecPairCollection dummyPATDiElecPairCollection;
    edm::Wrapper<PATDiElecPairCollection> dummyPATDiElecPairCollectionWrapper;
    edm::Ref<PATDiElecPairCollection> dummyPATDiElecPairRef;
    edm::RefVector<PATDiElecPairCollection> dummyPATDiElecPairRefVector;
    edm::RefProd<PATDiElecPairCollection> dummyPATDiElecPairRefProd;
    edm::Ptr<PATDiElecPair> dummyPATDiElecPairPtr;
    edm::AssociationVector<edm::RefProd<PATDiElecPairCollection>, std::vector<int> > dummyPATDiElecPairToMEtAssociation;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATDiElecPairCollection>, std::vector<int> > > dummyPATDiElecPairToMEtAssociationWrapper;

    PATDiElecPairVBFEvent dummyPATDiElecPairVBFEvent;
    edm::Wrapper<PATDiElecPairVBFEvent> dummyPATDiElecPairVBFEventWrapper;
    PATDiElecPairVBFEventCollection dummyPATDiElecPairVBFEventCollection;
    edm::Wrapper<PATDiElecPairVBFEventCollection> dummyPATDiElecPairVBFEventCollectionWrapper;
    edm::Ref<PATDiElecPairVBFEventCollection> dummyPATDiElecPairVBFEventRef;
    edm::RefProd<PATDiElecPairVBFEventCollection> dummyPATDiElecPairVBFEventRefProd;
    edm::RefVector<PATDiElecPairVBFEventCollection> dummyPATDiElecPairVBFEventRefVector;
    
    /// create dictionaries for PATDiMuPair objects
    PATDiMuPair dummyPATDiMuPair;
    edm::Wrapper<PATDiMuPair> dummyPATDiMuPairWrapper;
    PATDiMuPairCollection dummyPATDiMuPairCollection;
    edm::Wrapper<PATDiMuPairCollection> dummyPATDiMuPairCollectionWrapper;
    edm::Ref<PATDiMuPairCollection> dummyPATDiMuPairRef;
    edm::RefVector<PATDiMuPairCollection> dummyPATDiMuPairRefVector;
    edm::RefProd<PATDiMuPairCollection> dummyPATDiMuPairRefProd;
    edm::Ptr<PATDiMuPair> dummyPATDiMuPairPtr;
    edm::AssociationVector<edm::RefProd<PATDiMuPairCollection>, std::vector<int> > dummyPATDiMuPairToMEtAssociation;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATDiMuPairCollection>, std::vector<int> > > dummyPATDiMuPairToMEtAssociationWrapper;

    PATDiMuPairVBFEvent dummyPATDiMuPairVBFEvent;
    edm::Wrapper<PATDiMuPairVBFEvent> dummyPATDiMuPairVBFEventWrapper;
    PATDiMuPairVBFEventCollection dummyPATDiMuPairVBFEventCollection;
    edm::Wrapper<PATDiMuPairVBFEventCollection> dummyPATDiMuPairVBFEventCollectionWrapper;
    edm::Ref<PATDiMuPairVBFEventCollection> dummyPATDiMuPairVBFEventRef;
    edm::RefProd<PATDiMuPairVBFEventCollection> dummyPATDiMuPairVBFEventRefProd;
    edm::RefVector<PATDiMuPairVBFEventCollection> dummyPATDiMuPairVBFEventRefVector;

    /// create dictionaries for PATElecTauPair objects
    PATElecTauPair dummyPATElecTauPair;
    edm::Wrapper<PATElecTauPair> dummyPATElecTauPairWrapper;
    PATElecTauPairCollection dummyPATElecTauPairCollection;
    edm::Wrapper<PATElecTauPairCollection> dummyPATElecTauPairCollectionWrapper;
    edm::Ref<PATElecTauPairCollection> dummyPATElecTauPairRef;
    edm::RefVector<PATElecTauPairCollection> dummyPATElecTauPairRefVector;
    edm::RefProd<PATElecTauPairCollection> dummyPATElecTauPairRefProd;
    edm::Ptr<PATElecTauPair> dummyPATElecTauPairPtr;
    edm::AssociationVector<edm::RefProd<PATElecTauPairCollection>, std::vector<int> > dummyPATElecTauPairToMEtAssociation;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATElecTauPairCollection>, std::vector<int> > > dummyPATElecTauPairToMEtAssociationWrapper;

    PATElecTauPairVBFEvent dummyPATElecTauPairVBFEvent;
    edm::Wrapper<PATElecTauPairVBFEvent> dummyPATElecTauPairVBFEventWrapper;
    PATElecTauPairVBFEventCollection dummyPATElecTauPairVBFEventCollection;
    edm::Wrapper<PATElecTauPairVBFEventCollection> dummyPATElecTauPairVBFEventCollectionWrapper;
    edm::Ref<PATElecTauPairVBFEventCollection> dummyPATElecTauPairVBFEventRef;
    edm::RefProd<PATElecTauPairVBFEventCollection> dummyPATElecTauPairVBFEventRefProd;
    edm::RefVector<PATElecTauPairVBFEventCollection> dummyPATElecTauPairVBFEventRefVector;

    /// create dictionaries for ZllHypothesisElecTau objects
    ZllHypothesisElecTau dummyZllHypothesisElecTau;
    edm::Wrapper<ZllHypothesisElecTau> dummyZllHypothesisElecTauWrapper;
    ZllHypothesisElecTauCollection dummyZllHypothesisElecTauCollection;
    edm::Wrapper<ZllHypothesisElecTauCollection> dummyZllHypothesisElecTauCollectionWrapper;

    /// create dictionaries for ZtautauVisMassHypothesisElecTau objects
    edm::AssociationVector<edm::RefProd<PATElecTauPairCollection>, std::vector<float> > dummyZtautauVisMassHypothesesElecTau;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATElecTauPairCollection>, std::vector<float> > > dummyZtautauVisMassHypothesesElecTauWrapper;

    /// create dictionaries for PATMuTauPair objects
    PATMuTauPair dummyPATMuTauPair;
    edm::Wrapper<PATMuTauPair> dummyPATMuTauPairWrapper;
    PATMuTauPairCollection dummyPATMuTauPairCollection;
    edm::Wrapper<PATMuTauPairCollection> dummyPATMuTauPairCollectionWrapper;
    edm::Ref<PATMuTauPairCollection> dummyPATMuTauPairRef;
    edm::RefVector<PATMuTauPairCollection> dummyPATMuTauPairRefVector;
    edm::RefProd<PATMuTauPairCollection> dummyPATMuTauPairRefProd;
    edm::Ptr<PATMuTauPair> dummyPATMuTauPairPtr;
    edm::AssociationVector<edm::RefProd<PATMuTauPairCollection>, std::vector<int> > dummyPATMuTauPairToMEtAssociation;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATMuTauPairCollection>, std::vector<int> > > dummyPATMuTauPairToMEtAssociationWrapper;

    PATMuTauPairVBFEvent dummyPATMuTauPairVBFEvent;
    edm::Wrapper<PATMuTauPairVBFEvent> dummyPATMuTauPairVBFEventWrapper;
    PATMuTauPairVBFEventCollection dummyPATMuTauPairVBFEventCollection;
    edm::Wrapper<PATMuTauPairVBFEventCollection> dummyPATMuTauPairVBFEventCollectionWrapper;
    edm::Ref<PATMuTauPairVBFEventCollection> dummyPATMuTauPairVBFEventRef;
    edm::RefProd<PATMuTauPairVBFEventCollection> dummyPATMuTauPairVBFEventRefProd;
    edm::RefVector<PATMuTauPairVBFEventCollection> dummyPATMuTauPairVBFEventRefVector;
    
    /// create dictionaries for ZllHypothesisMuTau objects
    ZllHypothesisMuTau dummyZllHypothesisMuTau;
    edm::Wrapper<ZllHypothesisMuTau> dummyZllHypothesisMuTauWrapper;
    ZllHypothesisMuTauCollection dummyZllHypothesisMuTauCollection;
    edm::Wrapper<ZllHypothesisMuTauCollection> dummyZllHypothesisMuTauCollectionWrapper;

    /// create dictionaries for ZtautauVisMassHypothesisMuTau objects
    edm::AssociationVector<edm::RefProd<PATMuTauPairCollection>, std::vector<float> > dummyZtautauVisMassHypothesesMuTau;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATMuTauPairCollection>, std::vector<float> > > dummyZtautauVisMassHypothesesMuTauWrapper;

    /// create dictionaries for PATDiTauPair objects
    PATDiTauPair dummyPATDiTauPair;
    edm::Wrapper<PATDiTauPair> dummyPATDiTauPairWrapper;
    PATDiTauPairCollection dummyPATDiTauPairCollection;
    edm::Wrapper<PATDiTauPairCollection> dummyPATDiTauPairCollectionWrapper;
    edm::Ref<PATDiTauPairCollection> dummyPATDiTauPairRef;
    edm::RefVector<PATDiTauPairCollection> dummyPATDiTauPairRefVector;
    edm::RefProd<PATDiTauPairCollection> dummyPATDiTauPairRefProd;
    edm::Ptr<PATDiTauPair> dummyPATDiTauPairPtr;
    edm::AssociationVector<edm::RefProd<PATDiTauPairCollection>, std::vector<int> > dummyPATDiTauPairToMEtAssociation;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATDiTauPairCollection>, std::vector<int> > > dummyPATDiTauPairToMEtAssociationWrapper;
    
    PATDiTauPairVBFEvent dummyPATDiTauPairVBFEvent;
    edm::Wrapper<PATDiTauPairVBFEvent> dummyPATDiTauPairVBFEventWrapper;
    PATDiTauPairVBFEventCollection dummyPATDiTauPairVBFEventCollection;
    edm::Wrapper<PATDiTauPairVBFEventCollection> dummyPATDiTauPairVBFEventCollectionWrapper;
    edm::Ref<PATDiTauPairVBFEventCollection> dummyPATDiTauPairVBFEventCollectionRef;
    edm::RefProd<PATDiTauPairVBFEventCollection> dummyPATDiTauPairVBFEventCollectionRefProd;
    edm::RefVector<PATDiTauPairVBFEventCollection> dummyPATDiTauPairVBFEventCollectionRefVector;

    /// create dictionaries for ZllHypothesisDiTau objects
    ZllHypothesisDiTau dummyZllHypothesisDiTau;
    edm::Wrapper<ZllHypothesisDiTau> dummyZllHypothesisDiTauWrapper;
    ZllHypothesisDiTauCollection dummyZllHypothesisDiTauCollection;
    edm::Wrapper<ZllHypothesisDiTauCollection> dummyZllHypothesisDiTauCollectionWrapper;

    /// create dictionaries for ZtautauVisMassHypothesisDiTau objects
    edm::AssociationVector<edm::RefProd<PATDiTauPairCollection>, std::vector<float> > dummyZtautauVisMassHypothesesDiTau;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATDiTauPairCollection>, std::vector<float> > > dummyZtautauVisMassHypothesesDiTauWrapper;

    /// create dictionaries for PATElecMuPair objects
    PATElecMuPair dummyPATElecMuPair;
    edm::Wrapper<PATElecMuPair> dummyPATElecMuPairWrapper;
    PATElecMuPairCollection dummyPATElecMuPairCollection;
    edm::Wrapper<PATElecMuPairCollection> dummyPATElecMuPairCollectionWrapper;
    edm::Ref<PATElecMuPairCollection> dummyPATElecMuPairRef;
    edm::RefVector<PATElecMuPairCollection> dummyPATElecMuPairRefVector;
    edm::RefProd<PATElecMuPairCollection> dummyPATElecMuPairRefProd;
    edm::Ptr<PATElecMuPair> dummyPATElecMuPairPtr;
    edm::AssociationVector<edm::RefProd<PATElecMuPairCollection>, std::vector<int> > dummyPATElecMuPairToMEtAssociation;
    edm::Wrapper<edm::AssociationVector<edm::RefProd<PATElecMuPairCollection>, std::vector<int> > > dummyPATElecMuPairToMEtAssociationWrapper;

    PATElecMuPairVBFEvent dummyPATElecMuPairVBFEvent;
    edm::Wrapper<PATElecMuPairVBFEvent> dummyPATElecMuPairVBFEventWrapper;
    PATElecMuPairVBFEventCollection dummyPATElecMuPairVBFEventCollection;
    edm::Wrapper<PATElecMuPairVBFEventCollection> dummyPATElecMuPairVBFEventCollectionWrapper;
    edm::Ref<PATElecMuPairVBFEventCollection> dummyPATElecMuPairVBFEventRef;
    edm::RefProd<PATElecMuPairVBFEventCollection> dummyPATElecMuPairVBFEventRefProd;
    edm::RefVector<PATElecMuPairVBFEventCollection> dummyPATElecMuPairVBFEventRefVector;
    
    /// create dictionaries for ZllHypothesisElecMu objects
    ZllHypothesisElecMu dummyZllHypothesisElecMu;
    edm::Wrapper<ZllHypothesisElecMu> dummyZllHypothesisElecMuWrapper;
    ZllHypothesisElecMuCollection dummyZllHypothesisElecMuCollection;
    edm::Wrapper<ZllHypothesisElecMuCollection> dummyZllHypothesisElecMuCollectionWrapper;

    /// create dictionaries for ZtautauVisMassHypothesisElecMu objects
    //std::pair<edm::Ref<PATElecMuPairCollection>, float> dummyZtautauVisMassHypothesisElecMuEntry;
    //std::vector<std::pair<edm::Ref<PATElecMuPairCollection>, float> > dummyZtautauVisMassHypothesisElecMuCollection;
    edm::AssociationVector<PATElecMuPairRefProd, std::vector<float> > dummyZtautauVisMassHypothesesElecMu;
    edm::Wrapper<edm::AssociationVector<PATElecMuPairRefProd, std::vector<float> > > dummyZtautauVisMassHypothesesElecMuWrapper;

    /// create dictionaries for PATTauNuPair objects
    PATElecNuPair dummyPATElecNuPair;
    edm::Wrapper<PATElecNuPair> dummyPATElecNuPairWrapper;
    PATElecNuPairCollection dummyPATElecNuPairCollection;
    edm::Wrapper<PATElecNuPairCollection> dummyPATElecNuPairCollectionWrapper;
    edm::Ptr<PATElecNuPair> dummyPATElecNuPairPtr;

    PATTauNuPair dummyPATMuNuPair;
    edm::Wrapper<PATMuNuPair> dummyPATMuNuPairWrapper;
    PATMuNuPairCollection dummyPATMuNuPairCollection;
    edm::Wrapper<PATMuNuPairCollection> dummyPATMuNuPairCollectionWrapper;
    edm::Ptr<PATMuNuPair> dummyPATMuNuPairPtr;

    PATTauNuPair dummyPATTauNuPair;
    edm::Wrapper<PATTauNuPair> dummyPATTauNuPairWrapper;
    PATTauNuPairCollection dummyPATTauNuPairCollection;
    edm::Wrapper<PATTauNuPairCollection> dummyPATTauNuPairCollectionWrapper;
    edm::Ptr<PATTauNuPair> dummyPATTauNuPairPtr;

    /// create dictionaries for PATLeptonRecoilEnergyFromJets objects
    PATTauRecoilEnergyFromJets dummyPATTauRecoilEnergyFromJets;
    edm::Wrapper<PATTauRecoilEnergyFromJets> dummyPATTauRecoilEnergyFromJetsWrapper;
    PATTauRecoilEnergyFromJetsCollection dummyPATTauRecoilEnergyFromJetsCollection;
    edm::Wrapper<PATTauRecoilEnergyFromJetsCollection> dummyPATTauRecoilEnergyFromJetsCollectionWrapper;

    /// create dictionaries for PATLeptonRecoilEnergyFromCaloTowers objects
    PATTauRecoilEnergyFromCaloTowers dummyPATTauRecoilEnergyFromCaloTowers;
    edm::Wrapper<PATTauRecoilEnergyFromCaloTowers> dummyPATTauRecoilEnergyFromCaloTowersWrapper;
    PATTauRecoilEnergyFromCaloTowersCollection dummyPATTauRecoilEnergyFromCaloTowersCollection;
    edm::Wrapper<PATTauRecoilEnergyFromCaloTowersCollection> dummyPATTauRecoilEnergyFromCaloTowersCollectionWrapper;

    /// create dictionaries for GenPhaseSpaceEventInfo objects
    GenPhaseSpaceEventInfo dummyGenPhaseSpaceEventInfo;
    edm::Wrapper<GenPhaseSpaceEventInfo> dummyGenPhaseSpaceEventInfoWrapper;

    /// create dictionaries for MEtTopology objects
    MEtTopology dummyMEtTopology;
    edm::Wrapper<MEtTopology> dummyMEtTopologyWrapper;
    MEtTopologyCollection dummyMEtTopologyCollection;
    edm::Wrapper<MEtTopologyCollection> dummyMEtTopologyCollectionWrapper;

	/// create dictionaries for HtRatio objects
	HtRatio dummyHtRatio;
	edm::Wrapper<HtRatio> dymmyHtRatioWrapper;
	HtRatioCollection dummyHtRatioCollection;
	edm::Wrapper<HtRatioCollection> dummyHtRatioCollectionWrapper;
  };
}
