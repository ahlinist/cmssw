#include "TAnaTrack.hh"
#include <iostream>

ClassImp(TAnaTrack)

using namespace std;

TAnaTrack::TAnaTrack(int index) { 
  fIndex = index;
}

void TAnaTrack::clear() {
  fMCID     = -99999;
  fIndex    = -1;
  fGenIndex = -1;
}


void TAnaTrack::dump() {
cout<<"------AnaTrack"<<endl;
 cout<<" fIndex="<<fIndex<<" fMCID="<<fMCID<<" fGenIndex="<<fGenIndex<<" fIndxj="<<fIndxj<<" muID="<<fMuID<<endl;
 cout<<" PT="<<fPlab.Perp()<<" eta="<< fPlab.Eta()<<" fQ="<<fQ<<" PTrel="<< fMuonPtrel<<endl;
 cout<<" fHits="<<fHits<<" fChi2="<< fChi2<<" fTrChi2norm="<<fTrChi2norm<<" vch2= "<<fMuonVertexChi2<< endl;
cout<<"  fDxybs="<< fDxybs<<" fDxypv="<<fDxypv<<" fDxypvnm="<<fDxypvnm<<" fTip="<<fTip<<endl;
cout<<"  fDzbs="<< fDzbs<<" fDzpv="<<fDzpv<<" fLip="<<fLip<<endl;
cout<<"  isolPT="<<fMuonIsosumPT<<" fMuonIsoEcal= "<<fMuonIsoEcal<<" fMuonIsoHcal="<<fMuonIsoHcal<<endl;
 
}
  
