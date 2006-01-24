///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoLayer.cc
// Description: Persistent Layer and Hit informaion for SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#include "SimG4Validation/Hcal/interface/PHcalValidInfoLayer.h"
#include "SimG4CMS/Calo/interface/HcalTestNumberingScheme.h"

#include <iostream>
#include <cmath>

void PHcalValidInfoLayer::fillLayers(double el[], double ed[], double ho,
				     double hbhe) {

  eLayer.resize(20);
  for (int i = 0; i < 20; i++) {
    double en  = 0.001*el[i];
    eLayer[i] = en;
  }
  eDepth.resize(4);
  for (int i = 0; i < 4; i++) {
    double en  = 0.001*ed[i];
    eDepth[i] = en;
  }
  eHO   = ho;
  eHBHE = hbhe;
}

void PHcalValidInfoLayer::fillHF(double fibl, double fibs, double enec,
				 double enhc) {
  elongHF  = fibl;
  eshortHF = fibs;
  eEcalHF  = enec;
  eHcalHF  = enhc;
}

void PHcalValidInfoLayer::fillHits(int iv, std::vector<CaloHit> hitcache) {

  std::cout << "Enter PHcalValidInfoLayer::fillHits with verbos " << iv
	    << " and " << hitcache.size() << " hits " << std::endl;
  int nHit = hitcache.size();
  int hit  = 0;
  int i;
  std::vector<CaloHit>::iterator itr;
  std::vector<CaloHit*> lhits(nHit);
  for (i = 0, itr = hitcache.begin(); itr != hitcache.end(); i++, itr++) {
    uint32_t unitID=itr->id();
    int   subdet, zside, group, ieta, iphi, lay;
    HcalTestNumberingScheme::unpackHcalIndex(unitID,subdet,zside,group,
                                             ieta,iphi,lay);
    subdet = itr->det();
    lay    = itr->layer();
    group  = (subdet&15)<<20;
    group += ((lay-1)&31)<<15;
    group += (zside&1)<<14;
    group += (ieta&127)<<7;
    group += (iphi&127);
    itr->setId(group);
    lhits[i] = &hitcache[i];
    if (iv>2) {
      std::cout << "PHcalValidInfoLayer::fillHits:Original " << i << " " 
		<< hitcache[i] << std::endl;
      std::cout << "PHcalValidInfoLayer::fillHits:Copied   " << i << " " 
		<< *lhits[i]    << std::endl;
    }
  }
  sort(lhits.begin(),lhits.end(),CaloHitIdMore());
  std::vector<CaloHit*>::iterator k1, k2;
  if (iv>2) {
    for (i = 0, k1 = lhits.begin(); k1 != lhits.end(); i++, k1++)
      std::cout << "PHcalValidInfoLayer::fillHits:Sorted " << i << " " 
		<< **k1 << std::endl;
  }
  int nHits = 0;
  for (i = 0, k1 = lhits.begin(); k1 != lhits.end(); i++, k1++) {
    double       ehit  = (**k1).e();
    double       t     = (**k1).t();
    uint32_t     unitID= (**k1).id();
    int          jump  = 0;
    if (iv>2) 
      std::cout << "PHcalValidInfoLayer::fillHits:Start " << i << " U/T/E"
		<< " 0x" << std::hex << unitID << std::dec << " "  << t
		<< " " << ehit;
    for (k2 = k1+1; k2 != lhits.end() && (t-(**k2).t()) < 1 &&
	   (t-(**k2).t()) > -1 && unitID == (**k2).id(); k2++) {
      ehit += (**k2).e();
      if (iv>2) std::cout << " + " << (**k2).e();
      jump++;
    }
    if (iv>2)
      std::cout << " = " << ehit << " in " << jump << std::endl;

    double eta  = (*k1)->eta();
    double phi  = (*k1)->phi();
    int lay     = ((unitID>>15)&31) + 1;

    hits.resize(nHits+1);
    hits[nHits].layer = lay;
    hits[nHits].id    = unitID;
    hits[nHits].eta   = eta;
    hits[nHits].phi   = phi;
    hits[nHits].e     = ehit;
    hits[nHits].t     = t;
    nHits++;

    int subdet = (unitID>>20)&15;
    int zside  = (unitID>>14)&1;
    int ieta   = (unitID>>7)&127;
    int iphi   = (unitID)&127;
    if (iv > 1) 
      std::cout << "PHcalValidInfoLayer::fillHits:Hit " << nHits << " " << i 
                << " ID 0x" << std::hex << unitID << std::dec << " " << subdet 
                << " " << lay << " " << zside << " " << ieta << " " << iphi 
                << " Time " << t << " E " << ehit << std::endl;

    i  += jump;
    k1 += jump;
  }

  if (iv > 1) 
    std::cout << "PHcalValidInfoLayer::fillHits called with " << nHit 
              << " hits" << " and writes out " << nHits << '(' << hit 
              << ") hits" << std::endl;

}
