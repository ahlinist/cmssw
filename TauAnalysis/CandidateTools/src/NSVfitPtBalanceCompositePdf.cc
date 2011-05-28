#include "TauAnalysis/CandidateTools/interface/NSVfitPtBalanceCompositePdf.h"

#include <boost/foreach.hpp>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooArgSet.h"
#include "RooWorkspace.h"
#include "RooBinning.h"
#include "RooRealVar.h"

#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TVectorD.h"
#include "TMath.h"

NSVfitPtBalanceCompositePdf::~NSVfitPtBalanceCompositePdf() { }

NSVfitPtBalanceCompositePdf::NSVfitPtBalanceCompositePdf(
    const edm::ParameterSet& pset) {

  phiBinning_ = NULL;

  verbosity_ = pset.exists("verbosity") ? pset.getParameter<int>("verbosity") : 0;

  interpolate_ = pset.exists("interpolate") ?
    pset.getParameter<bool>("interpolate") : false;

  if (verbosity_)
    std::cout << "<NSVfitPtBalanceCompositePdf::NSVfitPtBalanceCompositePdf>"
      << std::endl;

  std::string xVarDefaultName =
    pset.getParameter<std::string>("xVarDefaultName");
  std::string massVarDefaultName =
    pset.getParameter<std::string>("massVarDefaultName");
  std::string modelDefaultName =
    pset.getParameter<std::string>("modelDefaultName");
  std::string workspaceDefaultName =
    pset.getParameter<std::string>("workspaceDefaultName");

  // Get all of the phi slices
  typedef std::vector<edm::ParameterSet> VPSet;
  VPSet phiSlices = pset.getParameter<VPSet>("phiSlices");

  pdfs_.resize(phiSlices.size());

  if (verbosity_)
    std::cout << "<NSVfitPtBalanceCompositePdf> Adding " << phiSlices.size()
      << " phi slices" << std::endl;

  BOOST_FOREACH(const edm::ParameterSet& slicePSet, phiSlices) {
    if (verbosity_)
      std::cout << "<NSVfitPtBalanceCompositePdf> loading pset: " << slicePSet
      << std::endl;

    // Allow overrides
    std::string xVarName = slicePSet.exists("xVarName") ?
      slicePSet.getParameter<std::string>("xVarName") : xVarDefaultName;
    std::string massVarName = slicePSet.exists("massVarName") ?
      slicePSet.getParameter<std::string>("massVarName") : massVarDefaultName;
    std::string modelName = slicePSet.exists("modelName") ?
      slicePSet.getParameter<std::string>("modelName") : modelDefaultName;
    std::string workspaceName = slicePSet.exists("workspaceName") ?
      slicePSet.getParameter<std::string>("workspaceName") : workspaceDefaultName;

    edm::FileInPath pdfFilePath = slicePSet.getParameter<edm::FileInPath>("pdfFile");

    if (!pdfFilePath.isLocal()) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Failed to find file = " << pdfFilePath.fullPath() << " !!\n";
    }

    TFile* file = TFile::Open(pdfFilePath.fullPath().data(), "READ");
    RooWorkspace* ws = dynamic_cast<RooWorkspace*>(
        file->Get(workspaceName.c_str()));
    if (!ws) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Couldn't load workspace " << workspaceName << " from "
        << pdfFilePath.fullPath().data() << std::endl;
    }

    RooAbsPdf* pdf = ws->pdf(modelName.c_str());
    if (!pdf) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Couldn't load pdf " << modelName << " from "
        << pdfFilePath.fullPath().data() << std::endl;
    }


    RooRealVar* xVar = ws->var(xVarName.c_str());
    if (!xVar) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Couldn't load dependent var " << xVarName << " from "
        << pdfFilePath.fullPath().data() << std::endl;
    }

    RooRealVar* massVar = ws->var(massVarName.c_str());
    if (!massVar) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Couldn't load mass var " << massVarName << " from "
        << pdfFilePath.fullPath().data() << std::endl;
    }

    // Get binning information
    RooBinning* binning = dynamic_cast<RooBinning*>(ws->obj("phiBinning"));
    if (!binning) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Couldn't load binning info [phiBinning] (a RooBinning) from "
        << pdfFilePath.fullPath().data() << std::endl;
    }

    // If this is the first binning, use it.
    if (!phiBinning_)
      phiBinning_ = binning;

    TVectorD* phiInfo = dynamic_cast<TVectorD*>(ws->obj("phiInfo"));
    if (!phiInfo) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " Couldn't load binning info [phiInfo] (a TVectorD) from "
        << pdfFilePath.fullPath().data() << std::endl;
    }

    int phiBinIndex = TMath::Nint( (*phiInfo)[2] );
    // sanity check, should always match.
    double phiLow = (*phiInfo)[0];
    double phiHigh = (*phiInfo)[1];

    double expectedLow = phiBinning_->binLow(phiBinIndex);
    double expectedHigh = phiBinning_->binHigh(phiBinIndex);

    double tolerance = 1e-7;
    if (TMath::Abs(phiLow - expectedLow) > tolerance ||
        TMath::Abs(phiHigh - expectedHigh) > tolerance) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " The phi binning defined in "
        << pdfFilePath.fullPath().data()
        << " does not match the binning used in a previous file!"
        << std::endl;
    }

    if (phiBinIndex < 0 || phiBinIndex >= (int)pdfs_.size()) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " The pdf in "
        << pdfFilePath.fullPath().data()
        << " reports it's phi bin as " << phiBinIndex
        << ", but there are only " << pdfs_.size() << " slices"
        << " defined in the PSet, there is a missing bin or misconfiguration!"
        << std::endl;
    }

    if (pdfs_[phiBinIndex].isValid()) {
      throw cms::Exception("NSVfitPtBalanceCompositePdf")
        << " The pdf in "
        << pdfFilePath.fullPath().data()
        << " reports it's phi bin as " << phiBinIndex
        << ", but there is already a PDF for that phi bin! "
        << " There is a missing bin or misconfiguration!"
        << std::endl;
    }

    /*
    PdfInfo myPdf;
    myPdf.pdf_ = pdf;
    // Make sure there are no weird limits
    xVar->setMax(100);
    myPdf.xVar_ = xVar;
    myPdf.mass_ = massVar;
    // We need to specify which variable should be integrated to get the
    // normalization
    myPdf.normalizationVariable_.reset(new RooArgSet(*xVar, "toIntegrate"));
    myPdf.print(std::cout);
    */

    NSVfitCachingPdfWrapper myPdf(
        pdf, xVar, massVar, 400, 0, 4, 300, 0, 600);
    pdfs_[phiBinIndex] = myPdf;

    delete file;
  }
  /*
  if (verbosity_) {
    std::cout << "<NSVfitPtBalanceCompositePdf::ListPdfs>"
      << std::endl;
    for (size_t iPdf = 0; iPdf < pdfs_.size(); ++iPdf) {
      std::cout << " PDF " << iPdf << ") " << std:: endl;
      pdfs_[iPdf].print(std::cout);
    }
  }
  */
}

double NSVfitPtBalanceCompositePdf::operator()(
    double pt, double mass, double deltaPhi) const {

  int maxBin = pdfs_.size()-1;

  int containingBin = phiBinning_->binNumber(deltaPhi);
  if (containingBin < 0)
    containingBin = 0;
  if (containingBin > maxBin)
    containingBin = maxBin;

  if (!interpolate_) {
    return pdfs_[containingBin].getVal(pt, mass);
  }

  double centerOfBin = phiBinning_->binCenter(containingBin);
  int otherBin  = (deltaPhi > centerOfBin) ? containingBin + 1 : containingBin - 1;

  if (otherBin < 0)
    otherBin = 0;
  if (otherBin > maxBin)
    otherBin = maxBin;

  if (verbosity_ > 1) {
    std::cout << "<NSVfitPtBalanceCompositePdf::operator()>"
      << " pt: " << pt << " mass: " << mass
      << " deltaPhi: " << deltaPhi << std::endl;
    std::cout << "containing bin: " << containingBin
      << " other bin: " << otherBin << std::endl;
  }

  // Check if can't interpolate, because we are on the edges, or super close.
  double precision = 1e-6;
  if (otherBin == containingBin ||
      TMath::Abs(deltaPhi - centerOfBin) < precision) {
    return pdfs_[containingBin].getVal(pt, mass);
  } else {
    double centerOfOtherBin = phiBinning_->binCenter(otherBin);

    double contributionContainingBin = TMath::Abs(
        1.0/(deltaPhi - centerOfBin));

    double contributionOtherBin = TMath::Abs(
        1.0/(deltaPhi - centerOfOtherBin));

    double total = contributionOtherBin + contributionContainingBin;

    double pdf1 = (contributionContainingBin/total)*pdfs_[containingBin].getVal(pt, mass);
    double pdf2 = (contributionOtherBin/total)*pdfs_[otherBin].getVal(pt, mass);

    return pdf1 + pdf2;
  }
}
