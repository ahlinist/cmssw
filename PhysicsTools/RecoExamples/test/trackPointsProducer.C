{
TFile f( "ntp.root" );
TCanvas canv;

TH1F pt( "pt", "transverse momentum", 50, 0, 20 );
Events.Project( "pt", "ctfWithMaterialTracks.pt()" );
pt.Draw();
canv.SaveAs( "pt.eps" );

TH2F rVsEta( "rVsEta","radius vs #eta", 50, -2.5, 2.5, 50, 0, 120 );
Events.Project( "rVsEta", "outer.rho():outer.eta()" );
rVsEta.Draw( "box" );
canv.SaveAs( "rVsEta.eps" );

TH1F outerR( "outerR", "outermost hit radius", 50, 0, 120 );
Events.Project( "outerR", "outer.rho()", "fabs(outer.eta())<1" );
outerR.Draw();
canv.SaveAs( "outerR.eps" );

TH1F deltaR( "deltaR", "#Delta radius (outer - inner)", 50, 0, 120 );
Events.Project( "deltaR", "outer.rho() - inner.rho()", "fabs(outer.eta())<1" );
deltaR.Draw();
canv.SaveAs( "deltaR.eps" );

TH1F deltaPhi( "deltaPhi", "#Delta #phi (outer - inner)", 80, -1.5, 1.5 );
Events.Project( "deltaPhi", "outer.phi() - inner.phi()", "fabs(outer.eta())<1" );
deltaPhi.Draw();
canv.SaveAs( "deltaPhi.eps" );

TH2F deltaPhiVsPt( "deltaPhiVsPt", "#Delta #phi vs pt", 70, 0, 20, 40, -1.5, 1.5 );
Events.Project( "deltaPhiVsPt", "outer.phi()-inner.phi():ctfWithMaterialTracks.pt()", "fabs(outer.eta())<1" );
deltaPhiVsPt.Draw( "box" );
canv.SaveAs( "deltaPhiVsPt.eps" );

}
