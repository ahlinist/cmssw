LAS=DetDiagLaserReader
LED=DetDiagLEDReader
PED=DetDiagPedestalReader

all: 
	if [ -h $(LAS) ]; then rm $(LAS); fi
	if [ -h $(PED) ]; then rm $(PED); fi
	if [ -h $(LED) ]; then rm $(LED); fi
	scramv1 b -r
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(LAS) $(LAS)
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(LED) $(LED)
	ln -s $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(PED) $(PED)
	
	

