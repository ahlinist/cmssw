#ifndef ElectroWeakAnalysis_MultiBosons_Histogrammer_h
#define ElectroWeakAnalysis_MultiBosons_Histogrammer_h

/** \class MuonHistogrammer
 *  Base class for all FWLite Histogrammers
 *  Users just need to define the options for whatever they template to.
 *  \author Jan Veverka, Caltech
 */

#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "CommonTools/Utils/interface/ExpressionHisto.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"

#include "TH2F.h"
#include "TNtuple.h"
#include <string>
#include <algorithm>
#include <vector>

namespace tokenizer {
  // stolen shamelessly from the internet
  void tokenize(const std::string& str,
		std::vector<std::string>& tokens,
		const std::string& delimiters = " ")
  {
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
    
    while (std::string::npos != pos || std::string::npos != lastPos)
      {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
      }
  }
  
}

template <typename HistObject>
class Histogrammer {
 public:
  typedef void (*func_type)(const HistObject&);

  /// ctor from parameter set
  Histogrammer(const edm::ParameterSet &, fwlite::TFileService &);
  /// dtor
  virtual ~Histogrammer();
  /// process an event
  void analyze(const edm::EventBase &);
  /// process a base collection
  void analyze(const std::vector<HistObject> &);
  /// process a collection of shallow clone pointers to the base collection
  void analyze(const std::vector<reco::ShallowClonePtrCandidate> &);
  /// the event weight
  const double& weight() const { return weight_; }

 protected:
  
  // in the base class register functions which take one object of type HistObject
  // special histograms are only filled for selected objects!
  void registerFunction(const std::string& s, func_type fcn ) { funcNameToFunc_[s] = fcn; }  

  std::map<std::string, func_type > funcNameToFunc_; // filled using registerFunction in derived class
  std::map<std::string,TObject*> specHistos_; // histo name to histogram
  std::map<std::string,std::string> funcNameToHist_; // function name to histogram

 private:

  void initSpecialHistograms(TFileDirectory &,const edm::VParameterSet&);
  void fillSpecialHistograms(const HistObject &);

  /// label of the input collection
  edm::InputTag src_;
  const double weight_;
  /// vector of pointers to histograms
  std::vector<ExpressionHisto<HistObject>* > vhistograms_selected_;
  std::vector<ExpressionHisto<HistObject>* > vhistograms_all_;
};

template <typename HistObject>
Histogrammer<HistObject>::Histogrammer(const edm::ParameterSet & config,
					       fwlite::TFileService &fs
					       ) :
  src_(config.getParameter<edm::InputTag>("src") ),
  weight_(config.getParameter<double>("eventWeight"))
{
  //   string fileName         = config.getParameter<string>("outputFile");
  std::string dirName          = config.getParameter<std::string>("outputDirectory");
  edm::VParameterSet histosCfg = config.getParameter<edm::VParameterSet>("histograms");
    
  // TODO Check if the rhs can be omitted
  //   fwlite::TFileService fs;/* = fwlite::TFileService(fileName);*/
  
  std::vector<std::string> dirTokens;

  tokenizer::tokenize(dirName,dirTokens,"/");

  TFileDirectory lastdir = fs.mkdir(*(dirTokens.begin()));

  for(std::vector<std::string>::const_iterator i = dirTokens.begin()+1;
      i != dirTokens.end(); ++i)
    lastdir = lastdir.mkdir(*i);
  
  TFileDirectory allh = lastdir.mkdir("all");
  TFileDirectory selh = lastdir.mkdir("selected");
  
  for (edm::VParameterSet::const_iterator iCfg = histosCfg.begin();
       iCfg != histosCfg.end(); ++iCfg)
    {
      ExpressionHisto<HistObject> *hist_all = new ExpressionHisto<HistObject>(*iCfg);
      ExpressionHisto<HistObject> *hist_sel = new ExpressionHisto<HistObject>(*iCfg);
      hist_all->initialize(allh);
      hist_sel->initialize(selh);
      vhistograms_all_.push_back(hist_all);
      vhistograms_selected_.push_back(hist_sel);
    }
  
  if(config.existsAs<edm::VParameterSet>("specializedHistograms"))
    initSpecialHistograms(selh, config.getParameter<edm::VParameterSet>("specializedHistograms"));
}

template<typename HistObject>
Histogrammer<HistObject>::~Histogrammer()
{
  
  // delete all histograms and clear the vector of pointers
  typename std::vector< ExpressionHisto<HistObject>* >::const_iterator hist;
  for (hist = vhistograms_all_.begin(); hist != vhistograms_all_.end(); ++hist)
    (*hist)->~ExpressionHisto<HistObject>();
  for (hist = vhistograms_selected_.begin(); hist != vhistograms_selected_.end(); ++hist)
    (*hist)->~ExpressionHisto<HistObject>();

  vhistograms_all_.clear();
  vhistograms_selected_.clear();
}

template<typename HistObject>
void 
Histogrammer<HistObject>::initSpecialHistograms(TFileDirectory & dir,const edm::VParameterSet& histoConf) 
{
  for (edm::VParameterSet::const_iterator iCfg = histoConf.begin();
       iCfg != histoConf.end(); ++iCfg)
    {
      std::string type = iCfg->getUntrackedParameter<std::string>("type");
      
      if(type == "TNtuple") {
	
	std::string name = iCfg->getUntrackedParameter<std::string>("name");
	std::string desc = iCfg->getUntrackedParameter<std::string>("description");
	std::string vars = iCfg->getUntrackedParameter<std::string>("vars");
	std::string func = iCfg->getUntrackedParameter<std::string>("associatedFunction");

	funcNameToHist_[func] = name;
	specHistos_[name] = dir.make<TNtuple>(name.c_str(),
					      desc.c_str(),
					      vars.c_str());

      } else if (type == "TH1F") {

      std::string name = iCfg->getUntrackedParameter<std::string>("name");
      std::string desc = iCfg->getUntrackedParameter<std::string>("desc");
      std::string func = iCfg->getUntrackedParameter<std::string>("associatedFunction");
      double min = iCfg->getUntrackedParameter<double>("min");
      double max = iCfg->getUntrackedParameter<double>("max");      
      int nbins = iCfg->getUntrackedParameter<int>("nbins");

      funcNameToHist_[func] = name;
      specHistos_[name] = dir.make<TH1F>(name.c_str(),
					 desc.c_str(),
					 nbins,min,max);

      } else if (type == "TH2F") {

	std::string name = iCfg->getUntrackedParameter<std::string>("name");
	std::string desc = iCfg->getUntrackedParameter<std::string>("desc");
	std::string func = iCfg->getUntrackedParameter<std::string>("associatedFunction");
	double xmin = iCfg->getUntrackedParameter<double>("xmin");
	double xmax = iCfg->getUntrackedParameter<double>("xmax");      
	int xnbins = iCfg->getUntrackedParameter<int>("xnbins");
	double ymin = iCfg->getUntrackedParameter<double>("ymin");
	double ymax = iCfg->getUntrackedParameter<double>("ymax");      
	int ynbins = iCfg->getUntrackedParameter<int>("ynbins");
	
	funcNameToHist_[func] = name;
	specHistos_[name] = dir.make<TH2F>(name.c_str(),
					   desc.c_str(),
					   xnbins,xmin,xmax,
					   ynbins,ymin,ymax);
	
      } else {
	throw cms::Exception("InvalidInput") << "type : \'" << type << "\' is not allowed!" << std::endl;
      }
    }
}

template<typename HistObject>
void
Histogrammer<HistObject>::analyze(const edm::EventBase &iEvent)
{
  edm::Handle<std::vector<HistObject> > collection;
  iEvent.getByLabel(src_, collection);

  analyze(*collection);
}

template<typename HistObject>
void
Histogrammer<HistObject>::analyze(const std::vector<HistObject> &collection)
{
  // loop over histograms
  typename std::vector<ExpressionHisto<HistObject>*>::const_iterator hist;
  for (hist = vhistograms_all_.begin(); hist != vhistograms_all_.end(); ++hist)
  {
    // loop over collection
    typename std::vector<HistObject>::const_iterator element, begin = collection.begin();
    for (element = begin; element != collection.end(); ++element) {
      if ( !(*hist)->fill(*element, weight(), element-begin) ) break;
      
    } // loop over collection
  } // loop over histograms
}

template<typename HistObject>
void
Histogrammer<HistObject>::analyze(const std::vector<reco::ShallowClonePtrCandidate> &collection)
{
  // loop over histograms
  typename std::vector<ExpressionHisto<HistObject>*>::const_iterator hist;
  for (hist = vhistograms_selected_.begin(); hist != vhistograms_selected_.end(); ++hist)
  {
    // loop over collection
    std::vector<reco::ShallowClonePtrCandidate>::const_iterator it, begin;
    it = begin = collection.begin();
    for (; it != collection.end(); ++it) {
      // convert the iterator to a pointer to its master
      reco::CandidatePtr ptr = it->masterClonePtr();
      const HistObject * element = dynamic_cast<const HistObject*>( ptr.get() );

      if ( !(*hist)->fill(*element, weight(), it-begin) ) break;
      fillSpecialHistograms(*element);
    } // loop over collection
  } // loop over histograms
}

template<typename HistObject>
void 
Histogrammer<HistObject>::fillSpecialHistograms(const HistObject & obj) 
{
  for(typename std::map<std::string,func_type>::const_iterator i = funcNameToFunc_.begin();
      i != funcNameToFunc_.end();
      ++i) 
    i->second(obj);
}


#endif
