#ifndef PLOTSPECIAL_H_
#define PLOTSPECIAL_H_

class PlotSpecial {
public:
	PlotSpecial(TObject* o, TStyle* s, std::string drawOptions,
			std::string preferredName) :
		obj_(o), style_(s), options_(drawOptions),
				preferredName_(preferredName) {
	}
	virtual ~PlotSpecial() {};
	TObject* obj_;
	TStyle* style_;
	std::string options_;
	std::string preferredName_;
};

#endif /*PLOTSPECIAL_H_*/
