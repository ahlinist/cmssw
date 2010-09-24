#include "TAnaJet.hh"
#include <iostream>

ClassImp(TAnaJet)

using namespace std;

TAnaJet::TAnaJet(int index) { 
  fIndex = index;
  fNtracks = 0; 

}

void TAnaJet::clear() {
  fIndex    = -1;
  
}


void TAnaJet::dump() {
 cout<<"------AnaJet"<<endl;
 cout<<" fIndex="<<fIndex<<"ftrjpvindx="<<ftrjpvindx<<" fbtag="<< fbtag<<endl; 
 cout<<" ET="<<fPlab.Perp()<<" eta="<< fPlab.Eta()<<" fM="<<fM<<endl;
 cout<<" fEMEnergy="<<fEMEnergy<<" fHADEnergy="<<fHADEnergy<<" fn60="<<fn60<<" fn90="<<fn90<<endl;
  cout << " trks: (" << fNtracks << ")";
  for (int i = 0; i < fNtracks; ++i) cout << getTrack(i) << ", ";
    
  cout << endl;
}
  
void TAnaJet::addTrack(int index) {
  if (fNtracks == TANAJET_MAXTRK-1) {
    cout << "TAnaJet: Too many tracks in jet. Not adding track!" << endl;
    return;
  }

  //  cout << "TAnaVertex: Adding track index " << index << " at array position " << fNtracks << endl;
  fTracksIndex[fNtracks] = index;
  ++fNtracks;
}
