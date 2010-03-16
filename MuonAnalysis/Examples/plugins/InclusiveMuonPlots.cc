/** \class InclusiveMuonPlots
 *  Make inclusive muon plots
 *
 *  \author G. Petrucciani - UCSD (Giovanni.Petrucciani@cern.ch), ...
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

// for "luminosity"
#include "DataFormats/Common/interface/MergeableCounter.h"

// for selection cut
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include <TH1.h>

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

    private:
        edm::InputTag muons_;
        StringCutObjectSelector<pat::Muon> selector_;

        edm::InputTag primaryVertices_;
        edm::InputTag normalization_;

        // we don't care too much about performance
        std::map<std::string, TH1*> plots;

        TH1D *luminosity;
};

/// Constructor
InclusiveMuonPlots::InclusiveMuonPlots(const edm::ParameterSet& pset):
    muons_(pset.getParameter<edm::InputTag>("muons")),
    selector_(pset.getParameter<std::string>("selection")),
    primaryVertices_(pset.getParameter<edm::InputTag>("primaryVertices")),
    luminosity(0) // by default, we don't have luminosity info
{

    edm::Service<TFileService> fs;
    book(*fs, pset, "p"); 
    book(*fs, pset, "pt"); 
    book(*fs, pset, "eta"); 
    book(*fs, pset, "phi"); 
    book(*fs, pset, "charge"); 

    book(*fs, pset, "dxyCoarse");
    book(*fs, pset, "dxyFine");
    book(*fs, pset, "dzCoarse");
    book(*fs, pset, "dzFine");

    book(*fs, pset, "pixelHits");
    book(*fs, pset, "trackerHits");
    book(*fs, pset, "muonHits");
    book(*fs, pset, "muonBadHits", "muonHits");
    book(*fs, pset, "globalHits");
    book(*fs, pset, "trackerChi2n");
    book(*fs, pset, "muonChi2n");
    book(*fs, pset, "globalChi2n");

    book(*fs, pset, "trackIso05", "isolation");
    book(*fs, pset, "ecalIso05",  "isolation");
    book(*fs, pset, "hcalIso05",  "isolation");
    book(*fs, pset, "trackIso03", "isolation");
    book(*fs, pset, "ecalIso03",  "isolation");
    book(*fs, pset, "hcalIso03",  "isolation");

    if (pset.existsAs<edm::InputTag>("normalization")) {
        normalization_ = pset.getParameter<edm::InputTag>("normalization");
        luminosity = fs->make<TH1D>("normalization", "normalization", 1, 0, 1);
        luminosity->Sumw2();
    }
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

void InclusiveMuonPlots::analyze(const edm::Event & event, const edm::EventSetup& eventSetup){
    using namespace edm;
    using namespace std;

    Handle<View<reco::Muon> > muons;
    event.getByLabel(muons_, muons);

    Handle<vector<reco::Vertex> > vertices;
    event.getByLabel(primaryVertices_, vertices);

    foreach (const reco::Muon &recomu, *muons) {
        // we want to make a pat::Muon so that we can access directly muonID in the cuts
        const pat::Muon &mu = (typeid(recomu) == typeid(pat::Muon) ? static_cast<const pat::Muon &>(recomu) : pat::Muon(recomu));
        
        if (!selector_(mu)) continue;
    
        plots["p"  ]->Fill(mu.p());
        plots["pt" ]->Fill(mu.pt());
        plots["eta"]->Fill(mu.eta());
        plots["phi"]->Fill(mu.phi());
        plots["charge"]->Fill(mu.charge());

        if (mu.innerTrack().isNonnull()) {
            plots["pixelHits"  ]->Fill(mu.innerTrack()->hitPattern().numberOfValidPixelHits());
            plots["trackerHits"]->Fill(mu.innerTrack()->hitPattern().numberOfValidHits());
            plots["trackerChi2n"]->Fill(mu.innerTrack()->normalizedChi2());

            if (!vertices->empty() && !vertices->front().isFake()) {
                const reco::Vertex &vtx = vertices->front();
                plots["dxyCoarse"]->Fill(mu.innerTrack()->dxy(vtx.position()));
                plots["dzCoarse"]->Fill(mu.innerTrack()->dz(vtx.position()));
                plots["dxyFine"]->Fill(mu.innerTrack()->dxy(vtx.position()));
                plots["dzFine"]->Fill(mu.innerTrack()->dz(vtx.position()));
            }
        }
        if (mu.outerTrack().isNonnull()) {
            plots["muonHits"]->Fill(mu.outerTrack()->numberOfValidHits());
            plots["muonBadHits"]->Fill(mu.outerTrack()->recHitsSize() - mu.outerTrack()->numberOfValidHits());
            plots["muonChi2n"]->Fill(mu.outerTrack()->normalizedChi2());
        }
        if (mu.globalTrack().isNonnull()) {
            plots["globalHits"]->Fill(mu.globalTrack()->numberOfValidHits());
            plots["globalChi2n"]->Fill(mu.globalTrack()->normalizedChi2());
        }

        if (mu.isIsolationValid()) {
            plots["trackIso05"]->Fill(mu.isolationR05().sumPt);
            plots[ "ecalIso05"]->Fill(mu.isolationR05().emEt);
            plots[ "hcalIso05"]->Fill(mu.isolationR05().hadEt);
            plots["trackIso03"]->Fill(mu.isolationR03().sumPt);
            plots[ "ecalIso03"]->Fill(mu.isolationR03().emEt);
            plots[ "hcalIso03"]->Fill(mu.isolationR03().hadEt);
        }

    }
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







