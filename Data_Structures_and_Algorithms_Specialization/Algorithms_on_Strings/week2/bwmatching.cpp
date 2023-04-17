#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::istringstream;
using std::map;
using std::string;
using std::vector;

// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position 
//       of this character in the sorted array of 
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string &bwt,
                   map<char, int> &starts,
                   map<char, vector<int> > &occ_count_before) {

    occ_count_before.insert({'$', std::vector<int>(bwt.size() + 1, 0)});
    occ_count_before.insert({'A', std::vector<int>(bwt.size() + 1, 0)});
    occ_count_before.insert({'C', std::vector<int>(bwt.size() + 1, 0)});
    occ_count_before.insert({'G', std::vector<int>(bwt.size() + 1, 0)});
    occ_count_before.insert({'T', std::vector<int>(bwt.size() + 1, 0)});

    int cnt_a = 0, cnt_c = 0, cnt_g = 0;
    for (size_t i = 0; i < bwt.size(); ++i) {
        auto c = bwt[i];

        if (c == 'A') { ++cnt_a; }
        else if (c == 'C') { ++cnt_c; }
        else if (c == 'G') { ++cnt_g; }

        for (auto &kv: occ_count_before)
            kv.second[i + 1] = kv.second[i];
        ++occ_count_before[c][i + 1];
    }
    starts.insert({'T', 1 + cnt_a + cnt_c + cnt_g});
    starts.insert({'G', 1 + cnt_a + cnt_c});
    starts.insert({'C', 1 + cnt_a});
    starts.insert({'A', 1});
    starts.insert({'$', 0});
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string &pattern,
                     const string &bwt,
                     const map<char, int> &starts,
                     const map<char, vector<int> > &occ_count_before) {

    int cursor = static_cast<int>(pattern.size()) - 1;
    int top = 0;
    int bottom = static_cast<int>(bwt.size()) - 1;

    while (top <= bottom) {
        if (cursor < 0)
            return bottom - top + 1;

        auto symbol = pattern[cursor--];
        auto t = std::find(std::next(bwt.begin(), top), std::next(bwt.begin(), bottom + 1), symbol);
        if (t != std::next(bwt.begin(), bottom + 1)) {
            top = starts.at(symbol) + occ_count_before.at(symbol)[top];
            bottom = starts.at(symbol) + occ_count_before.at(symbol)[bottom + 1] - 1;
        } else {
            return 0;
        }
    }

    return 0;
}


int main() {
    string bwt;
    cin >> bwt;
    int pattern_count;
    cin >> pattern_count;
    // Start of each character in the sorted list of characters of bwt,
    // see the description in the comment about function PreprocessBWT
    map<char, int> starts;
    // Occurrence counts for each character and each position in bwt,
    // see the description in the comment about function PreprocessBWT
    map<char, vector<int> > occ_count_before;
    // Preprocess the BWT once to get starts and occ_count_before.
    // For each pattern, we will then use these precomputed values and
    // spend only O(|pattern|) to find all occurrences of the pattern
    // in the text instead of O(|pattern| + |text|).
    PreprocessBWT(bwt, starts, occ_count_before);
    for (int pi = 0; pi < pattern_count; ++pi) {
        string pattern;
        cin >> pattern;
        int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
        printf("%d ", occ_count);
    }
    printf("\n");
    return 0;
}
