{
TFile f("reco.root");

// plot track chi-squared
Events.Draw("tracks.normalizedChi2()", "tracks.chi2()<1000")

// plot number of rec hits
Events.Draw("trackExtras.recHitsSize()");

// plot pt at vertex vs pt at outermost point
Events.Draw("trackExtras.outerPt():tracks.pt()", "tracks.pt()>9", "box");

// plot Delta_phi for positive and negative tracks
TH1F dpplus ("dpplus",  "#Delta#phi", 200,-0.25, 0.25 );
TH1F dpminus("dpminus", "#Delta#phi", 200,-0.25, 0.25 );
Events->Project( "dpplus", "tracks.phi()-trackExtras.outerPhi()", "tracks.charge()>0" );
Events->Project( "dpminus", "tracks.phi()-trackExtras.outerPhi()", "tracks.charge()<0" );
dpplus.SetFillColor( kBlue );
dpminus.SetFillColor( kRed );
dpplus.Draw();
dpminus.Draw("same");

// plot recHits local position
Events.Draw("recHits.data_.localPosition().x()")

// plot position of first hit
Events.Draw( "recHits.data_[trackExtras.recHits_.refVector_.items_[0].index()].localPosition().x():recHits.data_[trackExtras.recHits_.refVector_.items_[1].index()].localPosition().x()", "trackExtras.recHitsSize()>1" );
// should become:
// Events.Draw("trackExtras.recHit(0).localPosition().x():trackExtras.recHit(1).localPosition().x()", "trackExtras.recHitsSize()>1");
}
