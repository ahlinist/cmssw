#define PYTHIA_WEIGHTS
#define BC_SEPARATELY
//#define DEBUG_DOWNSCALING 

#include <stdio.h>
#include <math.h>

/* maximum number of muons to generate: */
#define MAXMUON 10

/* flavour code for muon: */
#define IDMU 13

/* muon mass */
#define MASSMU 0.1057
#define MASSMU2 MASSMU*MASSMU

/* size of cylinder (allowed volume for vertices) */
#define RADIUS 2950
#define LENGTH 5600

extern struct
{
  int mstu[200];
  double paru[200];
  int mstj[200];
  double parj[200];
} pydat1_;

#define MSTU(a) pydat1_.mstu[a-1]
#define PARU(a) pydat1_.paru[a-1]
#define MSTJ(a) pydat1_.mstj[a-1]
#define PARJ(a) pydat1_.parj[a-1]


extern struct
{
  int kchg[4][500];
  double pmas[4][500];
  double parf[2000];
  double vckm[4][4];
} pydat2_;

#define KCHG(a,b) pydat2_.kchg[b-1][a-1]
#define PMAS(a,b) pydat2_.pmas[b-1][a-1]
#define PARF(a) pydat2_.parf[a-1]
#define VCKM(a,b) pydat2_.vckm[b-1][a-1]

/*
extern struct
{
  int mdcy[3][500];
  int mdme[2][4000];
  double brat[4000];
  int kfdp[5][4000];
} pydat3_;
*/
  
extern struct
{
  int mdcy[3][500];
  int mdme[2][8000];
  double brat[8000];
  int kfdp[5][8000];
} pydat3_;
  

#define MDCY(a,b) pydat3_.mdcy[b-1][a-1]
#define MDME(a,b) pydat3_.mdme[b-1][a-1]
#define BRAT(a) pydat3_.brat[a-1]
#define KFDP(a,b) pydat3_.kfdp[b-1][a-1]


extern struct
{
  int n,npad,k[5][4000];
  double p[5][4000],v[5][4000];
} pyjets_;

#define N pyjets_.n
#define NPAD pyjets_.npad
#define K(a,b) pyjets_.k[b-1][a-1]
#define P(a,b) pyjets_.p[b-1][a-1]
#define V(a,b) pyjets_.v[b-1][a-1]

//*-----------------------------------------------------------------------
//*
//*     General data card variables
//*     ---------------------------
//*
//      INTEGER       N_TRIG       ! No. of events to "trigger" (i.e. generate)
//      INTEGER       N_NSEL       ! No. of events required to pass selection
//      REAL          ENERGY_CMS   ! Centre of mass energy
//      INTEGER       KSEL         ! Selection flag
//*
//      INTEGER       NCUT         ! Number of CMKIN selection cuts
//      PARAMETER    (NCUT=100)
//*
//      INTEGER       ICUT         ! Integer CMKIN selection cuts
//      REAL          RCUT         ! Real    CMKIN selection cuts
//*
//      COMMON /KI_FFGEN/
//     &                   N_TRIG
//     &,                  N_NSEL
//     &,                  ENERGY_CMS
//     &,                  KSEL
//     &,                  ICUT (NCUT)
//     &,                  RCUT (NCUT)
//*-----------------------------------------------------------------------
#define NCUT 100
extern struct
{
  int n_trig,n_nsel;
  float energy_cms;
  int ksel;
  int icut[NCUT];
  float rcut[NCUT];
} ki_ffgen_;
#define N_TRIG ki_ffgen_.n_trig
#define N_NSEL ki_ffgen_.n_nsel
#define ENERGY_CMS ki_ffgen_.energy_cms
#define KSEL ki_ffgen_.ksel
#define ICUT(a) ki_ffgen_.icut[a-1];
#define RCUT(a) ki_ffgen_.rcut[a-1];


/* kinematic range of muons to be acccepted */
double ptminmu, etaminmu, etamaxmu;


/* number of times to decay any hadron in order to smooth probabilities */
#define NDECAY 50


/*      these particles can decay into mu + anything:

                      tau,rho0,K_L0,pi+,eta, K+, D+, D0,D_s+,J/psi,
                 B0,B+,B_s0,B_c+,Y,Lambda_c+,Xi_c0,Xi_c+,Omega_c0,
                 Lambda_b0,Xi_b-,Xi_b0,Omega_b-,psi',Y'
*/

int muon_parents[] = {15, 113, 130, 211,221,321,411,421,431, 443,
                  511,521,531,541,553, 4122, 4132, 4232, 4332,
                  5122,  5132,  5232,  5332,  100443,100553,0};

int decay_mode_list[2000];
int n_decay_mode;
int first_muon[500];
int last_muon[500];
int last_nonmuon[500];
int last_decay[500];
double brat_muon[500];
double brat_nonmuon[500];
double par_a[500];
double par_b[500];
double par_m[500];
double ptmin2[500];

int set_muon_decay (int mode, int kc)
/* select predefined set of decay modes for particle with compressed code kc
   mode = 0   switch on all default decay modes
          1   switch on only those modes which give a prompt muon
          2   switch on only decay modes without prompt muon
   returned:  1 if selected decay has nonzero branching ratio
            0 otherwise
*/
{
  int i,j,k,first,last;

  k=0;
  j=(mode!=2);
  last=last_muon[kc];
  for (i=first_muon[kc]; i<=last; ++i)
  {
    MDME(decay_mode_list[i],1)=j;
    k|=j;
  }
  last=last_decay[kc];
  j=(mode!=1);
  for (; i<=last; ++i)
  {
    MDME(decay_mode_list[i],1)=j;
    k|=j;
  }
  return(k);
}

void switch_off_decays()
/* inhibit decays of muon parents */
{
  int i,a,kc,kf;

  i=0;
  while (kf=muon_parents[i])
  {
    a=abs(kf);
    kc=pycomp_(&a);
    MDCY(kc,1)=0;
    ++i;
  }

  MSTJ(22)=1;            // allow decays everywhere (not only inside cylinder)
}



int initdc_(float *minpt, float *maxeta)
/***********************************************************************/
/*                                                                     */
/* Initialize n-muon generator                                         */
/*                                                                     */
/* The main task of this routine is to scan the JETSET decay table     */
/* for decay modes of hadrons with muons as daughters and store the    */
/* information which is necessary to select quickly modes with or      */
/* without prompt muons                                                */
/*                                                                     */
/* Input: *minpt  - minimum pt required for a muon to be accepted      */
/*        *maxeta - maximum pseudorapidity for accepted muons          */
/*                  (muons are accepted in the range -maxeta .. maxeta)*/
/*                                                                     */
/***********************************************************************/
{
  int i,j,l,first,last,a;
  int kf,kc,kcd,mu;
  int idecay;
  double mass,ptmin,pt2minmu;

  for (i=0; i<500; ++i)
    first_muon[i]=last_muon[i]=last_nonmuon[i]=last_decay[i]=-1;

//  printf("ki_ffgen : \n");
//  printf("n_trig=%d ,n_nsel=%d ,energy_cms=%f ,ksel=%d \n",N_TRIG,N_NSEL,ENERGY_CMS,KSEL);
//
//  for (i=1 ; i<=NCUT ; i++) 
//    printf("ICUT(%d)=%d ,RCUT(%d)=%f \n",i,ki_ffgen_.icut[i-1],i,ki_ffgen_.rcut[i-1]);

  i=0;
  idecay=0;
  kf=muon_parents[i];
  while (kf)
  {
    a=abs(kf);
    kc=pycomp_(&a);
    first_muon[kc]=idecay;
    brat_muon[kc]=0.;
    brat_nonmuon[kc]=0.;
    kcd=kc;
    first=MDCY(kcd,2);
    if (MDME(first,2)>=84 && MDME(first,2)<=88)
    {
      kcd=MDME(first,2);            // generic heavy quark decay
      first=MDCY(kcd,2);
    }
    last=MDCY(kcd,2)+MDCY(kcd,3);
    for (j=first; j<last; ++j)            // loop over decay modes
    {
      mu=0;
      for (l=1; l<=5; ++l)            // loop over decay products
      if (abs(KFDP(j,l))==IDMU ||      // and count muons
          (abs(KFDP(j,l))==443 &&      //  include also J/psi to muon channels
           a!=30443)) ++mu;            //  (but not for psi')
      if (mu)                        // find decay into muons
      {
      if (MDME(j,1)==1)            // decay switched on?
      {
        decay_mode_list[idecay++]=j;  // store decay mode index
        brat_muon[kc]+=BRAT(j);      // sum branching ratio
      }
      }
    }
    last_muon[kc]=idecay-1;
    for (j=first; j<last; ++j)            // loop again over decay modes
    {
      mu=0;
      for (l=1; l<=5; ++l)            // loop over decay products
      if (abs(KFDP(j,l))==IDMU) ++mu;      // and count muons
      if (!mu)                        // find decay without muons
      {
      if (MDME(j,1)==1)            // decay switched on?
      {
        decay_mode_list[idecay++]=j;  // store decay mode index
        brat_nonmuon[kc]+=BRAT(j);      // sum branching ratio
      }
      }
    }
    last_decay[kc]=last_nonmuon[kc]=idecay-1;

/* get pt and eta range for muons to select */
   ptminmu=*minpt;
   pt2minmu=ptminmu*ptminmu;
   etaminmu=-*maxeta;
   etamaxmu=*maxeta;

/* store some parameters of 2-body decay to allow quick estimate whether
   a muon coming out of the decay of the particle can possibly have a
   large transverse momentum
*/
    mass=PMAS(kc,1);
    par_a[kc]=0.5*(1.+MASSMU2/(mass*mass));
    par_b[kc]=1.-par_a[kc];
    par_m[kc]=mass;
    ptmin=ptminmu*(-par_b[kc]*mass*mass/
                (pt2minmu*(sqrt(MASSMU2/pt2minmu+1.)+1.)) +1.);
    printf("mass=%f, ptmin=%f\n",mass,ptmin); 
    if (ptmin>0.) ptmin2[kc]=ptmin*ptmin;
    else ptmin2[kc]=0.;

    kf=muon_parents[++i];            // get next hadron
  }

/* switch off neutral pion decay */
  a=111;
  MDCY(pycomp_(&a),1)=0;

  switch_off_decays();

  return idecay;
}

struct t_decayed_hadron
{
  int index;
  double prob[MAXMUON];
  struct t_decay *decay;
  struct t_decayed_hadron *next;
};
typedef struct t_decayed_hadron t_decayed_hadron;

struct t_decay
{
  double prob[MAXMUON];
  int first,last;
  t_decayed_hadron *daughter;
  struct t_decay *next;
};
typedef struct t_decay t_decay;

int nhadrons,ndecays;
t_decayed_hadron hadrons[4000];
t_decay decays[2000];

t_decayed_hadron *decay (int index);


void prob_nmuon(t_decayed_hadron *hadron, int nmuon, double *pn)
/***********************************************************************/
/*                                                                     */
/*  for a linked list of hadrons, calculate the probabilities to get   */
/*  more than n muons from their decays                                */
/*                                                                     */
/*  input: hadron - first hadron in the list, hadron->next is expected */
/*                  to point to the next one, the last has next=NULL   */
/*                  hadron->prob[n] is the probability to get >n muons */
/*                  from this hadron                                   */
/*         nmuon  - maximum number of muons to calculate               */
/*  output:  pn[] - array of probabilities                             */
/*                  p[0] is the probability to get >0 muons            */
/*                  p[1]  .  .      .       .   .  >1 muons            */
/*                   ..   .  .      .       .   .  ..  ....            */
/*                  p[nmuon] .      .       .   .  >nmuon muons        */
/*                                                                     */
/***********************************************************************/
{
  int i,j;
  t_decayed_hadron *h;

  for (i=0; i<=nmuon; ++i) pn[i]=0;

  for (h=hadron; h!=NULL; h=h->next)
    for (i=nmuon; i>=0; --i)
    {
      for (j=i-1; j>=0; --j)
        pn[i]+=h->prob[i-j-1]*(pn[j]-pn[j+1]);
      pn[i]+=h->prob[i]*((double)1-pn[0]);
    }
}


t_decay *loop_hadrons(int first,int last)
/***********************************************************************/
/*                                                                     */
/* loop over a list of hadrons, have them decay, and calculate         */
/* the combined probability to obtain >0 or >1 muons                   */
/*                                                                     */
/***********************************************************************/
{
  int i,n,mu;
  double pn[MAXMUON];
  t_decayed_hadron *h1;
  t_decayed_hadron *previous;
  t_decay d;
  t_decay *next;
  t_decayed_hadron *daughter;

  next=NULL;
  daughter=NULL;
  n=N;
  previous=NULL;

  for (i=first; i<=last; ++i)
  {
    if (K(i,1)>0 && K(i,1)<=10)            // undecayed particle
    {
      h1=decay(i);
      if (h1!=NULL)
      {
      pn[0]=h1->prob[0];
      if (pn[0]>0)                  // any chance to get a muon?
        {
        if (previous!=NULL) previous->next=h1;
        else daughter=h1;
        previous=h1;
      }
      }
    }
  }

  if (daughter!=NULL) prob_nmuon(daughter,MAXMUON-1,pn);
  else return NULL;

  if (pn[0]>0)
  {
    for (i=0; i<MAXMUON; ++i) decays[ndecays].prob[i]=pn[i];
    decays[ndecays].first=first;
    decays[ndecays].last=N;
    decays[ndecays].daughter=daughter;
    decays[ndecays].next=next;
    return (&(decays[ndecays++]));
  }
  else
    return NULL;
}


int muon_cut(int index)
/* test whether the muon at position index in the event record
   satisfies the selection criteria
   returns 1 if muon accepted, 0 otherwise
*/
{
  double px,py,pz,pt,pt2,eta,p;
  int iselp,iselm;
  iselp=ICUT(10);
  iselm=ICUT(1);
  px=P(index,1);
  py=P(index,2);
  pz=P(index,3);
  pt2=px*px+py*py;
  p=sqrt((double)(pt2+pz*pz));
  pt=sqrt((double)(pt2));

  eta=log((double)((p+fabs((double)pz))/pt))*((pz>0)-(pz<0));

    if(iselp == 1 && iselm==1)

//  special for single mu, pt1 (ICUT(10)=1) production (U.G.) :
      {
        
	if( pt > 4.0 ) return 0;
	if (eta<etaminmu || eta>etamaxmu) return 0;
	if (eta>etaminmu && eta<-1.7 && p<3.5) return 0;
	if (eta>-1.7 && eta<-1.2 && pt<1.8) return 0;
	if (eta>-1.2 && eta< 1.2 && pt<3.0) return 0;
	if (eta> 1.2 && eta< 1.7 && pt<1.8) return 0;
	if (eta> 1.7 && eta<etamaxmu && p<3.5) return 0;
      }

//  for all other productions :
    else
      {
	if (eta<etaminmu || eta>etamaxmu || pt<ptminmu) return 0;
      } 
  return 1;
}


int count_muons(int low, int high)
/***********************************************************************/
/* count the number of muons in the event record at index low .. high  */
/***********************************************************************/
{
  int i,n;

  n=0;
  for (i=low; i<=high; ++i)
    if (abs(K(i,2))==IDMU && K(i,1)>0 && K(i,1)<=10)
      if (muon_cut(i)) ++n;
  return n;
}


extern double pyr_();


double choose_decay_vertex(int index,int kc)
/***********************************************************************/
/* choose a decay vertex for long living particles which is inside the */
/* detector, and return the decay probability                          */
/***********************************************************************/
{

  int i;
  double prob;

  double lifetime,mass;
  double r,x0,y0;
  double px,py,pz,pt,p;
  double length,radius,v;

  lifetime=PMAS(kc,4);
  r=pyr_();
  
/* get momentum and determine length of track before intersection with
   the bounding cylinder */
  px=P(index,1);
  py=P(index,2);
  pz=P(index,3);
  pt=px*px+py*py;
  p=sqrt((double)(pt+pz*pz));
  pt=sqrt((double)(pt));

  length=LENGTH-V(index,3)*((pz>0)-(pz<0));
  v=V(index,1)*V(index,1)+V(index,2)*V(index,2);
  if (v>RADIUS*RADIUS || length<0)      // particle produced outside detector?
    return 0.;
  if (pt==0) radius=RADIUS;
  else
  {
    radius=V(index,1)*P(index,1)/pt;
    radius+=sqrt((double)radius*radius+RADIUS*RADIUS-v);
  }
  pz=fabs((double)pz);
  if (pt*length>pz*radius) x0=radius*p/pt;
  else x0=length*p/pz;

  mass=P(index,5);
  y0=exp(-mass*x0/(lifetime*p));

  prob=1.-y0;

  if (prob<=0)
  {
    if (prob<0) puts("choose_decay_vertex: prob<0!");
    return 0;
  }

  lifetime*=(-log((double)(y0*(1-r)+r)));      // choose random proper lifetime

  for (i=1; i<=4; ++i) V(index,i)+=lifetime*P(index,i)/mass;

  K(index,1)=5;            // force decay at a given vertex
  
  return prob;
  
}


pydecy_all(index)
/* decay a particle by calling PYDECY from JETSET, and decay also all its
 unstable decay products recursively */
{
  int i,n,kf,last;

  n=N+1;
  pydecy_(&index);
  last=N;

  for (i=n; i<=last; ++i)
  {
    if (K(i,1)>0 && K(i,1)<=10)
    {
      kf=abs(K(i,2));
      if (MDCY(pycomp_(&kf),1)) pydecy_all(i);
    }
  }
}


void undecay(t_decay *decay)
/***********************************************************************/
/* delete a decay and all decay products                               */
/***********************************************************************/
{
  int i,last;

  last=decay->last;
  for (i=decay->first; i<=last; ++i) K(i,1)=0;
}


void pyexec_nomu()
/***********************************************************************/
/*                                                                     */
/* decay all undecayed particles in the event record, but avoid muons  */
/* in the acceptance region as decay products                          */
/*                                                                     */
/***********************************************************************/
{
#define MAXTRYNOMU 50
  int i,kf,last,n,tried;

  last=N;

  for (i=1; i<=last; ++i)
  {
    kf=abs(K(i,2));
    if (K(i,1)==1) if (MDCY(pycomp_(&kf),1))
    {
      n=N+1;
      pydecy_all(i);                        // decay

      tried=0;
      while (count_muons(n,N))
      {
        if (++tried>=MAXTRYNOMU)
        {
          printf("error in pyexec_nomu: unable to get decay without muons after %d attempts\n",tried);
          N=n-1;
          K(i,1)=1;
          break;
        }
        N=n-1;
        K(i,1)=1;
        pydecy_all(i);
      }
    }
  }
}


void choose_nomuon(t_decayed_hadron *hadron)
/***********************************************************************/
/*                                                                     */
/* delete decay products of a hadron                                   */
/*                                                                     */
/***********************************************************************/
{
  t_decay *d;

  for (d=hadron->decay; d!=NULL; d=d->next)      // delete decay products
    undecay(d);
  K(hadron->index,1)=1;                        // declare undecayed
}


int pydecy_mu(int index,int ndecay)
/* decay a particle ndecay times and count the number of times
   an accepted muon was the direct daughter.
   Keep the first successful decay (if any), and return the
   number of successes
*/
{
  int i,j,found,k,kd,kf,n,nsuccess;

  n=N+1;
  
  nsuccess=0;
  k=kd=K(index,1);            // remember status code of undecayed particle
  for (i=0; i<ndecay; ++i)
  {
    pydecy_(&index);                  // decay the particle
    found=0;
    for (j=n; j<=N; ++j)            // loop over decay products
      if (abs(K(j,2))==IDMU)            // find muons
      if (muon_cut(j))
      {
        found++;
        break;
      }
    if (found)                        
    {
      ++nsuccess;                  // in case of success,
      for (j=n; j<=N; ++j)            // make sure all daughters are decayed
      if (K(j,1)>0 && K(j,1)<=10)
        {
          kf=abs(K(j,2));
          if (MDCY(pycomp_(&kf),1)) pydecy_all(j);
        }
      n=N+1;                        // keep decay products
      kd=K(index,1);                  // remember decayed status code
      break;
    }
    else                        // no muon found
    {
      K(index,1)=k;                  // forget about decay
      N=n-1;
    }
  }

  for (++i; i<ndecay; ++i)            // continue the loop
  {
    pydecy_(&index);                    // decay the particle again
    found=0;
    for (j=n; j<=N; ++j)                // loop over decay products
      if (abs(K(j,2))==IDMU)              // find muons
      if (muon_cut(j))
      {
        found++;
        break;
      }
    if (found)                          
      ++nsuccess;                       // count successes,
    K(index,1)=k;                  // but forget about the decay anyway
    N=n-1;
  }

  K(index,1)=kd;                  // update status code (possibly decayed)

  return nsuccess;
}


t_decayed_hadron *decay (int index)
/***********************************************************************/
/*                                                                     */
/* decay the hadron at the given index in the event record, and        */
/* store all relevant decay modes (relevant are those which can        */
/* eventually lead to the production of a muon in the accepted region) */
/*                                                                     */
/***********************************************************************/
{
  int kf,kc;
  int i,k,kd,a,n,first,last;
  t_decay *d1;
  t_decay *d2;
  double v[6];
  double pv;
  double px,py,pt2;
  double prob[MAXMUON];

  kf=K(index,2);

  if (abs(kf)==IDMU)            // muon
  {
    if (muon_cut(index))
    {
      hadrons[nhadrons].prob[0]=1;
      for (i=1; i<MAXMUON; ++i) hadrons[nhadrons].prob[i]=0.;
      hadrons[nhadrons].index=index;
      hadrons[nhadrons].decay=NULL;
      hadrons[nhadrons].next=NULL;
      return (&(hadrons[nhadrons++]));
    }
    else
      return NULL;
  }

  a=abs(kf);
  kc=pycomp_(&a);
  if (last_decay[kc]<0) return NULL;      // no possible muon parent

  if (ptmin2[kc]>0.)            // test whether muon is kinematically possible
  {
    px=P(index,1);            // calculate pt of hadron
    py=P(index,2);
    pt2=px*px+py*py;
    if (pt2<ptmin2[kc]) return NULL;
  }

  for (i=0; i<MAXMUON; ++i) prob[i]=0;
  pv=1.;

  for (i=1; i<=5; ++i) v[i]=V(index,i);      // save production vertex position
  if (PMAS(kc,4)>100.)                  // lifetime long (>10cm ?)
  {
    pv=choose_decay_vertex(index,kc);
    if (pv<=0)
    {
      K(index,1)=4;      // mark particle as "could have decayed but didn't"
      return NULL;
    }
  }

  MDCY(kc,1)=1;                  // switch on decay of this particle

  k=kd=K(index,1);                  // remember status code

  if (set_muon_decay (1,kc))      // switch on decays into muons
  {
    n=N+1;
    {
      pydecy_all(index);

      d2=d1=loop_hadrons (n,N);      // decay daughters and count muons

      if (d1==NULL)            // no muons?
      {
        K(index,1)=k;            // declare particle as undecayed
        N=n-1;                  // forget all decay products
      }
      else
      for (i=0; i<MAXMUON; ++i) prob[i]=(d1->prob[i]*=brat_muon[kc]);
    }
  }
  if (set_muon_decay (2,kc))      // switch on decays without prompt muons
  {
    n=N+1;
    kd=K(index,1);            // remember status code
    K(index,1)=k;            // declare particle undecayed
    pydecy_all(index);            // have the particle decay again

    d2=loop_hadrons (n,N);      // loop over decay products

    if (d2==NULL)                  // no muons?
    {
      N=n-1;                  // => forget about decay
      K(index,1)=kd;            // restore old status code
      d2=d1;
    }
    else
    {
      for (i=0; i<MAXMUON; ++i)
      prob[i]+=(d2->prob[i]*=brat_nonmuon[kc]);      // add probabilities
      if (prob[0]>0)            // nonzero probabilities
      d2->next=d1;            // => keep decays (d1 may be NULL)
    }
  }
  MDCY(kc,1)=0;                  // switch decay off again

  for (i=1; i<=5; ++i) V(index,i)=v[i]; // restore production vertex position
  if (K(index,1)==5) K(index,1)=1;
  
  if (prob[0]>0)            // any muon finally?
  {
    for (i=0; i<MAXMUON; ++i)
      hadrons[nhadrons].prob[i]=pv*prob[i];      // take decay probability into account
    hadrons[nhadrons].index=index;
    hadrons[nhadrons].decay=d2;
    hadrons[nhadrons].next=NULL;
    return (&(hadrons[nhadrons++]));
  }
  else
    return (NULL);
}


void default_decay_modes()
/***********************************************************************/
/* restore default decay modes for all particles                       */
/***********************************************************************/
{
  int i,j,a,kc,kf;

  i=0;
  while (kf=muon_parents[i])
  {
    a=abs(kf);
    kc=pycomp_(&a);
    for (j=first_muon[kc]; j<=last_decay[kc]; ++j)
      MDME(decay_mode_list[j],1)=1;
    MDCY(kc,1)=1;
    ++i;
  }

  MSTJ(22)=4;            // limit decay to cylinder
  PARJ(73)=RADIUS;
  PARJ(74)=LENGTH;

}


void choose_decay(t_decayed_hadron *hadron, unsigned int nmuon)
/***********************************************************************/
/*                                                                     */
/*  from the set of possible decay modes of a hadron,                  */
/*  choose one which gives exactly nmuon muons                         */
/*                                                                     */
/***********************************************************************/
{
  int i;
  double p;
  double r,x;
  double ph[MAXMUON+1],pn[MAXMUON];
  t_decay *d;
  t_decayed_hadron *h, *h1, *h2;

  if (!nmuon)
  {
    choose_nomuon(hadron);
    return;
  }

  if (nmuon>MAXMUON)
  {
    puts("error in choose_decay: too many muons requested");
    printf(" - won't get more than %d\n",MAXMUON);
    nmuon=MAXMUON;
  }

  d=hadron->decay;
  if (d==NULL) return;
  if (d->next!=NULL)                  // more than one decay
  {                              // => decision needed
    p=0;
    for (; d!=NULL; d=d->next)
      p+= (nmuon==MAXMUON ? d->prob[nmuon-1] : d->prob[nmuon-1]-d->prob[nmuon]);
    r=pyr_()*p;
    for (d=hadron->decay; d!=NULL; d=d->next)
    {
      r-= (nmuon==MAXMUON ? d->prob[nmuon-1] : d->prob[nmuon-1]-d->prob[nmuon]);
      if (r<0)
      {
      r+=2.;      // 1 should be sufficient
      hadron->decay=d;
      }
      else
      undecay (d);
    }
  }

  d=hadron->decay;
  d->next=NULL;                        // only one decay left

  // need to update daughter information here which may be
  // wrong because of multiple decays of the same particle:
  K(hadron->index,4)=d->first;
  K(hadron->index,5)=d->last;
  // note: d->last is the last of all particles arising
  //       from the decay, not only the immediate daughters
  //        (is this a problem?)

  h=d->daughter;

  if (h==NULL)
  {
    puts ("decay without products in choose_decay");
    return;
  }

  while (h->next!=NULL)                  // more than one decay product
  {                              // => decision needed
    if (nmuon)
    {
      prob_nmuon(h->next,nmuon,pn);
      if (nmuon==MAXMUON)      // for nmuon=MAXMUON, use probabilities for
      {                        // >= MAXMUON, but select only MAXMUON
      p=ph[0]=((double)1-h->prob[0])*pn[nmuon-1];
      for (i=1; i<nmuon; ++i)
        p+=(ph[i]=(h->prob[i-1]-h->prob[i])*(pn[nmuon-i-1]));
      p+=(ph[nmuon]=h->prob[nmuon-1]);
      }
      else
      {
      p=ph[0]=((double)1-h->prob[0])*(pn[nmuon-1]-pn[nmuon]);
      for (i=1; i<nmuon; ++i)
        p+=(ph[i]=(h->prob[i-1]-h->prob[i])*(pn[nmuon-i-1]-pn[nmuon-i]));
      p+=(ph[nmuon]=(h->prob[nmuon-1]-h->prob[nmuon])*((double)1-pn[0]));
      }

      r=pyr_()*p;      // choose number of muons for this hadron
      for (i=nmuon; i; --i)
      {
      r-=ph[i];
      if (r<0) break;
      }

      nmuon-=i;
    }
    else i=0;

    choose_decay(h,i);
    h=h->next;
  }
  choose_decay(h,nmuon);
}


float wgtmu_(int *nmu, float *fl)
/* force an event to have at least nmu muons
 input: *nmu      minimum number of desired muons
      *fl      minimum desired weight
 returned:      weight of the event (discard event if wgtmu_=0.)
*/
{

// bc_separately declaration

  int idpy,ndpy;
  int hard_b,hard_c;
  int ngs_b,ngs_c;
  int i,j,n,nmuon;
  t_decay *d;
  t_decayed_hadron h;
  double r;
  double weight,wtxs;
  double pn[MAXMUON];

  double x,y,z,e;

  weight=1.;

//  i=12;
//  pylist_(&i);

  nmuon=*nmu;

  if (i=count_muons(1,N))
  {
    printf ("%d muons already found before decay\n",i);
    //    j=1; pylist_(&j);
  }

  nmuon-=i;
  if (nmuon<=0)                  // already enough muons?
  {
    default_decay_modes();
    pyexec_();                  // decay the event without constraints
    switch_off_decays();      // prepare for next event
  }
  else
  {
    nhadrons=ndecays=0;
    d=loop_hadrons (9,N);      // decay all hadrons into muons

    if (d==NULL) return 0;      // no more muons?
  
    weight=d->prob[nmuon-1];
    if (weight==0) return 0;

    r=pyr_()*d->prob[nmuon-1];      // choose number of muons to select

    for (i=nmuon; i<MAXMUON; ++i)
      if (r>d->prob[i]) break;

    if (i==MAXMUON)
    {
      printf("Warning in WGTMU: Event wants %d or more muons,",i);
      printf(" but will never get more than %d.\n",i);
      puts("   If needed, this can be fixed by increasing the parameter MAXMUON in muons.c");
    }

    nmuon=i;
  }


#ifdef PYTHIA_WEIGHTS
/* get PYTHIA weight */
    pyevwt_(&wtxs);
    wtxs=weight/wtxs;
#else
    wtxs=weight;
#endif

#ifdef BC_SEPARATELY
// ============================================================================
//  identify leading order c-cbar and b-bbar production and flavour excitation
//  and reduce the weight of these events by factor RCUT(9)
//  These events have to generated separately and added in order to recover
//  a complete sample!
//  b-events are additionally scaled by 500mb/bbxsec(pthat=0,mstp(82)=1)=0.42
//  following recommendations for LHC b-physics
// ============================================================================
/* down-scale gluon splitting (GS) events and b,c events (FE + PF) */



    //  
    ngs_b =0;
    ngs_c =0;          
    hard_b=0;
    hard_c=0;
    ndpy=N;
 
    if (abs(K(7,2))==5 || abs(K(8,2))==5)hard_b=1;
    if (abs(K(7,2))==4 || abs(K(8,2))==4)hard_c=1;

        for (idpy=1; idpy<=ndpy; ++idpy) // N defined as N = pyjets_.n
      {
     	if ( abs(K(K(idpy,3),2))==21 &&  abs(K(idpy,2))==4 ) ++ngs_c;
     	if ( abs(K(K(idpy,3),2))==21 &&  abs(K(idpy,2))==5 ) ++ngs_b;
      }

//rescale by 10 to account for enriched luminosity

    if( ngs_b >= 2 || hard_b==1 || ngs_c>=2 || hard_c==1 )wtxs*=RCUT(9); 

//additional rescaling for bb xsec

    if( ngs_b >= 2 || hard_b==1 ) wtxs*=0.42; 

// ============================================================================
#endif

/* downscale events with low weights      */
  if (wtxs>0.&& wtxs<*fl)
  {
    r=pyr_();
#ifdef DEBUG_DOWNSCALING
    printf ("to be downscaled: r=%f, weight=%f",r,wtxs);
#endif
    if (r<wtxs/(*fl))
    {
      wtxs=*fl;
#ifdef DEBUG_DOWNSCALING
      printf (" --> kept with weight=%f\n",*fl);
#endif
    }
    else
    {
#ifdef DEBUG_DOWNSCALING
      printf (" --> rejected\n");
#endif
      return 0.;
    }
  }
#ifdef DEBUG_DOWNSCALING
  else
    printf ("Event not downscaled, weight=%f\n",weight);
#endif

  if (nmuon>0)
  {
/* create dummy hadron in order to avoid duplicating the loop over hadrons */
    h.index=0;
    for (i=0; i<MAXMUON; ++i) h.prob[i]=d->prob[i];
    h.decay=d;
    h.next=NULL;
  
    default_decay_modes();
    choose_decay(&h,nmuon);      // select a decay mode with nmu muons
    switch_off_decays();      // prepare for next event

    i=12;
    pyedit_(&i);            // remove deleted particles from event record
  }
#ifdef DEBUG_DOWNSCALING
  printf("WGTMU: event weight=%f, after downscaling: %f\n",d->prob[1],wtxs);
#endif

//  i=1;
//  pylist_(&i);
  default_decay_modes();
  pyexec_nomu();            // decay all hadrons without any muons
  switch_off_decays();            // prepare for next event
//  pylist_(&i);


  return wtxs;
}

