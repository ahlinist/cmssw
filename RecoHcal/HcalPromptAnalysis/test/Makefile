LAS=DetDiagLaserReader
PED=DetDiagPedestalReader

all: 
	if [ -h $(LAS) ]; then rm  $(LAS); fi
	if [ -h $(PED) ]; then rm  $(PED); fi
	scramv1 b -r
	ln -s -f $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(LAS) $(LAS)
	ln -s -f $(CMSSW_BASE)/test/$(SCRAM_ARCH)/$(PED) $(PED)
	
	

