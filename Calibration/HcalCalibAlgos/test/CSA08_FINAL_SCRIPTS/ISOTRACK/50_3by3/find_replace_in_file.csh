#!/bin/tcsh -f
#

set i=1
set j=54

while ( $i <= $j )
#   sed -i 's/20000/-1/g' isoCal$i.cfg
#   sed -i 's/iso1.root/iso.root/g' isoCal$i.cfg	
#   sed -i 's/coef_without_noise_10mln_pure.txt/multiplied_coef_without_noise_10mln_pure_IsoTrackCoef_v1.txt/g' isoCal$i.cfg
#   sed -i 's/multiplied_coef_without_noise_10mln_pure_IsoTrackCoef_v1.txt/multiplied_coef_without_noise_10mln_pure_IsoTrackCoef_v1_w_20_50_scale.txt/g' isoCal$i.cfg
    sed -i 's/S156_before_conditions.txt/S156_w_HB_scaled_w_MC.txt/g' isoCal50GeV_1pb_3by3_$i.cfg 	
#    sed -i 's/multiplied_coef_without_noise_10mln_pure_IsoTrackCoef_v1_w_20_50_scale.txt/coef_without_noise_10mln_pure_1pb.txt/g' isoCal1pb_$i.cfg 
   @ i++
end 
