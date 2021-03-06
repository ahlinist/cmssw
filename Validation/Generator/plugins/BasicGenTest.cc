/*class BasicGenTest
 *  
 *  Class to fill dqm monitor elements from existing EDM file
 *
 *  BasicGenTest:
 *  $Date: 2010/01/06 12:39:48 $
 *  $Revision: 1.7 $
 *  \author Joseph Zennamo SUNY-Buffalo; Based on: ConverterTester*/
 
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "Validation/Generator/plugins/BasicGenTest.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

using namespace edm;

BasicGenTest::BasicGenTest(const edm::ParameterSet& iPSet)
{
  glunum = 0;
  dusnum = 0;
  cnum = 0;
  bnum = 0;
  topnumber = 0;
  Wnum = 0;
  Znum = 0;
  bjorken = 0.;
 
  dbe = 0;
  dbe = edm::Service<DQMStore>().operator->();
}

BasicGenTest::~BasicGenTest() {}

void BasicGenTest::beginJob()
{
  if(dbe){
    ///Setting the DQM top directories
    dbe->setCurrentFolder("Generator/Particles");
    ///Booking the ME's
    gluonNumber = dbe->book1D("gluonNumber", "No. gluons", 100, -.5, 299.5);
    dusNumber = dbe->book1D("dusNumber", "No. uds", 20, -.5, 59.5);
    cNumber = dbe->book1D("cNumber", "No. c", 15, -.5, 14.5);
    bNumber = dbe->book1D("bNumber", "No. b", 15, -.5, 14.5);
    tNumber = dbe->book1D("tNumber", "No. t", 6, -.5, 5.5);
    WNumber = dbe->book1D("WNumber", "No. W", 15, -.5, 14.5);
    ZNumber = dbe->book1D("ZNumber", "No. Z", 6, -.5, 5.5);
    stablepart = dbe->book1D("stablepart", "No. ptcls", 100, -.5, 1199.5);
    Part_ID = dbe->book1D("Part_ID", "No. parton by PDGID ", 100, -.5, 99.5);
    Bjorken_x = dbe->book1D("Bjorken_x", "Bjorken_x", 100000, 0.0, 1.0);

    dbe->tag(gluonNumber->getFullname(),1);
    dbe->tag(dusNumber->getFullname(),2);
    dbe->tag(cNumber->getFullname(),3);
    dbe->tag(bNumber->getFullname(),4);
    dbe->tag(tNumber->getFullname(),5);
    dbe->tag(WNumber->getFullname(),6);
    dbe->tag(ZNumber->getFullname(),7);
    dbe->tag(stablepart->getFullname(),8);
    dbe->tag(Part_ID->getFullname(),9);
    dbe->tag(Bjorken_x->getFullname(),10);
 }
  return;
}

void BasicGenTest::endJob(){return;}
void BasicGenTest::beginRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void BasicGenTest::endRun(const edm::Run& iRun,const edm::EventSetup& iSetup){return;}
void BasicGenTest::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 
  int counterstable = 0; // To count "stable", status == 1, particles
  glunum = 0;
  bnum = 0;
  topnumber = 0;
  Wnum = 0;
  dusnum = 0;
  cnum = 0;
  Znum = 0;
  bjorken = 0.;
  for(unsigned short int i=0; i < 100; ++i) part_counter[i] = 0;//clearing the array

  ///Gathering the HepMCProduct information
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel("generator", evt);
  HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
  HepMC::GenEvent::particle_const_iterator begin = myGenEvent->particles_begin();
  HepMC::GenEvent::particle_const_iterator end = myGenEvent->particles_end();
  HepMC::PdfInfo * pdf = myGenEvent->pdf_info();

  ///Looping through the particles in the event
  for(HepMC::GenEvent::particle_const_iterator it = begin;it!=end;++it)
     {
       HepMC::GenParticle* particle = *it;
       unsigned int Id = particle->pdg_id();
       unsigned short int status = particle->status();
       //double pT = particle->momentum().perp();

       ///counting multiplicities
       if (abs(Id) == 6) ++topnumber;
       if (abs(Id) == 24) ++Wnum;
       if(abs(Id) == 5) ++bnum;
       if(abs(Id) == 1 || abs(Id) == 2 || abs(Id) == 3) ++dusnum;
       if(abs(Id) == 4) ++cnum;
       if(abs(Id) == 23) ++Znum;
       if(abs(Id) == 21) ++glunum;
       if( 0 < Id && 100 > Id) ++part_counter[Id];
       if(status == 1) ++counterstable;
	
	}//for(HepMC::

  if(pdf){
	 bjorken = ((pdf->x1())/((pdf->x1())+(pdf->x2())));
  }

  //if((2*topnumber) != Wnum){ ///Since top decays from status 3, but W does not
  //  std::cout << "WOOT WOOT! We have a problem! Tops= " << topnumber << "!= W's = " << Wnum << std::endl;
  //  myGenEvent->print();
  //  }//if(top
  //if(bnum == 1 || bnum == 3 || bnum == 5){
  //  std::cout << "We have odd number of b's! b = " << bnum << std::endl;
  //  myGenEvent->print();
  //  }//if(bnum
  //if(Wnum == 0 && (cnum == 1 || cnum ==3 || cnum == 5 || cnum == 7)){
  //  std::cout << "We have odd number of c's! c = " << cnum << std::endl;
  //  myGenEvent->print();
  //  }//if(Wnum

  ///filling the ME's
  gluonNumber->Fill(glunum);
  dusNumber->Fill(dusnum);
  cNumber->Fill(cnum);
  bNumber->Fill(bnum);
  tNumber->Fill(topnumber);
  WNumber->Fill(Wnum);
  ZNumber->Fill(Znum);
  stablepart->Fill(counterstable);
  Bjorken_x->Fill(bjorken);
  
  for(unsigned short int i = 0; i < 100; ++i){
     Part_ID->Fill(float(i), float(part_counter[i]));
  }

  delete myGenEvent;

 }

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BasicGenTest);
