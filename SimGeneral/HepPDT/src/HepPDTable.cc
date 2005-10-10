// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2005/10/10 10:22:02 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepPDTable.

#include "SimGeneral/HepPDT/interface/HepPDTable.h"
#include "SimGeneral/HepPDT/interface/HepDecayMode.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include <fstream>
#include <iostream>
#include "SimGeneral/HepPDT/interface/StandardMatchers.h"
#include "SimGeneral/HepPDT/interface/HepCombinedMatcher.h"
#include "SimGeneral/HepPDT/interface/HepNotMatcher.h"
#include "SimGeneral/HepPDT/interface/HepJetsetDummyHandler.h"
#include "Utilities/General/interface/ConfigurationDictionary.h"
#include "Utilities/General/interface/FileInPath.h"
#include "Utilities/General/interface/pidInfo.h"

#ifdef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepPDTable.icc"
#endif

HepDecayMode * HepPDTable::constructDecayMode(HepString & tag) {
  HepInt level = 0;
  HepString::size_type end = 0;
  while ( end < tag.size() && ( tag[end] != ']' || level ) ) {
    switch ( tag[end++] ) {
    case '[':
      ++level;
      break;
    case ']':
      --level;
      break;
    }
  }
  ModeMap::iterator mit = allModes.find(tag.substr(0,end));
  if ( mit != allModes.end() ) {
    tag = tag.substr(end);
    return (*mit).second;
  }
  HepString::size_type next = tag.find(':');
  HepParticleData * pd = getParticleData(tag.substr(0,next));
  if ( !pd ) return 0;
  HepDecayMode * dm = new HepDecayMode;
  dm->parent(*pd);
  HepBoolean error = false;
  tag = tag.substr(next+1);
  do {
    switch ( tag[0] ) {
    case '[':
      {
	tag = tag.substr(1);
	HepDecayMode * cdm = constructDecayMode(tag);
	if ( cdm ) dm->addCascadeProduct(*cdm);
	else error = true;
      } break;
    case ']':
    case ',':
      tag = tag.substr(1);
      break;
    case '?':
      {
	next = std::min(tag.find(','), tag.find(';'));
	HepParticleMatcher * pm = getParticleMatcher(tag.substr(1,next-1));
	if ( pm ) dm->addProductMatcher(*pm);
	else error = true;
	tag = tag.substr(next);
      } break;
    case '!':
      {
	next = std::min(tag.find(','), tag.find(';'));
	HepParticleData * pd = getParticleData(tag.substr(1,next-1));
	if ( pd ) dm->addExcluded(*pd);
	else error = true;
	tag = tag.substr(next);
      } break;
    case '*':
      {
	next = std::min(tag.find(','), tag.find(';'));
	HepParticleMatcher * pm = getParticleMatcher(tag.substr(1,next-1));
	if ( pm ) dm->setWildMatcher(*pm);
	else error = true;
	tag = tag.substr(next);
      } break;
    default:
      {
	next = std::min(tag.find(','), tag.find(';'));
	HepParticleData * pdp = getParticleData(tag.substr(0,next));
	if ( pdp ) dm->addProduct(*pdp);
	else error = true;
	tag = tag.substr(next);
      } break;
    }
  } while ( tag[0] != ';' && tag.size() );
  if ( tag[0] != ';' || error ) {
    delete dm;
    return 0;
  }

  tag = tag.substr(1);  
  dm->tag(HepPDT::makeDecayTag(*dm));
  ModeMap::iterator mdit = allModes.find(dm->tag());
  if ( mdit != allModes.end() ) {
    delete dm;
    dm = (*mdit).second;
  } else
    allModes[dm->tag()] = dm;
  pd->addDecayMode(*dm);
  return dm;
}

HepPDTable::HepPDTable() {
  add(*(new HepAnyParticleMatcher));
  add(*(new HepChargedParticleMatcher));
  add(*(new HepPositiveParticleMatcher));
  add(*(new HepNegativeParticleMatcher));
  add(*(new HepNeutralParticleMatcher));
  add(*(new HepBaryonMatcher));
  add(*(new HepMesonMatcher));
  add(*(new HepLeptonMatcher));
  add(*(new HepNotMatcher<HepLeptonMatcher>("NotLepton")));
  add(*(new HepCombinedMatcher<HepMesonMatcher,logical_and<HepBoolean>,
	HepChargedParticleMatcher>("ChargedMeson")));
  add(*(new HepCombinedMatcher<HepMesonMatcher,logical_or<HepBoolean>,
	HepBaryonMatcher>("Hadron")));
  //  add(*(new HepMeVMassRangeMatcher<4500,5500>("MassRange4500-5500")));
  //  temporarily removed because of g++ compiler problems
  add(*(new HepAnyLightQuarkMatcher));
  add(*(new HepAnyLightAntiQuarkMatcher));

  add(*(new HepJetsetDummyHandler));
  std::string path(getenv("COBRA_DATA_PATH"));
  FileInPath f1(path,"GeneratorData/HepPDT/particle_table");
  if ( f1() == 0) {
    std::cout << "File GeneratorData/HepPDT/particle_table not found in " << path << std::endl;
    throw Genexception(" HepPDT particle list not found.");
  } else {
    std::cout << "Reading " << f1.name() << std::endl;
  }
  std::ifstream & ifile = *f1();
  
  this->read(ifile);
}

HepPDTable::~HepPDTable() {
  ParticleMap::iterator pit = allParticles.begin();
  while ( pit != allParticles.end() ) delete (*pit++).second;
  MatcherMap::iterator mit = allMatchers.begin();
  while ( mit != allMatchers.end() ) delete (*mit++).second;
  PackageMap::iterator pgit = allPackages.begin();
  while ( pgit != allPackages.end() ) delete (*pgit++).second;
  ModeMap::iterator mdit = allModes.begin();
  while ( mdit != allModes.end() ) delete (*mdit++).second;
}

void HepPDTable::add(HepParticleData & pd) {
  ParticleMap::iterator old = allParticles.find(pd.id());
  if ( old != allParticles.end() ) delete (*old).second;

  ParticleMap::iterator cc = allParticles.find(-pd.id());
  if ( cc != allParticles.end() ) {
    pd.CC(*((*cc).second));
    (*cc).second->CC(pd);
  }

  allParticles[pd.id()] = &pd;
  mapName2Stdhep[pd.name()] = pd.id();
}

void HepPDTable::add(HepParticleMatcher & pm) {
  MatcherMap::iterator old = allMatchers.find(pm.name());
  if ( old != allMatchers.end() ) delete (*old).second;

  ParticleMap::iterator pit = allParticles.begin();
  while ( pit != allParticles.end() ) pm.addIfMatch(*((*pit++).second));

  MatcherMap::iterator mit = allMatchers.begin();
  while ( mit != allMatchers.end() ) pm.addIfMatch(*((*mit++).second));

  mit = allMatchers.begin();
  while ( mit != allMatchers.end() ) (*mit++).second->addIfMatch(pm);

  allMatchers[pm.name()] = &pm;
}

HepPkg HepPDTable::add(HepPackageHandler & pkg) {
  PackageNameMap::iterator old = packageNumbers.find(pkg.name());
 HepPkg pnr = packageNumbers.size();
  if ( old != packageNumbers.end() ) {
    pnr = (*old).second;
    HepPackageHandler * ph = packageHandler(pnr);
    delete ph;
  }
  allPackages[pnr] = &pkg;
  packageNumbers[pkg.name()] = pnr;
  pkg.handlerId(pnr);
  return pnr;
}

std::ostream & HepPDTable::write(std::ostream & os) const {
  ParticleMap::const_iterator pit = allParticles.begin();
  while ( pit != allParticles.end() ) {
    const HepParticleData & pd = *((*pit++).second);
    os << "P " << pd.id() << ' '
       << pd.name() << ' '
       << (pd.isStable? 'y': 'n') << ' '
       << pd.mass()/MeV << ' '
       << pd.massErrPos()/MeV << ' '
       << pd.massErrNeg()/MeV << ' '
       << pd.w/MeV << ' '
       << pd.dwp/MeV << ' '
       << pd.dwn/MeV << ' '
       << pd.t/mm << ' '
       << pd.dtp/mm << ' '
       << pd.dtn/mm << ' '
       << (int) pd.iCharge() << ' '
       << (int) pd.iSpin() << ' '
       << (int) pd.iColor() << ' '
       << pd.producers.size();
    HepParticleData::ProducerMap::const_iterator prit = pd.producers.begin();
    while ( prit != pd.producers.end() ) {
      PackageMap::const_iterator phit = allPackages.find((*prit).first);
      const HepPackageHandler * ph = (*phit).second;
      if ( (*prit).first == pd.defaultProducer() )
	os << " *" << ph->name() << ' ';
      else
	os << ' ' << ph->name() << ' ';
      ph->writeParticleProducer(os, *((*prit).second));
      ++prit;
    }
    os << std::endl;
  }
  pit = allParticles.begin();
  while ( pit != allParticles.end() ) {
    const HepDecayTable & dt = (*pit++).second->decayTable();
    HepDecayTable::const_iterator dit = dt.begin();
    while ( dit != dt.end() ) {
      const HepDecayMode & d = *((*dit++).second);
      os << "D " << d.tag() <<  ' '
	 << d.bRatio() << ' '
	 << d.bRatioErrPos() << ' '
	 << d.bRatioErrNeg() << ' '
	 << d.models.size();
      HepDecayMode::ModelMap::const_iterator mod = d.models.begin();
      while ( mod != d.models.end() ) {
	PackageMap::const_iterator phit = allPackages.find((*mod).first);
	const HepPackageHandler * ph = (*phit).second;
	if ( (*mod).first == d.defaultModel() )
	  os << " *" << ph->name();
	else
	  os << ' ' << ph->name();
	os << ' ' << (*mod).second.second << ' ';
	ph->writeDecayModel(os, *((*mod).second.first));
	++mod;
      }
      os << std::endl;
    }
  }
  TranslationMap::const_iterator tit = map2stdhep.begin();
  while ( tit != map2stdhep.end() ) {
    const HepString & pname = packageHandler((*tit).first)->name();
    LongLongMap::const_iterator it = (*tit).second.begin();
    while ( it != (*tit).second.end() ) {
      os << "T " << pname << ' '
	 << (*it).second << ' ' << (*it).first << std::endl;
      ++it;
    }
    ++tit;
  }
  PackageParameterMap::const_iterator gpit = globalParameters.begin();
  while ( gpit != globalParameters.end() ) {
    const HepString & pkg = packageHandler((*gpit).first)->name();
    ParameterMap::const_iterator it = (*gpit).second.begin();
    while ( it != (*gpit).second.end() ) {
      os << "G " << pkg << ' '
	 << (*it).first << ' ' << (*it).second << std::endl;
      ++it;
    }
    ++gpit;
  }
  return os;
}

std::istream & HepPDTable::read(std::istream & is) {
  char token;
  HepDouble val, dvalp, dvaln;
  HepString name;
  HepLong id;
  HepParticleData * pd;
  while ( is >> token ) {
    switch ( token ) {
    case 'P':
      {
	is >> id >> name;
	pd = getParticleData(id);
	if ( !pd ) pd = new HepParticleData(id, name);
	mapName2Stdhep[name] = id;
	HepString stable;
	is >> stable;
	if ( stable == "y" || stable == "Y" ) pd->stable(true);
	if ( stable == "n" || stable == "N" ) pd->stable(false);
	is >> val >> dvalp >> dvaln;
	if ( val >= 0.0 ) pd->mass(val*MeV, dvalp*MeV, dvaln*MeV);
	is >> val >> dvalp >> dvaln;
	if ( val >= 0.0 ) {
	  pd->w = val*MeV;
	  pd->dwp = dvalp*MeV;
	  pd->dwn = dvaln*MeV;
	}
	is >> val >> dvalp >> dvaln;
	if ( val >= 0.0 ) {
	  pd->t = val*mm;
	  pd->dtp = dvalp*mm;
	  pd->dtn = dvaln*mm;
	}
	HepLong spin, charge, color;
	is >> charge >> spin >> color;
	if ( charge != HepPDT::Cundefined )
	  pd->iCharge(HepPDT::HepCharge(charge));
	if ( spin > HepPDT::Sundefined ) pd->iSpin(HepPDT::HepSpin(spin));
	if ( color != HepPDT::colunknown ) pd->iColor(HepPDT::HepColor(color));
	HepParticleData::ProducerMap::size_type n;
	is >> n;
	while ( n-- ) {
	  HepString package;
	  is >> package;
	  HepBoolean def = false;
	  if ( package[0] == '*' ) {
	    package = package.substr(1);
	    def = true;
	  }
	  HepPackageHandler * ph = packageHandler(package);
	  HepPkg pnr = packageNumber(package);
	  if ( ph ) {
	    HepParticleProducer * prod = ph->readParticleProducer(is, *pd);
	    if ( prod ) {
	      pd->addProducer(pnr, *prod);
	      if ( def ) pd->setDefaultProducer(pnr);
	    }
	  }
	}
	add(*pd);
      } break;
    case 'D':
      {
	is >> name >> val >> dvalp >> dvaln;
	HepDecayMode * dm = getDecayMode(name);
	if ( val >= 0.0 && dm ) dm->bRatio(val, dvalp, dvaln);
	HepInt n;
	is >> n;
	while ( n-- ) {
	  HepString package;
	  is >> package >> val;
	  HepBoolean def = false;
	  if ( package[0] == '*' ) {
	    package = package.substr(1);
	    def = true;
	  }
	  HepPackageHandler * ph = packageHandler(package);
	  HepPkg pnr = packageNumber(package);
	  if ( ph && dm ) {
	    HepDecayModel * mod = ph->readDecayModel(is, *dm);
	    if ( mod ) {
	      dm->addModel(pnr, *mod, val);
	      if ( def ) dm->setDefaultModel(pnr);
	    }
	  }
	}
      } break;
    case 'T':
      {
	HepString trans;
	HepLong stdhepId, id;
	is >> trans >> stdhepId >> id;
	map2stdhep[packageNumber(trans)][id] = stdhepId;
      } break;
    case 'G':
      {
	HepString pkg, name;
	HepDouble x;
	is >> pkg >> name >> x;
	globalParameters[packageNumber(pkg)][name] = x;
      } break;
    }
    while ( is && is.peek() != '\n' ) is.get();
    is.get();
  }
  init();
  return is;
}

std::istream & operator >> (std::istream & is, std::vector<HepString> & v) {
  HepString s;
  while ( is.peek() != '\n' && is >> s ) v.push_back(s);
  if ( is.peek() == '\n' ) is.get();
  return is;
}

std::vector<HepString> splitString(const HepString & s) {
  std::vector<HepString> list;
  HepString::const_iterator i = s.begin();
  HepString current = "";
  while ( i != s.end() ) {
    if ( *i == ',' ) {
      list.push_back(current);
      current = "";
    } else {
      current += *i;
    }
    ++i;
  }
  list.push_back(current);
  return list;
}

void HepPDTable::readPDGFormat(std::istream & is) {
  std::vector<HepString> line, input;
  while ( is >> input ) {
    line = input;
    input = std::vector<HepString>();
    if ( line[0][0] == '*' ) continue;
    std::vector<HepString> charges = splitString(line.back());
    line.pop_back();
    HepDouble val = atof(line[line.size() - 3].c_str());
    HepDouble dvalp = atof(line[line.size() - 2].substr(1,7).c_str());
    HepDouble dvaln = atof(line[line.size() - 2].substr(9,7).c_str());
    int i;
    for ( i = 0; i < int(charges.size()); ++i ) {
      HepLong id = atol(line[i+1].c_str());
      HepInt q1((id/10)%10), q2((id/100)%10), q3((id/1000)%10);
      ParticleMap::iterator pit = allParticles.find(id);
      HepParticleData * pd =
	(pit == allParticles.end()? (HepParticleData *)0: (*pit).second);
      pit = allParticles.find(-id);
      HepParticleData * pdcc =
	(pit == allParticles.end()? (HepParticleData *)0: (*pit).second);
      if ( !pd ) {
	HepString name = line.back();
	name += charges[i];
	pd = new HepParticleData(id, name);
	add(*pd);
	  if ( charges[i] == "0" ) pd->iCharge(HepPDT::C0);
	  else if ( charges[i] == "+" ) pd->iCharge(HepPDT::C1);
	  else if ( charges[i] == "++" ) pd->iCharge(HepPDT::C2);
	  else if ( charges[i] == "-" ) pd->iCharge(HepPDT::Cn1);
	  else if ( charges[i] == "--" ) pd->iCharge(HepPDT::Cn2);
	  if ( q2 ) pd->iSpin(HepPDT::HepSpin(id%10));
      }
      if ( !pdcc ) {
	HepString name = line.back();
	if ( charges[i] == "0" ) name += "0";
	else if ( charges[i] == "+" ) name += "-";
	else if ( charges[i] == "++" ) name += "--";
	else if ( charges[i] == "-" ) name += "+";
	else if ( charges[i] == "--" ) name += "++";
	if ( q3 || charges[i] == "0" ) name += "~";
	if ( charges[i] != "0" || ( id > 10 && id < 20 ) || q3 || 
	     ( q1 && q2 && q1 != q2 && id%10 ) ) {
	  pdcc = new HepParticleData(-id, name);
	  add(*pdcc);
	  if ( charges[i] == "0" ) pdcc->iCharge(HepPDT::C0);
	  else if ( charges[i] == "+" ) pdcc->iCharge(HepPDT::Cn1);
	  else if ( charges[i] == "++" ) pdcc->iCharge(HepPDT::Cn2);
	  else if ( charges[i] == "-" ) pdcc->iCharge(HepPDT::C1);
	  else if ( charges[i] == "--" ) pdcc->iCharge(HepPDT::C2);
	  if ( q2 ) pd->iSpin(HepPDT::HepSpin(id%10));
	}
      }
      switch ( line[0][0] ) {
      case 'M':
	{
	  if ( pd ) pd->mass(val*GeV, dvalp*GeV, dvaln*GeV);
	  if ( pdcc ) pdcc->mass(val*GeV, dvalp*GeV, dvaln*GeV);
	} break;
      case 'W':
	{
	  if ( pd ) pd->width(val*GeV, dvalp*GeV, dvaln*GeV);
	  if ( pdcc ) pdcc->width(val*GeV, dvalp*GeV, dvaln*GeV);
	} break;
      }
    }
  }
  init();
}

void HepPDTable::init() {
  MatcherMap::iterator mit = allMatchers.begin();
  while ( mit != allMatchers.end() ) (*mit++).second->clear();
  ParticleMap::iterator pit = allParticles.begin();
  while ( pit != allParticles.end() ) {
    HepParticleData * pd = (*pit++).second;
    pd->decayModes.init();
    mit = allMatchers.begin();
    while ( mit != allMatchers.end() ) 
      (*mit++).second->addIfMatch(*pd);
  }
  mit = allMatchers.begin();
  while ( mit != allMatchers.end() ) {
    MatcherMap::iterator mit2 = mit;
    HepParticleMatcher * pm = (*mit).second;
    while ( mit2 != allMatchers.end() ) {
      if ( mit2 != mit ) {
	pm->addIfMatch(*((*mit2).second));
	(*mit2).second->addIfMatch(*pm);
      }
      (*mit2++).second->CC(*pm);
    }
    ++mit;
  }
}

void HepPDTable::dumpEnumNames(std::ostream & os) const {
  os << "  enum HepParticleCodes {" << std::endl;
  ParticleMap::const_iterator pit = allParticles.begin();
  while ( pit != allParticles.end() ) {
    const HepParticleData * pd = (*pit++).second;
    os << "    ";
    HepString::const_iterator nit = pd->name().begin();
    while ( nit != pd->name().end() ) {
      switch ( *nit ) {
      case '~':
	os << "bar";
	break;
      case '\'':
	os << "prime";
	break;
      case '\"':
	os << "bis";
	break;
      case '+':
	os << "plus";
	break;
      case '-':
	os << "minus";
	break;
      case '*':
	os << "star";
	break;
      case '/':
	break;
      default:
	os << *nit;
      }
      ++nit;
    }
    os << " = " << pd->id();
    if ( pit != allParticles.end() )
      os << ',' << std::endl;
    else
      os << std::endl << "  };" << std::endl;
  }
}


