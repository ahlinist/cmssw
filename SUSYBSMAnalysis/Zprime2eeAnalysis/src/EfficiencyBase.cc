#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/EfficiencyBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// Root includes
#include "Math/GenVector/VectorUtil.h"


//---------------------------------------------------------------------
// EfficiencyBase::InitialiseHistograms
//   Initialises all pass / total histograms
void EfficiencyBase::InitialiseHistograms(const edm::ParameterSet &params)
{

  // is it an event efficiency for zee or otherwise?
  _eventEfficiency = params.getUntrackedParameter<bool>("EventEfficiency");

  // Load pT parameters
  _ptBinsEB = params.getUntrackedParameter<int>("PtBinsEB");
  _ptMinEB = params.getUntrackedParameter<double>("PtMinEB");
  _ptMaxEB = params.getUntrackedParameter<double>("PtMaxEB");
  _ptBinsEE = params.getUntrackedParameter<int>("PtBinsEE");
  _ptMinEE = params.getUntrackedParameter<double>("PtMinEE");
  _ptMaxEE = params.getUntrackedParameter<double>("PtMaxEE");
  _ptBins = params.getUntrackedParameter<int>("PtBins");
  _ptMin = params.getUntrackedParameter<double>("PtMin");
  _ptMax = params.getUntrackedParameter<double>("PtMax");

  // Load eta parameters
  _etaBinsEB = params.getUntrackedParameter<int>("EtaBinsEB");
  _etaMinEB = params.getUntrackedParameter<double>("EtaMinEB");
  _etaMaxEB = params.getUntrackedParameter<double>("EtaMaxEB");
  _etaBinsEE = params.getUntrackedParameter<int>("EtaBinsEE");
  _etaMinEE = params.getUntrackedParameter<double>("EtaMinEE");
  _etaMaxEE = params.getUntrackedParameter<double>("EtaMaxEE");
  _etaBins = params.getUntrackedParameter<int>("EtaBins");
  _etaMin = params.getUntrackedParameter<double>("EtaMin");
  _etaMax = params.getUntrackedParameter<double>("EtaMax");

  // Load phi parameters
  _phiBinsEB = params.getUntrackedParameter<int>("PhiBinsEB");
  _phiMinEB = params.getUntrackedParameter<double>("PhiMinEB");
  _phiMaxEB = params.getUntrackedParameter<double>("PhiMaxEB");
  _phiBinsEE = params.getUntrackedParameter<int>("PhiBinsEE");
  _phiMinEE = params.getUntrackedParameter<double>("PhiMinEE");
  _phiMaxEE = params.getUntrackedParameter<double>("PhiMaxEE");
  _phiBins = params.getUntrackedParameter<int>("PhiBins");
  _phiMin = params.getUntrackedParameter<double>("PhiMin");
  _phiMax = params.getUntrackedParameter<double>("PhiMax");

  // Get TFileService
  edm::Service<TFileService> fs;
  _ptEBPass = fs->make<TH1F>("PtEBPass", "p_{T} Pass EB", _ptBinsEB, _ptMinEB, _ptMaxEB);
  _ptEBTotal = fs->make<TH1F>("PtEBTotal", "p_{T} Total EB", _ptBinsEB, _ptMinEB, _ptMaxEB);
  _ptEEPass = fs->make<TH1F>("PtEEPass", "p_{T} Pass EE", _ptBinsEE, _ptMinEE, _ptMaxEE);
  _ptEETotal = fs->make<TH1F>("PtEETotal", "p_{T} Total EE", _ptBinsEE, _ptMinEE, _ptMaxEE);
  _ptPass = fs->make<TH1F>("PtPass", "p_{T} Pass", _ptBins, _ptMin, _ptMax);
  _ptTotal = fs->make<TH1F>("PtTotal", "p_{T} Total", _ptBins, _ptMin, _ptMax);

  _etaEBPass = fs->make<TH1F>("EtaEBPass", "#eta# Pass EB", _etaBinsEB, _etaMinEB, _etaMaxEB);
  _etaEBTotal = fs->make<TH1F>("EtaEBTotal", "#eta# Total EB", _etaBinsEB, _etaMinEB, _etaMaxEB);
  _etaEEPass = fs->make<TH1F>("EtaEEPass", "#eta# Pass EE", _etaBinsEE, _etaMinEE, _etaMaxEE);
  _etaEETotal = fs->make<TH1F>("EtaEETotal", "#eta# Total EE", _etaBinsEE, _etaMinEE, _etaMaxEE);
  _etaPass = fs->make<TH1F>("EtaPass", "#eta# Pass", _etaBins, _etaMin, _etaMax);
  _etaTotal = fs->make<TH1F>("EtaTotal", "#eta# Total", _etaBins, _etaMin, _etaMax);

  _phiEBPass = fs->make<TH1F>("PhiEBPass", "#phi# Pass EB", _phiBinsEB, _phiMinEB, _phiMaxEB);
  _phiEBTotal = fs->make<TH1F>("PhiEBTotal", "#phi# Total EB", _phiBinsEB, _phiMinEB, _phiMaxEB);
  _phiEEPass = fs->make<TH1F>("PhiEEPass", "#phi# Pass EE", _phiBinsEE, _phiMinEE, _phiMaxEE);
  _phiEETotal = fs->make<TH1F>("PhiEETotal", "#phi# Total EE", _phiBinsEE, _phiMinEE, _phiMaxEE);
  _phiPass = fs->make<TH1F>("PhiPass", "#phi# Pass", _phiBins, _phiMin, _phiMax);
  _phiTotal = fs->make<TH1F>("PhiTotal", "#phi# Total", _phiBins, _phiMin, _phiMax);

  // Create efficiency histograms
  _ptEBEff = fs->make<TH1F>("PtEBEff", "p_{T} Eff EB", _ptBinsEB, _ptMinEB, _ptMaxEB);
  _ptEEEff = fs->make<TH1F>("PtEEEff", "p_{T} Eff EE", _ptBinsEE, _ptMinEE, _ptMaxEE);
  _ptEff = fs->make<TH1F>("PtEff", "p_{T} Eff", _ptBins, _ptMin, _ptMax);
  _etaEBEff = fs->make<TH1F>("EtaEBEff", "#eta# Eff EB", _etaBinsEB, _etaMinEB, _etaMaxEB);
  _etaEEEff = fs->make<TH1F>("EtaEEEff", "#eta# Eff EE", _etaBinsEE, _etaMinEE, _etaMaxEE);
  _etaEff = fs->make<TH1F>("EtaEff", "#eta# Eff", _etaBins, _etaMin, _etaMax);
  _phiEBEff = fs->make<TH1F>("PhiEBEff", "#phi# Eff EB", _phiBinsEB, _phiMinEB, _phiMaxEB);
  _phiEEEff = fs->make<TH1F>("PhiEEEff", "#phi# Eff EE", _phiBinsEE, _phiMinEE, _phiMaxEE);
  _phiEff = fs->make<TH1F>("PhiEff", "#phi# Eff", _phiBins, _phiMin, _phiMax);


  // init the tree
  std::cout << "making new ttree" << std::endl;
  std::string treeFile = params.getUntrackedParameter<std::string>("treeFile");
  _file = new TFile(treeFile.c_str(), "RECREATE");
  _file->cd();
  _tree = 0;
  _tree = new TTree("T", "Test Tree");

//  const Int_t kMaxProbe = 5;
//  pass_ = new Int_t[kMaxProbe];
//  os_   = new Int_t[kMaxProbe];
//  pt_   = new Float_t[kMaxProbe];
//  eta_  = new Float_t[kMaxProbe];
//  phi_  = new Float_t[kMaxProbe];
//  weight_ = new Float_t[kMaxProbe];

//  _tree->Branch("nProbe", &nProbe_, "nProbe/I");
  _tree->Branch("weight", &weight_, "weight/F");
  _tree->Branch("pass", &pass_, "pass/I");
  _tree->Branch("os", &os_, "os/I");
  _tree->Branch("pt", &pt_, "pt/F");
  _tree->Branch("eta", &eta_, "eta/F");
  _tree->Branch("phi", &phi_, "phi/F");
  _tree->Branch("vx", &vx_, "vx/F");
  _tree->Branch("vy", &vy_, "vy/F");
  _tree->Branch("vz", &vz_, "vz/F");
  _tree->Branch("mass", &mass_, "mass/F");

  std::cout << "done" << std::endl;

}

void EfficiencyBase::AddToTree(const EgEff::EmObjectRef probe, const EgEff::EmObjectRef tag, double weight, bool pass, bool os)
{

  mass_ = ROOT::Math::VectorUtil::InvariantMass(tag->p4(), probe->p4());
  pass_ = static_cast<Int_t>(pass);
  os_ = static_cast<Int_t>(os);
  weight_ = weight;

  // Determine the detector eta
  Float_t eta = 0;
  if (probe->hasSuperCluster()) {
     eta = probe->superCluster()->eta();
  } else {
     //double EfficiencyBase::ecalEta(double &EtaParticle, double &Zvertex, double &RhoVertex)
     double z = probe->vertex().z();
     double rho = probe->vertex().Rho();
     double etaPhys = probe->eta();
     eta = ecalEta(etaPhys, z, rho);
  }
  eta_ = eta;

  phi_ = probe->phi();
  pt_ = probe->pt();
  vx_ = probe->vertex().x();
  vy_ = probe->vertex().y();
  vz_ = probe->vertex().z();

  _tree->Fill();


}

void EfficiencyBase::BeginTree()
{

  nProbe_ = 0;

}

void EfficiencyBase::FillTree()
{
  _tree->Fill();
}


//---------------------------------------------------------------------
// EfficiencyBase::Initialise
//   Intented to be overridden to allow trigger specific setup
//   Overloaded function must call InitialiseHistograms
void EfficiencyBase::Initialise(const edm::ParameterSet &params)
{
  InitialiseHistograms(params);
}

//---------------------------------------------------------------------
// EfficiencyBase::AddToTotalHistograms
//   Adds the given EmObjectRef to the total trigger histograms with
//   event weight of 1
void EfficiencyBase::AddToTotalHistograms(const EgEff::EmObjectRef obj)
{
  AddToTotalHistograms(obj, 1.0);
}

//---------------------------------------------------------------------
// EfficiencyBase::AddToPassHistograms
//   Adds the given EmObjectRef to the pass trigger histograms with
//   event weight of 1
void EfficiencyBase::AddToPassHistograms(const EgEff::EmObjectRef obj)
{
  AddToPassHistograms(obj, 1.0);
}

//---------------------------------------------------------------------
// EfficiencyBase::AddToTotalHistograms
//   Adds the given EmObjectRef to the total trigger histograms with
//   given event weight
void EfficiencyBase::AddToTotalHistograms(const EgEff::EmObjectRef obj, double weight)
{
  if(fabs(obj->eta()) < 1.4442)
    {
      _ptEBTotal->Fill(obj->pt(), weight);
      _etaEBTotal->Fill(obj->eta(), weight);
      _phiEBTotal->Fill(obj->phi(), weight);
    }
  else
    {
      _ptEETotal->Fill(obj->pt(), weight);
      _etaEETotal->Fill(obj->eta(), weight);
      _phiEETotal->Fill(obj->phi(), weight);
    }
  _ptTotal->Fill(obj->pt(), weight);
  _etaTotal->Fill(obj->eta(), weight);
  _phiTotal->Fill(obj->phi(), weight);
}

//---------------------------------------------------------------------
// EfficiencyBase::AddToPassHistograms
//   Adds the given EmObjectRef to the pass trigger histograms with
//   given event weight
void EfficiencyBase::AddToPassHistograms(const EgEff::EmObjectRef obj, double weight)
{
  if(fabs(obj->eta()) < 1.4442 )
    {
      _ptEBPass->Fill(obj->pt(), weight);
      _etaEBPass->Fill(obj->eta(), weight);
      _phiEBPass->Fill(obj->phi(), weight);
    }
  else
    {
      _ptEEPass->Fill(obj->pt(), weight);
      _etaEEPass->Fill(obj->eta(), weight);
      _phiEEPass->Fill(obj->phi(), weight);
    }
  _ptPass->Fill(obj->pt(), weight);
  _etaPass->Fill(obj->eta(), weight);
  _phiPass->Fill(obj->phi(), weight);
}

//---------------------------------------------------------------------
// EfficiencyBase::FinishHistograms
//   Creates efficiency histograms from the pass / fail histograms
//   To be called from analysis endJob method
void EfficiencyBase::FinishHistograms()
{

  // Fill histograms
  FillEfficiencyHistogram(_ptEBPass, _ptEBTotal, _ptEBEff);
  FillEfficiencyHistogram(_ptEEPass, _ptEETotal, _ptEEEff);
  FillEfficiencyHistogram(_ptPass, _ptTotal, _ptEff);
  FillEfficiencyHistogram(_etaEBPass, _etaEBTotal, _etaEBEff);
  FillEfficiencyHistogram(_etaEEPass, _etaEETotal, _etaEEEff);
  FillEfficiencyHistogram(_etaPass, _etaTotal, _etaEff);
  FillEfficiencyHistogram(_phiEBPass, _phiEBTotal, _phiEBEff);
  FillEfficiencyHistogram(_phiEEPass, _phiEETotal, _phiEEEff);
  FillEfficiencyHistogram(_phiPass, _phiTotal, _phiEff);

  // Now compute all efficiencies
  double ebPass = _ptEBPass->Integral();
  double ebTotal = _ptEBTotal->Integral();
  double eePass = _ptEEPass->Integral();
  double eeTotal = _ptEETotal->Integral();
  double pass = _ptPass->Integral();
  double total = _ptTotal->Integral();

  double ebEff, ebErr, eeEff, eeErr, eff, err;
  CalculateEventEfficiency(ebPass, ebTotal, ebEff, ebErr);
  CalculateEventEfficiency(eePass, eeTotal, eeEff, eeErr);
  CalculateEventEfficiency(pass, total, eff, err);

  std::cout << "-------------------------" << std::endl << "Efficiencies for " << _name << std::endl;
  std::cout << "EB:  " << ebEff << " ± " << ebErr << std::endl;
  std::cout << "EE:  " << eeEff << " ± " << eeErr << std::endl;
  std::cout << "All: " << eff << " ± " << err << std::endl;
  std::cout << "-------------------------" << std::endl;


  std::cout << "writting tree" << std::endl;
   _file->cd();
   _tree->Write();
   delete _tree;
   _tree = 0;
//   _file->Write();
//   delete _file;
//   _file = 0;

//   delete os_;
//   delete pass_;
//   delete eta_; 
 //  delete phi_;
//   delete pt_;

  std::cout << "done" << std::endl;


//  std::cout << std::endl;
// std::cout << "same sign probeEE tagEE: " << _sameProbeEETagEE << std::endl;
//  std::cout << "same sign probeEE tagEB: " << _sameProbeEETagEB << std::endl;
//  std::cout << "same sign probeEB tagEE: " << _sameProbeEBTagEE << std::endl;
//  std::cout << "same sign probeEB tagEB: " << _sameProbeEBTagEB << std::endl;

}

//---------------------------------------------------------------------
// EfficiencyBase::CalculateEventEfficiency
//   Calculates the event efficiency from counts
void EfficiencyBase::CalculateEventEfficiency(const double pass, const double total, double &eff, double &err)
{
  double ratio = pass / total;

  if (_eventEfficiency) {
    eff = ((2.0 * ratio) - (ratio * ratio));
  }  else eff = ratio;

  eff =  pass / total;
  err = sqrt((eff * (1.0 - eff)) / total);
}

//---------------------------------------------------------------------
// EfficiencyBase::FillEfficiencyHistogram
//   Fills a trigger efficiency histogram, given combinatorics for
//   an event to pass given the probe passed
void EfficiencyBase::FillEfficiencyHistogram(TH1F *pass, TH1F *total, TH1F *eff)
{
  int numBins = pass->GetNbinsX();
  for(int i = 1; i <= numBins; ++i)
    {
      double passCount = pass->GetBinContent(i);
      double totalCount = total->GetBinContent(i);
      totalCount = ((totalCount == 0) ? 1 : totalCount);
      double ratio = passCount / totalCount;
      double effVal = 0.;

      if (_eventEfficiency) {
        effVal = ((2.0 * ratio) - (ratio * ratio));
      }  else effVal = ratio;

      double errVal = sqrt((effVal * (1.0 - effVal)) / totalCount);
      eff->SetBinContent(i, effVal);
      eff->SetBinError(i, errVal);
    }
}

//---------------------------------------------------------------------
// EfficiencyBase::dR
//   Calculates dR for two sets of eta / phi coordinates
double EfficiencyBase::dR(double eta1, double phi1, double eta2, double phi2)
{
  return sqrt(((eta1-eta2)*(eta1-eta2)) + (dPhi(phi1, phi2) * dPhi(phi1, phi2)));
}

//---------------------------------------------------------------------
// EfficiencyBase::dPhi
//   Calculates dPhi for two sets of coordinates
double EfficiencyBase::dPhi(double phi1, double phi2)
{
  double diff = fabs(phi2 - phi1);
  double corr = 2*acos(-1.) - diff;
  if (diff < acos(-1.)){ return diff;} else { return corr;}
}

double EfficiencyBase::ecalEta(double &EtaParticle, double &Zvertex, double &RhoVertex)
{

  const double R_ECAL = 136.5;
  const double etaBarrelEndcap = 1.479;
  const double Z_Endcap = 328.0;
  const double pi = 3.14159265;

  if (EtaParticle!= 0.)
    {
      double Theta = 0.0  ;
      double ZEcal = (R_ECAL-RhoVertex)*sinh(EtaParticle)+Zvertex;

      if(ZEcal != 0.0) Theta = atan(R_ECAL/ZEcal);
      if(Theta<0.0) Theta = Theta+pi;

      double ETA = - log(tan(0.5*Theta));

      if( fabs(ETA) > etaBarrelEndcap )
   {
     double Zend = Z_Endcap ;
     if(EtaParticle<0.0 )  Zend = -Zend ;
     double Zlen = Zend - Zvertex ;
     double RR = Zlen/sinh(EtaParticle);
     Theta = atan((RR+RhoVertex)/Zend);
     if(Theta<0.0) Theta = Theta+pi;
     ETA = - log(tan(0.5*Theta));
   }
      return ETA;
    }
  else
    {
      edm::LogWarning("") << "[EcalPositionFromTrack::etaTransformation] Warning: Eta equals to zero, not correcting";
      return EtaParticle;
    }
}

double EfficiencyBase::ecalPhi(double &PtParticle, double &EtaParticle, double &PhiParticle, int ChargeParticle, double &Rstart)
{
  //Magnetic field
  const double RBARM = 1.357 ;  // was 1.31 , updated on 16122003
  const double ZENDM = 3.186 ;  // was 3.15 , updated on 16122003
  double Rbend = RBARM-(Rstart/100.); //Assumed Rstart in cm
  double Bend  = 0.3 * 4. * Rbend/ 2.0 ;
  const double pi = 3.14159265;
  const double etaBarrelEndcap = 1.479;

  //---PHI correction
  double PHI = 0.0 ;
  if( fabs(EtaParticle) <=  etaBarrelEndcap)
    {
      if (fabs(Bend/PtParticle)<=1.)
        {
          PHI = PhiParticle - asin(Bend/PtParticle)*ChargeParticle;
          if(PHI >  pi) {PHI = PHI - 2*pi;}
          if(PHI < -pi) {PHI = PHI + 2*pi;}
        }
      else
        {
          edm::LogWarning("") << "[EcalPositionFromTrack::phiTransformation] Warning:Too low Pt, giving up ";
          return PhiParticle;
        }
    }

  if( fabs(EtaParticle) >  etaBarrelEndcap )
    {
      double Rhit = 0.0 ;
      Rhit = ZENDM / sinh(fabs(EtaParticle));
      if (fabs(((Rhit-(Rstart/100.))/Rbend)*Bend/PtParticle)<=1.)
        {
          PHI = PhiParticle - asin(((Rhit-(Rstart/100.))/Rbend)*Bend/PtParticle)*ChargeParticle;
          if(PHI >  pi) {PHI = PHI - 2*pi;}
          if(PHI < -pi) {PHI = PHI + 2*pi;}
        }
      else
        {
          edm::LogWarning("") <<"[EcalPositionFromTrack::phiTransformation] Warning:Too low Pt, giving up ";
          return PhiParticle;
        }

    }

  //---Return the result
  return PHI;
}

