#include "GeneratorInterface/MadGraphInterface/interface/PdfAnalyzer.h"

PdfAnalyzer::PdfAnalyzer(const edm::ParameterSet& iConfig) { }
PdfAnalyzer::~PdfAnalyzer() { }
void PdfAnalyzer::beginJob(const edm::EventSetup&) { }
void PdfAnalyzer::endJob() { }

void PdfAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace edm; using namespace std;

   Handle<HepMCProduct> evt; iEvent.getByType(evt);

   const HepMC::GenEvent * genEvt = evt->GetEvent(); //   genEvt->print();

   // NOT STORED!
   HepMC::PdfInfo* pdfstuff = genEvt->pdf_info();
   if (pdfstuff != 0) cout << "Momentum of first incoming parton:  (id/flavour = " << pdfstuff->id1() << ")  " <<  pdfstuff->x1() << endl
        << "Momentum of second incoming parton: (id/flavour = " << pdfstuff->id2() << ")  " <<  pdfstuff->x2() << endl
        << "Scale = " << pdfstuff->scalePDF() << endl;
   else cout << "PDFstuff not set!" << endl;
}

