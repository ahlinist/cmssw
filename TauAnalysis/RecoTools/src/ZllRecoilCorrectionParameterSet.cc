#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionParameterSet.h"

#include <fstream>

ZllRecoilCorrectionParameterSet::ZllRecoilCorrectionParameterSet(const edm::ParameterSet& cfg)
  : d_("d", cfg.getParameter<edm::ParameterSet>("d")),
    k1_("k1", cfg.getParameter<edm::ParameterSet>("k1")),
    k2_("k2", cfg.getParameter<edm::ParameterSet>("k2")),
    k3_("k3", cfg.getParameter<edm::ParameterSet>("k3")),
    sigma1_("sigma1", cfg.getParameter<edm::ParameterSet>("sigma1")),
    b1_("b1", cfg.getParameter<edm::ParameterSet>("b1")),
    c1_("c1", cfg.getParameter<edm::ParameterSet>("c1")),
    sigma2_("sigma2", cfg.getParameter<edm::ParameterSet>("sigma2")),
    b2_("b2", cfg.getParameter<edm::ParameterSet>("b2")),
    c2_("c2", cfg.getParameter<edm::ParameterSet>("c2"))
{}

ZllRecoilCorrectionParameterSet::ZllRecoilCorrectionParameterSet(
  double d, double dErr, double k1, double k1Err, double k2, double k2Err, double k3, double k3Err, 
  double sigma1, double sigma1Err, double b1, double b1Err, double c1, double c1Err, 
  double sigma2, double sigma2Err, double b2, double b2Err, double c2, double c2Err)
  : d_("d", d, dErr),
    k1_("k1", k1, k1Err),
    k2_("k2", k2, k2Err),
    k3_("k3", k3, k3Err),
    sigma1_("sigma1", sigma1, sigma1Err),
    b1_("b1", b1, b1Err),
    c1_("c1", c1, c1Err),
    sigma2_("sigma2", sigma2, sigma2Err),
    b2_("b2", b2, b2Err),
    c2_("c2", c2, c2Err)
{}

void ZllRecoilCorrectionParameterSet::print(std::ostream& stream) const
{
  stream << "<ZllRecoilCorrectionParameterSet::print>:" << std::endl;
  d_.print(stream);
  k1_.print(stream);
  k2_.print(stream);
  k3_.print(stream);
  sigma1_.print(stream);
  b1_.print(stream);
  c1_.print(stream);
  sigma2_.print(stream);
  b2_.print(stream);
  c2_.print(stream);
}

void ZllRecoilCorrectionParameterSet::writePythonConfig(const std::string& fileName) const
{
  std::ofstream* file = new std::ofstream(fileName.data());
  (*file) << "recoZllRecoilCorrectionParameters = cms.PSet(" << std::endl;
  d_.writePythonConfig(*file);
  k1_.writePythonConfig(*file);
  k2_.writePythonConfig(*file);
  k3_.writePythonConfig(*file);
  sigma1_.writePythonConfig(*file);
  b1_.writePythonConfig(*file);
  c1_.writePythonConfig(*file);
  sigma2_.writePythonConfig(*file);
  b2_.writePythonConfig(*file);
  c2_.writePythonConfig(*file);
  (*file) << ")" << std::endl;
  delete file;
}

