#include "PhysicsTools/ttHAnalysis/interface/NtupleReader.h"
#include <iostream>
using namespace std;
using namespace tth;

NtupleReader::NtupleReader( const string & fileName ) :
  chain_( "GeneralVariables" ),
  current_( 0 ) {
  cout << "ttHAnalysis NtupleReader: reading " <<  fileName << endl;
  chain_.Add( fileName.c_str() );
  cout << "ttHAnalysis NtupleReader: access tree " << endl;

  chain_.SetBranchAddress( "Event", & data_.Event );
  chain_.SetBranchAddress( "Run", & data_.Run );
  chain_.SetBranchAddress( "TotalMuonX", & data_.TotalMuonX );
  chain_.SetBranchAddress( "TotalMuonY", & data_.TotalMuonY );
  chain_.SetBranchAddress( "TowerEtX", & data_.TowerEtX );
  chain_.SetBranchAddress( "TowerEtY", & data_.TowerEtY );

  chain_.SetBranchAddress("NumbLep",&data_.NumbLep);
  chain_.SetBranchAddress("LeptonPx",data_.LeptonPx);
  chain_.SetBranchAddress("LeptonPy",data_.LeptonPy);
  chain_.SetBranchAddress("LeptonPz",data_.LeptonPz);
  chain_.SetBranchAddress("LeptonPtIso",data_.LeptonPtIso);

  chain_.SetBranchAddress("NumbJet",&data_.NumbJet);
  chain_.SetBranchAddress("JetE",data_.JetE);
  chain_.SetBranchAddress("JetEt",data_.JetEt);
  chain_.SetBranchAddress("JetEta",data_.JetEta);
  chain_.SetBranchAddress("JetPhi",data_.JetPhi);
  chain_.SetBranchAddress("JetTheta",data_.JetTheta);
  chain_.SetBranchAddress("JetMass",data_.JetMass);
  chain_.SetBranchAddress("JetEcalFraction",data_.JetEcalFraction);
  chain_.SetBranchAddress("BJetFlag",data_.BJetFlag);
  chain_.SetBranchAddress("BDiscriminator",data_.BDiscriminator);
 
  Long64_t entries = chain_.GetEntries();
  cout << "Entries found: " << entries << endl;
}

void NtupleReader::readNext() {
  chain_.GetEntry( current_ );
  
  current_ ++;
}
