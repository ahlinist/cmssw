#include "RecoHcal/MuonFilter/interface/MuonFilter.h"

using namespace edm;
using namespace reco;
using namespace std;

MuonFilter::MuonFilter(const edm::ParameterSet & iConfig) {
    // now do what ever initialization is needed
    NTotal = 0;
    NRejected = 0;
    fIP_z = iConfig.getUntrackedParameter < double >("IPZ", 20.);
    fIP_r = iConfig.getUntrackedParameter < double >("IPR", 20.);

}


MuonFilter::~MuonFilter() {

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}

bool MuonFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

    ++NTotal;
    bool accept = true;

    Handle < TrackCollection > muons;
    iEvent.getByLabel("cosmicMuons", muons);
    if (!muons.isValid()) {
        LogTrace("") << ">>> Cosmic Muon collection does not exist !!!";
        accept = false;
    }

    const TrackCollection tC = *(muons.product());

    if (tC.size() == 0) {
        accept = false;
    }
    int NGoodMuonTracks = 0;
    double radius_ip = fIP_r;   // if you want to select events passing the vicinity of the beampipe.
    double ip_z_width = fIP_z;

    for (reco::TrackCollection::const_iterator track = tC.begin(); track != tC.end(); track++) {

        double ip_thetap = -999.;
        double ip_etap = -999.;
        double ip_phip = -999.;
        double ip_thetam = -999.;
        double ip_etam = -999.;
        double ip_phim = -999.;

        bool isCloseToIP_ip = false;

        // extrapolate to IP
        isCloseToIP_ip = Extrapolate(
          // inputs
          track->innerPosition().x(), track->innerPosition().y(), track->innerPosition().z(),
          track->innerMomentum().x(), track->innerMomentum().y(), track->innerMomentum().z(), radius_ip, ip_z_width,
          // outputs
          &ip_thetap, &ip_phip, &ip_thetam, &ip_phim);

        if (ip_thetam != -999.) {
            ip_etam = -1 * TMath::Log(TMath::Tan(ip_thetam / 2.));
            ip_etap = -1 * TMath::Log(TMath::Tan(ip_thetap / 2.));
            if (TMath::IsNaN(ip_etam))
                ip_etam = -999;
            if (TMath::IsNaN(ip_etap))
                ip_etap = -999;
        }
        if (isCloseToIP_ip) {
            ++NGoodMuonTracks;
        }
    }
    if (NGoodMuonTracks < 1) {
        NRejected++;
        accept = false;
    }
    return accept;
}

void MuonFilter::beginJob(const edm::EventSetup &) {
}

void MuonFilter::endJob() {
    cout << "END OF JOB: TOTAL EVENTS: " << NTotal << " remainig: " << NTotal - NRejected << endl;
}

bool MuonFilter::Extrapolate(
  // inputs
  double ox, double oy, double oz, double px, double py, double pz, double ra, double zwidth,
  // outputs
  double *thetap_out, double *phip_out, double *thetam_out, double *phim_out) {

    bool isCloseToIP = false;

    if (px == 0.) {
        std::cout << "px is exactly 0 - the extrapolation can not handle this case currently, sorry. Not extrapolating."
          << std::endl;
        return isCloseToIP;
    }


    double xp = -99999.;
    double yp = -99999.;
    double zp = -99999.;

    double xm = -99999.;
    double ym = -99999.;
    double zm = -99999.;

    double a = py / px;
    double b = oy - a * ox;

    double notnegative = (ra * ra - b * b) / (1 + a * a) + TMath::Power(a * b / (1 + a * a), 2.);

    if (notnegative > 0.) {
        xp = TMath::Sqrt(notnegative) - a * b / (1 + a * a);
        yp = (py / px) * (xp - ox) + oy;
        zp = (pz * (xp - ox) / px) + oz;
        double phip = TMath::ATan2(yp, xp);

        if (phip < 0.)
            phip = phip + (2. * TMath::Pi());
        double thetap = TMath::Pi() / 2. - TMath::ATan2(zp, TMath::Sqrt(xp * xp + yp * yp));

        xm = -TMath::Sqrt(notnegative) - a * b / (1 + a * a);
        ym = py / px * (xm - ox) + oy;
        zm = pz * (xm - ox) / px + oz;

        double phim = TMath::ATan2(ym, xm);

        if (phim < 0.)
            phim = phim + (2. * TMath::Pi());
        double thetam = TMath::Pi() / 2. - TMath::ATan2(zm, TMath::Sqrt(xm * xm + ym * ym));

        // set the output values if we are in a reasonable z range
        // if(TMath::Abs(zp) < 433. && TMath::Abs(zm) < 433.) {
        // restrict to +-20 cms around z=0
        if (TMath::Abs(zp) < zwidth && TMath::Abs(zm) < zwidth) {
            *thetap_out = thetap;
            *phip_out = phip;
            *thetam_out = thetam;
            *phim_out = phim;

            isCloseToIP = true;
        }
        // std::cout << "We have a cosmic muon close to the IP " << zp << " , " << zm << std::endl;

    } else {
        // std::cout<<"No intersection of StandAlone Muon with selected cylinder."<<std::endl;
        isCloseToIP = false;
    }

    return isCloseToIP;

}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonFilter);
