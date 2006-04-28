#include "PhysicsTools/ttHAnalysis/interface/TTHNuReco.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

void TTHNuReco::momentum(float missEtX, float missEtY, float pxLep,float pyLep,
			 float pzLep, Candidate::LorentzVector & pNuP, 
			 Candidate::LorentzVector & pNuM ) {
  float missEtZplus =0.;
  float missEtZminus =0.;
  //Parameters for PzNu
  //  const float muMass = 0.10566;
  const float eMass = 0.000511;
  const float wMass = 80.3427;

  float muNrg =sqrt((eMass*eMass)+(pxLep*pxLep + pyLep*pyLep + pzLep*pzLep));
  float firstTerm =  2*(4*muNrg*muNrg - 4*pzLep*pzLep);
  
  /*
  float secondTerm = -4*pzLep*(- wMass*wMass - 2*pxLep*missEtX - pxLep*pxLep - pyLep*pyLep  - pzLep*pzLep + muNrg*muNrg - 2*pyLep*missEtY);  

  float thirdTerm = (secondTerm*secondTerm) - 4*(4*muNrg*muNrg - 4*pzLep*pzLep)*(- wMass*wMass*wMass*wMass - 4*pxLep*missEtX*wMass*wMass - 4*pxLep*pxLep*missEtX*missEtX - 
									       2*pxLep*pxLep*wMass*wMass - 4*pxLep*pxLep*pxLep*missEtX - pxLep*pxLep*pxLep*pxLep - 
									       2*pyLep*pyLep*wMass*wMass - 4*pxLep*pyLep*pyLep*missEtX - 2*pxLep*pxLep*pyLep*pyLep -
									       pyLep*pyLep*pyLep*pyLep - 2*pzLep*pzLep*wMass*wMass - 4*pxLep*missEtX*pzLep*pzLep - 
									       2*pxLep*pxLep*pzLep*pzLep - 2*pyLep*pyLep*pzLep*pzLep -  pzLep*pzLep*pzLep*pzLep + 
									       4*missEtX*missEtX*muNrg*muNrg + 2*wMass*wMass*muNrg*muNrg + 4*muNrg*muNrg*missEtX*pxLep +
									       2*muNrg*muNrg*pxLep*pxLep + 2*muNrg*muNrg*pyLep*pyLep + 2*muNrg*muNrg*pzLep*pzLep - 
									       muNrg*muNrg*muNrg*muNrg - 4*wMass*wMass*pyLep*missEtY - 
									       8*missEtX*pxLep*pyLep*missEtY - 4*pxLep*pxLep*pyLep*missEtY - 4*pyLep*pyLep*pyLep*missEtY - 
									       4*pyLep*pzLep*pzLep*missEtY + 4*pyLep*muNrg*muNrg*missEtY - 4*pyLep*pyLep*missEtY*missEtY +
									       4*muNrg*muNrg*missEtY*missEtY);

  */

  float secondTerm = -4*pzLep*(- wMass*wMass + eMass*eMass - 2*pxLep*missEtX - 2*pyLep*missEtY);  

  float thirdTerm = secondTerm*secondTerm - 4*(4*muNrg*muNrg - 4*pzLep*pzLep)*
    (- wMass*wMass*wMass*wMass - eMass*eMass*eMass*eMass 
     - 4*(pxLep*missEtX + pyLep*missEtY)*(pxLep*missEtX + pyLep*missEtY) 
     + 4* muNrg * muNrg * ( missEtX*missEtX+missEtY*missEtY ) 
     + 2* wMass*wMass*eMass*eMass
     - 4* wMass*wMass*(pxLep*missEtX + pyLep*missEtY)
     + 4* eMass*eMass*(pxLep*missEtX + pyLep*missEtY) );

  //  cout << "FIRST TERM = " << firstTerm << endl;
  //  cout << "SECOND TERM = " << secondTerm << endl;
  //  cout << "THIRD TERM = " << thirdTerm << endl;

  if(thirdTerm > 0.&& firstTerm != 0.){
    missEtZplus = (secondTerm + sqrt(thirdTerm))/firstTerm;
    missEtZminus = (secondTerm - sqrt(thirdTerm))/firstTerm;
  }
  else{
    missEtZplus = 0.;
    missEtZminus = 0.; 
  }  

  pNuP.SetPx( missEtX );
  pNuP.SetPy( missEtY );
  pNuP.SetPz( missEtZplus );
  pNuP.SetE(sqrt(missEtX*missEtX + missEtY*missEtY + missEtZplus*missEtZplus));

  pNuM.SetPx( missEtX );
  pNuM.SetPy( missEtY );
  pNuM.SetPz( missEtZminus );
  pNuM.SetE(sqrt(missEtX*missEtX + missEtY*missEtY + missEtZminus*missEtZminus));


}

