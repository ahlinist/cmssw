LAS=DetDiagLaserReader
LED=DetDiagLEDReader
PED=DetDiagPedestalReader
TEMP=DetDiagSiPMTempReader

all: 
	if [ -h $(LAS) ]; then rm $(LAS); fi
	if [ -h $(PED) ]; then rm $(PED); fi
	if [ -h $(LED) ]; then rm $(LED); fi
	if [ -h $(TEMP) ]; then rm $(TEMP); fi
	scramv1 b -r
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(LAS) $(LAS)
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(LED) $(LED)
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(PED) $(PED)
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(TEMP) $(TEMP)
	
	

