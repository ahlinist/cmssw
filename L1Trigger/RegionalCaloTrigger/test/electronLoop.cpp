#include "L1Trigger/RegionalCaloTrigger/interface/L1RCT.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTLookupTables.h"
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"

#include <vector>
#include <iostream>
using std::vector;
using std::cout;
using std::endl;

vector<vector<vector<unsigned short> > > zeroVec(){
  vector<vector<vector<unsigned short> > > v(18,vector<vector<unsigned short> >(7,
				           vector<unsigned short>(64)));
  return v;
} 

int main(){
  // For testing use 1:1 LUT
  std::vector<double> eGammaECalScaleFactors(32, 1.0);
  std::vector<double> eGammaHCalScaleFactors(32, 1.0);
  std::vector<double> jetMETECalScaleFactors(32, 1.0);
  std::vector<double> jetMETHCalScaleFactors(32, 1.0);
  L1RCTParameters* rctParameters = 
    new L1RCTParameters(1.0,                       // eGammaLSB
			1.0,                       // jetMETLSB
			3.0,                       // eMinForFGCut
			40.0,                      // eMaxForFGCut
			0.5,                       // hOeCut
			1.0,                       // eMinForHoECut
			50.0,                      // eMaxForHoECut
			2.0,                       // eActivityCut
			3.0,                       // hActivityCut
			3.0,                       // eicIsolationThreshold
			false,                     // ignoreTowerHB
			false,                     // ignoreTowerHEplus
			false,                     // ignoreTowerHEminus
			eGammaECalScaleFactors,
			eGammaHCalScaleFactors,
			jetMETECalScaleFactors,
			jetMETHCalScaleFactors
			);
  L1RCTLookupTables* lut = new L1RCTLookupTables();
  lut->setRCTParameters(rctParameters);  // transcoder and etScale are not used
  L1RCT rct(lut);
  vector<vector<unsigned short> > hf(18,vector<unsigned short>(8));
  vector<vector<vector<unsigned short> > > barrel(18,vector<vector<unsigned short> >(7,
					    vector<unsigned short>(64)));
  for(int j = 0; j<4; j++){
    for(int i = 0; i<8; i++){
      barrel = zeroVec();
      barrel.at(0).at(0).at(j+4*i) = 10;
      rct.input(barrel,hf);
      rct.processEvent();
      //rct.printEIC(0,0);
      
      barrel = zeroVec();
      barrel.at(0).at(0).at(j+4*i) = 5;
      if(i<7)
	barrel.at(0).at(0).at(j+4*(i+1)) = 5;
      else
	barrel.at(0).at(2).at(j) = 5;
      rct.input(barrel,hf);
      rct.processEvent();
      rct.printEIC(0,0);
      //rct.printEICEdges(0,0);
      rct.printEIC(0,2);
      
    }
  }
  
}
