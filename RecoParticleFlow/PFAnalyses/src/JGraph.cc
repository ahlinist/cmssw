#include "RecoParticleFlow/PFAnalyses/interface/JGraph.h"
#include "TVector.h"
#include <TGraphErrors.h>
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include <cassert>

using namespace std;

JGraph::JGraph(std::string rootName) :
	hasErrors_(false), rootName_(rootName) {
}

JGraph::~JGraph() {
}

void JGraph::addPoint(double x, double y) {
	xvals_.push_back(x);
	exvals_.push_back(0);
	yvals_.push_back(y);
	eyvals_.push_back(0);
}

void JGraph::addPoint(double x, double ex, double y, double ey) {
	hasErrors_ = true;
	xvals_.push_back(x);
	exvals_.push_back(ex);
	yvals_.push_back(y);
	eyvals_.push_back(ey);

}

TGraph JGraph::finalise() {

	TGraph graph(xvals_.size(), vecToArr(xvals_), vecToArr(yvals_));
	graph.SetName(rootName_.c_str());
	return graph;
}

TGraphErrors JGraph::finaliseWithErrors() {

	TGraphErrors eGraph(xvals_.size(), vecToArr(xvals_), vecToArr(yvals_),
			vecToArr(exvals_), vecToArr(eyvals_));
	eGraph.SetName(rootName_.c_str());
	return eGraph;

}

std::ostream& JGraph::operator<<(std::ostream& s) {
	assert(xvals_.size() == yvals_.size());

	if (xvals_.size() == exvals_.size()) {
		s << "## JGraph with errors and " << xvals_.size() << " entries\n";
		s << "## x\tex\ty\tey\n";
		for (unsigned u(0); u < xvals_.size(); ++u) {
			s << xvals_[u] << "\t" << exvals_[u] << "\t" << yvals_[u] << "\t"
					<< eyvals_[u] << "\n";
		}
	} else {
		s << "## JGraph with " << xvals_.size() << " entries\n";
		s << "## x\ty\n";
		for (unsigned u(0); u < xvals_.size(); ++u) {
			s << xvals_[u] << "\t" << yvals_[u] << "\n";
		}
	}
	return s;
}

