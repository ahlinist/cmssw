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

std::ostream& operator<<(std::ostream& s, const JGraph& q) {
	assert(q.xvals_.size() == q.yvals_.size());

	if (q.hasErrors_) {
		s << "## JGraph with errors and " << q.xvals_.size() << " entries\n";
		s << "## x\tex\ty\tey\n";
		for (unsigned u(0); u < q.xvals_.size(); ++u) {
			s << q.xvals_[u] << "\t" << q.exvals_[u] << "\t" << q.yvals_[u] << "\t"
					<< q.eyvals_[u] << "\n";
		}
	} else {
		s << "## JGraph with " << q.xvals_.size() << " entries\n";
		s << "## x\ty\n";
		for (unsigned u(0); u < q.xvals_.size(); ++u) {
			s << q.xvals_[u] << "\t" << q.yvals_[u] << "\n";
		}
	}
	return s;
}



