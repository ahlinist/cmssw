{
TFile f("reco.root");
TH1F dpplus ("dpplus",  "#Delta#phi", 200,-0.25, 0.25 );
TH1F dpminus("dpminus", "#Delta#phi", 200,-0.25, 0.25 );
Events->Project( "dpplus", "tracks.phi()-trackExtras.outerPhi()", "tracks.charge()>0" );
Events->Project( "dpminus", "tracks.phi()-trackExtras.outerPhi()", "tracks.charge()<0" );
dpplus.SetFillColor( kBlue );
dpminus.SetFillColor( kRed );
dpplus.Draw();
dpminus.Draw("same");
}
