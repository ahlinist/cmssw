{
	_file0->cd("runValidation");
	c1 = new TCanvas();
	c1->Print("ValidationPlots.ps[");
	c1->SetLogz();

	gStyle->SetOptStat(111111);
	c1->SetLogx(1);
	c1->SetLogy(1);
	dataCollectionSize->Draw();
	c1->Print("ValidationPlots.ps");
	emulCollectionSize->Draw();
	c1->Print("ValidationPlots.ps");
	c1->SetLogx(0);
	//Error Flags
	errflag_HBHE->Draw();
	c1->Print("ValidationPlots.ps");
	errflag_HF->Draw();
	c1->Print("ValidationPlots.ps");

	gStyle->SetOptStat(000000);
	c1->SetLogy(0);
	alldata_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	allemul_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	dataonly_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	emulonly_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	mismatch_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	mismatch_FG_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	emul_ZS_map->Draw("colz");
	c1->Print("ValidationPlots.ps");
	
	etcorr_HBHE->Draw("colz");
	c1->Print("ValidationPlots.ps");
	etcorr_HF->Draw("colz");
	c1->Print("ValidationPlots.ps");
	etcorr2_HBHE->Draw("colz");
	c1->Print("ValidationPlots.ps");
	etcorr2_HF->Draw("colz");
	c1->Print("ValidationPlots.ps");

	gStyle->SetOptStat(111111);
	c1->SetLogy(1);
	alldata_et_HBHE->Draw();
	c1->Print("ValidationPlots.ps");

	allemul_et_HBHE->Draw();
	c1->Print("ValidationPlots.ps");

	dataonly_et_HBHE->Draw();
	c1->Print("ValidationPlots.ps");

	emulonly_et_HBHE->Draw();
	c1->Print("ValidationPlots.ps");

	emulZS_et_HBHE->Draw();
	c1->Print("ValidationPlots.ps");

	alldata_et_HF->Draw();
	c1->Print("ValidationPlots.ps");

	allemul_et_HF->Draw();
	c1->Print("ValidationPlots.ps");

	dataonly_et_HF->Draw();
	c1->Print("ValidationPlots.ps");

	emulonly_et_HF->Draw();
	c1->Print("ValidationPlots.ps");

	emulZS_et_HF->Draw();
	c1->Print("ValidationPlots.ps");
	//---------------------------------------
	
	//FG bit
	data_FG_HF->Draw();
	c1->Print("ValidationPlots.ps");
	emul_FG_HF->Draw();
	c1->Print("ValidationPlots.ps");

	data_FGet_HF->Draw();
	c1->Print("ValidationPlots.ps");
	emul_FGet_HF->Draw();
	c1->Print("ValidationPlots.ps");

	c1->Print("ValidationPlots.ps]");
}
