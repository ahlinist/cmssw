#!/bin/sh

function make_diff(){
    file=$1
    echo "only in ${file}_w"
    sdiff ${file}_w ${file}_r | grep "<" | wc -l
    echo "only in ${file}_p"
    sdiff ${file}_w ${file}_r | grep ">" | wc -l
    echo "differ"
    sdiff ${file}_w ${file}_r | grep "|" | wc -l

    echo out of `wc -l ${file}_r`
}

function compare_conn() {
    rm -f conn_w conn_r
    echo "parse ${out_write}"
    grep "Connections ifed" ${out_write} | awk '{print $3" "$7" "$11" "$15" "$19}' > conn_w
    grep "Connections ifed" ${out_read}  | awk '{print $3" "$5" "$7" "$11" "$13}' > conn_r

    make_diff conn
}

function compare_noise() {
    rm -f noise_w noise_r
    echo "parse ${out_write}"
    grep "Noise detid" ${out_write} | awk '{print $3" "$5" "int($6*10+.5)/10" "$7}' > noise_w
    echo "parse ${out_read}"
    grep "Noise detid" ${out_read}  | awk '{print $3" "$5" "$6" "$7}' > noise_r
    
    make_diff noise
}

function compare_ped() {
    rm -f ped_w ped_r
    echo "parse ${out_write}"
    grep "Pedestals detid" ${out_write} | awk '{print $3" "$5" "int($6)" "int($7*5+.5)/5." "int($8*5+.5)/5.}' > ped_w
    echo "parse ${out_read}"
    grep "Pedestals detid" ${out_read}  | awk '{print $3" "$5" "$6" "$7" "$8}' > ped_r
    
    make_diff ped
}



############
# MAIN
############

out_write=out_write
out_read=out_read
