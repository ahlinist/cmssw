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
  //  const float muMass = 0.10566;
  const float eMass = 0.000511;
  const float wMass = 80.3427;

  float muNrg =sqrt((eMass*eMass)+(pxLep*pxLep + pyLep*pyLep + pzLep*pzLep));
  float firstTerm =  2*(4*muNrg*muNrg - 4*pzLep*pzLep);
  
  float secondTerm = -4*pzLep*(- wMass*wMass + eMass*eMass - 2*pxLep*missEtX - 2*pyLep*missEtY);  

  float thirdTerm = secondTerm*secondTerm - 4*(4*muNrg*muNrg - 4*pzLep*pzLep)*
    (- wMass*wMass*wMass*wMass - eMass*eMass*eMass*eMass 
     - 4*(pxLep*missEtX + pyLep*missEtY)*(pxLep*missEtX + pyLep*missEtY) 
     + 4* muNrg * muNrg * ( missEtX*missEtX+missEtY*missEtY ) 
     + 2* wMass*wMass*eMass*eMass
     - 4* wMass*wMass*(pxLep*missEtX + pyLep*missEtY)
     + 4* eMass*eMass*(pxLep*missEtX + pyLep*missEtY) );

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

