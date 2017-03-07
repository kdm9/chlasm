#include <getopt.h>

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
    while ((c = getopt(argc, argv, "bthv")) > 0) {
        int ret = EXIT_FAILURE;
        switch (c) {
            case 'b':
                bamfile = string(optarg);
                break;
            case 't':
                targets.push_back(optarg);
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
        ssize_t nread = chlasm.add_reads(bamfile, target);
        cerr << "Added " << nread << " reads that matched '" << target << "'" << endl;
    }

    if (chlasm.num_reads() < 1) {
        cerr << "No on-target reads have been found. Check target range and bamfile" << endl;
        return EXIT_FAILURE;
    }

    // retrieve the contigs
    vector<string> contigs = chlasm.perform_assembly();

    // write as a fasta to stdout
    for (size_t i = 0; i < contigs.size(); ++i) {
        std::cout << ">contig" << i << std::endl << contigs[i] << std::endl;
    }
    return 0;
}
