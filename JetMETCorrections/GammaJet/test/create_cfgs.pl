###!/usr/local/bin/perl
#!/usr/bin/env perl

my %h_dataset_xsec = ();
my %h_dataset_name = ();


$h_dataset_xsec{'/WToENu_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 7899.;
$h_dataset_name{'/WToENu_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'WToENu_TuneZ2_7TeV-pythia6_Fall10';

$h_dataset_xsec{'/QCD_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 8.159e+08;
$h_dataset_xsec{'/QCD_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 5.312e+07;
$h_dataset_xsec{'/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 6.359e+06;
$h_dataset_xsec{'/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 7.843e+05;
$h_dataset_xsec{'/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 1.151e+05;
$h_dataset_xsec{'/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 2.426e+04;
$h_dataset_xsec{'/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 1.168e+03;
$h_dataset_xsec{'/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 7.022e+01;
$h_dataset_xsec{'/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO'} = 1.555e+01;
$h_dataset_xsec{'/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 1.844e+00;
$h_dataset_xsec{'/QCD_Pt_1000to1400_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 3.321e-01;
$h_dataset_xsec{'/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO '} = 1.087e-02;

$h_dataset_name{'/QCD_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_15to30_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_30to50_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_50to80_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_80to120_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_120to170_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_170to300_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_300to470_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_470to600_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO'} = 'QCD_Pt_600to800_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_800to1000_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_1000to1400_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 'QCD_Pt_1000to1400_TuneZ2_7TeV_pythia6';
$h_dataset_name{'/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO '} = 'QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6';

$h_dataset_xsec{'/G_Pt_0to15_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'} = 8.420e+07;
$h_dataset_xsec{'/G_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 1.717e+05;
$h_dataset_xsec{'/G_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 1.669e+04;
$h_dataset_xsec{'/G_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 2.722e+03;
$h_dataset_xsec{'/G_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 4.472e+02;
$h_dataset_xsec{'/G_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 8.417e+01;
$h_dataset_xsec{'/G_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 2.264e+01;
$h_dataset_xsec{'/G_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 1.493e+00;
$h_dataset_xsec{'/G_Pt_470to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 1.323e-01;
$h_dataset_xsec{'/G_Pt_800to1400_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 3.481e-03;
$h_dataset_xsec{'/G_Pt_1400to1800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 1.270e-05;

$h_dataset_name{'/G_Pt_0to15_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}       = 'G_Pt_0to15_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}      = 'G_Pt_15to30_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}      = 'G_Pt_30to50_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}      = 'G_Pt_50to80_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}     = 'G_Pt_80to120_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}    = 'G_Pt_120to170_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}    = 'G_Pt_170to300_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}    = 'G_Pt_300to470_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_470to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}    = 'G_Pt_470to800_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_800to1400_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}   = 'G_Pt_800to1400_TuneZ2_7TeV_pythia6_Offset_2';
$h_dataset_name{'/G_Pt_1400to1800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'}  = 'G_Pt_1400to1800_TuneZ2_7TeV_pythia6_Offset_2';


while ( my ($key, $value) = each(%h_dataset_name) ) {
  #print "$key => $value\n";
  #`cp config_NOTRIG_MC_TEMPL_cfg.py config_NOTRIG_MC_$value_cfg.py`;
  $new_config = "config_NOTRIG_MC_".$value."_cfg.py";
  $xsec = $h_dataset_xsec{$key};
  #`cat $new_name | sed -e \"s%CHANGECROSSECTION%${xsec}%g\" > ${templatePython}_${lastRun}_${lastLS}.py`;
  `cat config_NOTRIG_MC_TEMPL_cfg.py | sed "s/CHANGECROSSECTION/${xsec}/" > $new_config`;

  $new_crab = "crab_MC_".$value.".cfg";
  # make the dataset sed-readable:
  $key =~ s/\//\\\//g;
  `cat crab_MC_TEMPL.cfg | sed -e "s/CHANGEDATASET/${key}/" -e "s/CHANGECONFIG/${new_config}/" -e "s/CHANGEOUTDIR/${value}/" -e "s/CHANGEWORKINGDIR/${value}/" > $new_crab`;
}

