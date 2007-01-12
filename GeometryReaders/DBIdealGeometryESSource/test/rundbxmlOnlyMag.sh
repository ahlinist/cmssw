echo start
date
cd workarea
cd db

rm testMag.db
rm twMag.out
rm trMdb.out
rm dumpSpecsDBMagRead
rm dumpGeoHistoryDBMagRead
rm dumpGeoHistoryMagWrite
rm dumpSpecsMagWrite
rm *.log
rm PoolFileCatalog.xml*

#uncomment these with 1_2_0_pre4
touch testMag.db

echo start write Mag
date
cmsRun ../../testwriteMag.cfg >twMag.out
echo end write Mag
date

mv dumpGeoHistory dumpGeoHistoryMagWrite
mv dumpSpecs dumpSpecsMagWrite


echo start DB read mag only
date
cmsRun ../../testreadDBMagOnly.cfg >trMdb.out
echo end DB read mag only
date

mv dumpGeoHistory dumpGeoHistoryDBMagRead
mv dumpSpecs dumpSpecsDBMagRead

echo end all DB reads
date
cd ../xml
rm trMxml.out
rm dumpGeoHistoryXMLMagRead
rm dumpSpecsXMLMagRead

echo start XML read mag only
date
cmsRun ../../testreadXMLMagOnly.cfg >trMxml.out
echo end XML read mag only
date

mv dumpGeoHistory dumpGeoHistoryXMLMagRead
mv dumpSpecs dumpSpecsXMLMagRead
echo done with reading XML

echo doing seds to replace -0 with 0.
date
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryXMLMagRead
cd ../db
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryDBMagRead
cd ../xml

date
echo this will show if there are inconsistencies when only reading the Ideal
diff dumpGeoHistoryXMLMagRead ../db/dumpGeoHistoryDBMagRead > diffgeomMagOnly.out

echo ALL DONE!


