
void makeEfficiencyCorrectionRootFile() {

    // numbers from AN-11-390 v6, Table 13

    // do electron isolation/ID efficiency corrections

    TH2F* elecIsoCorr = new TH2F("electronIsolationEfficiencyCorrection","electronIsolationEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    elecIsoCorr->SetBinContent(1,1,1.19); // EE-, 10 < Pt < 15
    elecIsoCorr->SetBinContent(1,2,1.06); // EE-, 15 < Pt < 20
    
    elecIsoCorr->SetBinContent(2,1,1.11); // EB-, 10 < Pt < 15
    elecIsoCorr->SetBinContent(2,2,0.98); // EB-, 15 < Pt < 20
    
    elecIsoCorr->SetBinContent(3,1,1.11); // EB+, 10 < Pt < 15
    elecIsoCorr->SetBinContent(3,2,0.98); // EB+, 15 < Pt < 20
    
    elecIsoCorr->SetBinContent(4,1,1.19); // EE+, 10 < Pt < 15
    elecIsoCorr->SetBinContent(4,2,1.06); // EE+, 15 < Pt < 20
    
    for ( int i = 3; i <= 500; i++ ) {
        elecIsoCorr->SetBinContent(1,i,1.05); // EE-, 20 < Pt 
        elecIsoCorr->SetBinContent(2,i,0.99); // EB-, 20 < Pt 
        elecIsoCorr->SetBinContent(3,i,0.99); // EB+, 20 < Pt 
        elecIsoCorr->SetBinContent(4,i,1.05); // EE+, 20 < Pt 
    }

    TFile elecIsoFile("../data/electronIsolationEfficiencyCorrection.root","RECREATE");
    elecIsoCorr->Write();
    elecIsoFile.Close();


    // do electron trigger efficiency corrections

    TH2F* elecTriggCorr = new TH2F("electronTriggerEfficiencyCorrection","electronTriggerEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    elecTriggCorr->SetBinContent(1,1,0.97); // EE-, 10 < Pt < 15
    elecTriggCorr->SetBinContent(1,2,1.05); // EE-, 15 < Pt < 20
    elecTriggCorr->SetBinContent(1,3,1.00); // EE-, 20 < Pt < 25
    elecTriggCorr->SetBinContent(1,4,1.00); // EE-, 25 < Pt < 30
    
    elecTriggCorr->SetBinContent(2,1,0.98); // EB-, 10 < Pt < 15
    elecTriggCorr->SetBinContent(2,2,1.00); // EB-, 15 < Pt < 20
    elecTriggCorr->SetBinContent(2,3,1.001); // EB-, 20 < Pt < 25
    elecTriggCorr->SetBinContent(2,4,1.001); // EB-, 25 < Pt < 30
    
    elecTriggCorr->SetBinContent(3,1,0.98); // EB+, 10 < Pt < 15
    elecTriggCorr->SetBinContent(3,2,1.00); // EB+, 15 < Pt < 20
    elecTriggCorr->SetBinContent(3,3,1.001); // EB+, 20 < Pt < 25
    elecTriggCorr->SetBinContent(3,4,1.001); // EB+, 25 < Pt < 30

    elecTriggCorr->SetBinContent(4,1,0.97); // EE+, 10 < Pt < 15
    elecTriggCorr->SetBinContent(4,2,1.05); // EE+, 15 < Pt < 20
    elecTriggCorr->SetBinContent(4,3,1.00); // EE+, 20 < Pt < 25
    elecTriggCorr->SetBinContent(4,4,1.00); // EE+, 25 < Pt < 30
    
    for ( int i = 5; i <= 500; i++ ) {
        elecTriggCorr->SetBinContent(1,i,1.008); // EE-, 30 < Pt 
        elecTriggCorr->SetBinContent(2,i,1.003); // EB-, 30 < Pt 
        elecTriggCorr->SetBinContent(3,i,1.003); // EB+, 30 < Pt 
        elecTriggCorr->SetBinContent(4,i,1.008); // EE+, 30 < Pt 
    }

    TFile elecTrigFile("../data/electronTriggerEfficiencyCorrection.root","RECREATE");
    elecTriggCorr->Write();
    elecTrigFile.Close();



    // do muon isolation/ID efficiency corrections

    TH2F* muIsoCorr = new TH2F("muonIsolationEfficiencyCorrection","muonIsolationEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    muIsoCorr->SetBinContent(1,1,0.98); // EE-, 10 < Pt < 15
    muIsoCorr->SetBinContent(1,2,0.962); // EE-, 15 < Pt < 20
    
    muIsoCorr->SetBinContent(2,1,0.92); // EB-, 10 < Pt < 15
    muIsoCorr->SetBinContent(2,2,0.948); // EB-, 15 < Pt < 20
    
    muIsoCorr->SetBinContent(3,1,0.92); // EB+, 10 < Pt < 15
    muIsoCorr->SetBinContent(3,2,0.948); // EB+, 15 < Pt < 20
    
    muIsoCorr->SetBinContent(4,1,0.98); // EE+, 10 < Pt < 15
    muIsoCorr->SetBinContent(4,2,0.962); // EE+, 15 < Pt < 20
    
    for ( int i = 3; i <= 500; i++ ) {
        muIsoCorr->SetBinContent(1,i,0.9982); // EE-, 20 < Pt 
        muIsoCorr->SetBinContent(2,i,0.9932); // EB-, 20 < Pt 
        muIsoCorr->SetBinContent(3,i,0.9932); // EB+, 20 < Pt 
        muIsoCorr->SetBinContent(4,i,0.9982); // EE+, 20 < Pt 
    }

    TFile muIsoFile("../data/muonIsolationEfficiencyCorrection.root","RECREATE");
    muIsoCorr->Write();
    muIsoFile.Close();


    // do muon trigger efficiency corrections

    TH2F* muTrigCorr = new TH2F("muonTriggerEfficiencyCorrection","muonTriggerEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    muTrigCorr->SetBinContent(1,1,1.03); // EE-, 10 < Pt < 15
    muTrigCorr->SetBinContent(1,2,1.07); // EE-, 15 < Pt < 20
    muTrigCorr->SetBinContent(1,3,1.04); // EE-, 20 < Pt < 25
    muTrigCorr->SetBinContent(1,4,1.04); // EE-, 25 < Pt < 30
    
    muTrigCorr->SetBinContent(2,1,1.01); // EB-, 10 < Pt < 15
    muTrigCorr->SetBinContent(2,2,0.99); // EB-, 15 < Pt < 20
    muTrigCorr->SetBinContent(2,3,0.99); // EB-, 20 < Pt < 25
    muTrigCorr->SetBinContent(2,4,0.99); // EB-, 25 < Pt < 30
    
    muTrigCorr->SetBinContent(3,1,1.01); // EB+, 10 < Pt < 15
    muTrigCorr->SetBinContent(3,2,0.99); // EB+, 15 < Pt < 20
    muTrigCorr->SetBinContent(3,3,0.99); // EB+, 20 < Pt < 25
    muTrigCorr->SetBinContent(3,4,0.99); // EB+, 25 < Pt < 30

    muTrigCorr->SetBinContent(4,1,1.03); // EE+, 10 < Pt < 15
    muTrigCorr->SetBinContent(4,2,1.07); // EE+, 15 < Pt < 20
    muTrigCorr->SetBinContent(4,3,1.04); // EE+, 20 < Pt < 25
    muTrigCorr->SetBinContent(4,4,1.04); // EE+, 25 < Pt < 30
    
    for ( int i = 5; i <= 500; i++ ) {
        muTrigCorr->SetBinContent(1,i,1.06); // EE-, 30 < Pt 
        muTrigCorr->SetBinContent(2,i,0.992); // EB-, 30 < Pt 
        muTrigCorr->SetBinContent(3,i,0.992); // EB+, 30 < Pt 
        muTrigCorr->SetBinContent(4,i,1.06); // EE+, 30 < Pt 
    }

    TFile muTrigFile("../data/muonTriggerEfficiencyCorrection.root","RECREATE");
    muTrigCorr->Write();
    muTrigFile.Close();
}
