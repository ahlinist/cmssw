#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/McEmObjectAlgo.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

void McEmObjectAlgo::initialise(const edm::ParameterSet &params)
{
   inputProducer_ = params.getParameter<std::string>("InputProducer");
   fromMother_ = params.getParameter<bool>("FromMother");
   motherPdgId_ = params.getParameter<int>("MotherPdgId");
}

void McEmObjectAlgo::run(const edm::Event &event, EgEff::EmObjectCollection &outCol)
{
   // Get electron collection
   edm::Handle<edm::HepMCProduct> mcHandle;
   try
   {
      event.getByLabel(inputProducer_, mcHandle);
   }
   catch(cms::Exception &ex)
   {
      edm::LogError("McEmObjectAlgo") << "Error! Can't get collection " << inputProducer_;
      throw ex;
   }
   
   // Find all electrons in the collection
   const edm::HepMCProduct *mcProd = mcHandle.product();
   const HepMC::GenEvent *evt = mcProd->GetEvent();

   for(HepMC::GenEvent::particle_const_iterator pitr = evt->particles_begin(); pitr != evt->particles_end(); ++pitr)
   {
      // Only choose electrons
      if(abs((*pitr)->pdg_id()) == 11)
      {
	 if(fromMother_)
	 {

	    HepMC::GenVertex::particles_out_const_iterator mother
               = (*pitr)->production_vertex()->particles_out_const_begin();

            if(((*pitr)->production_vertex()->particles_out_size() == 1) 
              && ((*mother)->pdg_id() == motherPdgId_))
	    {
               HepMC::FourVector p = (*pitr)->momentum();
	       math::XYZTLorentzVector p4(p.x(), p.y(), p.z(), p.t());
	       char charge = ((*pitr)->pdg_id() > 0 ? 1 : -1);
	       math::XYZPoint vtx((*pitr)->production_vertex()->point3d().x(), (*pitr)->production_vertex()->point3d().y(), (*pitr)->production_vertex()->point3d().z());
	       EgEff::EmObject obj(charge, p4, vtx);
	       outCol.push_back(obj);
	    }
	 }
	 else
	 {
	    HepMC::FourVector p = (*pitr)->momentum();
	    math::XYZTLorentzVector p4(p.x(), p.y(), p.z(), p.t());
	    char charge = ((*pitr)->pdg_id() > 0 ? 1 : -1);
	    math::XYZPoint vtx((*pitr)->production_vertex()->point3d().x(), (*pitr)->production_vertex()->point3d().y(), (*pitr)->production_vertex()->point3d().z());
	    EgEff::EmObject obj(charge, p4, vtx);
	    outCol.push_back(obj);
	 }
      }
   }
}

