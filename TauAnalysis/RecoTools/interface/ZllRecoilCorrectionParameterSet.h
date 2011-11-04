#ifndef TauAnalysis_RecoTools_ZllRecoilCorrectionParameterSet_h
#define TauAnalysis_RecoTools_ZllRecoilCorrectionParameterSet_h

/** \class ZllRecoilCorrectionParameterSet
 *
 * Auxiliary class to store parameters of fitting hadronic recoil 
 * as function of Z transverse momentum in Z --> l+ l- events
 * (cf. CMS AN-10-264 for a description of the method)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ZllRecoilCorrectionParameterSet.h,v 1.1 2011/08/15 17:10:32 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include <string>
#include <iostream>

class ZllRecoilCorrectionParameterSet
{
 public:

  ZllRecoilCorrectionParameterSet(const edm::ParameterSet&);
  ZllRecoilCorrectionParameterSet(double, double, double, double, double, double, double, double, double, double,
				  double, double, double, double, double, double, double, double, double, double);
  ~ZllRecoilCorrectionParameterSet() {}

  double d()         const { return d_.value_;            }
  double dErr()      const { return d_.uncertainty_;      }
  double k1()        const { return k1_.value_;           }
  double k1Err()     const { return k1_.uncertainty_;     }
  double k2()        const { return k2_.value_;           }
  double k2Err()     const { return k2_.uncertainty_;     }
  double k3()        const { return k3_.value_;           }
  double k3Err()     const { return k3_.uncertainty_;     }
  double sigma1()    const { return sigma1_.value_;       }
  double sigma1Err() const { return sigma1_.uncertainty_; }
  double b1()        const { return b1_.value_;           }
  double b1Err()     const { return b1_.uncertainty_;     }
  double c1()        const { return c1_.value_;           }
  double c1Err()     const { return c1_.uncertainty_;     }
  double sigma2()    const { return sigma2_.value_;       }
  double sigma2Err() const { return sigma2_.uncertainty_; }
  double b2()        const { return b2_.value_;           }
  double b2Err()     const { return b2_.uncertainty_;     }
  double c2()        const { return c2_.value_;           }
  double c2Err()     const { return c2_.uncertainty_;     }

  void print(std::ostream&) const;

  void writePythonConfig(const std::string&) const;

 private:

  class ZllRecoilCorrectionParameter
  {
   public: 

    ZllRecoilCorrectionParameter(const std::string& name, const edm::ParameterSet& cfg)
      : name_(name),
        value_(cfg.getParameter<double>("value")),
        uncertainty_(cfg.getParameter<double>("uncertainty"))
    {}
    ZllRecoilCorrectionParameter(const std::string& name, double value, double uncertainty)
      : name_(name),
        value_(value),
        uncertainty_(uncertainty)
    {}
    ~ZllRecoilCorrectionParameter() {}

    void print(std::ostream& stream) const
    {
      stream << " " << name_ << ": " << value_ << " +/- " << uncertainty_ << std::endl;
    }

    void writePythonConfig(std::ostream& stream) const
    {
      stream << "    " << name_ << " = cms.PSet(" << std::endl;
      stream << "        value = cms.double(" << value_ << ")," << std::endl;
      stream << "        uncertainty = cms.double(" << uncertainty_ << ")" << std::endl;
      stream << "    )," << std::endl;
    }

    std::string name_;

    double value_;
    double uncertainty_;
  };

  ZllRecoilCorrectionParameter d_;
  ZllRecoilCorrectionParameter k1_;
  ZllRecoilCorrectionParameter k2_;
  ZllRecoilCorrectionParameter k3_;
  ZllRecoilCorrectionParameter sigma1_;
  ZllRecoilCorrectionParameter b1_;
  ZllRecoilCorrectionParameter c1_;
  ZllRecoilCorrectionParameter sigma2_;
  ZllRecoilCorrectionParameter b2_;
  ZllRecoilCorrectionParameter c2_;
};

#endif


