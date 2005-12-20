{
cout << "Loading lcg_Cintex..." << endl;
gSystem->Load("liblcg_Cintex");

cout << "Loading TrackReco..." << endl;
gSystem->Load("libDataFormatsTrackReco");

cout << "Loading MuonReco..." << endl;
gSystem->Load("libDataFormatsMuonReco");

cout << "Loading VertexReco..." << endl;
gSystem->Load("libDataFormatsVertexReco");

cout << "Loading EGammaReco..." << endl;
gSystem->Load("libDataFormatsEGammaReco");

cout << "Loading Framework..." << endl;
gSystem->Load("libFWCoreFramework");

cout << "Cintex init..." << endl;
Cintex::enable();

cout << "Opening RECO file" << endl;
TFile f("reco.root");
}
