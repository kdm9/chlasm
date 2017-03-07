# Copyright (c) 2017 Kevin Murray <kdmfoss@gmail.com>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
