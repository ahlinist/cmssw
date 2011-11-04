#ifndef TauAnalysis_RecoTools_PATObjectLUTvalueExtractorFromKNN_h
#define TauAnalysis_RecoTools_PATObjectLUTvalueExtractorFromKNN_h

/** \class PATObjectLUTvalueExtractorFromKNN
 *
 * Extract efficiency/fake-rate values for pat::Electron, pat::Muon and pat::Tau objects
 *
 * NOTE: The values to be extracted need to be stored in a k-NearestNeighbour tree
 *      ( part of TMVA, see http://tmva.sourceforge.net/docu/TMVAUsersGuide.pdf )
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATObjectLUTvalueExtractorFromKNN.h,v 1.1 2011/10/21 16:24:11 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TauAnalysis/RecoTools/interface/PATObjectLUTvalueExtractorBase.h"

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"

#include <vector>
#include <string>

template<typename T>
class PATObjectLUTvalueExtractorFromKNN : public PATObjectLUTvalueExtractorBase<T>
{
 public:

  explicit PATObjectLUTvalueExtractorFromKNN(const edm::ParameterSet& cfg)
    : PATObjectLUTvalueExtractorBase<T>(cfg),
      kNN_(0)
  {
    edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
    if ( !inputFileName.isLocal() ) 
      throw cms::Exception("PATObjectLUTvalueExtractorFromTH1") 
	<< " Failed to find file = " << inputFileName << " !!\n";

    TMVA::Tools::Instance();
    
    kNN_ = new TMVA::Reader("!Color:!Silent");   

    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgParametrization = cfg.getParameter<vParameterSet>("parametrization");
    for ( vParameterSet::const_iterator cfgVar = cfgParametrization.begin();
	  cfgVar != cfgParametrization.end(); ++cfgVar ) {
      std::string varName = cfgVar->getParameter<std::string>("name");
      std::string varExpression = cfgVar->getParameter<std::string>("expression");
      
      stringFunctionEntryType* stringFunction = new stringFunctionEntryType(varName, varExpression);
      stringFunctions_.push_back(stringFunction);
      
      std::cout << "--> adding " << varName << " to k-NN tree." << std::endl;
      kNN_->AddVariable(varName.data(), &stringFunction->varValue_);
    }
    
    kNN_->BookMVA("kNN", inputFileName.fullPath().data());
  }

  ~PATObjectLUTvalueExtractorFromKNN() 
  {
    delete kNN_;

    for ( typename std::vector<stringFunctionEntryType*>::iterator it = stringFunctions_.begin();
	  it != stringFunctions_.end(); ++it ) {
      delete (*it);
    }
  }

 protected:

  double extract_value(const T& obj) const
  {
    for ( typename std::vector<stringFunctionEntryType*>::const_iterator stringFunction = stringFunctions_.begin();
	  stringFunction != stringFunctions_.end(); ++stringFunction ) {
      (*stringFunction)->varValue_ = (*(*stringFunction)->varExpression_)(obj);
    }

    return kNN_->EvaluateMVA("kNN");
  }

 private:

  TMVA::Reader* kNN_;

  struct stringFunctionEntryType
  {
    stringFunctionEntryType(const std::string& varName, const std::string& varExpression)
      : varName_(varName)
    {
      varExpression_ = new StringObjectFunction<T>(varExpression);
    }
    ~stringFunctionEntryType()
    {
      delete varExpression_;
    }
    std::string varName_;
    mutable Float_t varValue_;
    StringObjectFunction<T>* varExpression_;
  };
  std::vector<stringFunctionEntryType*> stringFunctions_;
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATObjectLUTvalueExtractorFromKNN<pat::Electron> PATElectronLUTvalueExtractorFromKNN;
typedef PATObjectLUTvalueExtractorFromKNN<pat::Muon> PATMuonLUTvalueExtractorFromKNN;
typedef PATObjectLUTvalueExtractorFromKNN<pat::Tau> PATTauLUTvalueExtractorFromKNN;

#endif

