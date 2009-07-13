#include "RecoParticleFlow/PFAnalyses/interface/PFPretendTrackProducer.h"
#include "DataFormats/ParticleFlowReco/interface/PFTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/ParticleFlowReco/interface/PFTrajectoryPoint.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/GsfPFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "Rtypes.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "Math/GenVector/PositionVector3D.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <TTree.h>
#include <TFile.h>
#include <cmath>

using namespace edm;
using namespace reco;
using namespace pftools;

PFPretendTrackProducer::PFPretendTrackProducer(
		const edm::ParameterSet& parameters) :
	debug_(0), transformer_(0), justCreateEmptyCollections_(false) {

	produces<reco::TrackCollection> ("tracks");
	produces<reco::TrackExtraCollection> ();
	produces<reco::PFRecTrackCollection> ("pfRecTracks");

	produces<GsfPFRecTrackCollection> ("gsfPfRecTracks");
	produces<reco::PFClusterCollection> ("pfPS");
	produces<reco::PFClusterCollection> ("pfHFHAD");
	produces<reco::PFClusterCollection>("pfHFEM");
	produces<reco::MuonCollection> ("muons");
	produces<reco::VertexCollection> ("offlinePrimaryVertices");

	debug_ = parameters.getParameter<int> ("debug");

	justCreateEmptyCollections_ = parameters.getParameter<bool>("justCreateEmptyCollections");

	std::string cuts = parameters.getParameter<std::string> ("runinfo_cuts");
	TFile* file = TFile::Open(cuts.c_str());
	TTree* tree(0);
	thisRun_ = new RunInfo();
	if (debug_ > 0) {
		std::cout << "Resurrecting run infos from " << cuts << std::endl;
	}
	if (file != 0) {
		tree = (TTree*) file->FindObjectAny("RunInfo");
		if (tree != 0) {
			//Found tree successfully... loop over entries and resurrect infos
			tree->SetBranchAddress("RunInfo", &thisRun_);
			if (debug_ > 3) {
				std::cout << "Cut file has " << tree->GetEntries()
						<< " entries." << std::endl;
			}
			for (unsigned entry(0); entry < tree->GetEntries(); ++entry) {
				tree->GetEntry(entry);
				//Copy run info
				if (debug_ > 4) {
					std::cout << "Copying run info for run "
							<< thisRun_->runNumber_ << std::endl;
					std::cout << *thisRun_;
				}
				RunInfo* aRun = new RunInfo(*thisRun_);
				runInfos_[thisRun_->runNumber_] = aRun;
			}
		} else {
			std::cout << "Tree pointer is null!" << std::endl;
		}
		file->Close();
	} else {
		std::cout << "Couldn't open run info file!" << std::endl;
	}
}

PFPretendTrackProducer::~PFPretendTrackProducer() {
	delete transformer_;
}

void PFPretendTrackProducer::beginJob(const edm::EventSetup& iSetup) {
	ESHandle<MagneticField> magneticField;
	iSetup.get<IdealMagneticFieldRecord> ().get(magneticField);
	transformer_ = new PFTrackTransformer(math::XYZVector(
			magneticField->inTesla(GlobalPoint(0, 0, 0))));
	transformer_->OnlyProp();
}

void PFPretendTrackProducer::endJob() {

}

void PFPretendTrackProducer::produce(edm::Event& event,
		const edm::EventSetup& setup) {

	//first check for new run!
	bool runok(true);
	if (event.run() != thisRun_->runNumber_) {
		std::cout << __PRETTY_FUNCTION__ << ": New run detected :"
				<< event.run() << ".\n";
		thisRun_ = runInfos_[event.run()];
	}
	if (!runok) {
		std::cout << __PRETTY_FUNCTION__ << ": problem looking up run info?!"
				<< std::endl;
		return;
	}

	//create the empty collections
	std::auto_ptr<TrackCollection> trackColl(new TrackCollection());
	std::auto_ptr<PFRecTrackCollection> pfRecTrackColl(
			new PFRecTrackCollection());
	std::auto_ptr<GsfPFRecTrackCollection> gsfpfRecTrackColl(
			new GsfPFRecTrackCollection());
	std::auto_ptr<PFClusterCollection> pfPSColl(new PFClusterCollection());
	std::auto_ptr<PFClusterCollection> pfHFHADColl(new PFClusterCollection());
	std::auto_ptr<PFClusterCollection> pfHFEMColl(new PFClusterCollection());
	std::auto_ptr<MuonCollection> muonColl(new MuonCollection());
	std::auto_ptr<VertexCollection> vertexColl(new VertexCollection());
	std::auto_ptr<TrackExtraCollection> trackExtrasColl(
			new TrackExtraCollection());

	if(justCreateEmptyCollections_) {
		event.put(trackColl, "tracks");
		event.put(trackExtrasColl);
		event.put(pfRecTrackColl, "pfRecTracks");
		event.put(gsfpfRecTrackColl, "gsfPfRecTracks");
		event.put(pfPSColl, "pfPS");
		event.put(pfHFHADColl, "pfHFHAD");
		event.put(pfHFEMColl, "pfHFEM");
		event.put(muonColl, "muons");
		event.put(vertexColl, "offlinePrimaryVertices");
		return;
	}

	//Track starts from Experiment Origin
	math::XYZPoint originXYZ(0, 0, 0);

	//Convert beamEnergy to transverse beam momentum using cosh(eta) function:
	//That's the mass of a pion at the end, in GeV
	//TODO: if these eta/phi are wrong, tracks won't associate with clusters!
	//math::PtEtaPhiMLorentzVector originVec(thisRun_->beamEnergy_ / cosh(
	//		thisRun_->ecalEta_), thisRun_->ecalEta_, thisRun_->ecalPhi_, 0.14);

	math::PtEtaPhiMLorentzVector originVec(thisRun_->beamEnergy_ / cosh(
			thisRun_->tableEta_), thisRun_->tableEta_, thisRun_->tablePhi_, 0.14);

	math::XYZTLorentzVector dirXYZ;
	dirXYZ.SetPxPyPzE(originVec.Px(), originVec.Py(), originVec.Pz(),
			originVec.E());
	PFTrajectoryPoint originPoint(-1, PFTrajectoryPoint::ClosestApproach,
			originXYZ, dirXYZ);

	TrackBase::CovarianceMatrix m;
	math::XYZVector dir(originVec.Px(), originVec.Py(), originVec.Pz());

	//Construct a track
	//Chisq = 1, ndof = 0, position, direction, charge, dummy covariance matrix
	//UPDATE: need to not use "undefined algorithm" - cases a hissy fit in PFBlockAlgo in 3_X
	//Track track(1.0, 0.0, originXYZ, dir, -1, m);
	Track track(1.0, 0.0, originXYZ, dir, -1, m, TrackBase::ctf);
	//TODO: this doesn't seem to have any effect?
	math::XYZPoint caloSurface(1.0, 0.0, 0.0);

	TrackExtraRefProd trackExtrasRef = event.getRefBeforePut<
			TrackExtraCollection> ();

	TrackExtra extra(originXYZ, dir, true, caloSurface, dir, true, m, 0, m, 1,
			alongMomentum);
	trackExtrasColl->push_back(extra);
	OrphanHandle<TrackExtraCollection> theRecoTrackExtras = event.put(
			trackExtrasColl);

	const TrackExtraRef theTrackExtraRef(theRecoTrackExtras, 0);
	track.setExtra(theTrackExtraRef);
	if (debug_ > 1) {
		std::cout << "Track momentum: " << track.p() << std::endl;
		std::cout << "\tTrack vertex: " << track.vertex() << std::endl;
		std::cout << "\tTrack outer momentum: " << track.outerMomentum()
				<< std::endl;
	}

	trackColl->push_back(track);
	OrphanHandle<TrackCollection> recoTracks = event.put(trackColl, "tracks");
	//DONE making standard tracks

	//MAKE PFRecTracks
	const TrackRef trackRef(recoTracks, 0);
	PFRecTrack pftrackAlternative(track.charge(), reco::PFRecTrack::KF, 0,
			trackRef);
	Trajectory fakeTraj;
	bool valid = transformer_->addPoints(pftrackAlternative, *trackRef,
			fakeTraj);
	if (!valid)
		std::cout << "Transformer failed to make transformation!" << std::endl;
	if (debug_ > 1) {
		std::cout << "PFRecTrack: " << pftrackAlternative << std::endl;
		const reco::PFTrajectoryPoint & atECAL =
				pftrackAlternative.extrapolatedPoint(
						reco::PFTrajectoryPoint::ECALEntrance);
		std::cout << "Extrapolated to ECAL : " << atECAL << std::endl;
	}
	pfRecTrackColl->push_back(pftrackAlternative);

	//FINISH UP
	event.put(pfRecTrackColl, "pfRecTracks");
	event.put(gsfpfRecTrackColl, "gsfPfRecTracks");
	event.put(pfPSColl, "pfPS");
	event.put(pfHFHADColl, "pfHFHAD");
	event.put(pfHFEMColl, "pfHFEM");
	event.put(muonColl, "muons");
	event.put(vertexColl, "offlinePrimaryVertices");

}

//define this as a plug-in
DEFINE_FWK_MODULE( PFPretendTrackProducer);
