#include "../interface/CSCTT_CSCTrackerFilter.h"
#include <iostream>
#include <TH1F.h>
#include <TFile.h>

CSCTT_CSCTrackerFilter::CSCTT_CSCTrackerFilter(){
totalHits=0;
matchHits=0;
for(int i=0;i<5;i++) match[i]=false;
  h_windowEta =  new TH1F("h_windowEta","Window #eta ",60,0,6);
  h_windowPhi =  new TH1F("h_windowPhi","Window #varphi ",80,-4,4);
  h_windowPt  =  new TH1F("h_windowPt","Window Pt ",0,0,6);
  h_windowZ   =  new TH1F("h_windowZ","Window Z ",1000,0,1000);
}

void CSCTT_CSCTrackerFilter::ProcessEvent(){
  
 const float ptscale[33] = { 
    -1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
      4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
    16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
    50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 };
  
  for(int h=0;h<5;h++)
    match[h]=false;
  
  matched1=false;
  int tfPt = 0;
  int tfEta = 0;
  double trackerZ = 0;
  double trackerPhi =0;
  double delta = 0;
  double deltaZ =0;
  double deltaPhi =0;
  double tfPhi = 0;
  int tfPhiPacked = 0;
  int tfEtaPacked = 0;
  int charge = 0;
  double matchPhiCenter = 0;
  double matchZCenter = 0;
  CSCTT_CSCMatch cscMatch;
  _cscCable -> Reset();
  
  //const TrajectoryStateOnSurface *trackerHit;

  //
  const CSCTT_MyStub *trackerHit;
  //
  //std::cout<<"A"<<std::endl; 
  
 // std::cout << "before first read" << std::endl;
  
  const L1MuRegionalCand *tfStub = _cscCable->ReadHit();
//std::cout<<"B"<<std::endl; 
 // std::cout << "after first read" << std::endl;

  for ( ;tfStub != 0;tfStub = _cscCable->ReadHit()){
    
     tfPt = tfStub->pt_packed();
     tfEta = tfStub->eta_packed();
    
     //std::cout << "tfEta (from tracker Filter): " << tfEta << std::endl; 
     
     tfPhi = (2.5*( tfStub->phi_packed() ))*(M_PI)/180 + 0.0218;
     tfPhiPacked = tfStub->phi_packed();
     tfEtaPacked = tfStub->eta_packed();
     charge= tfStub->charge_packed();
     cscMatch.SetStub(*tfStub);
    
  //std::cout<<"c"<<std::endl;   
      h_windowPt->Fill(ptscale[tfPt]);
      for(int station=0;station<5;station++)
       {
  //      std::cout<<"c1"<<std::endl; 
         _lookupTable->PhiBend(charge,station,tfPt,tfEtaPacked);
  //      std::cout<<"c2"<<std::endl; 
	 _lookupTable->Z(charge,station,tfEta);
  //      std::cout<<"c3"<<std::endl; 
         matchPhiCenter = _lookupTable -> phiMean;
//	std::cout<<"c4"<<std::endl; 
         matchZCenter   = _lookupTable -> zMean;
	//std::cout<<"c5"<<std::endl; 
	
	//std::cout << 3* _lookupTable -> zSigma << std::endl;
         h_windowPhi->Fill(ConvertAngle(tfPhi - _lookupTable -> phiMean));
	 h_windowZ->Fill(_lookupTable -> zMean);
	
         _trackerCables[station] -> Reset(); 
 //std::cout<<"d"<<std::endl;     
         for(  trackerHit = _trackerCables[station]->ReadHit(); trackerHit != 0; trackerHit = _trackerCables[station]->ReadHit() )
	  {
            totalHits++;

	     trackerZ   = trackerHit->z;
	    trackerPhi = trackerHit->phi;	

	    delta = ConvertAngle(ConvertAngle(tfPhi)-ConvertAngle(trackerPhi));
	
	    deltaPhi = delta  - matchPhiCenter; 
	    //deltaPhi = 2  - matchPhiCenter; 
	    deltaZ   = trackerZ   - matchZCenter;
	    
	    //std::<< 3* _lookupTable -> zSigma << std::endl;

	
		if ( fabs(deltaPhi) > 3 * _lookupTable -> phiSigma ) 
		{
		//std::cout << "PHI PROBLEM" <<std::endl;
	
		//std::cout  << "Station " << station << std::endl
		//  <<" TF Pt " << tfPt <<std::endl
		//  << " delta: " << delta <<std::endl
		//  << " Center " << matchPhiCenter << std::endl
		//  << " deltaPhi " << deltaPhi <<std::endl
		//  << " phiWin " <<"("<< (matchPhiCenter - 3 * _lookupTable -> phiSigma) << ","<< (matchPhiCenter + 3 * _lookupTable -> phiSigma)<<")"<<std::endl
		//  << " eta packed: " << tfStub->eta_packed()<<std::endl;
		continue;
		}
//	std::cout<<"g"<<std::endl; 
		if ( fabs(deltaZ)   > 3 * _lookupTable -> zSigma   ) 
		{
		// std::cout << "Z PROBLEM" <<std::endl;
		// std::cout
		//  << "Station " << station << std::endl
		//  << " trackerZ " << trackerZ 
		//  << " matchZCenter " << matchZCenter 
		//  << " deltaZ " << deltaZ 
		//  << " zWin " << 3* _lookupTable -> zSigma << std::endl;
		continue;
		}
	
//	std::cout<<"h"<<std::endl; 

		matchHits++; 
		match[station]=true;
		cscMatch.AddHit(&*trackerHit);
		//std::cout << "match size: " << cscMatch.GetNumHits() << std::endl;
		//std::cout<< " Match Hits " << matchHits << ", Total Hits: " << totalHits<<std::endl;
		matched1=true;
	}
      
	}
       
	 
	_outCable->WriteMatch(cscMatch);
   
	//std::cout << "OUTCABLE SIZE: " << _outCable -> NumMatchs() << std::endl;
 	

    //  std::cout<< cscMatch.GetNumHits() <<std::endl;
    
    //SLHCCSCTFCable::Hit* cscHit = _cscCable.ReadHit();
    
    //std::cout<<trackerHit.eta()<<std::endl;
    
	}

}




