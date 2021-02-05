#include "pattern_matching_with_suffix_array.h"
#include "better_bw_matching.h"

#include <iostream>
#include <fstream>
#include <random>
#include <map>
#include <time.h>
using namespace std;

int main()
{
    cout << "Enter '1' to generate random genome and reads.\n"
        "Enter '2' to generate new reads from existing genome.\n"
        "Otherwise, enter '0' or anything else to proceed to algorithms." << endl;
    
    string *genome = new string;
    int *input = new int;
    cin >> *input;

    switch (*input)
    {
    case 1:
        /* Generate human genome with 1 million nucleotides with random As, Cs, Ts, and Gs */
    {
        string Text = *genome;
        int genome_length = 0;

        cout << "Enter the number of nucleotides of human genome to randomly generate." << endl;
        cin >> genome_length;
        cout << "Generating genome . . ." << endl;

        ofstream file_genome;
        file_genome.open("random_human_genome.txt");
        char nucleotides[4] = { 'A', 'C', 'G', 'T' };
        char nt = 0; // temporarily hold value of random nucleotide
        int num_nucleotides = 0;

        srand(time(0)); // initialize seed for rand()
        for (int i = 0; i < genome_length; ++i)
        {
            nt = nucleotides[rand() % 4];
            file_genome << nt; // store in file
            Text += nt; // store in genome string
            ++num_nucleotides;
        }
        file_genome.close();

        if (num_nucleotides == 1000000)
            cout << "Human genome of 1 million nucleotides generated!\n" << endl;
        else
            cout << "Human genome of " << num_nucleotides << " nucleotides generated!\n" << endl;

        *genome = Text;
    }
    case 2:
        /* Generate X number of reads with length of Y nucleotides */
    {
        if (genome->empty()) // genome already exists -> extract from existing file
        {
            ifstream file_genome("random_human_genome.txt");
            if (!file_genome)
            {
                cerr << "Failed to read genome file!" << endl;
                return -1;
            }
            getline(file_genome, *genome);
        }

        string Text = *genome;
        int Text_size = Text.size();
        int X = 0; // number of reads
        int Y = 0; // length of reads

        if (Text_size == 1000000)
            cout << "Current genome is size of 1 million nucleotides.\n" << endl;
        else
            cout << "Current genome is size of " << Text_size << " nucleotides.\n" << endl;
        while (true)
        {
            cout << "Enter the number of reads to randomly generate." << endl;
            cin >> X;
            if (X > 0)
                break;
            cout << "Number of reads must be one (1) or more. e.g. 100000\n" << endl;
        }
        while (true)
        {
            cout << "Enter the number of nucleotides in each read." << endl;
            cin >> Y;
            if (Y <= Text_size && Y > 0)
                break;
            cout << "Size of read must be at least one (1) nucleotide long but at most \n"
                "the size of the genome (" << Text_size << "). e.g. 100 for a genome \n"
                "of 1 million nucleotides.\n" << endl;
        }
        cout << "Generating reads . . ." << endl;

        //// analogous to numpy.random.randint() in Python ////
        random_device rd; // initialize (seed) engine
        mt19937 gen(rd()); // random-number engine used (Mersenne-Twister in this case)
        uniform_int_distribution<int> uni(0, Text_size - Y); // (min, max) of index for genome
        //// https://stackoverflow.com/questions/7560114/random-number-c-in-some-range ////

        ofstream file_reads;
        file_reads.open("random_human_reads.txt");
        for (int i = 0; i < X; ++i)
            file_reads << Text.substr(uni(gen), Y) << endl;
        file_reads.close();
    }
    default:
        delete input; // input no longer needed

        cout << "Building Text . . ." << endl;
        if (genome->empty()) // genome already exists -> extract from existing file
        {
            ifstream file_genome("random_human_genome.txt");
            if (!file_genome)
            {
                cerr << "Failed to read genome file!" << endl;
                return -1;
            }
            getline(file_genome, *genome);
        }
        string Text = *genome; // BREAKPOINT 1
        Text += '$'; // append '$' character to genome
        int Text_size = Text.size(); // Text.size() was O(1) anyway, did not know until after data was taken

        cout << "Building SuffixArray . . ." << endl;
        /* Build SuffixArray input for PatternMatchingWithSuffixArray */
        map<string, int> sort; // alphabetically sort cyclic rotations

        for (int i = 0; i < Text_size; ++i)
        {
            string rotation = Text.substr(i) + Text.substr(0, i); // cyclic rotation
            sort.insert({ rotation, i }); // behaves like M(Text) matrix
        }

        int* SuffixArray = new int[Text_size];
        int i = 0;
        for (auto p : sort)
            SuffixArray[i++] = p.second;

        cout << "Building LastColumn . . ." << endl;
        /* Build LastColumn input for BetterBWMatching */
        string LastColumn; // BWT(Text)
        for (auto p : sort)
            LastColumn += p.first[Text_size - 1];

        sort.clear(); // remove M(Text) from heap

        /* Initialize FirstOccurence array input for BetterBWMatching */
        int FirstOccurrence[5] = {}; // complicated, algorithm described in Report

        cout << "Building Count matrix . . ." << endl;
        /* Build Count matrix input for BetterBWMatching */
        auto Count = new int[Text_size + 1][5]; // column for '$', 'A', 'C', 'G', 'T' respectively; rows are length LastColumn+1; true type is int (*Count)[5]
        int count_row[5] = {}; // holds current row of Count values; initialized to zeros; '$', 'A', 'C', 'G', 'T' respectively

        for (int i = 0; i < 5; ++i)
            Count[0][i] = count_row[i]; // first row of Count matrix is zeros
        for (size_t i = 1; i < Text_size + 1; ++i) // LastColumn length == Text length
        {
            switch ((LastColumn)[i - 1]) // building Count_symbol(i, LastColumn)
            {
            case '$':
                ++count_row[0];
                for (int j = 0; j < 5; ++j)
                    Count[i][j] = count_row[j];
                ++FirstOccurrence[1];
                break;
            case 'A':
                ++count_row[1];
                for (int j = 0; j < 5; ++j)
                    Count[i][j] = count_row[j];
                ++FirstOccurrence[2];
                break;
            case 'C':
                ++count_row[2];
                for (int j = 0; j < 5; ++j)
                    Count[i][j] = count_row[j];
                ++FirstOccurrence[3];
                break;
            case 'G':
                ++count_row[3];
                for (int j = 0; j < 5; ++j)
                    Count[i][j] = count_row[j];
                ++FirstOccurrence[4];
                break;
            case 'T':
                ++count_row[4];
                for (int j = 0; j < 5; ++j)
                    Count[i][j] = count_row[j];
                break;
            }
        }

        cout << "Building FirstOccurrence array . . ." << endl;
        /* Build FirstOccurence array input for BetterBWMatching */
        for (int i = 2; i < 5; ++i)
            FirstOccurrence[i] = FirstOccurrence[i] + FirstOccurrence[i - 1]; // complicated, algorithm described in Report

        cout << "Inputs built successfully!\n" << endl;

        /* Run algorithms */
        ifstream file_reads("random_human_reads.txt");
        if (!file_reads)
        {
            cerr << "Failed to read reads file!" << endl;
            return -1;
        }

        string Pattern;
        cout << "Running PatternMatchingWithSuffixArray . . ." << endl; // BREAKPOINT 2
        while (getline(file_reads, Pattern))
            PatternMatchingWithSuffixArray(Text, Pattern, SuffixArray);
        file_reads.clear(); // restart pattern reading index
        file_reads.seekg(0, ios::beg);
        cout << "PatternMatchingWithSuffixArray successful!\n" << endl;

        cout << "Running BetterBWMatching . . ." << endl; // BREAKPOINT 3
        while (getline(file_reads, Pattern))
            BetterBWMatching(FirstOccurrence, LastColumn, Pattern, Count);
        file_reads.clear(); // no need to restart pattern reading index here, but included to be consistent
        file_reads.seekg(0, ios::beg);
        cout << "BetterBWMatching successful!\n" << endl;
    } // BREAKPOINT 4

    delete genome;

    return 0;
}