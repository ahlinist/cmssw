#!/usr/bin/perl5.6.1
###############################################
# Usage:  perl runTestJob.pl <passwd> <signal> <x> <y> <a> <b> <c>
###############################################


$passwd = $ARGV[0];
$signal = $ARGV[1];
$x = $ARGV[2];
$y = $ARGV[3];
$a = $ARGV[4];
$b = $ARGV[5];
$targetTime = 1;
$edition = "quaero_development";
$inputType = "PythiaInput";

if ( $signal eq "top_massXsec" )
{
	$pythiaCommands = "msel=6 \n pmas(6,1)=$x \n xsec=$y \n";
}
if ( $signal eq "rsGraviton" )
{
    $kOverMpl = $y;
    $coupling = 5.4*$kOverMpl;
    $pythiaCommands = "msub(391)=1 \n msub(392)=1 \n pmas(347,1)=$x \n parp(50)=$coupling \n";
}
if ( $signal eq "rightHandedTop" )
{
	$inputType = "MadEventInput";
	$rightHandedCoupling = $x;
	`cat mad_rightHandedTop.txt | sed -e 's/right_handed_coupling/'$rightHandedCoupling'/' > tmp.madevent`;
	$inputFile = "tmp.madevent";
}

if ( $signal eq "lq" )
{
    if(0==1)
    {
	$pythiaCommands = "msub(145)=1 \n paru(151)=1 \n kfdp(539,1)=2 \n kfdp(539,2)=-11 \n pmas(42,1)=$x \n mrpy(1)=$b \n";
#	$targetTime = 3;
    }
    else
    {
	$inputType = "MadEventInput";
	$lq_mass = $x;
	$lambda = $y;
	$lq_q = $a;
	$lq_l = $b;
	$lq_spin = $ARGV[6];
	$lq_width = 1;
	if( $lq_spin eq "s" )
	{
	    $lq_width = 1./(4*4*3.141)*($lambda*$lambda)*$lq_mass;	
	}
	if( $lq_spin eq "v" )
	{
	    $lq_width = 1./(6*4*3.141)*($lambda*$lambda)*$lq_mass;	
	}
	`cat mad_lq_${lq_spin}.txt | sed -e 's/lq_mass/'$lq_mass'/' > tmp1.madevent`;
	`cat tmp1.madevent | sed -e 's/lq_width/'$lq_width'/' > tmp2.madevent`;
	`cat tmp2.madevent | sed -e 's/lambda/'$lambda'/' > tmp3.madevent`;
	`cat tmp3.madevent | sed -e 's/lq_spin/'$lq_spin'/' > tmp4.madevent`;
	`cat tmp4.madevent | sed -e 's/ lq_q / '$lq_q' /' > tmp5.madevent`;
	`cat tmp5.madevent | sed -e 's/ lq_l / '$lq_l' /' > tmp.madevent`;
	`rm tmp[12345].madevent`;
	$inputFile = "tmp.madevent";
    }
}


if ( $signal eq "excitedElectron" )
{
#    $pythiaCommands = "msub(146)=1 \n rtcm(43)=0.3 \n rtcm(44)=0.3 \n rtcm(45)=0.0 \n rtcm(41)=$y \n pmas(345,1)=$x \n";
    $pythiaCommands = "msub(146)=1 \n rtcm(43)=0.287 \n rtcm(44)=1 \n rtcm(45)=0.0 \n rtcm(41)=$y \n pmas(345,1)=$x \n";
}
if ( $signal eq "llX" )
{
    $pythiaCommands = "msub(35)=1 \n kfactor=$x \n";
    `echo 'Make sure you have excluded llX from the background estimate!'`;
}
if ( $signal eq "mSugra" )
{
#    $generateHiggs = "msub(3)=1 \n msub(24)=1 \n msub(26)=1 \n msub(102)=1 \n msub(103)=1 \n msub(123)=1 \n msub(124)=1 \n msub(153)=1 \n msub(158)=1 \n msub(171)=1 \n msub(173)=1 \n msub(174)=1 \n msub(176)=1 \n msub(178)=1 \n msub(179)=1 \n msub(143)=1 \n ";
#    $generateSusy = "msub(201)=1 \n msub(202)=1 \n msub(203)=1 \n msub(204)=1 \n msub(205)=1 \n msub(206)=1 \n msub(207)=1 \n msub(208)=1 \n msub(209)=1 \n msub(210)=1 \n msub(211)=1 \n msub(212)=1 \n msub(213)=1 \n msub(214)=1 \n msub(216)=1 \n msub(217)=1 \n msub(218)=1 \n msub(219)=1 \n msub(220)=1 \n msub(221)=1 \n msub(222)=1 \n msub(223)=1 \n msub(224)=1 \n msub(225)=1 \n msub(226)=1 \n msub(227)=1 \n msub(228)=1 \n msub(229)=1 \n msub(230)=1 \n msub(231)=1 \n msub(232)=1 \n msub(233)=1 \n msub(234)=1 \n msub(235)=1 \n msub(236)=1 \n msub(237)=1 \n msub(238)=1 \n msub(239)=1 \n msub(240)=1 \n msub(241)=1 \n msub(242)=1 \n msub(243)=1 \n msub(244)=1 \n msub(246)=1 \n msub(247)=1 \n msub(248)=1 \n msub(249)=1 \n msub(250)=1 \n msub(251)=1 \n msub(252)=1 \n msub(253)=1 \n msub(254)=1 \n msub(256)=1 \n msub(258)=1 \n msub(259)=1 \n msub(261)=1 \n msub(262)=1 \n msub(263)=1 \n msub(264)=1 \n msub(265)=1 \n msub(271)=1 \n msub(272)=1 \n msub(273)=1 \n msub(274)=1 \n msub(275)=1 \n msub(276)=1 \n msub(277)=1 \n msub(278)=1 \n msub(279)=1 \n msub(280)=1 \n msub(281)=1 \n msub(282)=1 \n msub(283)=1 \n msub(284)=1 \n msub(285)=1 \n msub(286)=1 \n msub(287)=1 \n msub(288)=1 \n msub(289)=1 \n msub(290)=1 \n msub(291)=1 \n msub(292)=1 \n msub(293)=1 \n msub(294)=1 \n msub(295)=1 \n msub(296)=1 \n ";
    $generateHiggs = "";
    $generateSusy = "msel=39 \n ";
    $sgnMu = 1;
    $tanBeta = 10;
    $generateSusyParameters = "imss(1) = 2 \n rmss(1) = $y \n rmss(4) = $sgnMu \n rmss(5) = $tanBeta \n rmss(8) = $x \n rmss(16)= 0.0 \n kfactor=1.0 \n";
    $pythiaCommands = "$generateHiggs $generateSusy $generateSusyParameters";
}
if ( $signal eq "susyBenchmark" )
{
    $pythiaCommands = "`cat Quaero/doc/examples/susyBenchmark${x}.txt`";
}
if ( $signal eq "smHiggs" )
{
#    $pythiaCommands = "pmas(25,1)=$x \n msub(24)=1 \n msub(26)=1 \n msub(123)=1 \n msub(124)=1 \n kfactor=1 \n mrpy(1)=$b  ";
    $pythiaCommands = "pmas(25,1)=$x \n msub(24)=1 \n msub(26)=1 \n msub(123)=1 \n msub(124)=1 \n kfactor=1 \n mrpy(1)=$b  ";
    $targetTime = 1;
}
if ( $signal eq "smHiggs1" )
{
    $pythiaCommands = "kfactor=$y  \n  pmas(25,1)=$x \n msub(24)=1     \n msub(26)=1     \n mdme(210,1)=1  \n mdme(211,1)=1  \n mdme(212,1)=1  \n mdme(213,1)=1  \n mdme(214,1)=1  \n mdme(215,1)=1  \n mdme(218,1)=1  \n mdme(219,1)=1  \n mdme(220,1)=1  \n mdme(222,1)=1  \n mdme(223,1)=1  \n mdme(224,1)=1  \n mdme(225,1)=1  \n mdme(226,1)=1  \n mdme(190,1)=0  \n mdme(191,1)=0  \n mdme(194,1)=0  \n mdme(195,1)=0  \n mdme(198,1)=0  \n mdme(199,1)=0  \n mdme(206,1)=1  \n mdme(207,1)=1  \n mdme(208,1)=1  \n mdme(174,1)=0  \n mdme(175,1)=0  \n mdme(176,1)=0  \n mdme(177,1)=0  \n mdme(178,1)=0  \n mdme(182,1)=1  \n mdme(183,1)=0  \n mdme(184,1)=1  \n mdme(185,1)=0  \n mdme(186,1)=1  \n mdme(187,1)=0 "; 
    $targetTime = 1;
}
if ( $signal eq "smHiggsGammaGamma" )
{
    $pythiaCommands = "pmas(25,1)=$x \n msub(24)=1 \n msub(26)=1 \n msub(123)=1 \n msub(124)=1 \n kfactor=$y \n mrpy(1)=$b \n mdme(210,1)=-1 \n mdme(211,1)=-1 \n mdme(212,1)=-1 \n mdme(213,1)=-1 \n mdme(214,1)=-1 \n mdme(215,1)=-1 \n mdme(218,1)=-1 \n mdme(219,1)=-1 \n mdme(220,1)=-1 \n mdme(222,1)=-1 \n mdme(224,1)=-1 \n mdme(225,1)=-1 \n mdme(226,1)=-1 \n kfactor=$y ";
    $targetTime = 1;
}
if ( $signal eq "chargedHiggs" )
{
    $pythiaCommands = "msub(301) = 1 \n pmas(37,1) = $x \n";
    $targetTime = 1;
}
if ( $signal eq "doublyChargedHiggs" )
{
    if(0==1)
# old HERA analysis
    {
	$inputType = "SignalFile";
	$inputFile = "/disk/data12b/highpt/Quaero_experiments/h1_files/signal/doublyChargedHiggs/htm_m${x}_g$y.stdhep";
    }
    else
# new LEP2 ALEPH analysis
    {
	$pythiaCommands = "msub(349)=1 \n msub(350)=1 \n pmas(353,1)=$x \n pmas(354,1)=$x \n";
    }
}
if ( $signal eq "chargedHiggs" )
{
    $pythiaCommands = "msub(301) = 1 \n pmas(37,1) = $x \n";
    $targetTime = 1;
}
if ( $signal eq "doublyChargedHiggs_ee" )
{
    $inputType = "MadEventInput";
    $hpp_mass = $x;
    $h_ee = $y;
    $hpp_width = 1./(16*3.141)*($h_ee*$h_ee)*$hpp_mass;
    `cat mad_doublyChargedHiggs_ee.txt | sed -e 's/hpp_mass/'$hpp_mass'/' > tmp1.madevent`;
    `cat tmp1.madevent | sed -e 's/hpp_width/'$hpp_width'/' > tmp2.madevent`;
    `cat tmp2.madevent | sed -e 's/h_ee/'$h_ee'/' > tmp.madevent`;
    `rm tmp[12].madevent`;
    $inputFile = "tmp.madevent";
}
if ( $signal eq "singleTop" )
{
# old HERA single top, with Emmanuelle's files
    if(0==1)
    {
	$inputType = "SignalFile";
	$inputFile = "/disk/data12b/highpt/Quaero_experiments/h1_files/signal/singleTop/anotop_k$x.stdhep";
    }
# new LEP+HERA single top
    else
    {
	$inputType = "MadEventInput";
	$topQuarkMass = 175.;
	$alpha = 1./128.;
	$e0 = (4*3.141592*$alpha)**0.5;
	$thetaW = 0.5015;
	$q_u = 2./3;
	$lambda = $topQuarkMass;
	$g = $e0/sin($thetaW);
	$kappa_gamma_coupling = $x*$e0*$q_u/($lambda);
	$kappa_z_coupling =     $y*$g/(2.*cos($thetaW)*$lambda);
	`cat mad_singleTop.txt | sed -e 's/kappa_gamma_coupling/'$kappa_gamma_coupling'/' > tmp1.madevent`;
	`cat tmp1.madevent | sed -e 's/kappa_z_coupling/'$kappa_z_coupling'/' > tmp.madevent`;
	`rm tmp[1].madevent`;
	$inputFile = "tmp.madevent";
    }
}
if ( $signal eq "rpvSusy" )
{
    $inputType = "SignalFile";
    $inputFile = "/disk/data12b/highpt/Quaero_experiments/h1_files/signal/rpvSusy/sq${x}_lambda${y}.stdhep";
}
if ( $signal eq "muonSneutrinoLsp" )
{
    $inputType = "MadEventInput";
    `cat mad_muonSneutrinoLsp.txt | sed -e 's/CHIMASS/'$x'/' > tmp.madevent`;
    $inputFile = "tmp.madevent";
}
if ( $signal eq "excitedQuark" )
{
    $inputType = "MadEventInput";
    $f = $x;
    $fPrime = $y;
    $mass_excited_u_quark = $a;
    $mass_excited_d_quark = $b;
    $lambda = 1.*$ARGV[6];
    $fs = 1.*$ARGV[7]+0.00000001;

# These couplings are the negative of the couplings as specified in Brigitte's thesis
    $alpha = 1./128;
    $alpha_s = 0.118;
    $e0 = (4*3.141592*$alpha)**0.5;
    $g_s = (4*3.141592*$alpha_s)**0.5;
    $thetaW = 0.5015;
    $q_u = 2./3;
    $q_d = -1./3;

    $tL_u = 0.5;
    $tL_d = -0.5;
    $f_u_gamma_lambda = -($q_u*$fPrime+$tL_u*($f-$fPrime))/2/$lambda;
    $f_u_w_lambda = -($f/(2**0.5 *sin($thetaW)))/2/$lambda;
    $f_u_z_lambda = -($tL_u*(cos($thetaW)**2 * $f + sin($thetaW)**2 * $fPrime) - q_u*sin($thetaW)**2 *$fPrime)/(sin($thetaW)*cos($thetaW))/2/$lambda;
    $f_d_gamma_lambda = -($q_d*$fPrime+$tL_d*($f-$fPrime))/2/$lambda;
    $f_d_w_lambda = -($f/(2**0.5 *sin($thetaW)))/2/$lambda;
    $f_d_z_lambda = -($tL_d*(cos($thetaW)**2 * $f + sin($thetaW)**2 * $fPrime) - q_d*sin($thetaW)**2 *$fPrime)/(sin($thetaW)*cos($thetaW))/2/$lambda;
#    $f_u_gamma_lambda = -0.667/2*$f_over_lambda;
#    $f_u_w_lambda = -1.47/2*$f_over_lambda;
#    $f_u_z_lambda = -0.825/2*$f_over_lambda;
#    $f_d_gamma_lambda = 0.333/2*$f_over_lambda;
#    $f_d_w_lambda = -1.47/2*$f_over_lambda;
#    $f_d_z_lambda = 1.00/2*$f_over_lambda;
    $f_q_g_lambda = $fs/2/$lambda;

    $width_excited_u_quark = 1;
    $width_excited_d_quark = 1;	

    $mass_gamma = 0.;
    $mass_w = 80.4;
    $mass_z = 91.1;
    $width_excited_u_quark = 0;
    $width_excited_u_quark += 
	($alpha_s)*
	($f_q_g_lambda ** 2)*
	($mass_excited_u_quark ** 3);
    $width_excited_u_quark += 
	($alpha)*
	($f_u_gamma_lambda ** 2)*
	($mass_excited_u_quark ** 3)*
	((1-($mass_gamma/$mass_excited_u_quark)**2) ** 2)*
	(1+(($mass_gamma/$mass_excited_u_quark)**2)/2);
    $width_excited_u_quark += 
	($alpha)*
	($f_u_w_lambda ** 2)*
	($mass_excited_u_quark ** 3)*
	((1-($mass_w/$mass_excited_u_quark)**2) ** 2)*
	(1+(($mass_w/$mass_excited_u_quark)**2)/2);
    $width_excited_u_quark += 
	($alpha)*
	($f_u_z_lambda ** 2)*
	($mass_excited_u_quark ** 3)*
	((1-($mass_z/$mass_excited_u_quark)**2) ** 2)*
	(1+(($mass_z/$mass_excited_u_quark)**2)/2);
    
    $width_excited_d_quark = 0;
    $width_excited_d_quark += 
	($alpha_s)*
	($f_q_g_lambda ** 2)*
	($mass_excited_d_quark ** 3);
    $width_excited_d_quark += 
	($alpha)*
	($f_d_gamma_lambda ** 2)*
	($mass_excited_d_quark ** 3)*
	((1-($mass_gamma/$mass_excited_d_quark)**2) ** 2)*
	(1+(($mass_gamma/$mass_excited_d_quark)**2)/2);
    $width_excited_d_quark += 
	($alpha)*
	($f_d_w_lambda ** 2)*
	($mass_excited_d_quark ** 3)*
	((1-($mass_w/$mass_excited_d_quark)**2) ** 2)*
	(1+(($mass_w/$mass_excited_d_quark)**2)/2);
    $width_excited_d_quark += 
	($alpha)*
	($f_d_z_lambda ** 2)*
	($mass_excited_d_quark ** 3)*
	((1-($mass_z/$mass_excited_d_quark)**2) ** 2)*
	(1+(($mass_z/$mass_excited_d_quark)**2)/2);

#Force the widths to 1 GeV
#    $width_excited_u_quark = 1;
#    $width_excited_d_quark = 1;	

    $f_u_gamma_lambda *= $e0*2;
    $f_u_w_lambda *= $e0*2;
    $f_u_z_lambda *= $e0*2;
    $f_d_gamma_lambda *= $e0*2; 
    $f_d_w_lambda *= $e0*2;
    $f_d_z_lambda *= $e0*2;
    $f_q_g_lambda *= $g_s*2;

    `cat mad_excitedQuark.txt | sed -e 's/f_u_gamma\\/lambda/'${f_u_gamma_lambda}'/' > tmp1.madevent`;
    `cat tmp1.madevent | sed -e 's/f_u_w\\/lambda/'$f_u_w_lambda'/' > tmp2.madevent`;
    `cat tmp2.madevent | sed -e 's/f_u_z\\/lambda/'$f_u_z_lambda'/' > tmp3.madevent`;
    `cat tmp3.madevent | sed -e 's/f_d_gamma\\/lambda/'${f_d_gamma_lambda}'/' > tmp4.madevent`;
    `cat tmp4.madevent | sed -e 's/f_d_w\\/lambda/'${f_d_w_lambda}'/' > tmp5.madevent`;
    `cat tmp5.madevent | sed -e 's/f_d_z\\/lambda/'${f_d_z_lambda}'/' > tmp6.madevent`;
    `cat tmp6.madevent | sed -e 's/mass_excited_u_quark/'$mass_excited_u_quark'/' > tmp7.madevent`;
    `cat tmp7.madevent | sed -e 's/mass_excited_d_quark/'$mass_excited_d_quark'/' > tmp8.madevent`;
    `cat tmp8.madevent | sed -e 's/width_excited_u_quark/'$width_excited_u_quark'/' > tmp9.madevent`;
    `cat tmp9.madevent | sed -e 's/width_excited_d_quark/'$width_excited_d_quark'/' > tmp10.madevent`;
    `cat tmp10.madevent | sed -e 's/f_q_g\\/lambda/'$f_q_g_lambda'/' > tmp.madevent`;
    `rm tmp[1-9]*.madevent`;
    $inputFile = "tmp.madevent";

    $x = $mass_excited_u_quark; $y = $f/$lambda;
    $a=$ARGV[7]; $b=0;
}

if( $inputType eq "PythiaInput" )
{
    `echo '$pythiaCommands' > tmp.pythia`;
    $inputFile = "tmp.pythia";
}


print `perl ../../bin/submitQuaeroRequest.pl "$edition" "$inputType" "$inputFile" "$signal ($x,$y)($a,$b)" "$targetTime" "$passwd"`;




















