#include "RecoParticleFlow/PFAnalyses/interface/JGraph.h"
#include "TVector.h"
#include <TGraphErrors.h>
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

JGraph::JGraph(std::string rootName, bool suppressXErrors, bool suppressYErrors) :
	hasErrors_(false), rootName_(rootName), suppressXErrors_(suppressXErrors), suppressYErrors_(suppressYErrors) {
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

	TGraphErrors eGraph(xvals_.size(), vecToArr(xvals_), vecToArr(yvals_), vecToArr(exvals_), vecToArr(eyvals_));
	eGraph.SetName(rootName_.c_str());
	return eGraph;

}

bool JGraph::streamToFile(const std::string& filename, bool recreate) {

	//If recreate = false, check the file doesn't already exist first
	if (!recreate) {
		fstream fin;
		fin.open(filename.c_str(), std::ios::in);
		if (fin.is_open()) {
			cout << "File already exists and recreate flag = false.\n";
			fin.close();
			return false;
		}
		fin.close();
	}

	ofstream file(filename.c_str());
	if (file.is_open()) {
		for (unsigned u(0); u < xvals_.size(); ++u) {
					file << setw(6) << setprecision(5) << xvals_[u];
					suppressXErrors_ ? file << "\t" : file << exvals_[u] << "\t";
					file << yvals_[u];
					suppressYErrors_ ? file << "\n" : file << "\t" << eyvals_[u] << "\n";
		}
		file.close();
		return true;
	} else
		cout << "Unable to open file\n";
	return false;
}

std::ostream& operator<<(std::ostream& s, const JGraph& q) {
	assert(q.xvals_.size() == q.yvals_.size());

	if (q.hasErrors_) {
		s << "## JGraph with errors and " << q.xvals_.size() << " entries\n";
		s << "## x";
		q.suppressXErrors_ ? s << "\ty" : s << "\tex\ty";
		q.suppressYErrors_ ? s << "\n" : s << "\tey\n";
		for (unsigned u(0); u < q.xvals_.size(); ++u) {
			s << setw(6) << setprecision(5) << q.xvals_[u];
			q.suppressXErrors_ ? s << "\t" : s << q.exvals_[u] << "\t";
			s << q.yvals_[u];
			q.suppressYErrors_ ? s << "\n" : s << "\t" << q.eyvals_[u] << "\n";
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

