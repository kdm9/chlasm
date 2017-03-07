#include <getopt.h>
#include <cstring>

#include "chlasm.hh"

using namespace chlasm;
using std::cerr;
using std::endl;

void printhelp()
{
    cerr << "USAGE:" << endl
         << "    chlasm [options]" << endl
         << endl
         << "OPTIONS:" << endl
         << "    -t TARGET      Target coordinates in samtools form, e.g. chr1:1-100" << endl
         << "    -b BAMFILE     Input BAM filename" << endl;
}

int main(int argc, char *argv[])
{
    string bamfile;
    vector<string> targets;
    ChlAsm chlasm;

    int c;
    while ((c = getopt(argc, argv, "b:t:hv")) > 0) {
        int ret = EXIT_FAILURE;
        switch (c) {
            case 'b':
                bamfile = string(optarg);
                break;
            case 't':
                cerr << optarg << endl;
                targets.push_back(string(optarg));
                break;
            case 'v':
                chlasm.set_verbosity(atoi(optarg));
                break;
            case 'h':
                ret = EXIT_SUCCESS;
            default:
                printhelp();
                return ret;
        }
    }

    if (bamfile.size() < 1 || targets.size() < 1) {
        printhelp();
        return EXIT_FAILURE;
    }

    for (auto &target: targets) {

        try {
            ssize_t nread = chlasm.add_reads(bamfile, target);
            cerr << "Added " << nread << " reads that matched '" << target << "'" << endl;
        } catch (std::exception &e) {
            cerr << "Failed to add reads that matched '" << target << "' in '" << 
                bamfile << "'" << endl;
            cerr << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    if (chlasm.num_reads() < 1) {
        cerr << "No on-target reads have been found. Check target range and bamfile" << endl;
        return EXIT_FAILURE;
    }

    // retrieve the contigs
    try {
        vector<string> contigs = chlasm.perform_assembly();
        // write as a fasta to stdout
        for (size_t i = 0; i < contigs.size(); ++i) {
            std::cout << ">contig" << i << std::endl << contigs[i] << std::endl;
        }
    } catch (std::exception &e) {
        cerr << "Failed to assemble contigs" << endl;
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return 0;
}
