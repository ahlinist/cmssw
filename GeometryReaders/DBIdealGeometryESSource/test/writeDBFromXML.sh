echo start
date
mkdir workarea
cd workarea
mkdir db
mkdir xml
cd db
rm *
#uncomment these with 1_2_0_pre4
#touch testMag.db
#touch testIdeal.db

echo start write Mag
date
cmsRun ../../testwriteMagQuiet.cfg
echo end write Mag
date

echo start write Ideal
date
cmsRun ../../testwriteIdealQuiet.cfg
echo end write Ideal
date

echo done with all DB writes.

echo ALL DONE!
echo =====================================
echo    To test that this worked you can try testreadIdealOnlyQuiet.cfg and testreadDBMagOnlyQuiet.cfg.
echo    These will read from the database and do very minimal output if they succeed.
echo =====================================


