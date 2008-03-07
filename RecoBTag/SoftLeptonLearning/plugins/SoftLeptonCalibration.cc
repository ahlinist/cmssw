#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class TrackAssociatorBase;
class TrackingParticle;

#ifdef BUILD_AS_ANALYZER
class SoftLeptonCalibration : public edm::EDAnalyzer {
#else
class SoftLeptonCalibration : public edm::EDProducer {
#endif
public:
  explicit SoftLeptonCalibration(const edm::ParameterSet& config);
  virtual ~SoftLeptonCalibration();
#ifdef BUILD_AS_ANALYZER
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
#else
  virtual void produce(edm::Event& event, const edm::EventSetup& setup);
#endif
  virtual void beginJob(const edm::EventSetup& setup);
  virtual void endJob(void);

private:
  edm::InputTag m_genPartons;
  edm::InputTag m_genJets;
  //edm::InputTag m_genLeptons;
  edm::InputTag m_trackingParticles;
  edm::InputTag m_primaryVertex;
  edm::InputTag m_caloJets;
  edm::InputTag m_recoJTAs;
  edm::InputTag m_recoLeptons;          // tracks, global muon tracks, electron tracks (TODO gsfTracks)

  std::string m_jetCorrectorMCJet;
  std::string m_jetCorrectorFlavour;

  double m_leptonAssociationRadius;     // associate reconstructed lepton to the jet
  double m_genJetAssociationRadius;     // associate generator-level jet to the jet

private:
  // lepton identification
  unsigned int m_leptonFlavour;        
  double       m_leptonPtMin;
  double       m_leptonIdRadius;
  double       m_leptonIdChi2;
  inline bool isValid(const TrackingParticle & tp) const;
  
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SoftLeptonCalibration);

#include "FWCore/Utilities/interface/Exception.h"
#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "RecoBTag/SoftLeptonLearning/interface/SoftLeptonCalibrationData.h"

SoftLeptonCalibration::SoftLeptonCalibration(const edm::ParameterSet& config) : 
  m_genPartons(config.getParameter<edm::InputTag>("GenPartons")),
  m_genJets(config.getParameter<edm::InputTag>("GenJets")),
  //m_genLeptons(config.getParameter<edm::InputTag>("GenLeptons")),
  m_trackingParticles(config.getParameter<edm::InputTag>("TrackingParticles")),
  m_primaryVertex(config.getParameter<edm::InputTag>("PrimaryVertex")),
  m_caloJets(config.getParameter<edm::InputTag>("CaloJets")),
  m_recoJTAs(config.getParameter<edm::InputTag>("jetTracksAssociation")),
  m_recoLeptons(config.getParameter<edm::InputTag>("RecoLeptons")),
  m_jetCorrectorMCJet(config.getParameter<std::string>("JetCorrectionMCJet")),
  m_jetCorrectorFlavour(config.getParameter<std::string>("JetCorrectionFlavour")),
  m_leptonAssociationRadius(config.getParameter<double>("LeptonAssociationRadius")),
  m_genJetAssociationRadius(config.getParameter<double>("GenJetAssociationRadius")),
  m_leptonFlavour(config.getParameter<unsigned int>("LeptonFlavour")),
  m_leptonPtMin(config.getParameter<double>("LeptonPtMin")),
  m_leptonIdRadius(config.getParameter<double>("LeptonIdentificationRadius")),
  m_leptonIdChi2(config.getParameter<double>("LeptonIdentificationChi2"))
{
#ifndef BUILD_AS_ANALYZER
  produces<std::vector<SoftLeptonCalibrationData> >();
#endif
}

SoftLeptonCalibration::~SoftLeptonCalibration(void) {
}

#ifdef BUILD_AS_ANALYZER
void SoftLeptonCalibration::analyze(const edm::Event& event, const edm::EventSetup& setup) {
#else
void SoftLeptonCalibration::produce(edm::Event& event, const edm::EventSetup& setup) {
#endif

  // making licene plates: EventSetup

  edm::ESHandle <JetCorrector> h_jetCorrector;
  setup.get<JetCorrectionsRecord>().get(m_jetCorrectorMCJet, h_jetCorrector);
  const JetCorrector & jetCorrectorMCJet  = * h_jetCorrector.product();

  edm::ESHandle<TrackAssociatorBase> h_associatorByHits;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits", h_associatorByHits);  
  const TrackAssociatorBase & associatorByHits = * h_associatorByHits.product();
  
  edm::ESHandle<TrackAssociatorBase> h_associatorByChi2;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2", h_associatorByChi2);
  const TrackAssociatorBase & associatorByChi2 = * h_associatorByChi2.product();

  // making license plates: Event
  
  edm::Handle<reco::JetFlavourMatchingCollection> h_genPartons;
  event.getByLabel(m_genPartons, h_genPartons);
  const reco::JetFlavourMatchingCollection & genPartons = * h_genPartons;

  edm::Handle<TrackingParticleCollection> h_trackingParticles;
  event.getByLabel(m_trackingParticles, h_trackingParticles);
  const TrackingParticleCollection & trackingParticles = * h_trackingParticles;

  edm::Handle<reco::GenJetCollection> h_genJets;
  event.getByLabel(m_genJets, h_genJets);
  const reco::GenJetCollection & genJets = * h_genJets;

  edm::Handle<reco::CaloJetCollection> h_caloJets;
  event.getByLabel(m_caloJets, h_caloJets);
  const reco::CaloJetCollection & caloJets = * h_caloJets;

  edm::Handle<reco::VertexCollection> h_primaryVertex;
  event.getByLabel(m_primaryVertex, h_primaryVertex);
  //const reco::Vertex & primaryVertex = (* h_primaryVertex)[0];      // PV is always present (auto fallback to BS)

  edm::Handle<reco::JetTracksAssociationCollection> h_jta;
  event.getByLabel(m_recoJTAs, h_jta);
  const reco::JetTracksAssociationCollection & jta = * h_jta;
 
  /* TrackAssociator does not suppoert RefToBase / View in 1.6.X
  edm::Handle<edm::View<reco::Track> > h_recoLeptons;
  event.getByLabel(m_recoLeptons, h_recoLeptons);
  const edm::View<reco::Track> & recoLeptons = * h_recoLeptons;
  */
  edm::Handle<reco::TrackCollection> h_recoLeptons;
  event.getByLabel(m_recoLeptons, h_recoLeptons);
  const reco::TrackCollection & recoLeptons = * h_recoLeptons;

  // now comes the interesting stuff

  reco::SimToRecoCollection simLeptonAssociationByChi2;
  reco::RecoToSimCollection recoLeptonAssociationByChi2;
  try {
    simLeptonAssociationByChi2  = associatorByChi2.associateSimToReco(h_recoLeptons, h_trackingParticles, & event);
  } catch (cms::Exception e) {
    std::cerr << e << std::endl;
  }
  try {
    recoLeptonAssociationByChi2 = associatorByChi2.associateRecoToSim(h_recoLeptons, h_trackingParticles, & event);
  } catch (cms::Exception e) {
    std::cerr << e << std::endl;
  }
 
  std::auto_ptr<std::vector<SoftLeptonCalibrationData> > calibrationData(new std::vector<SoftLeptonCalibrationData>( caloJets.size() ));
  
  // loop on all reconstructed jets
  for (unsigned int j = 0; j < caloJets.size(); ++j) {
    const reco::CaloJet &       jet  = caloJets[j];
    SoftLeptonCalibrationData & data = (*calibrationData)[j];

    // matched generator parton
    data.partonMomentum = genPartons[j].second.getLorentzVector();
    data.partonFlavour  = genPartons[j].second.getFlavour();

    // loop on generator jets and associate to the closest (if close enough)
    int closest = -1;
    double distance = m_genJetAssociationRadius;
    for (unsigned int g = 0; g < genJets.size(); ++g) {
        double d = ROOT::Math::VectorUtil::DeltaR(jet.momentum(), genJets[g].momentum());
        if (d < distance) {
          distance = d;
          closest  = g;
        }
    }
    if (closest >= 0) {
      data.generatorMomentum = genJets[closest].p4();
    }

    // XXX in 1.6.X primary vertex automatic fallback to beamspot is NOT available
    if (h_primaryVertex->empty()) {
      edm::Handle<reco::BeamSpot> h_beamSpot;
      event.getByType(h_beamSpot);
      data.vertex = reco::Vertex(h_beamSpot->position(), h_beamSpot->covariance3D(), 1, 1, 0);
    } else {
      data.vertex = (*h_primaryVertex)[0];
    }
    
    // calorimetric jet momentum and energy
    data.jetMomentum = jet.p4();

    // origin-corrected jet direction
    data.jetMomentumQuick = math::RhoEtaPhiVector(
      jet.momentum().rho(),
      jet.physicsEtaQuick(data.vertex.z()),
      jet.momentum().phi()
    );

    // origin-corrected jet direction
    data.jetMomentumDetailed = math::RhoEtaPhiVector(
      jet.momentum().rho(),
      jet.physicsEtaDetailed(data.vertex.z()),
      jet.momentum().phi()
    );

    // corrected jet energy
    data.jetEnergyCorrected = jet.energy() * jetCorrectorMCJet.correction(jet);

    // loop over associated tracks:
    //  - compute jet energy and direction as sum of momenta
    //  - TODO look for good/fake tracks
    for (unsigned int t = 0; t < jta[j].second.size(); ++t) {
      const reco::Track & track = *(jta[j].second[t]);
      data.jetMomentumTracks += track.momentum();
      data.jetTracks.push_back(track);
    }
    
    // loop on all reconstructed leptons
    for (unsigned int l = 0; l < recoLeptons.size(); ++l) {
      const reco::Track & lepton = recoLeptons[l];
      
      // check for a lepton inside the jet
      if (ROOT::Math::VectorUtil::DeltaR(lepton.momentum(), jet.momentum()) > m_leptonAssociationRadius)
        continue;

      // store a copy of the leptons inside the jet
      data.jetRecoLeptons.push_back(lepton);

      // match to MonteCarlo
      reco::TrackRef leptonRef(h_recoLeptons, l);
      if (recoLeptonAssociationByChi2.find(leptonRef) != recoLeptonAssociationByChi2.end()) {
        reco::RecoToSimCollection::result_type match = recoLeptonAssociationByChi2[leptonRef];
        // TODO restructure the if block to also check for
        //  - in-flight decay (MC lepton is descendent of associated MC particle) (also check decay vertex ?)
        //  - association by dR (lepton is close to an MC lepton)
        for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = match.begin(); it != match.end(); ++it)
          if ( abs(it->first->pdgId()) == m_leptonFlavour ) {
            // associated to a lepton
            data.jetGoodLeptons.push_back(true);
            goto leptonRecoToSimDone;
          }
        // associated to a particle which is not a lepton
        data.jetGoodLeptons.push_back(false);
        data.jetFakeLeptons.push_back(abs(match.begin()->first->pdgId()));
      } else {
        // not associated (by chi2) - consider a fake
        data.jetGoodLeptons.push_back(false);
        data.jetFakeLeptons.push_back(0);
      }
      
      leptonRecoToSimDone:
        while (false) {}
    }
    
    // loop on all simulated leptons
    for (unsigned int l = 0; l < trackingParticles.size(); ++l) {
      const TrackingParticle & lepton = trackingParticles[l];
      if (not isValid(lepton))
        continue;
      
      // check for a lepton inside the jet
      if (ROOT::Math::VectorUtil::DeltaR(lepton.momentum(), jet.momentum()) > m_leptonAssociationRadius)
        continue;

      // store a copy of the leptons inside the jet
      data.jetSimLeptons.push_back(lepton);

      // match to reconstructed leptons
      TrackingParticleRef leptonRef(h_trackingParticles, l);
      if (simLeptonAssociationByChi2.find(leptonRef) != simLeptonAssociationByChi2.end()) {
        data.jetLostLeptons.push_back(false);
      } else {
        // TODO restructure the if block to also check for
        //  - association by dR (MC lepton is close to a reconstucted lepton)
        data.jetLostLeptons.push_back(true);
      }
    }
    
  }

#ifndef BUILD_AS_ANALYZER
  event.put( calibrationData );
#endif
}

void SoftLeptonCalibration::beginJob(const edm::EventSetup& setup) {
}

void SoftLeptonCalibration::endJob(void){
}

inline bool SoftLeptonCalibration::isValid (const TrackingParticle & tp) const {
  return ( (abs(tp.pdgId()) == m_leptonFlavour) and (tp.pt() >= m_leptonPtMin) );
}
