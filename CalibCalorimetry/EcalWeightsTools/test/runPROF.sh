sed -e "s/rrrrr/$1/1" buildprofile2006Pool.config > buildprofile2006Pool_${1}.cfg
cmsRun buildprofile2006Pool_${1}.cfg
rm -rf buildprofile2006Pool_${1}.cfg
rm -rf core.*
