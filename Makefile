CXXFLAGS += -isystem SeqLib/ -isystem SeqLib/htslib/ -O3 -Wall -std=gnu++14
LDFLAGS += -static -LSeqLib/bin
LIBS += -lseqlib  -lfml -lbwa -lhts -lz -lpthread


chlasm: chlasm.cc chlasm.hh
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

