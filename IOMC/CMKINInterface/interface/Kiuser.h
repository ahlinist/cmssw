// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kiuser.h -> CMKIN kiuser fortran selection rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/18
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kiuser_h
#define Kiuser_h

#ifdef _WIN32
# pragma once
# ifdef DLLEXPORT
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllexport)
#  endif
# else
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllimport)
#  endif
# endif
#endif
#ifndef DLLLINKAGE
# define DLLLINKAGE
#endif

#ifndef _WIN32
#define kir_anal kir_anal_
#define kir_init kir_init_
#define kir_end kir_end_
#define kis_b_c kis_b_c_
#define kis_bbbar_ex kis_bbbar_ex_
#define kis_bkg kis_bkg_
#define kis_bkgtth kis_bkgtth_
#define kis_bsmumu kis_bsmumu_
#define kis_etaulike kis_etaulike_
#define kis_gj kis_gj_
#define kis_gj_bg kis_gj_bg_
#define kis_hptaulike kis_hptaulike_
#define kis_hwg_user kis_hwg_user_
#define kis_hzz_4l kis_hzz_4l_
#define kis_hzz_4l_oscar kis_hzz_4l_oscar_
#define kis_isa_user kis_isa_user_
#define kis_mufilter kis_mufilter_
#define kis_muprod kis_muprod_
#define kis_qq_qqh2taulj kis_qq_qqh2taulj_
#define kis_qq_qqh_inv kis_qq_qqh_inv_
#define kis_simple kis_simple_
#define kis_susyh2taujj kis_susyh2taujj_
#define kis_susyh2taulj kis_susyh2taulj_
#define kis_susyh2taull kis_susyh2taull_
#define kis_taulike kis_taulike_
#define kis_ttH_had kis_ttH_had_
#define kis_ttH_lep kis_ttH_lep_
#define kis_tt_4l kis_tt_4l_
#define kis_ug kis_ug_
#define kis_user kis_user_
#define kis_zbb_4l kis_zbb_4l_
#define kis_zbb_cc_4l kis_zbb_cc_4l_
#define kis_zbb_lc_4l kis_zbb_lc_4l_
#define ksel_Hmutaujet ksel_Hmutaujet_
#define ksel_eg_ele ksel_eg_ele_
#define ksel_eg_gam ksel_eg_gam_
#define ksel_eg_lvl1 ksel_eg_lvl1_
#define ksel_eg_wwzz ksel_eg_wwzz_
#define ksel_filter_dc04 ksel_filter_dc04_
//
#else
//
#define kir_anal KIR_ANAL
#define kir_init KIR_INIT
#define kir_end KIR_END
#define kis_b_c KIS_B_C
#define kis_bbbar_ex KIS_BBBAR_EX
#define kis_bkg KIS_BKG
#define kis_bkgtth KIS_BKGTTH
#define kis_bsmumu KIS_BSMUMU
#define kis_etaulike KIS_ETAULIKE
#define kis_gj KIS_GJ
#define kis_gj_bg KIS_GJ_BG
#define kis_hptaulike KIS_HPTAULIKE
#define kis_hwg_user KIS_HWG_USER
#define kis_hzz_4l KIS_HZZ_4L
#define kis_hzz_4l_oscar KIS_HZZ_4L_OSCAR
#define kis_isa_user KIS_ISA_USER
#define kis_mufilter KIS_MUFILTER
#define kis_muprod KIS_MUPROD
#define kis_qq_qqh2taulj KIS_QQ_QQH2TAULJ
#define kis_qq_qqh_inv KIS_QQ_QQH_INV
#define kis_simple KIS_SIMPLE
#define kis_susyh2taujj KIS_SUSYH2TAUJJ
#define kis_susyh2taulj KIS_SUSYH2TAULJ
#define kis_susyh2taull KIS_SUSYH2TAULL
#define kis_taulike KIS_TAULIKE
#define kis_ttH_had KIS_TTH_HAD
#define kis_ttH_lep KIS_TTH_LEP
#define kis_tt_4l KIS_TT_4L
#define kis_ug KIS_UG
#define kis_user KIS_USER
#define kis_zbb_4l KIS_ZBB_4L
#define kis_zbb_cc_4l KIS_ZBB_CC_4L
#define kis_zbb_lc_4l KIS_ZBB_LC_4L
#define ksel_Hmutaujet KSEL_HMUTAUJET
#define ksel_eg_ele KSEL_EG_ELE
#define ksel_eg_gam KSEL_EG_GAM
#define ksel_eg_lvl1 KSEL_EG_LVL1
#define ksel_eg_wwzz KSEL_EG_WWZZ
#define ksel_filter_dc04 KSEL_FILTER_DC04
#endif

//From ROOT file TCallf77
//(From Pythia6Functions.h, I.Gonzalez Caballero)
//#ifndef WIN32
//# define type_of_call
//#else
//# define type_of_call  _stdcall
//#endif




extern "C" {
  
  // Action: User analysis for CMKIN "Reading ntuples" task
  //         Called once per event (e.g. to fill histograms from HEPEVT)
  //         Initialise the histograms in src/kine/kiuser/kir_init.F
  // Input:  None
  // Output: None
  extern "C" void type_of_call kir_anal();



  // Action: User termination for CMKIN "Reading ntuples" task
  //         N.B. histogram storage to file is taken care of automatically
  // Input:  None
  // Output: None
  extern "C" void type_of_call kir_end();



  // Action: User initialisation for CMKIN "Reading ntuples" task
  // Input:  None
  // Output: None
  extern "C" void type_of_call kir_init();





  // Action: User routine to select event within CMKIN package
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event

  extern "C" bool type_of_call kis_b_c(int *IFLAG, bool *IWANT);
  extern "C" bool type_of_call kis_bbbar_ex(int *IFLAG, bool *IWANT);
  extern "C" bool type_of_call kis_bkg(int *IFLAG, bool *IWANT);
  extern "C" bool type_of_call kis_bkgtth(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_bsmumu(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_etaulike(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_gj(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_gj_bg(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_hptaulike(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_hwg_user(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_hzz_4l(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_hzz_4l_oscar(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_isa_user(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_mufilter(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_muprod(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_qq_qqh2taulj(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_qq_qqh_inv(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_simple(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_susyh2taujj(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_susyh2taulj(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_susyh2taull(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_taulike(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_ttH_had(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_ttH_lep(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_tt_4l(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_ug(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_user(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_zbb_4l(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_zbb_cc_4l(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call kis_zbb_lc_4l(int &I_FLAG, bool *IWANT);


  // Action: User routine to select event within CMKIN package
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call ksel_Hmutaujet(int &I_FLAG, bool *IWANT);


  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call ksel_eg_ele(int &I_FLAG, bool *IWANT);


  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call ksel_eg_gam(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call ksel_eg_lvl1(int &I_FLAG, bool *IWANT);
  extern "C" bool type_of_call ksel_eg_wwzz(int &I_FLAG, bool *IWANT);

  // Action: Multi filtering routine for DC04
  extern "C" bool type_of_call ksel_filter_dc04(int &I_FLAG, bool *IWANT);

}

#endif
