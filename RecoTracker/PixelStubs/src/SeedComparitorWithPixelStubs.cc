#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h"
#include "RecoTracker/PixelStubs/interface/SimpleHelix.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedingHitSet.h"
#include "RecoLocalTracker/Records/interface/TkPixelCPERecord.h"
#include "RecoLocalTracker/SiPixelRecHits/interface/PixelCPETemplateReco.h"

SeedComparitorWithPixelStubs::SeedComparitorWithPixelStubs(const edm::ParameterSet &cfg)
:		tempCutFactor_(cfg.getParameter<double>("tempCutFactor"))
{}

SeedComparitorWithPixelStubs::~SeedComparitorWithPixelStubs()
{}

// Check to see if the vector of hits are compatible based on
// their local angle beta as extracted using the PixelStubs. If one of the 
// hits is not comptible with another, then the whole set is declared incompatible
bool SeedComparitorWithPixelStubs::compatible(const SeedingHitSet &seedhits, const edm::EventSetup &es) {
	edm::ESHandle<PixelClusterParameterEstimator> hCPE;
	es.get<TkPixelCPERecord>().get("PixelCPETemplateReco",hCPE);
	const PixelClusterParameterEstimator &cpe(*hCPE);
	const	PixelCPETemplateReco * theCPE = dynamic_cast<const PixelCPETemplateReco *> (&cpe);

	edm::ESHandle<TrackerGeometry> geom;
	es.get<TrackerDigiGeometryRecord>().get(geom);

	edm::LogInfo ("PixStub") << "Number of hits: " << seedhits.size();
	
	unsigned int hit_size = seedhits.size();
	LocalPoint temp_lp, lp1, lp2, lp3;
	GlobalPoint temp_gp;

	std::vector<double> p1,p2,p3,d1,d2,d3;
	double alpha1, alpha2, alpha3, beta1, beta2, beta3, prob1, prob2, prob3;
	
	if(hit_size<2 || hit_size>3) throw cms::Exception("SeedComparitorWithPixelStubs") << "Too few or too many hits found in the seed hit set. Expected 2 or 3";
	for(unsigned int i=0; i<hit_size-1; ++i) {
		edm::LogInfo ("PixStub") << "Sub Det ID " << i+1 << " = " << seedhits[i]->geographicalId().subdetId();
		if ( seedhits[i]->geographicalId().subdetId() != 1 && seedhits[0]->geographicalId().subdetId() != 2) {
			edm::LogInfo ("PixStub") << "Not in pixels";
			return true;
		}
	}
	
	const SiPixelRecHit &pix1 = dynamic_cast<const SiPixelRecHit&>((*seedhits[0]->hit()));
	const GeomDetUnit *geoUnit1 = geom->idToDetUnit(seedhits[0]->geographicalId());
	temp_lp = pix1.localPosition();
	temp_gp = geoUnit1->surface().toGlobal( temp_lp);
	p1.push_back(temp_gp.x());
	p1.push_back(temp_gp.y());
	p1.push_back(temp_gp.z());

	const SiPixelRecHit &pix2 = dynamic_cast<const SiPixelRecHit&>((*seedhits[1]->hit()));
	const GeomDetUnit *geoUnit2 = geom->idToDetUnit(seedhits[1]->geographicalId());
	temp_lp = pix2.localPosition();
	temp_gp = geoUnit2->surface().toGlobal( temp_lp);
	p2.push_back(temp_gp.x());
	p2.push_back(temp_gp.y());
	p2.push_back(temp_gp.z());

	if(hit_size==2) {
		edm::LogInfo ("PixStub") << "Two Hits: "
														 << " p1.x = " << p1[0] << " p1.y = " << p1[1] << " p1.z = " << p1[2]
														 << " p2.x = " << p2[0] << " p2.y = " << p2[1] << " p2.z = " << p2[2]
														 << " p3.x = " << p3[0] << " p3.y = " << p3[1] << " p3.z = " << p3[2];

		SimpleHelix Helix(p1,p2);
		Helix.directions(d1,d2,d3);
		edm::LogInfo ("PixStub") << "Two Cosines: "
														 << " d1.x = " << d1[0] << " d1.y = " << d1[1] << " d1.z = " << d1[2]
														 << " d2.x = " << d2[0] << " d2.y = " << d2[1] << " d2.z = " << d2[2]
														 << " d3.x = " << d3[0] << " d3.y = " << d3[1] << " d3.z = " << d3[2];
		
		GlobalPoint gp1(d1[0],d1[1],d1[2]);
		lp1 = geoUnit1->surface().toLocal(gp1);
		
		GlobalPoint gp2(d2[0],d2[1],d2[2]);
		lp2 = geoUnit1->surface().toLocal(gp2);
		
		GlobalPoint gp3(d3[0],d3[1],d3[2]);
		lp3 = geoUnit1->surface().toLocal(gp3);
	}
	else {
		const SiPixelRecHit &pix3 = dynamic_cast<const SiPixelRecHit&>((*seedhits[2]->hit()));
		const GeomDetUnit *geoUnit3 = geom->idToDetUnit(seedhits[2]->geographicalId());
		temp_lp = pix3.localPosition();
		temp_gp = geoUnit3->surface().toGlobal( temp_lp);
		p3.push_back(temp_gp.x());
		p3.push_back(temp_gp.y());
		p3.push_back(temp_gp.z());

		edm::LogInfo ("PixStub") << "Three Hits: "
														 << " p1.x = " << p1[0] << " p1.y = " << p1[1] << " p1.z = " << p1[2]
														 << " p2.x = " << p2[0] << " p2.y = " << p2[1] << " p2.z = " << p2[2]
														 << " p3.x = " << p3[0] << " p3.y = " << p3[1] << " p3.z = " << p3[2];

		
		SimpleHelix Helix(p1,p2,p3);
		Helix.directions(d1,d2,d3);
		
		edm::LogInfo ("PixStub") << "Three Cosines: "
														 << " d1.x = " << d1[0] << " d1.y = " << d1[1] << " d1.z = " << d1[2]
														 << " d2.x = " << d2[0] << " d2.y = " << d2[1] << " d2.z = " << d2[2]
														 << " d3.x = " << d3[0] << " d3.y = " << d3[1] << " d3.z = " << d3[2];
		
		GlobalPoint gp1(d1[0],d1[1],d1[2]);
		lp1 = geoUnit1->surface().toLocal(gp1);
		
		GlobalPoint gp2(d2[0],d2[1],d2[2]);
		lp2 = geoUnit1->surface().toLocal(gp2);
		
		GlobalPoint gp3(d3[0],d3[1],d3[2]);
		lp3 = geoUnit1->surface().toLocal(gp3);
		
		alpha3 = TMath::ATan2(lp3.z(),lp3.x());
		beta3  = TMath::ATan2(lp3.z(),lp3.y());
		
		SiPixelCluster cluster3 = *(pix3).cluster();

		PixelClusterParameterEstimator::LocalValues lv = theCPE->localParameters(cluster3, *geoUnit3, alpha3, beta3);
		double probX = theCPE->probabilityX();
		double probY = theCPE->probabilityY();
		edm::LogInfo ("PixStub") << "Probability X3 is " << probX << "\tProbability Y3 is " << probY
														 << " filled prob = " << theCPE->hasFilledProb();
		edm::LogInfo ("PixStub") << "Cluster X Size = " << cluster3.sizeX() << "\tCluster Y Size = "
														 << cluster3.sizeY();
		edm::LogInfo ("PixStub") << "Local Cosines = " << lp3;
		if (probX==0 || probY==0 || !(theCPE->hasFilledProb())) return false;
		else prob3 = probX*probY * (1 - log(probX*probY));
		if (prob3 < tempCutFactor_) return false;
	}
	
	alpha1 = TMath::ATan2(lp1.z(),lp1.x());
	beta1  = TMath::ATan2(lp1.z(),lp1.y());
	
	SiPixelCluster cluster1 = *(pix1).cluster();
	
	PixelClusterParameterEstimator::LocalValues lv = theCPE->localParameters(cluster1, *geoUnit1, alpha1, beta1);
	double probX = theCPE->probabilityX();
	double probY = theCPE->probabilityY();
	edm::LogInfo ("PixStub") << "Probability X1 is " << probX << "\tProbability Y1 is " << probY
													 << "\tfilled prob = " << theCPE->hasFilledProb();
	edm::LogInfo ("PixStub") << "Cluster X Size = " << cluster1.sizeX() << "\tCluster Y Size = "
													 << cluster1.sizeY();
	edm::LogInfo ("PixStub") << "Local Cosines = " << lp1;
	if (probX==0 || probY==0 || !(theCPE->hasFilledProb())) return false;
	else prob1 = probX*probY * (1 - log(probX*probY));
	
	alpha2 = TMath::ATan2(lp2.z(),lp2.x());
	beta2  = TMath::ATan2(lp2.z(),lp2.y());
	
	SiPixelCluster cluster2 = *(pix2).cluster();
	
	lv = theCPE->localParameters(cluster2, *geoUnit2, alpha2, beta2);
	probX = theCPE->probabilityX();
	probY = theCPE->probabilityY();
	edm::LogInfo ("PixStub") << "Probability X2 is " << probX << "\tProbability Y2 is " << probY
													 << "\tfilled prob = " << theCPE->hasFilledProb();
	edm::LogInfo ("PixStub") << "Cluster X Size = " << cluster2.sizeX() << "\tCluster Y Size = "
													 << cluster2.sizeY();
	edm::LogInfo ("PixStub") << "Local Cosines = " << lp2;
	if (probX==0 || probY==0 || !(theCPE->hasFilledProb())) return false;
	else prob2 = probX*probY * (1 - log(probX*probY));

	if(prob1 < tempCutFactor_ || prob2 < tempCutFactor_) return false;
	else return true;
}

