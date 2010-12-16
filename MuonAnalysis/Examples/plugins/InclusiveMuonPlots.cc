/** \class InclusiveMuonPlots
 *  Make inclusive muon plots
 *
 *  \author G. Petrucciani - UCSD (Giovanni.Petrucciani@cern.ch), ...
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "MuonAnalysis/Examples/interface/muonStations.h"

// for "luminosity"
#include "DataFormats/Common/interface/MergeableCounter.h"

// for selection cut
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

// for tracker muon propagation
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
#include "RecoMuon/Records/interface/MuonRecoGeometryRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include <TH1.h>
#include <TProfile.h>
#include <TObjString.h>
#include <TDirectory.h>

#include <map>
#include <string>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

class InclusiveMuonPlots: public edm::EDAnalyzer {
    public:
        /// Constructor
        InclusiveMuonPlots(const edm::ParameterSet& pset) ;

        /// Destructor
        virtual ~InclusiveMuonPlots() ;

        // Operations
        void analyze(const edm::Event & event, const edm::EventSetup& eventSetup) ;

        void endLuminosityBlock(const edm::LuminosityBlock &, const edm::EventSetup&);

        void book(const TFileService &fs, const edm::ParameterSet &pset, const std::string &name, const std::string &basename) ;
        void book(const TFileService &fs, const edm::ParameterSet &pset, const std::string &name) { book(fs,pset,name,name); }

        void bookProf(const TFileService &fs, const edm::ParameterSet &pset, const std::string &name, const std::string &basename) ;
        void bookProf(const TFileService &fs, const edm::ParameterSet &pset, const std::string &name) { bookProf(fs,pset,name,name); }

    private:
        edm::InputTag muons_;
        StringCutObjectSelector<pat::Muon> selector_;
        StringCutObjectSelector<pat::Muon> subSelector_;
        bool onlyLeadingMuon_;

        bool old36Xdata_;

        edm::InputTag primaryVertices_;
        edm::InputTag normalization_;

        bool doTrackExtrapolations_;

        bool extraPlots_;
        edm::InputTag jets_;
        edm::InputTag met_;

        // we don't care too much about performance
        std::map<std::string, TH1*>      plots;
        std::map<std::string, TProfile*> profiles;

        // stuff for track propagation
        edm::ESHandle<MagneticField> bField;
        edm::ESHandle<Propagator> propagator;
        edm::ESHandle<MuonDetLayerGeometry> muonGeometry;
        edm::ESHandle<TransientTrackBuilder> theTTBuilder;

        // this will be the MB1 surface
        const  BoundCylinder *barrelCylinder_;
        // these are the ME1 surfaces.  ME 1/1 is closer in Z to the interaction point, so we need 2 surfaces for each endcap
        const  BoundDisk *endcapDisk11Pos_, *endcapDisk11Neg_, *endcapDisk123Pos_, *endcapDisk123Neg_; 

        TH1D *luminosity;

};

/// Constructor
InclusiveMuonPlots::InclusiveMuonPlots(const edm::ParameterSet& pset):
    muons_(pset.getParameter<edm::InputTag>("muons")),
    selector_(pset.getParameter<std::string>("selection")),
    subSelector_(pset.existsAs<std::string>("subSelection") ? pset.getParameter<std::string>("subSelection") : ""),
    onlyLeadingMuon_(pset.getParameter<bool>("onlyLeadingMuon")),
    old36Xdata_(pset.existsAs<bool>("old36Xdata") ? pset.getParameter<bool>("old36Xdata") : true),
    primaryVertices_(pset.getParameter<edm::InputTag>("primaryVertices")),
    doTrackExtrapolations_(pset.existsAs<bool>("doTrackExtrapolations") ? pset.getParameter<bool>("doTrackExtrapolations") : true),
    extraPlots_(pset.existsAs<bool>("extraPlots") ? pset.getParameter<bool>("extraPlots") : false),
    jets_(extraPlots_ ? pset.getParameter<edm::InputTag>("jets") : edm::InputTag("NONE")),
    met_(extraPlots_ ? pset.getParameter<edm::InputTag>("met") : edm::InputTag("NONE")),
    luminosity(0) // by default, we don't have luminosity info
{

    edm::Service<TFileService> fs;

    book(*fs, pset, "nMuons"),

    book(*fs, pset, "p"); 
    book(*fs, pset, "pt"); 
    book(*fs, pset, "eta"); 
    book(*fs, pset, "phi"); 
    book(*fs, pset, "charge"); 
    book(*fs, pset, "qp"); 
    book(*fs, pset, "qpt"); 

    book(*fs, pset, "ptErrorOverPt"); 

    book(*fs, pset, "pSta",   "p"); 
    book(*fs, pset, "ptSta",  "pt"); 
    book(*fs, pset, "etaSta", "eta"); 
    book(*fs, pset, "phiSta", "phi"); 

    book(*fs, pset, "ptStaOverPt");

    book(*fs, pset, "dxyCoarse");
    book(*fs, pset, "dxyFine");
    book(*fs, pset, "dzCoarse");
    book(*fs, pset, "dzFine");
    book(*fs, pset, "tip");
    book(*fs, pset, "tipSig");
    book(*fs, pset, "ip3d");
    book(*fs, pset, "ip3dSig");

    book(*fs, pset, "pixelHits");
    book(*fs, pset, "pixelLayers");
    book(*fs, pset, "trackerHits");
    book(*fs, pset, "trackerLostHitsInner",  "trackerLostHits");
    book(*fs, pset, "trackerLostHitsMiddle", "trackerLostHits");
    book(*fs, pset, "trackerLostHitsOuter",  "trackerLostHits");
    book(*fs, pset, "muonHits");
    book(*fs, pset, "muonBadHits");
    book(*fs, pset, "globalHits");
    book(*fs, pset, "globalMuonHits","muonHits");
    book(*fs, pset, "trackerChi2n");
    book(*fs, pset, "muonChi2n");
    book(*fs, pset, "globalChi2n");

    book(*fs, pset, "trackIso05", "isolation");
    book(*fs, pset, "ecalIso05",  "isolation");
    book(*fs, pset, "hcalIso05",  "isolation");
    book(*fs, pset, "trackIso03", "isolation");
    book(*fs, pset, "ecalIso03",  "isolation");
    book(*fs, pset, "hcalIso03",  "isolation");
    book(*fs, pset, "trackIsoVeto", "isolation");
    book(*fs, pset, "ecalIsoVeto",  "isolation");
    book(*fs, pset, "hcalIsoVeto",  "isolation");
    book(*fs, pset, "combRelIso03",   "relIso");
    book(*fs, pset, "combRelIso05",   "relIso");
    book(*fs, pset, "combRelIsoVeto", "relIso");

    book(*fs, pset, "muonStationsValid",    "muonStations");
    book(*fs, pset, "muonStationsAny",      "muonStations");
    book(*fs, pset, "muonStationsDTValid",  "muonStations");
    book(*fs, pset, "muonStationsDTAny",    "muonStations");
    book(*fs, pset, "muonStationsCSCValid", "muonStations");
    book(*fs, pset, "muonStationsCSCAny",   "muonStations");
    book(*fs, pset, "muonStationsRPCValid", "muonStations");
    book(*fs, pset, "muonStationsRPCAny",   "muonStations");
    book(*fs, pset, "segmentMatchesArb",     "segmentMatches"); 
    book(*fs, pset, "segmentMatchesNoArb",   "segmentMatches"); 
    book(*fs, pset, "segmentMatchesFailArb", "segmentMatches"); 
    book(*fs, pset, "segmentCompatArb",      "segmentCompat"); 
    book(*fs, pset, "segmentCompatNoArb",    "segmentCompat"); 
    book(*fs, pset, "caloCompat",            "caloCompat"); 
    book(*fs, pset, "timeAtIpInOut");
    book(*fs, pset, "timeAtIpInOutSig");

    if (doTrackExtrapolations_) {
        book(*fs, pset, "trkPhi_at_pME1_1",      "trkPhiAtSurface");
        book(*fs, pset, "trkPhi_at_mME1_1",      "trkPhiAtSurface");
        book(*fs, pset, "trkPhi_at_pME1_23",     "trkPhiAtSurface");
        book(*fs, pset, "trkPhi_at_mME1_23",     "trkPhiAtSurface");
        book(*fs, pset, "trkPhi_at_MB1",         "trkPhiAtSurface");
    }

    if (pset.existsAs<edm::InputTag>("normalization")) {
        normalization_ = pset.getParameter<edm::InputTag>("normalization");
        luminosity = fs->make<TH1D>("normalization", "normalization", 1, 0, 1);
        luminosity->Sumw2();
    }

    if (extraPlots_) {
        book(*fs, pset, "stip",     "stip");
        book(*fs, pset, "stipSig",  "stipSig");
        book(*fs, pset, "sip3d",    "sip3d");
        book(*fs, pset, "sip3dSig", "sip3dSig");
        book(*fs, pset, "sdl3d",    "sdl3d");
        book(*fs, pset, "sdl3dSig", "sdl3dSig");

        book(*fs, pset, "met");
        book(*fs, pset, "projMetSame", "met");
        book(*fs, pset, "projMetAny",  "met");
        book(*fs, pset, "mt");

        book(*fs, pset, "ptRel");

        book(*fs, pset, "jetFragmentation");
        book(*fs, pset, "oppoJetMuonPtRatio");
        book(*fs, pset, "oppoJetFragmentation");
    }

    TDirectory *md_dir = gDirectory->mkdir("metadata");   
    md_dir->WriteTObject(new TObjString(muons_.encode().c_str()), "muons");
    md_dir->WriteTObject(new TObjString(pset.getParameter<std::string>("selection").c_str()), "selection");
}

/// Destructor
InclusiveMuonPlots::~InclusiveMuonPlots()
{

}

void InclusiveMuonPlots::book(const TFileService &fs, const edm::ParameterSet &pset, const std::string &name, const std::string &basename) 
{
    typedef std::vector<double> vdouble;
    if (pset.existsAs<vdouble>(basename+"Bins")) {
        vdouble bins = pset.getParameter<vdouble>(basename+"Bins");
        plots[name] = fs.make<TH1D>(name.c_str(), name.c_str(), bins.size()-1, &bins[0]);
    } else {
        uint32_t nbins = pset.getParameter<uint32_t>(basename+"Bins");
        vdouble  range = pset.getParameter<vdouble>(basename+"Range");
        if (range.size() != 2) throw cms::Exception("Configuration") << "parameter '" << basename << "Range' is not of the form (min, max).\n";
        plots[name] = fs.make<TH1D>(name.c_str(), name.c_str(), nbins, range[0], range[1]);
    }
}

void InclusiveMuonPlots::bookProf(const TFileService &fs, const edm::ParameterSet &pset, const std::string &name, const std::string &basename) 
{
    typedef std::vector<double> vdouble;
    if (pset.existsAs<vdouble>(basename+"Bins")) {
        vdouble bins = pset.getParameter<vdouble>(basename+"Bins");
        profiles[name] = fs.make<TProfile>(name.c_str(), name.c_str(), bins.size()-1, &bins[0]);
    } else {
        uint32_t nbins = pset.getParameter<uint32_t>(basename+"Bins");
        vdouble  range = pset.getParameter<vdouble>(basename+"Range");
        if (range.size() != 2) throw cms::Exception("Configuration") << "parameter '" << basename << "Range' is not of the form (min, max).\n";
        profiles[name] = fs.make<TProfile>(name.c_str(), name.c_str(), nbins, range[0], range[1]);
    }
}


void InclusiveMuonPlots::analyze(const edm::Event & event, const edm::EventSetup& eventSetup){
    using namespace edm;
    using namespace std;

    eventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
    if (doTrackExtrapolations_) {
    eventSetup.get<IdealMagneticFieldRecord>().get(bField);
    eventSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong",propagator);
    eventSetup.get<MuonRecoGeometryRecord>().get(muonGeometry);
    }

    Handle<View<reco::Muon> > muons;
    event.getByLabel(muons_, muons);

    Handle<vector<reco::Vertex> > vertices;
    event.getByLabel(primaryVertices_, vertices);

    Handle<View<reco::Jet> > jets;
    Handle<View<reco::MET> > met;
    if (extraPlots_) {
        event.getByLabel(jets_, jets);
        event.getByLabel(met_, met);
    }

    size_t nmu = 0;
    foreach (const reco::Muon &recomu, *muons) {
        // we want to make a pat::Muon so that we can access directly muonID in the cuts
        const pat::Muon &mu = (typeid(recomu) == typeid(pat::Muon) ? static_cast<const pat::Muon &>(recomu) : pat::Muon(recomu));
        
        if (!selector_(mu)) continue; // apply before counting
        nmu++;
        if (!subSelector_(mu)) continue; // apply after counting
        if (onlyLeadingMuon_ && nmu > 1) break; // we already did ++, so it counts from 1
    
        plots["p"  ]->Fill(mu.p());
        plots["pt" ]->Fill(mu.pt());
        plots["eta"]->Fill(mu.eta());
        plots["phi"]->Fill(mu.phi());
        plots["charge"]->Fill(mu.charge());
        plots["qp"  ]->Fill(mu.p()*mu.charge());
        plots["qpt" ]->Fill(mu.pt()*mu.charge());

        if (mu.innerTrack().isNonnull()) {
            plots["pixelHits"  ]->Fill(mu.innerTrack()->hitPattern().numberOfValidPixelHits());
            plots["pixelLayers"]->Fill(mu.innerTrack()->hitPattern().pixelLayersWithMeasurement());
            plots["trackerHits"]->Fill(mu.innerTrack()->hitPattern().numberOfValidHits());
            plots["trackerLostHitsMiddle"]->Fill(mu.innerTrack()->hitPattern().numberOfLostHits());
            plots["trackerLostHitsInner"]->Fill(mu.innerTrack()->trackerExpectedHitsInner().numberOfLostHits());
            plots["trackerLostHitsOuter"]->Fill(mu.innerTrack()->trackerExpectedHitsOuter().numberOfLostHits());
            plots["trackerChi2n"]->Fill(mu.innerTrack()->normalizedChi2());

            plots["ptErrorOverPt"]->Fill(mu.innerTrack()->ptError()/mu.innerTrack()->pt());

            if (!vertices->empty() && !vertices->front().isFake()) {
                const reco::Vertex &vtx = vertices->front();
                plots["dxyCoarse"]->Fill(mu.innerTrack()->dxy(vtx.position()));
                plots["dzCoarse"]->Fill(mu.innerTrack()->dz(vtx.position()));
                plots["dxyFine"]->Fill(mu.innerTrack()->dxy(vtx.position()));
                plots["dzFine"]->Fill(mu.innerTrack()->dz(vtx.position()));

                reco::TransientTrack ttk = theTTBuilder->build(mu.innerTrack());
                std::pair<bool,Measurement1D> tip  = IPTools::absoluteTransverseImpactParameter(ttk, vtx);
                if (tip.first ) { 
                    plots["tip"]->Fill(tip.second.value());
                    plots["tipSig"]->Fill(tip.second.significance());
                }
                std::pair<bool,Measurement1D> ip3d = IPTools::absoluteImpactParameter3D(ttk, vtx);
                if (ip3d.first ) { 
                    plots["ip3d"]->Fill(ip3d.second.value());
                    plots["ip3dSig"]->Fill(ip3d.second.significance());
                }
            }
            
        }
        if (mu.outerTrack().isNonnull()) {
            plots["pSta"  ]->Fill(mu.outerTrack()->p());
            plots["ptSta" ]->Fill(mu.outerTrack()->pt());
            plots["etaSta"]->Fill(mu.outerTrack()->eta());
            plots["phiSta"]->Fill(mu.outerTrack()->phi());

            plots["muonHits"]->Fill(mu.outerTrack()->numberOfValidHits());
            if (old36Xdata_) {
                if (mu.outerTrack()->extra().isAvailable()) {
                    plots["muonBadHits"]->Fill(mu.outerTrack()->recHitsSize() - mu.outerTrack()->numberOfValidHits());
                }
            } else {
                plots["muonBadHits"]->Fill(mu.outerTrack()->hitPattern().numberOfBadMuonHits());
            }
            plots["muonChi2n"]->Fill(mu.outerTrack()->normalizedChi2());

            if (mu.innerTrack().isNonnull()) {
                 plots["ptStaOverPt"]->Fill(mu.outerTrack()->pt()/mu.innerTrack()->pt());
            }

            if (old36Xdata_) {
                if ( ( mu.outerTrack()->extra().isAvailable()   ) && 
                     ( mu.outerTrack()->recHitsSize() > 0       ) &&
                     ( mu.outerTrack()->recHit(0).isAvailable() )     ) {
                    plots["muonStationsValid"]->Fill(muon::muonStations(mu.outerTrack(), 0, true));
                    plots["muonStationsAny"  ]->Fill(muon::muonStations(mu.outerTrack(), 0, false));
                    float abseta = std::abs(mu.outerTrack()->eta());
                    if (abseta <= 1.2) {
                        plots["muonStationsDTValid"]->Fill(muon::muonStations(mu.outerTrack(),MuonSubdetId::DT, true));
                        plots["muonStationsDTAny"  ]->Fill(muon::muonStations(mu.outerTrack(),MuonSubdetId::DT, false));
                    } 
                    if (abseta <= 1.6) {
                        plots["muonStationsRPCValid"]->Fill(muon::muonStations(mu.outerTrack(),MuonSubdetId::RPC, true));
                        plots["muonStationsRPCAny"  ]->Fill(muon::muonStations(mu.outerTrack(),MuonSubdetId::RPC, false));
                    } 
                    if (abseta >= 0.8) {
                        plots["muonStationsCSCValid"]->Fill(muon::muonStations(mu.outerTrack(),MuonSubdetId::CSC, true));
                        plots["muonStationsCSCAny"  ]->Fill(muon::muonStations(mu.outerTrack(),MuonSubdetId::CSC, false));
                    }
                }
            } else {
                const reco::HitPattern &hp = mu.outerTrack()->hitPattern();
                /*
                if (plots["nMuons"]->GetEntries() < 100) {
                    std::cout << "Pt " << mu.pt() << ", eta " << mu.eta() << std::endl;
                    std::cout << "Mu  Valid/Any " << hp.muonStationsWithValidHits() << "/" << hp.muonStationsWithAnyHits() << std::endl;
                    std::cout << "DT  Valid/Any " << hp.dtStationsWithValidHits()   << "/" << hp.dtStationsWithAnyHits()   << std::endl;
                    std::cout << "CSC Valid/Any " << hp.cscStationsWithValidHits()  << "/" << hp.cscStationsWithAnyHits()  << std::endl;
                    std::cout << "RPC Valid/Any " << hp.rpcStationsWithValidHits()  << "/" << hp.rpcStationsWithAnyHits()  << std::endl;
                    hp.print(std::cout);
                }
                */
                plots["muonStationsValid"]->Fill(hp.muonStationsWithValidHits());
                plots["muonStationsAny"  ]->Fill(hp.muonStationsWithAnyHits());
                float abseta = std::abs(mu.outerTrack()->eta());
                if (abseta <= 1.2) {
                    plots["muonStationsValid"]->Fill(hp.dtStationsWithValidHits());
                    plots["muonStationsAny"  ]->Fill(hp.dtStationsWithAnyHits());
                } 
                if (abseta <= 1.6) {
                    plots["muonStationsValid"]->Fill(hp.rpcStationsWithValidHits());
                    plots["muonStationsAny"  ]->Fill(hp.rpcStationsWithAnyHits());
                } 
                if (abseta >= 0.8) {
                    plots["muonStationsValid"]->Fill(hp.cscStationsWithValidHits());
                    plots["muonStationsAny"  ]->Fill(hp.cscStationsWithAnyHits());
                }
            }
        }
        if (mu.globalTrack().isNonnull()) {
            plots["globalHits"]->Fill(mu.globalTrack()->numberOfValidHits());
            plots["globalMuonHits"]->Fill(mu.globalTrack()->hitPattern().numberOfValidMuonHits());
            plots["globalChi2n"]->Fill(mu.globalTrack()->normalizedChi2());
        }

        if (mu.isIsolationValid()) {
            plots["trackIso05"]->Fill(mu.isolationR05().sumPt);
            plots[ "ecalIso05"]->Fill(mu.isolationR05().emEt);
            plots[ "hcalIso05"]->Fill(mu.isolationR05().hadEt);
            plots["trackIso03"]->Fill(mu.isolationR03().sumPt);
            plots[ "ecalIso03"]->Fill(mu.isolationR03().emEt);
            plots[ "hcalIso03"]->Fill(mu.isolationR03().hadEt);
            plots["trackIsoVeto"]->Fill(mu.isolationR03().trackerVetoPt - mu.pt());
            plots[ "ecalIsoVeto"]->Fill(mu.isolationR03().emVetoEt);
            plots[ "hcalIsoVeto"]->Fill(mu.isolationR03().hadVetoEt);
            plots[ "combRelIso03"]->Fill( (mu.isolationR03().sumPt + mu.isolationR03().emEt + mu.isolationR03().hadEt) / mu.pt() );
            plots[ "combRelIso05"]->Fill( (mu.isolationR05().sumPt + mu.isolationR05().emEt + mu.isolationR05().hadEt) / mu.pt() );
            plots[ "combRelIsoVeto"]->Fill( (mu.isolationR03().trackerVetoPt - mu.pt() + mu.isolationR03().emVetoEt + mu.isolationR03().hadVetoEt - 3.) / mu.pt() );
        }
        
        if (mu.isMatchesValid()) {
            plots["segmentMatchesArb"    ]->Fill(mu.numberOfMatches(reco::Muon::SegmentAndTrackArbitration));
            plots["segmentMatchesNoArb"  ]->Fill(mu.numberOfMatches(reco::Muon::SegmentArbitration));
            plots["segmentMatchesFailArb"]->Fill(mu.numberOfMatches(reco::Muon::SegmentArbitration) - mu.numberOfMatches(reco::Muon::SegmentAndTrackArbitration));
            plots["segmentCompatArb"     ]->Fill(muon::segmentCompatibility(mu, reco::Muon::SegmentAndTrackArbitration));
            plots["segmentCompatNoArb"   ]->Fill(muon::segmentCompatibility(mu, reco::Muon::SegmentArbitration));
        }

        if (mu.isCaloCompatibilityValid()) {
            plots["caloCompat"]->Fill(mu.caloCompatibility());
        }

        if (mu.isTimeValid()) {
            plots["timeAtIpInOut"]->Fill(mu.time().timeAtIpInOut);
            if (mu.time().timeAtIpInOutErr != 0) {
                plots["timeAtIpInOutSig"]->Fill(mu.time().timeAtIpInOut/mu.time().timeAtIpInOutErr);
            }
        }

        // Andy's phi at ME/MB 1 surface
        if (doTrackExtrapolations_ && mu.innerTrack().isNonnull()) {
          barrelCylinder_ = dynamic_cast<const BoundCylinder *>(& muonGeometry->allDTLayers()[0]->surface());
          endcapDisk11Pos_  = dynamic_cast<const BoundDisk *>(& muonGeometry->forwardCSCLayers()[0]->surface());
          endcapDisk11Neg_  = dynamic_cast<const BoundDisk *>(& muonGeometry->backwardCSCLayers()[0]->surface());
          endcapDisk123Pos_  = dynamic_cast<const BoundDisk *>(& muonGeometry->forwardCSCLayers()[1]->surface());
          endcapDisk123Neg_  = dynamic_cast<const BoundDisk *>(& muonGeometry->backwardCSCLayers()[1]->surface());
          FreeTrajectoryState origin;
          TrajectoryStateOnSurface surface;
          const Propagator * prop = &*propagator;
          origin = TrajectoryStateTransform().initialFreeState(*(mu.track()), bField.product());
          float trkEtaAtOrigin = origin.momentum().eta();
          TrajectoryStateOnSurface trial = prop->propagate(origin, *barrelCylinder_);
          bool inBarrel = false;
          bool inME1_23 = false;
          bool inME1_1 = false;
          if (trial.isValid()){
            if (fabs(trial.globalPosition().z()) <= barrelCylinder_->bounds().length()/2){
              /*
              cout << "GLBMOMMOM: " << origin.momentum() << endl;
              cout << "GLBMOMPHI: " << origin.momentum().phi() << endl;
              cout << "GLBPOSPHI: " << trial.globalPosition().phi() << endl;
              cout << "GLBPOSX: " << trial.globalPosition().x() << endl;
              cout << "GLBPOSY: " << trial.globalPosition().y() << endl;
              cout << "GLBPOSZ: " << trial.globalPosition().z() << endl;
              cout << "GLBPOSMAG: " << trial.globalPosition().mag() << endl;
              cout << "GLBPOSRHO: " << trial.globalPosition().perp() << endl;
              cout << endl;
              */
              surface = trial;
              inBarrel = true;
            }
          }
          if (!trial.isValid() || !inBarrel){
            trial = prop->propagate(origin, (trkEtaAtOrigin > 0 ? *endcapDisk11Pos_ : *endcapDisk11Neg_));
            if (trial.isValid()){
              float radius = trial.globalPosition().perp();
              if ((radius >= endcapDisk11Pos_->innerRadius()) && (radius <= endcapDisk11Pos_->outerRadius())){
                surface = trial;
                inME1_1 = true;
              }
            }
            trial = prop->propagate(origin, (trkEtaAtOrigin > 0 ? *endcapDisk123Pos_ : *endcapDisk123Neg_));
            if (trial.isValid()){
              float radius = trial.globalPosition().perp();
              if ((radius >= endcapDisk123Pos_->innerRadius()) && (radius <= endcapDisk123Pos_->outerRadius())){
                surface = trial;
                inME1_23 = true;
              }
            }
          }
          if (surface.isValid()){
            if (trkEtaAtOrigin > 0 && inME1_1) plots["trkPhi_at_pME1_1"]->Fill(surface.globalPosition().phi());
            if (trkEtaAtOrigin < 0 && inME1_1) plots["trkPhi_at_mME1_1"]->Fill(surface.globalPosition().phi());
            if (trkEtaAtOrigin > 0 && inME1_23) plots["trkPhi_at_pME1_23"]->Fill(surface.globalPosition().phi());
            if (trkEtaAtOrigin < 0 && inME1_23) plots["trkPhi_at_mME1_23"]->Fill(surface.globalPosition().phi());
            if (inBarrel) plots["trkPhi_at_MB1"]->Fill(surface.globalPosition().phi());
          }
        }
        // end Andy's stuff

        if (extraPlots_) {
            double drMin = 999., oppoPtMin = 0; 
            const reco::Jet *match = 0;
            foreach(const reco::Jet &jet, *jets) {
                double dr = deltaR2(jet,mu);
                if (dr < drMin) { drMin = dr; match = &jet; }
                if (jet.pt() > oppoPtMin && std::abs(deltaPhi(jet,mu)) > 0.5*M_PI) {
                    oppoPtMin =  jet.pt(); 
                }
            }
            if (match != 0) {
                math::XYZVector jetDirection = match->momentum().Unit();

                plots["ptRel"]->Fill(jetDirection.Cross(mu.momentum()).R());
                plots["jetFragmentation"]->Fill(mu.pt()/match->pt());

                if (mu.innerTrack().isNonnull() && !vertices->empty() && !vertices->front().isFake()) {
                    GlobalVector jetDirectionGV(jetDirection.X(), jetDirection.Y(), jetDirection.Z()); // stupid dataformats
                    const reco::Vertex &vtx = vertices->front();
                    reco::TransientTrack ttk = theTTBuilder->build(mu.innerTrack());
                    std::pair<bool,Measurement1D> tip  = IPTools::signedTransverseImpactParameter(ttk, jetDirectionGV, vtx);
                    if (tip.first ) { 
                        plots["stip"]->Fill(tip.second.value());
                        plots["stipSig"]->Fill(tip.second.significance());
                    }
                    std::pair<bool,Measurement1D> ip3d = IPTools::signedImpactParameter3D(ttk, jetDirectionGV, vtx);
                    if (ip3d.first ) { 
                        plots["sip3d"]->Fill(ip3d.second.value());
                        plots["sip3dSig"]->Fill(ip3d.second.significance());
                    }
                    std::pair<bool,Measurement1D> dl3d = IPTools::signedDecayLength3D(ttk, jetDirectionGV, vtx);
                    if (dl3d.first ) { 
                        plots["sdl3d"]->Fill(dl3d.second.value());
                        plots["sdl3dSig"]->Fill(dl3d.second.significance());
                    }
                }

            }
            plots["oppoJetMuonPtRatio"  ]->Fill(oppoPtMin/mu.pt());
            plots["oppoJetFragmentation"]->Fill(oppoPtMin == 0 ? 9e9 : mu.pt()/oppoPtMin);

            const reco::MET & m = met->front();
            double dphi = std::abs(deltaPhi(mu.phi(), m.phi()));
            plots["met"]->Fill(m.pt());
            plots["projMetSame"]->Fill(dphi < 0.5*M_PI? m.pt()*sin(dphi) : m.pt());
            plots["projMetAny" ]->Fill(m.pt()*sin(dphi));
            plots["mt"]->Fill( std::sqrt(2*mu.pt()*m.pt()*(1-cos(dphi))) );
        }
    }
    plots["nMuons"]->Fill(nmu);
}

void InclusiveMuonPlots::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup) 
{
    if (luminosity != 0) {
        edm::Handle<edm::MergeableCounter> mc;
        iLumi.getByLabel(normalization_, mc);
        luminosity->Fill(0.5, double(mc->value));
        // set the correct uncertainty from counting statistics
        luminosity->SetBinError(1, sqrt(luminosity->GetBinContent(1)));
    }
}

DEFINE_FWK_MODULE(InclusiveMuonPlots);







