def sistriplaslayout(i, p, *rows): i["SiStripLAS/Layouts/" + p] = DQMItem(layout=rows)

sistriplaslayout(dqmitems, "00 - SiStripLAS TIB&TOB",
 [{ 'path': "SiStripLAS/NumberOfSignals_AlignmentTubes",
    'description': "NumberOfSignals_AlignmentTubes</a> ", 'draw': { 'withref': "no" }}])
sistriplaslayout(dqmitems, "01 - SiStripLAS TEC+",
 [{ 'path': "SiStripLAS/NumberOfSignals_TEC+R4",
    'description': "NumberOfSignals_TEC+R4</a> ", 'draw': { 'withref': "no" }}],
 [{ 'path': "SiStripLAS/NumberOfSignals_TEC+R6",
    'description': "NumberOfSignals_TEC+R6</a> ", 'draw': { 'withref': "no" }}])
sistriplaslayout(dqmitems, "02 - SiStripLAS TEC-",
 [{ 'path': "SiStripLAS/NumberOfSignals_TEC-R4",
    'description': "NumberOfSignals_TEC-R4</a> ", 'draw': { 'withref': "no" }}],
 [{ 'path': "SiStripLAS/NumberOfSignals_TEC-R6",
    'description': "NumberOfSignals_TEC-R6</a> ", 'draw': { 'withref': "no" }}])
