{
TFile f( "reco.root" );

// plot track chi-squared
Events.Draw("ctfWithMaterialTracks.normalizedChi2()", "ctfWithMaterialTracks.chi2()<1000")

// plot number of rec hits
Events.Draw("ctfWithMaterialTrackExtras.recHitsSize()");

// plot pt at vertex vs pt at outermost point
Events.Draw("ctfWithMaterialTrackExtras.outerPt():ctfWithMaterialTracks.pt()", "ctfWithMaterialTracks.pt()<100", "box");

// plot Delta_phi for positive and negative tracks
TH1F dpplus ("dpplus",  "#Delta#phi", 200,-1, 1 );
TH1F dpminus("dpminus", "#Delta#phi", 200,-1, 1 );
Events->Project( "dpplus", "ctfWithMaterialTracks.phi()-ctfWithMaterialTrackExtras.outerPhi()", "ctfWithMaterialTracks.charge()>0" );
Events->Project( "dpminus", "ctfWithMaterialTracks.phi()-ctfWithMaterialTrackExtras.outerPhi()", "ctfWithMaterialTracks.charge()<0" );
dpplus.SetFillColor( kBlue );
dpminus.SetFillColor( kRed );
dpplus.Draw();
dpminus.Draw("same");

// plot rec hits local position
Events.Draw("ctfWithMaterialRecHits.data_.localPosition().x()")

// plot local position of first hit
Events.Draw( "ctfWithMaterialRecHits.data_[ctfWithMaterialTrackExtras.recHits_.refVector_.items_[0].key()].localPosition().x()", "ctfWithMaterialTrackExtras.recHitsSize()>1" );
}
