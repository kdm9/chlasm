SL := ext/SeqLib
CXXFLAGS += -isystem $(SL) -isystem $(SL)/htslib/ -O3 -Wall -std=gnu++14 -g
LDFLAGS += -static
LIBS = $(SL)/src/libseqlib.a $(SL)/fermi-lite/libfml.a \
       $(SL)/bwa/libbwa.a $(SL)/htslib/libhts.a
LIBS += -lz -lpthread


chlasm: chlasm.cc chlasm.hh $(SL)/src/libseqlib.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

$(SL)/src/libseqlib.a:
	cd ext/seqlib && ./configure && $(MAKE)
