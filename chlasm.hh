// Copyright (c) 2017 Kevin Murray <kdmfoss@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef CHLASM_HH_VL9AJH3R
#define CHLASM_HH_VL9AJH3R

#include <vector>
#include <string>
#include <iostream>

#include "SeqLib/BamReader.h"
#include "SeqLib/FermiAssembler.h"

namespace chlasm {

using namespace SeqLib;
using std::string;
using std::vector;

class ChlAsm {
public:

    ChlAsm ()
        : verbosity_(0)
        , num_reads_(0)
    {
    }

    void set_verbosity(int verb)
    {
        verbosity_ = verb;
    }

    ssize_t add_reads(const string &bam, const string &tgt) 
    {
        // read in data from a BAM
        BamReader br;
        br.Open(bam);

        GenomicRegion reg(tgt, br.Header());
        br.SetRegion(reg);
        
        // retreive sequencing reads (up to 20,000)
        BamRecord r;
        size_t read_count = 0;
        while(br.GetNextRecord(r)) {
            read_count++;
            target_reads_.push_back(r);
        }
        num_reads_ += read_count;
        return read_count;
    }

    vector<string> perform_assembly()
    {
        FermiAssembler f;

        f.SetAggressiveTrim();
        f.SetMinOverlap(10);
        f.SetKmerMaxThreshold(1000);

        // add the reads and error correct them
        f.AddReads(target_reads_);
        f.CorrectReads();

        // peform the assembly
        f.PerformAssembly();

        // retrieve the contigs
        auto contigs = f.GetContigs();
        return contigs;
    }

    size_t num_reads() const {
        return num_reads_;
    }

private:
    BamRecordVector target_reads_;
    int verbosity_ = 0; 
    size_t num_reads_ = 0;
}; // class ChlAsm

}; // namespace chlasm


#endif /* end of include guard: CHLASM_HH_VL9AJH3R */
