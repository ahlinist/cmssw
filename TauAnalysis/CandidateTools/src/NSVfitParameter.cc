#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <Math/Minimizer.h>

#include <algorithm>
#include <iostream>
#include <iomanip>

std::vector<double> NSVfitParameter::defaultInitialValues_;
std::vector<std::pair<double, double> > NSVfitParameter::defaultLimits_;
std::vector<double> NSVfitParameter::defaultStepSizes_;
bool NSVfitParameter::defaultValues_initialized_ = false;

NSVfitParameter::NSVfitParameter(int idx, const std::string& name, int type,
				 double initialValue, double lowerLimit, double upperLimit, double stepSize, bool isFixed)
  : idx_(idx),
    name_(name),
    type_(type),
    initialValue_(initialValue),
    lowerLimit_(lowerLimit),
    upperLimit_(upperLimit),
    stepSize_(stepSize),
    isFixed_(isFixed)
{
  if ( !(type >= 0 && type <= nSVfit_namespace::kNu_phi_lab) ) throw cms::Exception("NSVfitParameter")
       << "Invalid type = " << type << " !!\n";

  reset();
}

NSVfitParameter::NSVfitParameter(int idx, const std::string& name, int type, bool isFixed)
  : idx_(idx),
    name_(name),
    type_(type),
    isFixed_(isFixed)
{
  if ( !defaultValues_initialized_ ) initializeDefaultValues();

  if ( !(type >= 0 && type <= nSVfit_namespace::kNu_phi_lab) ) throw cms::Exception("NSVfitParameter")
    << "Invalid type = " << type << " !!\n";
  
  initialValue_ = defaultInitialValues_[type];
  lowerLimit_ = defaultLimits_[type].first;
  upperLimit_ = defaultLimits_[type].second;
  stepSize_ = defaultStepSizes_[type]; 

  reset();
}

void NSVfitParameter::dump(std::ostream& stream) const 
{
  stream << "param #" << idx_ << "(name = " << get_name_incl_type(name_, type_) << "):" 
	 << " value = " << value_ << " + " << errUp_ << " - " << errDown_ << std::endl;
  stream << " initialValue = " << initialValue_ << " +/- " << stepSize_ << ","
	 << " limits = {" <<  lowerLimit_ << ", " << upperLimit_ << "}";
  if      ( IsFixed()       ) stream << ", isFixed"; 
  else if ( IsDoubleBound() ) stream << ", isDoubleBound"; 
  else if ( HasLowerLimit() ) stream << ", hasLowerLimit"; 
  else if ( HasUpperLimit() ) stream << ", hasUpperLimit"; 
  stream << std::endl;
}

// Friend helpers for print functions
std::ostream& operator<<(std::ostream& stream, const NSVfitParameter& param) 
{
  param.dump(stream);
  return stream;
}

std::string get_name_incl_type(const std::string& name, int type)
{
  std::string retVal = name;
  retVal.append("::");
  if      ( type == nSVfit_namespace::kPV_shiftX             ) retVal.append("PV_shiftX");
  else if ( type == nSVfit_namespace::kPV_shiftY             ) retVal.append("PV_shiftY");
  else if ( type == nSVfit_namespace::kPV_shiftZ             ) retVal.append("PV_shiftZ");
  else if ( type == nSVfit_namespace::kTau_visEnFracX        ) retVal.append("visEnFracX");
  else if ( type == nSVfit_namespace::kTau_phi_lab           ) retVal.append("phi_lab");
  else if ( type == nSVfit_namespace::kTau_decayDistance_lab ) retVal.append("decayDistance_lab");
  else if ( type == nSVfit_namespace::kTau_nuInvMass         ) retVal.append("nuInvMass");
  else if ( type == nSVfit_namespace::kTau_pol               ) retVal.append("pol");
  else if ( type == nSVfit_namespace::kTauVM_theta_rho       ) retVal.append("theta_rho");
  else if ( type == nSVfit_namespace::kTauVM_mass2_rho       ) retVal.append("mass2_rho");
  else if ( type == nSVfit_namespace::kTauVM_theta_a1        ) retVal.append("theta_a1");
  else if ( type == nSVfit_namespace::kTauVM_theta_a1r       ) retVal.append("theta_a1r");
  else if ( type == nSVfit_namespace::kTauVM_phi_a1r         ) retVal.append("phi_a1r");
  else if ( type == nSVfit_namespace::kTauVM_mass2_a1        ) retVal.append("mass2_a1");
  else if ( type == nSVfit_namespace::kLep_shiftEn           ) retVal.append("shiftEn");
  else if ( type == nSVfit_namespace::kNu_energy_lab         ) retVal.append("energy_lab");
  else if ( type == nSVfit_namespace::kNu_phi_lab            ) retVal.append("phi_lab");
  else if ( type == nSVfit_namespace::kW_theta_lab           ) retVal.append("W_theta_lab");
  else if ( type == nSVfit_namespace::kW_phi_lab             ) retVal.append("W_phi_lab");
  else if ( type == nSVfit_namespace::kW_mass                ) retVal.append("W_mass");
  else retVal.append("undefined");
  return retVal;
}

//
//-------------------------------------------------------------------------------
//

void NSVfitParameter::initializeDefaultValues()
{
  using namespace SVfit_namespace;

  defaultInitialValues_.resize(nSVfit_namespace::kNu_phi_lab + 1);
  defaultInitialValues_[nSVfit_namespace::kPV_shiftX]             =  0.;
  defaultInitialValues_[nSVfit_namespace::kPV_shiftY]             =  0.;
  defaultInitialValues_[nSVfit_namespace::kPV_shiftZ]             =  0.;
  defaultInitialValues_[nSVfit_namespace::kTau_visEnFracX]        =  0.5;
  defaultInitialValues_[nSVfit_namespace::kTau_phi_lab]           =  0.;
  defaultInitialValues_[nSVfit_namespace::kTau_decayDistance_lab] =  0.01; // 100 microns
  defaultInitialValues_[nSVfit_namespace::kTau_nuInvMass]         =  0.8;  // GeV
  defaultInitialValues_[nSVfit_namespace::kTau_pol]               =  0.;
  defaultInitialValues_[nSVfit_namespace::kTauVM_theta_rho]       =  0.25*TMath::Pi();
  defaultInitialValues_[nSVfit_namespace::kTauVM_mass2_rho]       =  0.8;
  defaultInitialValues_[nSVfit_namespace::kTauVM_theta_a1]        =  0.25*TMath::Pi();
  defaultInitialValues_[nSVfit_namespace::kTauVM_theta_a1r]       =  0.25*TMath::Pi();
  defaultInitialValues_[nSVfit_namespace::kTauVM_phi_a1r]         =  0.;
  defaultInitialValues_[nSVfit_namespace::kTauVM_mass2_a1]        =  1.2; // GeV
  defaultInitialValues_[nSVfit_namespace::kLep_shiftEn]           =  0.;
  defaultInitialValues_[nSVfit_namespace::kNu_energy_lab]         =  0.;
  defaultInitialValues_[nSVfit_namespace::kNu_phi_lab]            =  0.;
  defaultInitialValues_[nSVfit_namespace::kW_theta_lab]           =  0.50*TMath::Pi();
  defaultInitialValues_[nSVfit_namespace::kW_phi_lab]             =  0.;
  defaultInitialValues_[nSVfit_namespace::kW_mass]                = 80.399; // GeV

  defaultLimits_.resize(nSVfit_namespace::kNu_phi_lab + 1);
  defaultLimits_[nSVfit_namespace::kPV_shiftX]                    = pdouble(         -0.1,         +0.1); // cm
  defaultLimits_[nSVfit_namespace::kPV_shiftY]                    = pdouble(         -0.1,         +0.1); // cm
  defaultLimits_[nSVfit_namespace::kPV_shiftZ]                    = pdouble(         -2.,          +2.);  // cm
  defaultLimits_[nSVfit_namespace::kTau_visEnFracX]               = pdouble(          0.,           1.);  // dimensionless
  defaultLimits_[nSVfit_namespace::kTau_phi_lab]                  = pdouble(-TMath::Pi(), +TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kTau_decayDistance_lab]        = pdouble(        -4.,            1.);  // sigmas
  defaultLimits_[nSVfit_namespace::kTau_nuInvMass]                = pdouble(          0.,           0.);  // depends on decay: mTau - mVis
  defaultLimits_[nSVfit_namespace::kTau_pol]                      = pdouble(         -1.,          +1.);  // -1: left-handed, +1: right-handed
  defaultLimits_[nSVfit_namespace::kTauVM_theta_rho]              = pdouble(          0.,  TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kTauVM_mass2_rho]              = pdouble(square(chargedPionMass + neutralPionMass), tauLeptonMass2); // GeV^2
  defaultLimits_[nSVfit_namespace::kTauVM_theta_a1]               = pdouble(          0.,  TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kTauVM_theta_a1r]              = pdouble(          0.,  TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kTauVM_phi_a1r]                = pdouble(-TMath::Pi(), +TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kTauVM_mass2_a1]               = pdouble(square(chargedPionMass + 2*neutralPionMass), tauLeptonMass2); // GeV^2
  defaultLimits_[nSVfit_namespace::kLep_shiftEn]                  = pdouble(          0.,          10.);  // relative to measured lepton energy
  defaultLimits_[nSVfit_namespace::kNu_energy_lab]                = pdouble(          0.,        1.e+3);  // GeV
  defaultLimits_[nSVfit_namespace::kNu_phi_lab]                   = pdouble(          0.,  TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kW_theta_lab]                  = pdouble(          0.,  TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kW_phi_lab]                    = pdouble(-TMath::Pi(), +TMath::Pi());  // rad
  defaultLimits_[nSVfit_namespace::kW_mass]                       = pdouble(80.399 - 3.*2.085, 80.399 + 3.*2.085); // GeV

  defaultStepSizes_.resize(nSVfit_namespace::kNu_phi_lab + 1);
  defaultStepSizes_[nSVfit_namespace::kPV_shiftX]                 =  0.01;
  defaultStepSizes_[nSVfit_namespace::kPV_shiftY]                 =  0.01;
  defaultStepSizes_[nSVfit_namespace::kPV_shiftZ]                 =  0.01;
  defaultStepSizes_[nSVfit_namespace::kTau_visEnFracX]            =  0.1;
  defaultStepSizes_[nSVfit_namespace::kTau_phi_lab]               =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTau_decayDistance_lab]     =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTau_nuInvMass]             =  0.1;
  defaultStepSizes_[nSVfit_namespace::kTau_pol]                   =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTauVM_theta_rho]           =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTauVM_mass2_rho]           =  0.1;
  defaultStepSizes_[nSVfit_namespace::kTauVM_theta_a1]            =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTauVM_theta_a1r]           =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTauVM_phi_a1r]             =  0.25;
  defaultStepSizes_[nSVfit_namespace::kTauVM_mass2_a1]            =  0.1;
  defaultStepSizes_[nSVfit_namespace::kLep_shiftEn]               =  0.01;
  defaultStepSizes_[nSVfit_namespace::kNu_energy_lab]             = 10.;
  defaultStepSizes_[nSVfit_namespace::kNu_phi_lab]                =  0.25;
  defaultStepSizes_[nSVfit_namespace::kW_theta_lab]               =  0.25;
  defaultStepSizes_[nSVfit_namespace::kW_phi_lab]                 =  0.25;
  defaultStepSizes_[nSVfit_namespace::kW_mass]                    =  1.;
 
  defaultValues_initialized_ = true;
}
