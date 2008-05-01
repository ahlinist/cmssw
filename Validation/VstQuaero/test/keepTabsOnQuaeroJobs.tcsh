#!/bin/tcsh

setenv done 0
while ( $done == 0 )
  setenv done 1
  foreach i ( $* )
    if ( -e ~knuteson/Public/Quaero/quaero_development/requests/$i/answer.txt ) then
      if ( `cat ~knuteson/Public/Quaero/quaero_development/requests/$i/answer.txt | grep failed | wc -l` == 1 ) then
        echo "Quaero request $i failed." > tmp_$$.txt
        /usr/lib/sendmail knuteson@fnal.gov < tmp_$$.txt
        rm tmp_$$.txt
        exit
      endif
      rm ~knuteson/Public/Quaero/quaero_development/requests/$i/.htaccess >& /dev/null
    else
      setenv done 0
    endif
  end   
  sleep 60
end

echo "Quaero requests $* are done." > tmp_$$.txt
/usr/lib/sendmail knuteson@fnal.gov < tmp_$$.txt
rm tmp_$$.txt
