echo start
date
mkdir workarea
cd workarea
mkdir db
mkdir xml
cd db
rm *
touch test.db

echo start write Mag
date
cmsRun ../../testwriteMag.cfg >twMag.out
echo end write Mag
date

mv dumpGeoHistory dumpGeoHistoryMagWrite
mv dumpSpecs dumpSpecsMagWrite

echo start write Ideal
date
cmsRun ../../testwriteIdeal.cfg >twIdeal.out
echo end write Ideal
date

mv dumpGeoHistory dumpGeoHistoryIdealWrite
mv dumpSpecs dumpSpecsIdealWrite
echo done with all DB writes.

echo start all DB reads.
echo start DB read both
date
cmsRun ../../testreadBothDB.cfg > trbdb.out
echo done with read DB both
date

mv dumpGeoHistory dumpGeoHistoryDBBothRead
mv dumpSpecs dumpSpecsDBBothRead

echo start DB read ideal only
date
cmsRun ../../testreadDBIdealOnly.cfg >trIdb.out
echo end DB read ideal only
date

mv dumpGeoHistory dumpGeoHistoryDBIdealRead
mv dumpSpecs dumpSpecsDBIdealRead

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
rm *

echo start XML read both
date
cmsRun ../../testreadBothXML.cfg > trbxml.out
echo end XML read both
date

mv dumpGeoHistory dumpGeoHistoryXMLBothRead
mv dumpSpecs dumpSpecsXMLBothRead

echo start XML read ideal only
date
cmsRun ../../testreadXMLIdealOnly.cfg >trIxml.out
echo end XML read ideal only

mv dumpGeoHistory dumpGeoHistoryXMLIdealRead
mv dumpSpecs dumpSpecsXMLIdealRead

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
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryXMLBothRead
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryXMLIdealRead
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryXMLMagRead
cd ../db
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryDBBothRead
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryDBIdealRead
sed -i '{s/-0.0000/ 0.0000/g}' dumpGeoHistoryDBMagRead
cd ../xml

date
echo this will show if there are any inconsistencies when reading both Ideal and Magnet
diff dumpGeoHistoryXMLBothRead ../db/dumpGeoHistoryDBBothRead > diffgeomBoth.out

date
echo this will show if there are inconsistencies when only reading the Ideal
diff dumpGeoHistoryXMLIdealRead ../db/dumpGeoHistoryDBIdealRead > diffgeomIdealOnly.out

date
echo this will show if there are inconsistencies when only reading the Ideal
diff dumpGeoHistoryXMLMagRead ../db/dumpGeoHistoryDBMagRead > diffgeomMagOnly.out

echo ALL DONE!


