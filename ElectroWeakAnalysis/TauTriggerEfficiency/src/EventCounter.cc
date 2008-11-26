#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/EventCounter.h"

#include "TFile.h"
#include "TH1F.h"

EventCounter::EventCounter(const edm::ParameterSet& iConfig){
	name     = iConfig.getParameter<string>("name");
	fileName = iConfig.getUntrackedParameter<string>("fileName");
	counter  = 0;
}

EventCounter::~EventCounter(){
        cout << "counter " << name << " = " << counter << endl;

        TFile* file = TFile::Open(fileName.c_str(),"UPDATE");
        file->cd();

        TH1F* eventCounter = NULL;
        TH1F* tmpHisto = (TH1F*)file->Get("eventCounter");
        int nBins = 0;
        if(tmpHisto == NULL) {
                eventCounter = new TH1F("eventCounter","",1,0,1);
        }else{
                nBins = tmpHisto->GetNbinsX();
                eventCounter = new TH1F("eventCounter","",nBins+1,0,nBins+1);
        }

        eventCounter->SetBinContent(1,counter);
	eventCounter->GetXaxis()->SetBinLabel(1,name.c_str());

        for(int i = 1; i <= nBins; ++i) {
                eventCounter->SetBinContent(i+1,tmpHisto->GetBinContent(i));
                eventCounter->GetXaxis()->SetBinLabel(i+1,tmpHisto->GetXaxis()->GetBinLabel(i));
        }
	enum { 	kIsOnHeap	,
		kNotDeleted	,
		kZombie		,
		kBitMask	,
		kSingleKey	,
		kOverwrite	,
		kWriteDelete	
	};
        eventCounter->Write("eventCounter",kOverwrite);

        file->Close();
}

void EventCounter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	counter++;
}
