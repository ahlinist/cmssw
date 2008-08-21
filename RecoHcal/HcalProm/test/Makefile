include Makefile.arch
#----------------------------------------------------------------------------------
TESTS           := Golden.C
TESTO           := $(TESTS:.C=.o)
TEST            := Golden

OBJ             = $(TESTO)
PROGRAMS        = $(TEST)

CXXFLAGS      += 
CXXFLAGS      += 
GLIBS         += 
#----------------------------------------------------------------------------------
.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(TEST):        $(TESTO)
		$(LD) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt)$@
		@echo "$@ done"

clean:
		@rm -f $(OBJ) $(TESTO) core *~ src/*~ include/*~

.$(SrcSuf).$(ObjSuf):
		$(CXX) $(CXXFLAGS) -c $< -o $@
