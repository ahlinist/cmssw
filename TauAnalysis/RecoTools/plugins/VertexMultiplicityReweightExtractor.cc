#include "TauAnalysis/RecoTools/plugins/VertexMultiplicityReweightExtractor.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <vector>

//-------------------------------------------------------------------------------
// CV: code to computed genPileUp reweights
//     copied & pasted from SimGeneral/MixingModule/python/mix_E7TeV_FlatDist10_2011EarlyData_inTimeOnly_cfi.py
//     cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupReweighting
std::vector<double> generate_flat10_weights(TH1D* data_npu_estimated)
{
  const double npu_probs[25] = {
    0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584,
    0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584 /* <-- 10*/,
    0.0630151648, 0.0526654164, 0.0402754482, 0.0292988928, 0.0194384503, 0.0122016783, 0.007207042, 0.004003637, 0.0020278322,
    0.0010739954, 0.0004595759, 0.0002229748, 0.0001028162, 4.58337152809607E-05 /* <-- 24 */
  };

  std::vector<double> result(25);
  double s = 0.0;
  for ( int npu = 0; npu < 25; ++npu ) {
    double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));                              
    result[npu] = npu_estimated / npu_probs[npu];
    s += npu_estimated;
  }

  // normalize weights such that the total sum of weights over thw whole sample is 1.0, 
  // i.e. sum_i  result[i] * npu_probs[i] should be 1.0 (!)
  for ( int npu = 0; npu < 25; ++npu ) {
    result[npu] /= s;
  }
  
  return result;
}
//-------------------------------------------------------------------------------

VertexMultiplicityReweightExtractor::VertexMultiplicityReweightExtractor(const edm::ParameterSet& cfg)
  : src_(cfg.getParameter<edm::InputTag>("src")),
    inputFile_(0),
    lut_(0),
    type_(kUndefined)
{
  edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
  std::string lutName = cfg.getParameter<std::string>("lutName");
  if ( !inputFileName.isLocal()) throw cms::Exception("VertexMultiplicityReweightExtractor") 
    << " Failed to find File = " << inputFileName << " !!\n";

  std::string type_string = cfg.getParameter<std::string>("type");
  if      ( type_string == "gen" ) type_ = kGenLevel;
  else if ( type_string == "rec" ) type_ = kRecLevel;
  else throw cms::Exception("VertexMultiplicityReweightExtractor") 
    << " Invalid Configuration Parameter 'type' = " << type_string << " !!\n";

  if ( type_ == kGenLevel ) {
    inputFile_ = new TFile(inputFileName.fullPath().data());
    TH1D* data_npu_estimated = dynamic_cast<TH1D*>(inputFile_->Get(lutName.data()));
    if ( !data_npu_estimated ) throw cms::Exception("VertexMultiplicityReweightExtractor") 
      << " Failed to load LUT = " << lutName.data() << " from file = " << inputFileName.fullPath().data() << " !!\n";
    std::vector<double> flat10_weights = generate_flat10_weights(data_npu_estimated);
    //std::cout << "flat10_weights = " << format_vdouble(flat10_weights) << std::endl;
    lut_ = dynamic_cast<TH1*>(data_npu_estimated->Clone(TString(lutName.data()).Append("_flat10_weights").Data()));
    lut_->Reset();
    size_t maxGenPileUp = flat10_weights.size();
    for ( size_t iGenPileUp = 0; iGenPileUp < maxGenPileUp; ++iGenPileUp ) {
      int binIndex = lut_->FindBin(iGenPileUp);
      lut_->SetBinContent(binIndex, flat10_weights[iGenPileUp]);
    }
  } else {
    inputFile_ = new TFile(inputFileName.fullPath().data());
    lut_ = dynamic_cast<TH1*>(inputFile_->Get(lutName.data()));
    if ( !lut_ ) throw cms::Exception("VertexMultiplicityReweightExtractor") 
      << " Failed to load LUT = " << lutName.data() << " from file = " << inputFileName.fullPath().data() << " !!\n";
  }
}

VertexMultiplicityReweightExtractor::~VertexMultiplicityReweightExtractor() 
{
  delete inputFile_;
}

int getBin(TAxis* axis, double x)
{
  int bin = axis->FindFixBin(x);
  int nBins = axis->GetNbins();
  if ( bin < 1     ) bin = 1;
  if ( bin > nBins ) bin = nBins;
  return bin;
}

double VertexMultiplicityReweightExtractor::operator()(const edm::Event& evt) const
{
  //std::cout << "<VertexMultiplicityReweightExtractor::operator()>:" << std::endl;

  double numPileUp = -1.;
  if ( type_ == kGenLevel ) {
    typedef std::vector<PileupSummaryInfo> PileupSummaryInfoCollection;
    edm::Handle<PileupSummaryInfoCollection> genPileUpInfos;
    evt.getByLabel(src_, genPileUpInfos);
    for ( PileupSummaryInfoCollection::const_iterator genPileUpInfo = genPileUpInfos->begin();
	  genPileUpInfo != genPileUpInfos->end(); ++genPileUpInfo ) {
      // CV: in-time PU is stored in getBunchCrossing = 0, 
      //    cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupInformation
      if ( genPileUpInfo->getBunchCrossing() == 0 ) numPileUp = genPileUpInfo->getPU_NumInteractions();
    }
    if ( numPileUp == -1. ) throw cms::Exception("VertexMultiplicityReweightExtractor") 
      << " Failed to find PileupSummaryInfo object for in-time Pile-up !!\n";
  } else if ( type_ == kRecLevel ) {
    edm::Handle<reco::VertexCollection> vertices;
    evt.getByLabel(src_, vertices);
    numPileUp = vertices->size();
  } else assert(0);
  
  int bin = getBin(lut_->GetXaxis(), numPileUp);
  double weight = lut_->GetBinContent(bin);

  //std::cout << " numPileUp = " << numPileUp << ": weight = " << weight << std::endl;

  return weight;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, VertexMultiplicityReweightExtractor, "VertexMultiplicityReweightExtractor");

