{
cout << "Loading lcg_Cintex..." << endl;
gSystem->Load("liblcg_Cintex");

cout << "Loading TrackReco..." << endl;
gSystem->Load("libDataFormatsTrackReco");

cout << "Loading VertexReco..." << endl;
gSystem->Load("libDataFormatsVertexReco");

cout << "Cintex init..." << endl;
Cintex::enable();

cout << "Opening RECO file" << endl;
TFile f("reco.root");
}
